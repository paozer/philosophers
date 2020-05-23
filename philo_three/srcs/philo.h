/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 16:36:50 by pramella          #+#    #+#             */
/*   Updated: 2020/05/23 16:08:33 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <sys/time.h>
# include <pthread.h>
# include <semaphore.h>

# define MSG_THINKING 0
# define MSG_FORK 1
# define MSG_EATING 2
# define MSG_SLEEPING 3

int g_philo_died;
int g_philos_satiated;

typedef struct		s_semaphore
{
	sem_t			*fork;
	sem_t			*write;
	sem_t			*finished_meals;
	sem_t			*simulation_end;
}					t_semaphore;

typedef struct		s_rules
{
	int				nbr_of_philo;
	int				nbr_of_req_meals;
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
	pthread_t		tid;
	int				id;
	int				next_philo_id;
	int				meal_counter;
	unsigned long	time_of_last_meal_ms;
	sem_t			*last_meal;
	t_rules			*rules;
	t_semaphore		*sem;
}					t_philo;

/*
** MAIN
*/
void				run_simulation(t_rules *rules, t_semaphore *sem);
void				cleanup(pid_t *pid, pid_t pid2,
					t_rules *rules, t_semaphore *sem);

/*
** LIFE_CYCLE
*/
void				life_cycle(t_philo *philo);
void				eat(t_philo *philo);
void				print_status(t_philo *philo, int index);

/*
** MONITOR
*/
void				*monitor_death(void *ph);
void				*monitor_meals(void *ph);
void				print_exit(t_philo *philo, int index, unsigned long ts);

/*
** SETUP
*/
int					parse(char *av[], t_rules *rules);
int					init(t_philo **philo, t_semaphore *sem, int nbr_of_philo);
int					valid_arguments(char *av[]);
int					unlink_semaphores(int nbr_of_philo);
char				*get_sem_name(char *basename, int added_index);

/*
** UTILS
*/
unsigned long		get_timestamp_ms(void);
size_t				ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t				ft_strlen(const char *s);
unsigned long		ft_atol(const char *str);
void				ft_putnbr(unsigned long n);

/*
** ITOA
*/
char				*ft_itoa(int n);

#endif
