/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 10:55:37 by pramella          #+#    #+#             */
/*   Updated: 2020/05/21 14:31:50 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*life_cycle(void *ph)
{
	pthread_t	tid;
	t_philo		*philo;

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
		sem_post(philo->sem->fork);
		sem_post(philo->sem->fork);
		return (0);
	}
	sem_wait(philo->last_meal);
	philo->time_of_last_meal_ms = get_timestamp_ms();
	sem_post(philo->last_meal);
	usleep(philo->rules->time_to_eat_us);
	sem_post(philo->sem->fork);
	sem_post(philo->sem->fork);
	if (++philo->meal_counter == philo->rules->nbr_of_req_meals)
	{
		sem_wait(philo->sem->global_satiated);
		++g_philos_satiated;
		sem_post(philo->sem->global_satiated);
	}
	return (1);
}

int		take_forks(t_philo *philo)
{
	sem_wait(philo->sem->fork);
	if (!print_status(philo, TOOK_FORK))
	{
		sem_post(philo->sem->fork);
		return (0);
	}
	sem_wait(philo->sem->fork);
	if (!print_status(philo, TOOK_FORK))
	{
		sem_post(philo->sem->fork);
		sem_post(philo->sem->fork);
		return (0);
	}
	return (1);
}

int		print_status(t_philo *philo, int index)
{
	static char	*msg[4] = {" has taken a fork\n", " is eating\n",
							" is sleeping\n", " is thinking\n"};
	static int	len[4] = {18, 11, 13, 13};

	sem_wait(philo->sem->global_died);
	if (g_philo_died)
	{
		sem_post(philo->sem->global_died);
		return (0);
	}
	sem_wait(philo->sem->write);
	sem_post(philo->sem->global_died);
	ft_putnbr(get_timestamp_ms() - philo->rules->time_of_start_ms);
	write(1, "\t", 1);
	ft_putnbr(philo->id + 1);
	write(1, msg[index], len[index]);
	sem_post(philo->sem->write);
	return (1);
}
