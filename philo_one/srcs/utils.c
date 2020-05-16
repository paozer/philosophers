/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 16:36:26 by pramella          #+#    #+#             */
/*   Updated: 2020/05/16 18:53:25 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int				print_message(t_philo *philo, int index)
{
	static char	*message[4] = {" is thinking\n", " has taken a fork\n",
								" is eating\n", " is sleeping\n"};
	static int	len[4] = {13, 18, 11, 13};

	pthread_mutex_lock(&philo->mutex->gblvar);
	if (g_philo_has_died)
	{
		pthread_mutex_unlock(&philo->mutex->gblvar);
		return (0);
	}
	pthread_mutex_unlock(&philo->mutex->gblvar);
	pthread_mutex_lock(&philo->mutex->write);
	ft_putnbr(get_timestamp_ms() - philo->rules->time_of_start_ms);
	write(1, " ", 1);
	ft_putnbr(philo->id + 1);
	write(1, " ", 1);
	write(1, message[index], len[index]);
	pthread_mutex_unlock(&philo->mutex->write);
	return (1);
}

size_t			ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	while (s[i])
		++i;
	return (i);
}

unsigned long	ft_atol(const char *str)
{
	size_t			i;
	unsigned long	nb;

	i = 0;
	nb = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 - (str[i] - '0');
		++i;
	}
	return (-nb);
}

int				ft_atoi(const char *str)
{
	size_t	i;
	int		nb;

	i = 0;
	nb = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 - (str[i] - '0');
		++i;
	}
	if (str[i])
		return (-1);
	return (-nb);
}

void			ft_putnbr(unsigned long n)
{
	char c;

	if (n > 9)
		ft_putnbr(n / 10);
	c = n % 10 + '0';
	write(1, &c, 1);
}
