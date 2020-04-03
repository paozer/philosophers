#include "philo_one.h"

int g_philo_has_died;

void	philo_is_dead(t_philo *philo)
{
	unsigned long time_diff;
	unsigned long timestamp_ms;

	timestamp_ms = get_timestamp_ms();
	time_diff = timestamp_ms - philo->time_of_last_meal_ms;
	if (time_diff >= philo->rules->time_to_die_ms)
	{
		pthread_mutex_lock(&philo->mutex->write);
		pthread_mutex_lock(&philo->mutex->read);
		g_philo_has_died = 1;
		pthread_mutex_unlock(&philo->mutex->read);
		ft_putnbr(timestamp_ms - philo->rules->time_of_start_ms);
		write(1, " ", 1);
		ft_putnbr(philo->id + 1);
		write(1, " died\n", 6);
	}
}

// if action is sleeping check if sleeping will take to long
void	print_message(t_philo *philo, char *message)
{
	philo_is_dead(philo);
	pthread_mutex_lock(&philo->mutex->write);
	ft_putnbr(get_timestamp_ms() - philo->rules->time_of_start_ms);
	write(1, " ", 1);
	ft_putnbr(philo->id + 1);
	write(1, message, ft_strlen(message));
	pthread_mutex_unlock(&philo->mutex->write);
}

void	do_eating(t_philo *philo)
{
	int			i;

	i = (philo->id == philo->rules->nbr_of_philo - 1) ? 0 : philo->id + 1;
	pthread_mutex_lock(&philo->mutex->fork[philo->id]);
	print_message(philo, " has taken a fork\n");
	pthread_mutex_lock(&philo->mutex->fork[i]);
	print_message(philo, " has taken a fork\n");
	// need to check duration of eating ?
//	pthread_mutex_lock(&philo->eats);
	print_message(philo, " is eating\n");
	philo->time_of_last_meal_ms = get_timestamp_ms();
	usleep(philo->rules->time_to_eat_us);
//	pthread_mutex_unlock(&philo->eats);
	// in this block
	pthread_mutex_unlock(&philo->mutex->fork[philo->id]);
	pthread_mutex_unlock(&philo->mutex->fork[i]);
}

void	do_thinking(t_philo *philo)
{
	print_message(philo, " is thinking\n");
}

void	do_sleeping(t_philo *philo)
{
	print_message(philo, " is sleeping\n");
	usleep(philo->rules->time_to_sleep_us);
}

void	*life_cycle(void *ph)
{
	t_philo *philo;

	philo = (t_philo *)ph;
	while (1)
	{
		do_thinking(philo);
		do_eating(philo);
		do_sleeping(philo);
	}
	return (NULL);
}

int		main(int ac, char **av)
{
	int			i;
	t_philo		*philo;
	t_rules		rules;
	t_mutex		mutex;

	philo = NULL;
	if ((ac != 5 && ac != 6) || parsing(av, &rules) ||
		init(&philo, &mutex, rules.nbr_of_philo))
		return (1);
	i = 0;
	rules.time_of_start_ms = get_timestamp_ms();
	while (i < rules.nbr_of_philo)
	{
		philo[i].id = i;
		philo[i].rules = &rules;
		philo[i].mutex = &mutex;
		philo[i].time_of_last_meal_ms = get_timestamp_ms();
		pthread_create(&philo[i].tid, NULL, life_cycle, &philo[i]);
		pthread_detach(philo[i].tid);
		++i;
	}
	while (!pthread_mutex_lock(&mutex.read) && !g_philo_has_died)
	{
		pthread_mutex_unlock(&mutex.read);
		usleep(1000);
	}
	pthread_mutex_destroy(&mutex.write);
	pthread_mutex_destroy(&mutex.read);
	i = -1;
	while (++i < rules.nbr_of_philo)
		pthread_mutex_destroy(&mutex.fork[i]);
	free(mutex.fork);
	free(philo);
	return (0);
}
