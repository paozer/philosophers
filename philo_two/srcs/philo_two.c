#include "philo_two.h"

int g_philo_has_died_flag;
int g_philo_have_eaten_counter;

int		philo_is_dead(t_philo *philo, int sleep_flag)
{
	unsigned long time_diff;
	unsigned long timestamp_ms;

	timestamp_ms = get_timestamp_ms();
	timestamp_ms += (sleep_flag) ? philo->rules->time_to_sleep_ms : 0;
	time_diff = timestamp_ms - philo->time_of_last_meal_ms;
	if (time_diff >= philo->rules->time_to_die_ms)
	{
		(sleep_flag) ? usleep(1000 * (philo->time_of_last_meal_ms +
					philo->rules->time_to_die_ms - timestamp_ms -
					philo->rules->time_to_sleep_ms)) : 0;
		(sleep_flag) ? timestamp_ms = get_timestamp_ms() : 0;
		sem_wait(philo->sem->read);
		g_philo_has_died_flag = 1;
		sem_post(philo->sem->read);
		sem_wait(philo->sem->write);
		ft_putnbr(timestamp_ms - philo->rules->time_of_start_ms);
		write(1, " ", 1);
		ft_putnbr(philo->id + 1);
		write(1, " died\n", 6);
		return (1);
	}
	return (0);
}

int	print_message(t_philo *philo, int index)
{
	int		sleep_flag;
	char	*message[4] = {" is thinking\n", " has taken a fork\n", " is eating\n", " is sleeping\n"};
	size_t	len[4] = {13, 18, 11, 13};

	sleep_flag = (index == MSG_SLEEPING) ? 1 : 0;
	if (philo_is_dead(philo, sleep_flag))
		return (0);
	sem_wait(philo->sem->write);
	ft_putnbr(get_timestamp_ms() - philo->rules->time_of_start_ms);
	write(1, " ", 1);
	ft_putnbr(philo->id + 1);
	write(1, " ", 1);
	write(1, message[index], len[index]);
	sem_post(philo->sem->write);
	return (1);
}

int	do_eating(t_philo *philo)
{
	int			i;

	i = (philo->id == philo->rules->nbr_of_philo - 1) ? 0 : philo->id + 1;
	sem_wait(philo->sem->forks);
	if (!print_message(philo, MSG_FORK))
		return (0);
	sem_wait(philo->sem->forks);
	if (!print_message(philo, MSG_FORK))
		return (0);
	if (!print_message(philo, MSG_EATING))
		return (0);
	philo->time_of_last_meal_ms = get_timestamp_ms();
	usleep(philo->rules->time_to_eat_us);
	sem_post(philo->sem->forks);
	sem_post(philo->sem->forks);
	sem_wait(philo->sem->read);
	if (++philo->meal_counter == philo->rules->nbr_of_req_eats)
		++g_philo_have_eaten_counter;
	sem_post(philo->sem->read);
	return (1);
}

int	do_thinking(t_philo *philo)
{
	if (!print_message(philo, MSG_THINKING))
		return (0);
	return (1);
}

int	do_sleeping(t_philo *philo)
{
	if (!print_message(philo, MSG_SLEEPING))
		return (0);
	usleep(philo->rules->time_to_sleep_us);
	return (1);
}

void	*life_cycle(void *ph)
{
	t_philo *philo;

	philo = (t_philo *)ph;
	while (1)
	{
		if (!do_thinking(philo))
			break ;
		if (!do_eating(philo))
			break ;
		if (!do_sleeping(philo))
			break ;
	}
	return (NULL);
}

int		main(int ac, char **av)
{
	int			i;
	t_philo		*philo;
	t_rules		rules;
	t_semaphore	sem;

	philo = NULL;
	if ((ac != 5 && ac != 6) || parsing(av, &rules) ||
		init(&philo, &sem, rules.nbr_of_philo))
		return (1);
	i = 0;
	rules.time_of_start_ms = get_timestamp_ms();
	while (i < rules.nbr_of_philo)
	{
		philo[i].id = i;
		philo[i].rules = &rules;
		philo[i].sem = &sem;
		philo[i].meal_counter = 0;
		philo[i].time_of_last_meal_ms = get_timestamp_ms();
		pthread_create(&philo[i].tid, NULL, life_cycle, &philo[i]);
		pthread_detach(philo[i].tid);
		++i;
	}
	while (1)
	{
		sem_wait(sem.read);
		if (g_philo_has_died_flag)
		{
			sem_post(sem.read);
			break ;
		}
		if (rules.nbr_of_req_eats > 0 &&
			g_philo_have_eaten_counter == rules.nbr_of_philo)
		{
			sem_post(sem.read);
			sem_wait(sem.write);
			write(1, "All philosophers ate enough\n", 28);
			break ;
		}
		sem_post(sem.read);
		usleep(1000);
	}
	sem_unlink("/forks");
	sem_unlink("/write");
	sem_unlink("/read");
	free(philo);
	return (0);
}
