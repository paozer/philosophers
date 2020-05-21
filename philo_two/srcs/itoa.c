/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 14:57:39 by pramella          #+#    #+#             */
/*   Updated: 2020/05/21 12:25:42 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static size_t	ft_ncharsize(long n)
{
	size_t i;

	i = 0;
	while (n)
	{
		++i;
		n /= 10;
	}
	return (i + 1);
}

static void		ft_rec_itoa(long nb, char *str)
{
	char	c;
	size_t	i;

	if (nb > 9)
		ft_rec_itoa(nb / 10, str);
	i = ft_strlen(str);
	c = (nb % 10) + '0';
	str[i] = c;
}

char			*ft_itoa(int n)
{
	long	nb;
	size_t	size;
	char	*str;

	nb = n;
	size = ft_ncharsize(nb);
	if (!(str = malloc(sizeof(char) * (size + 1))))
		return (NULL);
	memset(str, '\0', size + 1);
	ft_rec_itoa(nb, str);
	return (str);
}
