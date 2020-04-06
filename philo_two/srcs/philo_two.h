#ifndef PHILO_TWO_H
# define PHILO_TWO_H

#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>

#include <string.h>
#include <sys/errno.h>

#include <pthread.h>
#include <semaphore.h>
#include "utils.h"
#include "time.h"

# define MSG_THINKING 0
# define MSG_FORK 1
# define MSG_EATING 2
# define MSG_SLEEPING 3

extern int g_philo_has_died_flag;
extern int g_philo_have_eaten_counter;

typedef struct		s_sem
{
	sem_t			*forks;
	sem_t			*write;
	sem_t			*read;
}					t_sem;

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
	pthread_t		tid;
	int				meal_counter;
	unsigned long	time_of_last_meal_ms;
	t_rules			*rules;
	t_sem			*semaphore;
}					t_philo;

int					parsing(char *av[], t_rules *rules);
int					init(t_philo **philo, t_sem *semaphore, int nbr_of_philo);

#endif
