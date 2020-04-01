#ifndef PHILO_ONE_H
# define PHILO_ONE_H

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "utils.h"

#include <stdio.h>

typedef struct		s_data
{
	pthread_t		*thread;
	pthread_mutex_t	*mutex;
	int				nbr_of_philo;
	int				nbr_of_forks;
	int				nbr_of_req_eats;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				exit_flag;
}					t_data;

typedef struct		s_philo
{
	int				id;
	int				time_since_last_meal;
	t_data			*data;
}					t_philo;

#endif
