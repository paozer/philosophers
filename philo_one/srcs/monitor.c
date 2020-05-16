/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 14:07:58 by pramella          #+#    #+#             */
/*   Updated: 2020/05/17 00:20:23 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** monitoring thread created by main to monitor if all the philosophers have
** eaten enougn meals
*/

void	*monitor_finished(void *ph)
{
	t_mutex *mutex;
	t_rules *rules;

	mutex = ((t_philo *)ph)->mutex;
	rules = ((t_philo *)ph)->rules;
	while (1)
	{

		pthread_mutex_lock(&mutex->gblvar);
		if (g_philo_has_died)
		{
			pthread_mutex_unlock(&mutex->gblvar);
			return (NULL);
		}
		if (g_philo_have_eaten == rules->nbr_of_philo)
		{
			g_philo_has_died = 1;
			pthread_mutex_unlock(&mutex->gblvar);
			print_exit(ph, 1, 1);
			return (NULL);
		}
		pthread_mutex_unlock(&mutex->gblvar);
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
		pthread_mutex_lock(&philo->eating);
		if ((timestamp = get_timestamp_ms()) - philo->time_of_last_meal_ms >
			philo->rules->time_to_die_ms)
		{
			pthread_mutex_unlock(&philo->eating);
			pthread_mutex_lock(&philo->mutex->gblvar);
			if (!g_philo_has_died)
			{
				g_philo_has_died = 1;
				pthread_mutex_unlock(&philo->mutex->gblvar);
				print_exit(philo, 0, timestamp);
				return (NULL);
			}
			pthread_mutex_unlock(&philo->mutex->gblvar);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->eating);
		usleep(1000);
	}
}

void	print_exit(t_philo *philo, int index, unsigned long timestamp)
{
	static char *msg[2] = {" has died\n",
							"All philosophers have eaten enough\n"};
	static int	len[2] = {10, 35};

	pthread_mutex_lock(&philo->mutex->write);
	if (index == 0)
	{
		ft_putnbr(timestamp - philo->rules->time_of_start_ms);
		write(1, " ", 1);
		ft_putnbr(philo->id + 1);
		write(1, " ", 1);
	}
	write(1, msg[index], len[index]);
	pthread_mutex_unlock(&philo->mutex->write);
}
