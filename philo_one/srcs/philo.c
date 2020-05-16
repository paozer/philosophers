/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 16:36:54 by pramella          #+#    #+#             */
/*   Updated: 2020/05/17 01:51:50 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		main(int ac, char **av)
{
	int			i;
	pthread_t	tid;
	t_philo		*philo;
	t_rules		rules;
	t_mutex		mutex;

	philo = NULL;
	if ((ac != 5 && ac != 6) || parsing(av, &rules) ||
		init(&philo, &mutex, rules.nbr_of_philo))
		return (1);
	i = -1;
	rules.time_of_start_ms = get_timestamp_ms();
	while (++i < rules.nbr_of_philo)
	{
		philo[i].id = i;
		philo[i].rules = &rules;
		philo[i].mutex = &mutex;
		philo[i].meal_counter = 0;
		pthread_create(&philo[i].tid, NULL, life_cycle, &philo[i]);
		if (i == 0 && rules.nbr_of_req_eats > 0)
			pthread_create(&tid, NULL, monitor_finished, &philo[i]);
		usleep(100);
	}
	(rules.nbr_of_req_eats > 0) ? pthread_join(tid, NULL) : 0;
	i = -1;
	while (++i < rules.nbr_of_philo)
		pthread_join(philo[i].tid, NULL);
	i = -1;
	while (++i < rules.nbr_of_philo)
	{
		pthread_mutex_destroy(&mutex.fork[i]);
		pthread_mutex_destroy(&mutex.fork_lookup[i]);
	}
	pthread_mutex_destroy(&mutex.write);
	pthread_mutex_destroy(&mutex.gblvar);
	free(mutex.fork);
	free(philo);
	return (0);
}

void	*life_cycle(void *ph)
{
	t_philo		*philo;
	pthread_t	tid;

	philo = ph;
	philo->next_philo_id = (philo->id == philo->rules->nbr_of_philo - 1) ? 0 : philo->id + 1;
	philo->time_of_last_meal_ms = get_timestamp_ms();
	pthread_create(&tid, NULL, monitor_death, philo);
	while (1)
	{
		if (!eat(philo))
			break ;
		if (!print_message(philo, MSG_SLEEPING))
			break ;
		else
			usleep(philo->rules->time_to_sleep_us);
		if (!print_message(philo, MSG_THINKING))
			break ;
	}
	pthread_join(tid, NULL);
	return (NULL);
}

int		eat(t_philo *philo)
{
	if (!take_forks(philo) || !print_message(philo, MSG_EATING))
		return (0);
	pthread_mutex_lock(&philo->eating);
	if (++philo->meal_counter == philo->rules->nbr_of_req_eats)
	{
		pthread_mutex_lock(&philo->mutex->gblvar);
		++g_philo_have_eaten;
		pthread_mutex_unlock(&philo->mutex->gblvar);
	}
	usleep(philo->rules->time_to_eat_us);
	philo->time_of_last_meal_ms = get_timestamp_ms();
	pthread_mutex_unlock(&philo->eating);
	put_forks(philo);
	return (1);
}

int		take_forks(t_philo *philo)
{
	if (philo->next_philo_id == 0)
		pthread_mutex_lock(&philo->mutex->fork[0]);
	else
		pthread_mutex_lock(&philo->mutex->fork[philo->id]);
	if (!print_message(philo, MSG_FORK))
		return (0);
	if (philo->next_philo_id == 0)
		pthread_mutex_lock(&philo->mutex->fork[philo->id]);
	else
		pthread_mutex_lock(&philo->mutex->fork[philo->next_philo_id]);
	if (!print_message(philo, MSG_FORK))
		return (0);
	return (1);
}

void	put_forks(t_philo *philo)
{
	t_mutex *mx;

	mx = philo->mutex;
	pthread_mutex_unlock(&mx->fork[philo->id]);
	pthread_mutex_lock(&mx->fork_lookup[philo->id]);
	mx->fork_is_taken[philo->id] = 0;
	pthread_mutex_unlock(&mx->fork_lookup[philo->id]);
	pthread_mutex_unlock(&mx->fork[philo->next_philo_id]);
	pthread_mutex_lock(&mx->fork_lookup[philo->next_philo_id]);
	mx->fork_is_taken[philo->next_philo_id] = 0;
	pthread_mutex_unlock(&mx->fork_lookup[philo->next_philo_id]);
}
