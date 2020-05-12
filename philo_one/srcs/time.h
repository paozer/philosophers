/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 16:36:40 by pramella          #+#    #+#             */
/*   Updated: 2020/05/10 14:08:17 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIME_H
# define TIME_H

#include <sys/time.h>

unsigned long	get_timestamp_us(void);
unsigned long	get_timestamp_ms(void);

#endif
