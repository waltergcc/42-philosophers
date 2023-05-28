/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 12:44:04 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/05/28 20:33:00 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	advance_time(t_philo *philo, long long stop)
{
	long long	now;

	now = get_time();
	while (!is_time_to_finish(philo, NO) && (get_time() - now) < stop)
		usleep(100);
}

void	print_action(t_philo *philo, const char *status)
{
	long long	now;

	pthread_mutex_lock(&philo->table->print_padlock);
	if (!is_time_to_finish(philo, NO))
	{
		now = get_time() - philo->table->start_time;
		printf("%lld %d %s\n", now, philo->id, status);
	}
	pthread_mutex_unlock(&philo->table->print_padlock);
	if (status[0] == 'f')
		printf("%s\n", FINISH_MSG);
}

long long	get_time(void)
{
	struct timeval	timeval;

	gettimeofday(&timeval, NULL);
	return ((timeval.tv_sec * 1000) + (timeval.tv_usec / 1000));
}

void	exit_error(char *msg, t_table *table, int n)
{
	printf("Error: %s\n", msg);
	if (n == 1)
		clean_table(table);
	if (n == 2)
		destroy_padlocks(table);
	if (n == 3)
		finish_dinner(table);
	exit(1);
}

int	ft_atoi(const char *s)
{
	int	r;
	int	sg;

	r = 0;
	sg = 1;
	if (*s == '+' || *s == '-')
	{
		if (*s == '-')
			sg *= -1;
		s++;
	}
	while (*s)
	{
		if (*s < '0' || *s > '9')
			return (-1);
		else if (*s >= '0' && *s <= '9')
			r = (r * 10) + (*s - '0');
		s++;
	}
	return (sg * r);
}
