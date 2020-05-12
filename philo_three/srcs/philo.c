/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 16:37:13 by pramella          #+#    #+#             */
/*   Updated: 2020/05/12 16:04:01 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		main(int ac, char **av)
{
	int			i;
	pthread_t	tid;
	pid_t		*pid;
	t_philo		philo;
	t_rules		rules;
	t_semaphore sem;
	t_mnt_data	mnt_data;

	if ((ac != 5 && ac != 6) || parsing(av, &rules) ||
		init(&pid, &sem, rules.nbr_of_philo))
		return (1);

	set_monitor_data(&mnt_data, rules, sem);
	pthread_create(&tid, NULL, monitor_finished, &mnt_data);
	i = -1;
	rules.time_of_start_ms = get_timestamp_ms();
	while (++i < rules.nbr_of_philo)
	{
		philo.id = i;
		philo.rules = rules;
		philo.meal_counter = 0;
		philo.time_of_last_meal_ms = get_timestamp_ms();
		if ((pid[i] = fork()) == -1)
			write(2, "forking error\n", 14);
		else if (pid[i] == 0)
			life_cycle(&philo);
	}
	sem_wait(sem.simulation_end);
	i = -1;
	while (++i < rules.nbr_of_philo)
		kill(pid[i], SIGKILL);
	sem_unlink("/forks");
	sem_unlink("/write");
	sem_unlink("/simulation_end");
	sem_unlink("/finished_eating");
	free(pid);
	return (0);
}

void	print_message(t_philo *philo, int index)
{
	static char		*message[4] = {" is thinking\n", " has taken a fork\n", " is eating\n", " is sleeping\n"};
	static size_t	len[4] = {13, 18, 11, 13};

	sem_wait(philo->sem.write);
	ft_putnbr(get_timestamp_ms() - philo->rules.time_of_start_ms);
	write(1, "\t", 1);
	ft_putnbr(philo->id + 1);
	write(1, " ", 1);
	write(1, message[index], len[index]);
	sem_post(philo->sem.write);
}

void	*life_cycle(void *ph)
{
	pthread_t	tid;
	t_philo		*philo;
	t_semaphore	*sem;

	philo = (t_philo *)ph;
	sem = &philo->sem;
	sem->forks = sem_open("/forks", 0644);
	sem->write = sem_open("/write", 0644);
	sem->simulation_end = sem_open("/simulation_end", 0644);
	sem->finished_eating = sem_open("/finished_eating", 0644);
	pthread_create(&tid, NULL, monitor_death, philo);
	while (1)
	{
		do_eating(philo);
		do_sleeping(philo);
		do_thinking(philo);
	}
	return (NULL);
}

void	do_eating(t_philo *philo)
{
	t_semaphore	sem;
	t_rules		rules;

	sem = philo->sem;
	rules = philo->rules;
	sem_wait(sem.forks);
	print_message(philo, MSG_FORK);
	sem_wait(sem.forks);
	print_message(philo, MSG_FORK);

	print_message(philo, MSG_EATING);
	philo->time_of_last_meal_ms = get_timestamp_ms();
	usleep(rules.time_to_eat_us);

	sem_post(sem.forks);
	sem_post(sem.forks);

	if (++philo->meal_counter == rules.nbr_of_req_eats)
		sem_post(sem.finished_eating);
}

void	do_thinking(t_philo *philo)
{
	print_message(philo, MSG_THINKING);
}

void	do_sleeping(t_philo *philo)
{
	print_message(philo, MSG_SLEEPING);
	usleep(philo->rules.time_to_sleep_us);
}
