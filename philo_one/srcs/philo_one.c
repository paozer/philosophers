#include "philo_one.h"

// number_of_philosophers
// time_to_die
// time_to_eat
// time_to_sleep
// [number_of_times_each_philosopher_must_eat]

int	parsing(char *av[], t_data *data)
{
	data->nbr_of_philo = ft_atoi(av[1]);
	data->nbr_of_forks = data->nbr_of_philo;
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->nbr_of_req_eats = (av[5]) ? ft_atoi(av[5]) : -1;
	if (data->nbr_of_philo < 0 || data->time_to_die < 0 ||
		data->time_to_eat < 0 || data->time_to_sleep < 0)
		return (1);
	data->thread = NULL;
	data->mx_fork = NULL;
	data->exit_flag = 1;
	(data->nbr_of_req_eats < 0) ? data->nbr_of_req_eats = -1 : 0;
	return (0);
}

void	print_message(t_data *data, int id, char *message)
{
	pthread_mutex_lock(&data->mx_write);
	ft_putnbr(id);
	write(1, message, ft_strlen(message));
	pthread_mutex_unlock(&data->mx_write);
}

int		philo_is_dead(t_philo *philo, t_data *data)
{
	struct timeval tp;

	if (data->exit_flag == 0)
		return (1);
	gettimeofday(&tp, NULL);
	if (tp.tv_usec - philo->time_of_last_meal > data->time_to_die)
	{
		data->exit_flag = 0;
		print_message(data, philo->id_clean, " died\n");
		return (1);
	}
	return (0);
}

int		get_forks(t_philo *philo, t_data *data)
{
	int i;
	int j;

	if (philo_is_dead(philo, data))
		return (0);
	i = (philo->id == 0) ? data->nbr_of_philo - 1 : philo->id - 1;
	j = (philo->id == data->nbr_of_philo) ? 0 : philo->id + 1;
	pthread_mutex_lock(&data->mx_fork[i]);
	if (philo_is_dead(philo, data))
		return (0);
	print_message(data, philo->id_clean, " has taken a fork\n");
	pthread_mutex_lock(&data->mx_fork[j]);
	if (philo_is_dead(philo, data))
		return (0);
	print_message(data, philo->id_clean, " has taken a fork\n");
	return (1);
}

void	*life_cycle(void *ph)
{
	t_philo *philo;
	t_data	*data;
	struct timeval tp;

	philo = (t_philo *)ph;
	data = philo->data;
	while (data->exit_flag)
	{
		print_message(data, philo->id_clean, " is thinking\n");
		if (!get_forks(philo, data))
			return (NULL);
		print_message(data, philo->id_clean, " is eating\n");
		gettimeofday(&tp, NULL);
		philo->time_of_last_meal = tp.tv_usec;
		usleep(data->time_to_eat);
		if (philo_is_dead(philo, data))
			return (NULL);
		print_message(data, philo->id_clean, " is sleeping\n");
		usleep(data->time_to_sleep);
	}
	return (NULL);
}

int		main(int ac, char **av)
{
	int i;
	t_data data;
	t_philo *philo;
	struct timeval tp;

	if ((ac != 5 && ac != 6) || parsing(av, &data))
	{
		write(1, "args error\n", 11);
		return (1);
	}
	philo = NULL;
	if (!(data.thread = malloc(sizeof(*data.thread) * data.nbr_of_philo)) ||
		!(data.mx_fork = malloc(sizeof(*data.mx_fork) * data.nbr_of_forks)) ||
		!(philo = malloc(sizeof(*philo) * data.nbr_of_philo)))
	{
		free(data.thread);
		free(data.mx_fork);
		free(philo);
		return (1);
	}
	i = 0;
	while (i < data.nbr_of_forks)
		pthread_mutex_init(&data.mx_fork[i++], NULL);
	pthread_mutex_init(&data.mx_write, NULL);
	i = 0;
	while (i < data.nbr_of_philo)
	{
		philo[i].id = i;
		philo[i].id_clean = i + 1;
		philo[i].data = &data;
		gettimeofday(&tp, NULL);
		philo[i].time_of_last_meal = tp.tv_usec;
		pthread_create(&data.thread[i], NULL, life_cycle, (void *)&philo[i]);
		++i;
	}
	i = 0;
	while (i < data.nbr_of_forks)
		pthread_mutex_destroy(&data.mx_fork[i++]);
	pthread_mutex_destroy(&data.mx_write);
	return (0);
}
