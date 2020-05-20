/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 16:36:54 by pramella          #+#    #+#             */
/*   Updated: 2020/05/20 20:52:18 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		main(int ac, char **av)
{
	t_philo		*philo;
	t_rules		rules;
	t_mutex		mutex;

	if ((ac != 5 && ac != 6))
	{
		write(2, "Usage: ./philo_one [nb_of_philos] [time_to_die] ", 48);
		write(2, "[time_to_eat] [time_to_sleep]\n", 30);
		write(2, "- optional: [nb_of_req_meals]\n- times in ms\n", 44);
		return (1);
	}
	philo = NULL;
	if (!parse(av, &rules) || !init(&philo, &mutex, rules.nbr_of_philo))
		return (1);
	run_simulation(philo, &rules, &mutex);
	return (0);
}

void	run_simulation(t_philo *philo, t_rules *rules, t_mutex *mutex)
{
	int			i;
	pthread_t	tid;

	i = -1;
	while (++i < rules->nbr_of_philo)
	{
		philo[i].id = i;
		philo[i].rules = rules;
		philo[i].mutex = mutex;
	}
	rules->time_of_start_ms = get_timestamp_ms();
	i = -1;
	while (++i < rules->nbr_of_philo)
	{
		pthread_create(&philo[i].tid, NULL, life_cycle, &philo[i]);
		if (i == 0 && rules->nbr_of_req_meals > 0)
			pthread_create(&tid, NULL, monitor_meals, &philo[i]);
		usleep(20);
	}
	(rules->nbr_of_req_meals > 0) ? pthread_join(tid, NULL) : 0;
	i = -1;
	while (++i < rules->nbr_of_philo)
		pthread_join(philo[i].tid, NULL);
	cleanup(philo);
}

void	cleanup(t_philo *philo)
{
	int i;

	i = -1;
	while (++i < philo->rules->nbr_of_philo)
		pthread_mutex_destroy(&philo->mutex->fork[i]);
	pthread_mutex_destroy(&philo->mutex->write);
	pthread_mutex_destroy(&philo->mutex->global_died);
	pthread_mutex_destroy(&philo->mutex->global_satiated);
	free(philo->mutex->fork);
	free(philo);
}
