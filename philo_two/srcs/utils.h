/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 16:37:56 by pramella          #+#    #+#             */
/*   Updated: 2020/05/09 16:37:56 by pramella         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

#include <stddef.h>
#include <unistd.h>

unsigned long	ft_atol(const char *str);
int				ft_atoi(const char *str);
size_t			ft_strlen(const char *s);
void			ft_putnbr(unsigned long n);

#endif
