/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 10:55:37 by pramella          #+#    #+#             */
/*   Updated: 2020/05/23 15:44:37 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	life_cycle(t_philo *philo)
{
	pthread_t	tid;
	char		*str;

	philo->next_philo_id = (philo->id == philo->rules->nbr_of_philo - 1) ?
							0 : philo->id + 1;
	philo->meal_counter = 0;
	philo->sem->fork = sem_open("/fork", 0644);
	philo->sem->write = sem_open("/write", 0644);
	philo->sem->finished_meals = sem_open("/finished_meals", 0644);
	philo->sem->simulation_end = sem_open("/simulation_end", 0644);
	str = get_sem_name("/last_meal", philo->id);
	philo->last_meal = sem_open(str, 0644);
	free(str);
	philo->time_of_last_meal_ms = philo->rules->time_of_start_ms;
	// philo->time_of_last_meal_ms = get_timestamp_ms();
	pthread_create(&tid, NULL, monitor_death, philo);
	while (1)
	{
		eat(philo);
		print_status(philo, IS_SLEEPING);
		usleep(philo->rules->time_to_sleep_us);
		print_status(philo, IS_THINKING);
	}
}

void	eat(t_philo *philo)
{
	sem_wait(philo->sem->fork);
	print_status(philo, TOOK_FORK);
	sem_wait(philo->sem->fork);
	print_status(philo, TOOK_FORK);
	sem_wait(philo->last_meal);
	philo->time_of_last_meal_ms = get_timestamp_ms();
	sem_post(philo->last_meal);
	usleep(philo->rules->time_to_eat_us);
	sem_post(philo->sem->fork);
	sem_post(philo->sem->fork);
	if (++philo->meal_counter == philo->rules->nbr_of_req_meals)
		sem_post(philo->sem->finished_meals);
}

void	print_status(t_philo *philo, int index)
{
	static char	*msg[4] = {" has taken a fork\n", " is eating\n",
							" is sleeping\n", " is thinking\n"};
	static int	len[4] = {18, 11, 13, 13};

	sem_wait(philo->sem->write);
	ft_putnbr(get_timestamp_ms() - philo->rules->time_of_start_ms);
	write(1, "\t", 1);
	ft_putnbr(philo->id + 1);
	write(1, msg[index], len[index]);
	sem_post(philo->sem->write);
}
