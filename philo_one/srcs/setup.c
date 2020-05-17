/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 16:36:47 by pramella          #+#    #+#             */
/*   Updated: 2020/05/17 15:10:40 by pramella         ###   ########lyon.fr   */
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
	rules->nbr_of_req_meals = (av[5]) ? ft_atoi(av[5]) : -1;
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
** mallocs and inits thread/mutex arrays
** inits write/exit mutex
*/

int		init(t_philo **philo, t_mutex *mutex, int nbr_of_philo)
{
	int i;

	mutex->fork = NULL;
	*philo = NULL;
	if (!(mutex->fork = malloc(sizeof(*mutex->fork) * nbr_of_philo)) ||
		!(*philo = malloc(sizeof(**philo) * nbr_of_philo)))
	{
		free(mutex->fork);
		free(*philo);
		return (1);
	}
	i = -1;
	while (++i < nbr_of_philo)
	{
		pthread_mutex_init(&mutex->fork[i], NULL);
		pthread_mutex_init(&(*philo)[i].is_eating, NULL);
	}
	pthread_mutex_init(&mutex->write, NULL);
	pthread_mutex_init(&mutex->global_death, NULL);
	pthread_mutex_init(&mutex->global_finished, NULL);
	return (0);
}
