/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 16:37:21 by pramella          #+#    #+#             */
/*   Updated: 2020/05/09 16:37:21 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "time.h"

unsigned long	get_timestamp_us(void)
{
	struct timeval tv;

	gettimeofday(&tv, 0);
	return (1000000 * tv.tv_sec + tv.tv_usec);
}

unsigned long	get_timestamp_ms(void)
{
	struct timeval tv;

	gettimeofday(&tv, 0);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
