#include "philo_two.h"

/*
** parses cli arguments
** time to ... are passed in ms and stored in ms/us
** for simpler use with usleep fct
*/

int		parsing(char *av[], t_rules *rules)
{
	rules->nbr_of_philo = ft_atoi(av[1]);
	if (rules->nbr_of_philo < 2)
		return (1);
	rules->nbr_of_req_eats = (av[5]) ? ft_atoi(av[5]) : -1;
	rules->time_to_die_ms = ft_atol(av[2]);
	rules->time_to_eat_ms = ft_atol(av[3]);
	rules->time_to_sleep_ms = ft_atol(av[4]);
	rules->time_to_die_us = rules->time_to_die_ms * 1000;
	rules->time_to_eat_us = rules->time_to_eat_ms * 1000;
	rules->time_to_sleep_us = rules->time_to_sleep_ms * 1000;
	return (0);
}

/*
** mallocs philo array
** inits fork/write/exit semaphores
*/

int		init(t_philo **philo, t_semaphore *sem, int nbr_of_philo)
{
	if (!(*philo = malloc(sizeof(**philo) * nbr_of_philo)))
		return (1);
	sem->forks = sem_open("/forks", O_CREAT | O_EXCL, 0644, nbr_of_philo);
	sem->write = sem_open("/write", O_CREAT | O_EXCL, 0644, 1);
	sem->read = sem_open("/read", O_CREAT | O_EXCL, 0644, 1);
	// make failure test individual
	if (sem->forks == SEM_FAILED || sem->write == SEM_FAILED ||
		sem->read == SEM_FAILED)
	{
		write(2, "malloc/sem_open error\n", 22);
		return (1);
	}
	return (0);
}
