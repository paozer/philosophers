#ifndef PHILO_ONE_H
# define PHILO_ONE_H

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include "utils.h"

#include <stdio.h>

typedef struct		s_data
{
	pthread_t		*thread;
	pthread_mutex_t	*mx_fork;
	pthread_mutex_t	mx_write;
	pthread_mutex_t	mx_exit;
	int				nbr_of_philo;
	int				nbr_of_forks;
	int				nbr_of_req_eats;
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	unsigned long	start_timestamp;
	int				exit_flag;
}					t_data;

typedef struct		s_philo
{
	int				id;
	int				id_clean;
	unsigned long	time_of_last_meal;
	t_data			*data;
}					t_philo;

#endif
