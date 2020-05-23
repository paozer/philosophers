/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 17:47:09 by pramella          #+#    #+#             */
/*   Updated: 2020/05/21 15:07:37 by pramella         ###   ########lyon.fr   */
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
	philo->meal_counter = 0;
	philo->time_of_last_meal_ms = philo->rules->time_of_start_ms;
	pthread_create(&tid, NULL, monitor_death, philo);
	while (1)
	{
		if (!eat(philo))
			break ;
		if (!print_status(philo, IS_SLEEPING))
			break ;
		usleep(philo->rules->time_to_sleep_us);
		if (!print_status(philo, IS_THINKING))
			break ;
	}
	pthread_join(tid, NULL);
	return (NULL);
}

int		eat(t_philo *philo)
{
	if (!take_forks(philo))
		return (0);
	if (!print_status(philo, IS_EATING))
	{
		pthread_mutex_unlock(&philo->mutex->fork[philo->id]);
		pthread_mutex_unlock(&philo->mutex->fork[philo->next_philo_id]);
		return (0);
	}
	pthread_mutex_lock(&philo->last_meal);
	philo->time_of_last_meal_ms = get_timestamp_ms();
	pthread_mutex_unlock(&philo->last_meal);
	usleep(philo->rules->time_to_eat_us);
	pthread_mutex_unlock(&philo->mutex->fork[philo->id]);
	pthread_mutex_unlock(&philo->mutex->fork[philo->next_philo_id]);
	if (++philo->meal_counter == philo->rules->nbr_of_req_meals)
	{
		pthread_mutex_lock(&philo->mutex->global_satiated);
		++g_philos_satiated;
		pthread_mutex_unlock(&philo->mutex->global_satiated);
	}
	return (1);
}

int		take_forks(t_philo *philo)
{
	int first;
	int second;

	first = (philo->next_philo_id == 0) ? 0 : philo->id;
	second = (philo->next_philo_id == 0) ? philo->id : philo->next_philo_id;
	pthread_mutex_lock(&philo->mutex->fork[first]);
	if (!print_status(philo, TOOK_FORK))
	{
		pthread_mutex_unlock(&philo->mutex->fork[first]);
		return (0);
	}
	pthread_mutex_lock(&philo->mutex->fork[second]);
	if (!print_status(philo, TOOK_FORK))
	{
		pthread_mutex_unlock(&philo->mutex->fork[first]);
		pthread_mutex_unlock(&philo->mutex->fork[second]);
		return (0);
	}
	return (1);
}

int		print_status(t_philo *philo, int index)
{
	static char	*msg[4] = {" has taken a fork\n", " is eating\n",
							" is sleeping\n", " is thinking\n"};
	static int	len[4] = {18, 11, 13, 13};

	pthread_mutex_lock(&philo->mutex->global_died);
	if (g_philo_died)
	{
		pthread_mutex_unlock(&philo->mutex->global_died);
		return (0);
	}
	pthread_mutex_unlock(&philo->mutex->global_died);
	pthread_mutex_lock(&philo->mutex->write);
	ft_putnbr(get_timestamp_ms() - philo->rules->time_of_start_ms);
	write(1, "\t", 1);
	ft_putnbr(philo->id + 1);
	write(1, msg[index], len[index]);
	pthread_mutex_unlock(&philo->mutex->write);
	return (1);
}
