/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 14:07:58 by pramella          #+#    #+#             */
/*   Updated: 2020/05/12 15:44:33 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_exit(t_philo *philo, int index)
{
	static char *msg[2] = {" has died\n", "All philosophers have eaten enough\n"};
	static int	len[2] = {10, 35};

	if (index == 0)
	{
		pthread_mutex_lock(&philo->mutex->write);
		ft_putnbr(get_timestamp_ms() - philo->rules->time_of_start_ms);
		write(1, " ", 1);
		ft_putnbr(philo->id + 1);
		write(1, " ", 1);
	}
	write(1, msg[index], len[index]);
}

void	*ft_monitor(void *ph)
{
	unsigned long last_meal;
	t_philo *philo;

	philo = (t_philo *)ph;
	while (1)
	{
		pthread_mutex_lock(&philo->last_meal);
		last_meal = philo->time_of_last_meal_ms;
		pthread_mutex_unlock(&philo->last_meal);
		if (get_timestamp_ms() - last_meal >
			philo->rules->time_to_die_ms)
		{
			pthread_mutex_lock(&philo->mutex->gblvar);
			g_philo_has_died = 1;
			pthread_mutex_unlock(&philo->mutex->gblvar);
			print_exit(philo, 0);
			return (NULL);
		}
		else if (philo->rules->nbr_of_req_eats > 0)
		{
			pthread_mutex_lock(&philo->mutex->gblvar);
			if (g_philo_have_eaten == philo->rules->nbr_of_philo)
			{
				g_philo_has_died = 1;
				pthread_mutex_unlock(&philo->mutex->gblvar);
				print_exit(NULL, 1);
				return (NULL);
			}
			pthread_mutex_unlock(&philo->mutex->gblvar);
		}
	}
}

//	while (1)
//	{
//		pthread_mutex_lock(&mutex.gblvar);
//		if (g_philo_has_died)
//		{
//			pthread_mutex_unlock(&mutex.gblvar);
//			break ;
//		}
//		if (rules.nbr_of_req_eats > 0 && g_philo_have_eaten == rules.nbr_of_philo)
//		{
//			pthread_mutex_unlock(&mutex.gblvar);
//			pthread_mutex_lock(&mutex.write);
//			write(1, "All philosophers ate enough\n", 28);
//			break ;
//		}
//		pthread_mutex_unlock(&mutex.gblvar);
//		usleep(1000);
//	}
//	usleep(10000);

//int		philo_is_dead(t_philo *philo, int sleep_flag)
//{
//	unsigned long time_diff;
//	unsigned long timestamp_ms;
//
//	timestamp_ms = get_timestamp_ms();
//	timestamp_ms += (sleep_flag) ? philo->rules->time_to_sleep_ms : 0;
//	time_diff = timestamp_ms - philo->time_of_last_meal_ms;
//	if (time_diff >= philo->rules->time_to_die_ms)
//	{
//		(sleep_flag) ? usleep(1000 * (philo->time_of_last_meal_ms +
//					philo->rules->time_to_die_ms - timestamp_ms -
//					philo->rules->time_to_sleep_ms)) : 0;
//		(sleep_flag) ? timestamp_ms = get_timestamp_ms() : 0;
//		pthread_mutex_lock(&philo->mutex->gblvar);
//		if (g_philo_has_died && !pthread_mutex_unlock(&philo->mutex->gblvar))
//			return (1);
//		g_philo_has_died = 1;
//		pthread_mutex_unlock(&philo->mutex->gblvar);
//		pthread_mutex_lock(&philo->mutex->write);
//		ft_putnbr(timestamp_ms - philo->rules->time_of_start_ms);
//		write(1, " ", 1);
//		ft_putnbr(philo->id + 1);
//		write(1, " died\n", 6);
//		return (1);
//	}
//	return (0);
//}
