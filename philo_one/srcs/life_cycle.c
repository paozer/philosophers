/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 10:55:37 by pramella          #+#    #+#             */
/*   Updated: 2020/05/17 15:25:39 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*life_cycle(void *ph)
{
	t_philo		*philo;
	pthread_t	tid;

	philo = ph;
	philo->next_philo_id = (philo->id == philo->rules->nbr_of_philo - 1) ?
                            0 : philo->id + 1;
	philo->time_of_last_meal_ms = get_timestamp_ms();
	pthread_create(&tid, NULL, monitor_death, philo);
	while (1)
	{
		if (!eat(philo))
			break ;
		if (!print_message(philo, IS_SLEEPING))
			break ;
		usleep(philo->rules->time_to_sleep_us);
		if (!print_message(philo, IS_THINKING))
			break ;
	}
	pthread_join(tid, NULL);
	return (NULL);
}

int		eat(t_philo *philo)
{
	if (!take_forks(philo) || !print_message(philo, IS_EATING))
		return (0);
	pthread_mutex_lock(&philo->is_eating);
	philo->time_of_last_meal_ms = get_timestamp_ms();
	usleep(philo->rules->time_to_eat_us);
	if (++philo->meal_counter == philo->rules->nbr_of_req_meals)
	{
		pthread_mutex_lock(&philo->mutex->global_finished);
		++g_philos_have_eaten_enough;
		pthread_mutex_unlock(&philo->mutex->global_finished);
	}
	pthread_mutex_unlock(&philo->is_eating);
	pthread_mutex_unlock(&philo->mutex->fork[philo->id]);
	pthread_mutex_unlock(&philo->mutex->fork[philo->next_philo_id]);
	return (1);
}

int		take_forks(t_philo *philo)
{
	int first;
	int second;

	first = (philo->next_philo_id == 0) ? 0 : philo->id;
	second = (philo->next_philo_id == 0) ? philo->id : philo->next_philo_id;
	pthread_mutex_lock(&philo->mutex->fork[first]);
	if (!print_message(philo, TOOK_FORK))
	{
		pthread_mutex_unlock(&philo->mutex->fork[first]);
		return (0);
	}
	pthread_mutex_lock(&philo->mutex->fork[second]);
	if (!print_message(philo, TOOK_FORK))
	{
		pthread_mutex_unlock(&philo->mutex->fork[first]);
		pthread_mutex_unlock(&philo->mutex->fork[second]);
		return (0);
	}
	return (1);
}
