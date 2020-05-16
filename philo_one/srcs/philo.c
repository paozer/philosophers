/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 16:36:54 by pramella          #+#    #+#             */
/*   Updated: 2020/05/16 18:59:52 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		main(int ac, char **av)
{
	int			i;
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
	}
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

	philo = (t_philo *)ph;
	philo->next_philo_id = (philo->id == philo->rules->nbr_of_philo - 1) ? 0 : philo->id + 1;
	philo->time_of_last_meal_ms = get_timestamp_ms();
	pthread_create(&tid, NULL, ft_monitor, philo);
	while (1)
	{
		if (!do_thinking(philo) || !do_eating(philo) || !do_sleeping(philo))
			break ;
	}
	pthread_join(tid, NULL);
	return (NULL);
}

int		do_eating(t_philo *philo)
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

int		do_thinking(t_philo *philo)
{
	return (print_message(philo, MSG_THINKING));
}

int		do_sleeping(t_philo *philo)
{
	if (!print_message(philo, MSG_SLEEPING))
		return (0);
	usleep(philo->rules->time_to_sleep_us);
	return (1);
}

int		take_forks(t_philo *philo)
{
	int		i;
	int		fork_count;

	i = philo->id;
	fork_count = 0;
	while (fork_count < 2)
	{
		pthread_mutex_lock(&philo->mutex->gblvar);
		if (g_philo_has_died)
		{
			pthread_mutex_unlock(&philo->mutex->gblvar);
			return (0);
		}
		pthread_mutex_unlock(&philo->mutex->gblvar);
		pthread_mutex_lock(&philo->mutex->fork_lookup[i]);
		if (!philo->mutex->fork_is_taken[i])
		{
			philo->mutex->fork_is_taken[i] = 1;
			pthread_mutex_unlock(&philo->mutex->fork_lookup[i]);
			pthread_mutex_lock(&philo->mutex->fork[i]);
			++fork_count;
			if (!print_message(philo, MSG_FORK))
				return (0);
		}
		pthread_mutex_unlock(&philo->mutex->fork_lookup[i]);
		i = (i == philo->id) ? philo->next_philo_id : philo->id;
	}
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
