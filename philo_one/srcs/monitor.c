/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 14:07:58 by pramella          #+#    #+#             */
/*   Updated: 2020/05/16 19:05:59 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_monitor(void *ph)
{
	t_philo			*philo;
	unsigned long	ts;

	philo = (t_philo *)ph;
	while (1)
	{
		pthread_mutex_lock(&philo->eating);
		if ((ts = get_timestamp_ms()) - philo->time_of_last_meal_ms >
			philo->rules->time_to_die_ms)
		{
			pthread_mutex_unlock(&philo->eating);
			pthread_mutex_lock(&philo->mutex->gblvar);
			if (!g_philo_has_died)
			{
				g_philo_has_died = 1;
				pthread_mutex_unlock(&philo->mutex->gblvar);
				print_exit(philo, 0, ts);
				return (NULL);
			}
			pthread_mutex_unlock(&philo->mutex->gblvar);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->eating);
		// if (philo->rules->nbr_of_req_eats > 0)
		// {
		// 	pthread_mutex_lock(&philo->mutex->gblvar);
		// 	if (g_philo_have_eaten == philo->rules->nbr_of_philo)
		// 	{
		// 		g_philo_has_died = 1;
		// 		pthread_mutex_unlock(&philo->mutex->gblvar);
		// 		print_exit(NULL, 1);
		// 		return (NULL);
		// 	}
		// 	pthread_mutex_unlock(&philo->mutex->gblvar);
		// }
		usleep(1000);
	}
}

void	print_exit(t_philo *philo, int index, unsigned long timestamp)
{
	static char *msg[2] = {" has died\n",
							"All philosophers have eaten enough\n"};
	static int	len[2] = {10, 35};

	if (index == 0)
	{
		pthread_mutex_lock(&philo->mutex->write);
		ft_putnbr(timestamp - philo->rules->time_of_start_ms);
		write(1, " ", 1);
		ft_putnbr(philo->id + 1);
		write(1, " ", 1);
	}
	write(1, msg[index], len[index]);
	pthread_mutex_unlock(&philo->mutex->write);
}
