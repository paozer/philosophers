/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 16:36:50 by pramella          #+#    #+#             */
/*   Updated: 2020/05/12 15:43:07 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>
#include "monitor.h"
#include "utils.h"
#include "time.h"

# define MSG_THINKING 0
# define MSG_FORK 1
# define MSG_EATING 2
# define MSG_SLEEPING 3

extern int g_philo_has_died;
extern int g_philo_have_eaten;

typedef struct		s_mutex
{
	pthread_mutex_t	*fork;

	int				*fork_is_taken;
	pthread_mutex_t *fork_lookup;

	pthread_mutex_t	write;
	pthread_mutex_t	gblvar;
}					t_mutex;

typedef struct		s_rules
{
	int				nbr_of_philo;
	int				nbr_of_req_eats;
	unsigned long	time_of_start_ms;
	unsigned long	time_to_die_ms;
	unsigned long	time_to_eat_ms;
	unsigned long	time_to_sleep_ms;
	unsigned long	time_to_die_us;
	unsigned long	time_to_eat_us;
	unsigned long	time_to_sleep_us;
}					t_rules;

typedef struct		s_philo
{
	int				id;
	int				next_philo_id;
	pthread_t		tid;
	int				meal_counter;
	unsigned long	time_of_last_meal_ms;
	t_rules			*rules;
	t_mutex			*mutex;
	pthread_mutex_t	last_meal;
}					t_philo;

int					parsing(char *av[], t_rules *rules);
int					init(t_philo **philo, t_mutex *mutex, int nbr_of_philo);
void				*ft_monitor(void *ph);

#endif
