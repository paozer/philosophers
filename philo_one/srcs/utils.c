/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 16:36:26 by pramella          #+#    #+#             */
/*   Updated: 2020/05/12 12:26:29 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

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
