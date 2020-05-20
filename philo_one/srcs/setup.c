/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 16:36:47 by pramella          #+#    #+#             */
/*   Updated: 2020/05/20 20:49:31 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		parse(char *av[], t_rules *rules)
{
	if (!valid_arguments(av))
	{
		write(2, "Error: only positive numeric arguments are allowed\n", 51);
		return (0);
	}
	rules->nbr_of_philo = ft_atol(av[1]);
	if (rules->nbr_of_philo < 2)
	{
		write(2, "Error: a minimum of two philosophers are required\n", 50);
		return (0);
	}
	rules->nbr_of_req_meals = (av[5]) ? ft_atol(av[5]) : -1;
	rules->time_to_die_ms = ft_atol(av[2]);
	rules->time_to_eat_us = ft_atol(av[3]) * 1000;
	rules->time_to_sleep_us = ft_atol(av[4]) * 1000;
	return (1);
}

int		init(t_philo **philo, t_mutex *mutex, int nbr_of_philo)
{
	int			i;
	static int	ret;

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
		ret += pthread_mutex_init(&mutex->fork[i], NULL);
		ret += pthread_mutex_init(&(*philo)[i].last_meal, NULL);
	}
	ret += pthread_mutex_init(&mutex->write, NULL);
	ret += pthread_mutex_init(&mutex->global_died, NULL);
	ret += pthread_mutex_init(&mutex->global_satiated, NULL);
	(ret != 0) ? cleanup(*philo) : 0;
	if (ret != 0)
		return (0);
	return (1);
}

int		valid_arguments(char *av[])
{
	int i;
	int j;

	i = 0;
	while (av[++i])
	{
		j = -1;
		while (av[i][++j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (0);
		}
	}
	return (1);
}
