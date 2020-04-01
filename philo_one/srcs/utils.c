#include "philo_one.h"

size_t	ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	while (s[i])
		++i;
	return (i);
}

int		ft_atoi(const char *str)
{
	size_t	i;
	int		sign;
	int		nb;

	i = 0;
	sign = -1;
	nb = 0;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || str[i] == '\f'
		|| str[i] == '\r' || str[i] == ' ')
		++i;
	if (str[i] == '+')
		++i;
	else if (str[i] == '-')
	{
		sign *= -1;
		++i;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 - (str[i] - '0');
		++i;
	}
	return (sign * nb);
}

void	ft_putnbr(int n)
{
	long ln;
	char c;

	ln = n;
	if (ln < 0)
	{
		ln *= -1;
		write(1, "-", 1);
	}
	if (ln > 9)
		ft_putnbr(ln / 10);
	c = ln % 10 + '0';
	write(1, &c, 1);
}
