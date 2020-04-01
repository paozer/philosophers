#include "philo_one.h"

int		philo_is_dead(t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&data->mx_exit);
	if (data->exit_flag == 0)
	{
		pthread_mutex_unlock(&data->mx_exit);
		return (1);
	}
	if (get_timestamp() - philo->time_of_last_meal >= data->time_to_die)
	{
		data->exit_flag = 0;
		pthread_mutex_unlock(&data->mx_exit);
		pthread_mutex_lock(&data->mx_write);
		ft_putnbr(get_timestamp() - data->start_timestamp);
		write(1, " ", 1);
		ft_putnbr(philo->id + 1);
		write(1, " died\n", 6);
		pthread_mutex_unlock(&data->mx_write);
		return (1);
	}
		pthread_mutex_unlock(&data->mx_exit);
	return (0);
}

int		print_message(t_data *data, t_philo *philo, char *message)
{
	if (philo_is_dead(philo, data))
		return (1);
	pthread_mutex_lock(&data->mx_write);
	ft_putnbr(get_timestamp() - data->start_timestamp);
	write(1, " ", 1);
	ft_putnbr(philo->id + 1);
	write(1, message, ft_strlen(message));
	pthread_mutex_unlock(&data->mx_write);
	return (0);
}

int		get_forks(t_philo *philo, t_data *data)
{
	int i;

	if (philo_is_dead(philo, data))
		return (1);
	// add locking mutex for locking mutex
	pthread_mutex_lock(&data->mx_fork[philo->id]);
	if (print_message(data, philo, " has taken a fork\n"))
		return (1);
	i = (philo->id == data->nbr_of_philo - 1) ? 0 : philo->id + 1;
	if (philo_is_dead(philo, data))
		return (1);
	// add unlocking of mutex for locking mutex
	pthread_mutex_lock(&data->mx_fork[i]);
	if (print_message(data, philo, " has taken a fork\n"))
		return (1);
	return (0);
}

int		reset_forks(t_philo *philo, t_data *data)
{
	int i;

	if (philo_is_dead(philo, data))
		return (1);
	i = (philo->id + 1 == data->nbr_of_philo) ? 0 : philo->id + 1;
	pthread_mutex_unlock(&data->mx_fork[philo->id]);
//	if (data->nbr_of_philo % 2)
//		usleep(10000);
	pthread_mutex_unlock(&data->mx_fork[i]);
	return (0);
}

void	*life_cycle(void *ph)
{
	t_philo *philo;
	t_data	*data;

	philo = (t_philo *)ph;
	data = philo->data;
	while (1)
	{
		pthread_mutex_lock(&data->mx_exit);
		if (!(data->exit_flag))
			return (NULL);
		pthread_mutex_unlock(&data->mx_exit);
		if (print_message(data, philo, " is thinking\n"))
			return (NULL);
		if (get_forks(philo, data))
			return (NULL);
		if (print_message(data, philo, " is eating\n"))
			return (NULL);
		philo->time_of_last_meal = get_timestamp();
		usleep(data->time_to_eat);
		if (reset_forks(philo, data))
			return (NULL);
		if (print_message(data, philo, " is sleeping\n"))
			return (NULL);
		usleep(data->time_to_sleep);
	}
	return (NULL);
}

int		main(int ac, char **av)
{
	int		i;
	t_data	data;
	t_philo	*philo;

	philo = NULL;
	if ((ac != 5 && ac != 6) || parsing(av, &data) || init(&philo, &data))
	{
		write(1, "error\n", 6);
		return (1);
	}
	i = 0;
	while (i < data.nbr_of_forks)
		pthread_mutex_init(&data.mx_fork[i++], NULL);
	pthread_mutex_init(&data.mx_write, NULL);
	pthread_mutex_init(&data.mx_exit, NULL);
	data.start_timestamp = get_timestamp();
	i = 0;
	while (i < data.nbr_of_philo)
	{
		philo[i].id = i;
		philo[i].data = &data;
		philo[i].time_of_last_meal = get_timestamp();
		pthread_create(&data.thread[i], NULL, life_cycle, (void *)&philo[i]);
		pthread_detach(data.thread[i]);
		++i;
	}
	i = 0;
	while (i < data.nbr_of_forks)
		pthread_mutex_destroy(&data.mx_fork[i++]);
	pthread_mutex_destroy(&data.mx_write);
	pthread_mutex_destroy(&data.mx_exit);
	free(data.thread);
	free(data.mx_fork);
	free(philo);
	return (0);
}
