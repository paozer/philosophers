/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 16:36:50 by pramella          #+#    #+#             */
/*   Updated: 2020/05/17 12:12:09 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stddef.h>
# include <pthread.h>
# include <sys/time.h>

# define TOOK_FORK 0
# define IS_EATING 1
# define IS_SLEEPING 2
# define IS_THINKING 3

int g_philo_has_died;
int g_philo_have_eaten;

typedef struct		s_mutex
{
	pthread_mutex_t	*fork;
	pthread_mutex_t	write;
	pthread_mutex_t	gblvar_death;
	pthread_mutex_t	gblvar_finished;
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
	int				meal_counter;
	unsigned long	time_of_last_meal_ms;
	t_rules			*rules;
	t_mutex			*mutex;
	pthread_mutex_t	eating;
	pthread_t		tid;
}					t_philo;

// MONITOR
void				*print_exit(t_philo *philo, int index,
					unsigned long timestamp);
void				*monitor_death(void *ph);
void				*monitor_finished(void *ph);

// PHILO
int					take_forks(t_philo *philo);
void				put_forks(t_philo *philo);
int					eat(t_philo *philo);
int					do_thinking(t_philo *philo);
int					do_sleeping(t_philo *philo);
void				*life_cycle(void *ph);

// SETUP
int					parsing(char *av[], t_rules *rules);
int					init(t_philo **philo, t_mutex *mutex, int nbr_of_philo);

// TIME
unsigned long		get_timestamp_us(void);
unsigned long		get_timestamp_ms(void);

// UTILS
int					print_message(t_philo *philo, int index);
size_t				ft_strlen(const char *s);
unsigned long		ft_atol(const char *str);
int					ft_atoi(const char *str);
void				ft_putnbr(unsigned long n);

#endif
