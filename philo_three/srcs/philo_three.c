#include "philo_three.h"

int	print_message(t_philo *philo, int index)
{
	char	*message[4] = {" is thinking\n", " has taken a fork\n", " is eating\n", " is sleeping\n"};
	size_t	len[4] = {13, 18, 11, 13};

	sem_wait(philo->sem.write);
	ft_putnbr(get_timestamp_ms() - philo->rules.time_of_start_ms);
	write(1, " ", 1);
	ft_putnbr(philo->id + 1);
	write(1, " ", 1);
	write(1, message[index], len[index]);
	sem_post(philo->sem.write);
	return (1);
}

void	do_eating(t_philo *philo)
{
	t_semaphore	sem;
	t_rules		rules;

	sem = philo->sem;
	rules = philo->rules;
	sem_wait(sem.forks);
	print_message(philo, MSG_FORK);
	sem_wait(sem.forks);
	print_message(philo, MSG_FORK);

	print_message(philo, MSG_EATING);
	philo->time_of_last_meal_ms = get_timestamp_ms();
	usleep(rules.time_to_eat_us);

	sem_post(sem.forks);
	sem_post(sem.forks);

	if (++philo->meal_counter == rules.nbr_of_req_eats)
		sem_post(sem.finished_eating);
}

void	do_thinking(t_philo *philo)
{
	print_message(philo, MSG_THINKING);
}

void	do_sleeping(t_philo *philo)
{
	print_message(philo, MSG_SLEEPING);
	usleep(philo->rules.time_to_sleep_us);
}

void	*monitor(void *ph)
{
	t_philo			*philo;
	unsigned long	timestamp_ms;

	philo = (t_philo *)ph;
	while (1)
	{
		timestamp_ms = get_timestamp_ms();
		if (timestamp_ms - philo->time_of_last_meal_ms >=
			philo->rules.time_to_die_ms)
		{
			sem_wait(philo->sem.write);
			sem_post(philo->sem.simulation_end);
			ft_putnbr(timestamp_ms - philo->rules.time_of_start_ms);
			write(1, " ", 1);
			ft_putnbr(philo->id + 1);
			write(1, " died\n", 6);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

void	*life_cycle(void *ph)
{
	pthread_t	tid;
	t_philo		*philo;
	t_semaphore	*sem;

	philo = (t_philo *)ph;
	sem = &philo->sem;
	sem->forks = sem_open("/forks", 0644);
	sem->write = sem_open("/write", 0644);
	sem->read = sem_open("/read", 0644);
	sem->simulation_end = sem_open("/simulation_end", 0644);
	sem->finished_eating = sem_open("/finished_eating", 0644);
	pthread_create(&tid, NULL, monitor, philo);
	while (1)
	{
		do_thinking(philo);
		do_eating(philo);
		do_sleeping(philo);
	}
	return (NULL);
}

typedef struct	s_monitor_data
{
	int			nbr_of_philo;
	int			nbr_of_req_eats;
	t_rules		rules;
	sem_t		*finished_eating;
	sem_t		*simulation_end;
	sem_t		*write;
}				t_monitor_data;

void	*monitor_finished_eating(void *monitor_data)
{
	t_monitor_data *data;
	int				count;

	count = 0;
	data = (t_monitor_data *)monitor_data;
	if (data->nbr_of_req_eats < 1)
		return (NULL);
	while (1)
	{
		sem_wait(data->finished_eating);
		sem_post(data->finished_eating);
		if (++count == data->nbr_of_philo)
		{
			sem_wait(data->write);
			sem_post(data->simulation_end);
			write(1, "All philosophers ate enough\n", 28);
			return (NULL);
		}
	}
	return (NULL);
}

int		main(int ac, char **av)
{
	int			i;
	pthread_t	tid;
	pid_t		*pid;
	t_philo		philo;
	t_rules		rules;
	t_semaphore sem;
	t_monitor_data monitor_data;

	if ((ac != 5 && ac != 6) || parsing(av, &rules) ||
		init(&pid, &sem, rules.nbr_of_philo) || rules.nbr_of_philo < 2)
		return (1);
	monitor_data.nbr_of_philo = rules.nbr_of_philo;
	monitor_data.nbr_of_req_eats = rules.nbr_of_req_eats;
	monitor_data.rules = rules;
	monitor_data.write = sem.write;
	monitor_data.finished_eating = sem.finished_eating;
	monitor_data.simulation_end = sem.simulation_end;
	pthread_create(&tid, NULL, monitor_finished_eating, &monitor_data);
	i = -1;
	rules.time_of_start_ms = get_timestamp_ms();
	while (++i < rules.nbr_of_philo)
	{
		philo.id = i;
		philo.rules = rules;
		philo.meal_counter = 0;
		philo.time_of_last_meal_ms = get_timestamp_ms();
		pid[i] = fork();
		if (pid[i] == -1)
			write(2, "forking error\n", 14);
		else if (pid[i] == 0)
		{
			life_cycle(&philo);
			// free necessary shit
			free(pid);
			exit(0);
		}
	}
	sem_wait(sem.simulation_end);
	i = -1;
	while (++i < rules.nbr_of_philo)
		kill(pid[i], SIGKILL);
	sem_unlink("/forks");
	sem_unlink("/write");
	sem_unlink("/read");
	sem_unlink("/simulation_end");
	sem_unlink("/finished_eating");
	free(pid);
	return (0);
}
