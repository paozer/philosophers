/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 14:07:58 by pramella          #+#    #+#             */
/*   Updated: 2020/05/17 15:25:39 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** monitoring thread created by main to monitor if all the philosophers have
** eaten enough meals
*/

void	*monitor_meals(void *philo)
{
	t_mutex *mutex;
	t_rules *rules;

	mutex = ((t_philo *)philo)->mutex;
	rules = ((t_philo *)philo)->rules;
	while (1)
	{
		pthread_mutex_lock(&mutex->global_death);
		if (g_philo_has_died)
		{
			pthread_mutex_unlock(&mutex->global_death);
			return (NULL);
		}
		pthread_mutex_lock(&mutex->global_finished);
		if (g_philos_have_eaten_enough == rules->nbr_of_philo)
		{
			pthread_mutex_unlock(&mutex->global_finished);
			g_philo_has_died = 1;
			pthread_mutex_unlock(&mutex->global_death);
			return (print_exit(philo, 1, 1));
		}
		pthread_mutex_unlock(&mutex->global_death);
		pthread_mutex_unlock(&mutex->global_finished);
		usleep(1000);
	}
}

/*
** monitoring thread created by each philosopher to monitor it's own health
** and to check if another philo died
*/

void	*monitor_death(void *ph)
{
	t_philo			*philo;
	unsigned long	timestamp;

	philo = ph;
	while (1)
	{
		pthread_mutex_lock(&philo->is_eating);
		if ((timestamp = get_timestamp_ms()) - philo->time_of_last_meal_ms >
			philo->rules->time_to_die_ms)
		{
			pthread_mutex_unlock(&philo->is_eating);
			pthread_mutex_lock(&philo->mutex->global_death);
			if (!g_philo_has_died)
			{
				g_philo_has_died = 1;
				pthread_mutex_unlock(&philo->mutex->global_death);
				return (print_exit(philo, 0, timestamp));
			}
			pthread_mutex_unlock(&philo->mutex->global_death);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->is_eating);
		usleep(1000);
	}
}

void	*print_exit(t_philo *philo, int index, unsigned long timestamp)
{
	static char		*msg[2] = {" has died\n",
							"All philosophers have eaten enough\n"};
	static size_t	len[2] = {10, 35};

	pthread_mutex_lock(&philo->mutex->write);
	if (index == 0)
	{
		ft_putnbr(timestamp - philo->rules->time_of_start_ms);
		write(1, "\t", 1);
		ft_putnbr(philo->id + 1);
	}
	write(1, msg[index], len[index]);
	pthread_mutex_unlock(&philo->mutex->write);
	return (NULL);
}
