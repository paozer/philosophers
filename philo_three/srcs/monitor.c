/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 15:59:00 by pramella          #+#    #+#             */
/*   Updated: 2020/05/12 16:00:53 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_death(void *ph)
{
	t_philo			*philo;
	unsigned long	timestamp_ms;

	philo = (t_philo *)ph;
	while (1)
	{
		sem_wait(philo->sem.write);
		timestamp_ms = get_timestamp_ms();
		if (timestamp_ms - philo->time_of_last_meal_ms >=
			philo->rules.time_to_die_ms)
		{
			sem_post(philo->sem.simulation_end);
			ft_putnbr(timestamp_ms - philo->rules.time_of_start_ms);
			write(1, "\t", 1);
			ft_putnbr(philo->id + 1);
			write(1, " died\n", 6);
			return (NULL);
		}
		sem_post(philo->sem.write);
		usleep(100);
	}
	return (NULL);
}

void	*monitor_finished(void *data)
{
	int			count;
	t_mnt_data	*mnt_data;
	t_semaphore	sem;

	mnt_data = (t_mnt_data *)data;
	sem = mnt_data->sem;
	if (mnt_data->nbr_of_req_eats < 1)
		return (NULL);
	count = 0;
	while (1)
	{
		sem_wait(sem.finished_eating);
		sem_post(sem.finished_eating);
		if (++count == mnt_data->nbr_of_philo)
		{
			sem_wait(sem.write);
			sem_post(sem.simulation_end);
			write(1, "All philosophers ate enough\n", 28);
			return (NULL);
		}
	}
	return (NULL);
}