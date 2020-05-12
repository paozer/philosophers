/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 16:37:09 by pramella          #+#    #+#             */
/*   Updated: 2020/05/12 16:04:53 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
** inits fork/write/exit sems
*/

int		init(pid_t **pid, t_semaphore *sem, int nbr_of_philo)
{
	if (!(*pid = malloc(sizeof(**pid) * nbr_of_philo)))
		return (1);
	sem->forks = sem_open("/forks", O_CREAT | O_EXCL, 0644, nbr_of_philo);
	sem->write = sem_open("/write", O_CREAT | O_EXCL, 0644, 1);
	sem->simulation_end = sem_open("/simulation_end", O_CREAT | O_EXCL, 0644, 0);
	sem->finished_eating = sem_open("/finished_eating", O_CREAT | O_EXCL, 0644, 0);
	if (sem->forks == SEM_FAILED || sem->write == SEM_FAILED ||
		sem->simulation_end == SEM_FAILED || sem->finished_eating == SEM_FAILED)
	{
		sem_unlink("/forks");
		sem_unlink("/write");
		sem_unlink("/simulation_end");
		sem_unlink("/finished_eating");
		free(*pid);
		return (1);
	}
	return (0);
}

void	set_monitor_data(t_mnt_data *mnt_data, t_rules rules, t_semaphore sem)
{
	mnt_data->nbr_of_philo = rules.nbr_of_philo;
	mnt_data->nbr_of_req_eats = rules.nbr_of_req_eats;
	mnt_data->rules = rules;
	mnt_data->sem = sem;
}