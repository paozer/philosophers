/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 16:36:47 by pramella          #+#    #+#             */
/*   Updated: 2020/05/23 14:54:23 by pramella         ###   ########lyon.fr   */
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

int		init(t_philo **philo, t_semaphore *sem, int nbr_of_philo)
{
	int		i;
	int		err_flag;
	char	*str;

	if (!(*philo = malloc(sizeof(**philo) * nbr_of_philo)))
		return (0);
	sem->fork = sem_open("/fork", O_CREAT | O_EXCL, 0644, nbr_of_philo);
	sem->write = sem_open("/write", O_CREAT | O_EXCL, 0644, 1);
	sem->finished_meals = sem_open("/finished_meals",
			O_CREAT | O_EXCL, 0644, 0);
	sem->simulation_end = sem_open("/simulation_end",
			O_CREAT | O_EXCL, 0644, 0);
	err_flag = 0;
	i = -1;
	while (++i < nbr_of_philo)
	{
		str = get_sem_name("/last_meal", i);
		((*philo)[i]).last_meal = sem_open(str, O_CREAT | O_EXCL, 0644, 1);
		free(str);
		err_flag += (((*philo)[i]).last_meal == SEM_FAILED) ? 1 : 0;
	}
	if (sem->fork == SEM_FAILED || sem->write == SEM_FAILED || err_flag ||
		sem->finished_meals == SEM_FAILED || sem->simulation_end == SEM_FAILED)
	{
		unlink_semaphores(nbr_of_philo);
		free(*philo);
		return (0);
	}
	return (1);
}

char	*get_sem_name(char *basename, int added_index)
{
	size_t	len;
	char	*str;
	char	*index;

	if (!(index = ft_itoa(added_index)))
		return (NULL);
	len = ft_strlen(basename) + ft_strlen(index);
	if (!(str = malloc(sizeof(*str) * (len + 1))))
	{
		free(index);
		return (NULL);
	}
	ft_strlcpy(str, basename, len);
	ft_strlcpy(str + ft_strlen(basename), index, len);
	free(index);
	return (str);
}

/*
** need to unlink all semaphores right after their initialization
** so they do not persist if we quit program using ctrl-c during
** inifinite simulation loop
*/

int		unlink_semaphores(int nbr_of_philo)
{
	int		i;
	char	*str;

	i = -1;
	while (++i < nbr_of_philo)
	{
		str = get_sem_name("/last_meal", i);
		sem_unlink(str);
		free(str);
	}
	sem_unlink("/fork");
	sem_unlink("/write");
	sem_unlink("/finished_meals");
	sem_unlink("/simulation_end");
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
