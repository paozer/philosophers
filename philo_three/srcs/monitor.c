/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 14:07:58 by pramella          #+#    #+#             */
/*   Updated: 2020/05/23 15:34:50 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** monitoring thread created by main to monitor if all the philosophers have
** eaten enough meals
*/

void	*monitor_meals(void *ph)
{
	t_philo			*philo;
	int				count;

	philo = ph;
	count = 0;
	while (1)
	{
		sem_wait(philo->sem->finished_meals);
		if (++count >= philo->rules->nbr_of_philo)
		{
			print_exit(ph, ARE_SATIATED, 1);
			exit(0);
		}
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
		sem_wait(philo->last_meal);
		if ((timestamp = get_timestamp_ms()) - philo->time_of_last_meal_ms >
			philo->rules->time_to_die_ms)
		{
			sem_post(philo->last_meal);
			print_exit(ph, HAS_DIED, timestamp);
			return (NULL);
		}
		sem_post(philo->last_meal);
		usleep(10);
	}
}

void	print_exit(t_philo *philo, int index, unsigned long timestamp)
{
	static char	*msg[2] = {" has died\n", "All philosophers ate enough\n"};
	static int	len[2] = {10, 28};

	sem_wait(philo->sem->write);
	if (index == HAS_DIED)
	{
		ft_putnbr(timestamp - philo->rules->time_of_start_ms);
		write(1, "\t", 1);
		ft_putnbr(philo->id + 1);
	}
	write(1, msg[index], len[index]);
	sem_post(philo->sem->simulation_end);
}
