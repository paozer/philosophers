/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 14:07:58 by pramella          #+#    #+#             */
/*   Updated: 2020/05/20 21:09:41 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** monitoring thread created by main to monitor if all the philosophers have
** eaten enough meals
*/

void	*monitor_meals(void *ph)
{
	t_mutex *mx;

	mx = ((t_philo *)ph)->mutex;
	while (1)
	{
		pthread_mutex_lock(&mx->global_died);
		if (g_philo_died)
		{
			pthread_mutex_unlock(&mx->global_died);
			return (NULL);
		}
		pthread_mutex_lock(&mx->global_satiated);
		if (g_philos_satiated == ((t_philo *)ph)->rules->nbr_of_philo)
		{
			pthread_mutex_unlock(&mx->global_satiated);
			g_philo_died = 1;
			pthread_mutex_unlock(&mx->global_died);
			return (print_exit(ph, ARE_SATIATED, 1));
		}
		pthread_mutex_unlock(&mx->global_satiated);
		pthread_mutex_unlock(&mx->global_died);
		usleep(1000);
	}
}

/*
** monitoring thread created by each philosopher to monitor it's own health
*/

void	*monitor_death(void *ph)
{
	t_philo			*philo;
	unsigned long	timestamp;

	philo = ph;
	while (1)
	{
		pthread_mutex_lock(&philo->last_meal);
		if ((timestamp = get_timestamp_ms()) - philo->time_of_last_meal_ms >
			philo->rules->time_to_die_ms)
		{
			pthread_mutex_unlock(&philo->last_meal);
			pthread_mutex_lock(&philo->mutex->global_died);
			if (!g_philo_died)
			{
				g_philo_died = 1;
				pthread_mutex_unlock(&philo->mutex->global_died);
				return (print_exit(philo, HAS_DIED, timestamp));
			}
			pthread_mutex_unlock(&philo->mutex->global_died);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->last_meal);
		usleep(1000);
	}
}

void	*print_exit(t_philo *philo, int index, unsigned long timestamp)
{
	static char *status[2] = {" has died\n",
							"All philosophers have eaten enough\n"};
	static int	len[2] = {10, 35};

	pthread_mutex_lock(&philo->mutex->write);
	if (index == HAS_DIED)
	{
		ft_putnbr(timestamp - philo->rules->time_of_start_ms);
		write(1, "\t", 1);
		ft_putnbr(philo->id + 1);
	}
	write(1, status[index], len[index]);
	pthread_mutex_unlock(&philo->mutex->write);
	return (NULL);
}
