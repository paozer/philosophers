/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 16:36:54 by pramella          #+#    #+#             */
/*   Updated: 2020/05/23 15:54:37 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		main(int ac, char **av)
{
	t_philo		*philo;
	t_rules		rules;
	t_semaphore	sem;

	if ((ac != 5 && ac != 6))
	{
		write(2, "Usage: ./philo_one [nb_of_philos] [time_to_die] ", 48);
		write(2, "[time_to_eat] [time_to_sleep]\n", 30);
		write(2, "- optional: [nb_of_req_meals]\n- times in ms\n", 44);
		return (1);
	}
	philo = NULL;
	if (!parse(av, &rules) || !init(&philo, &sem, rules.nbr_of_philo))
		return (1);
	run_simulation(&rules, &sem);
	return (0);
}

void	run_simulation(t_rules *rules, t_semaphore *sem)
{
	int			i;
	pid_t		*pid;
	pid_t		pid2;
	t_philo		philo;

	if (!(pid = malloc(sizeof(*pid) * rules->nbr_of_philo)))
		return ;
	rules->time_of_start_ms = get_timestamp_ms();
	i = -1;
	while (++i < rules->nbr_of_philo)
	{
		philo.id = i;
		philo.rules = rules;
		philo.sem = sem;
		(i == 0 && rules->nbr_of_req_meals > 0) ? (pid2 = fork()) : 0;
		(i == 0 && rules->nbr_of_req_meals > 0 && pid2 == 0) ?
			monitor_meals(&philo) : 0;
		if ((pid[i] = fork()) == 0)
		{
			free(pid);
			life_cycle(&philo);
		}
	}
	cleanup(pid, pid2, rules, sem);
}

void	cleanup(pid_t *pid, pid_t pid2, t_rules *rules, t_semaphore *sem)
{
	int i;

	sem_wait(sem->simulation_end);
	unlink_semaphores(rules->nbr_of_philo);
	i = -1;
	while (++i < rules->nbr_of_philo)
		kill(pid[i], SIGKILL);
	(rules->nbr_of_req_meals > 0) ? kill(pid2, SIGKILL) : 0;
	unlink_semaphores(rules->nbr_of_philo);
	free(pid);
}
