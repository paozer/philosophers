#ifndef PHILO_ONE_H
# define PHILO_ONE_H

#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>
#include "utils.h"
#include "time.h"

#include <stdio.h>

extern int g_philo_has_died;

typedef struct		s_mutex
{
	pthread_mutex_t	*fork;
	pthread_mutex_t	write;
	pthread_mutex_t	read;
}					t_mutex;

/*
** nbr_of_philo = nbr_of_forks
*/

typedef struct		s_rules
{
	int				nbr_of_philo;
	int				nbr_of_req_eats;
	unsigned long	time_to_die_ms;
	unsigned long	time_to_eat_ms;
	unsigned long	time_to_sleep_ms;
	unsigned long	time_to_die_us;
	unsigned long	time_to_eat_us;
	unsigned long	time_to_sleep_us;
	unsigned long	time_of_start_ms;
}					t_rules;

typedef struct		s_philo
{
	int				id;
	unsigned long	time_of_last_meal_ms;
	unsigned long	time_of_start_ms;
	pthread_t		tid;
	pthread_mutex_t eats;
	t_rules			*rules;
	t_mutex			*mutex;
}					t_philo;

int		parsing(char *av[], t_rules *rules);
int		init(t_philo **philo, t_mutex *mutex, int nbr_of_philo);

#endif
