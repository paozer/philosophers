/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 16:36:54 by pramella          #+#    #+#             */
/*   Updated: 2020/05/17 15:13:30 by pramella         ###   ########lyon.fr   */
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
		if (i == 0 && rules.nbr_of_req_meals > 0)
			pthread_create(&tid, NULL, monitor_meals, &philo[i]);
		usleep(100);
	}
	(rules.nbr_of_req_meals > 0) ? pthread_join(tid, NULL) : 0;
	i = -1;
	while (++i < rules.nbr_of_philo)
		pthread_join(philo[i].tid, NULL);
	i = -1;
	while (++i < rules.nbr_of_philo)
		pthread_mutex_destroy(&mutex.fork[i]);
	pthread_mutex_destroy(&mutex.write);
	pthread_mutex_destroy(&mutex.global_death);
	pthread_mutex_destroy(&mutex.global_finished);
	free(mutex.fork);
	free(philo);
	return (0);
}
