#include "philo_one.h"

int		parsing(char *av[], t_data *data)
{
	data->nbr_of_philo = ft_atoi(av[1]);
	data->nbr_of_forks = data->nbr_of_philo;
	data->time_to_die = ft_atoul(av[2]) * 1000;
	data->time_to_eat = ft_atoul(av[3]) * 1000;
	data->time_to_sleep = ft_atoul(av[4]) * 1000;
	data->nbr_of_req_eats = (av[5]) ? ft_atoi(av[5]) : -1;
	if (data->nbr_of_philo < 2)
		return (1);
	data->exit_flag = 1;
	(data->nbr_of_req_eats < 0) ? data->nbr_of_req_eats = -1 : 0;
	return (0);
}

int		init(t_philo **philo, t_data *data)
{
	if (!(data->thread = malloc(sizeof(*data->thread) * data->nbr_of_philo)) ||
		!(data->mx_fork = malloc(sizeof(*data->mx_fork) * data->nbr_of_forks)) ||
		!(*philo = malloc(sizeof(**philo) * data->nbr_of_philo)))
	{
		free(data->thread);
		free(data->mx_fork);
		free(*philo);
		return (1);
	}
	return (0);
}
