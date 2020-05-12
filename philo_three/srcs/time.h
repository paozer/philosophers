/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 16:37:23 by pramella          #+#    #+#             */
/*   Updated: 2020/05/09 16:37:24 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIME_H
# define TIME_H

#include <sys/time.h>

unsigned long	get_timestamp_us(void);
unsigned long	get_timestamp_ms(void);

#endif
