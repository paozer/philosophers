#ifndef PHILO_THREE_H
# define PHILO_THREE_H

#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include "utils.h"
#include "time.h"

# define MSG_THINKING 0
# define MSG_FORK 1
# define MSG_EATING 2
# define MSG_SLEEPING 3

typedef struct		s_semaphore
{
	sem_t			*forks;
	sem_t			*write;
	sem_t			*simulation_end;
	sem_t			*finished_eating;
}					t_semaphore;

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
	int				meal_counter;
	unsigned long	time_of_last_meal_ms;
	t_rules			rules;
	t_semaphore		sem;
}					t_philo;

typedef struct		s_mnt_data
{
	int				nbr_of_philo;
	int				nbr_of_req_eats;
	t_rules			rules;
	t_semaphore		sem;
}					t_mnt_data;

int					parsing(char *av[], t_rules *rules);
int					init(pid_t **pid, t_semaphore *sem, int nbr_of_philo);
void				set_monitor_data(t_mnt_data *mnt_data, t_rules rules, t_semaphore sem);

#endif
