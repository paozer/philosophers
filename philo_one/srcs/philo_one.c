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
	data->mutex = NULL;
	(data->nbr_of_req_eats < 0) ? data->nbr_of_req_eats = -1 : 0;
	return (0);
}

void	*start_life_cycle(void *data)
{
	t_philo *kek;

	kek = (t_philo *)data;
	ft_putnbr(kek->id);
	return (NULL);
}

void	debug(t_data data)
{
	fprintf(stderr, "data.nbr_of_philo %d\n", data.nbr_of_philo);
	fprintf(stderr, "data.nbr_of_forks %d\n", data.nbr_of_forks);
	fprintf(stderr, "data.nbr_of_req_eats %d\n", data.nbr_of_req_eats);
	fprintf(stderr, "data.time_to_die %d\n", data.time_to_die);
	fprintf(stderr, "data.time_to_eat %d\n", data.time_to_eat);
	fprintf(stderr, "data.time_to_sleep %d\n", data.time_to_sleep);
}

int		main(int ac, char **av)
{
	int		i;
	t_data	data;
	t_philo	*philo;

	if ((ac != 5 && ac != 6) || parsing(av, &data))
	{
		write(1, "args error\n", 11);
		return (1);
	}
	philo = NULL;
	if (!(data.thread = malloc(sizeof(*data.thread) * data.nbr_of_philo)) ||
		!(data.mutex = malloc(sizeof(*data.mutex) * data.nbr_of_forks)) ||
		!(philo = malloc(sizeof(*philo) * data.nbr_of_philo)))
	{
		free(data.thread);
		free(data.mutex);
		free(philo);
		return (1);
	}
	i = 0;
	debug(data);
	while (i < data.nbr_of_philo)
	{
		philo[i].id = i;
		philo[i].data = &data;
		philo[i].time_since_last_meal = 0;
		pthread_create(&data.thread[i], NULL, start_life_cycle, (void *)&philo[i]);
		++i;
	}
	return (0);
}
