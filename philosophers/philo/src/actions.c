/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 19:51:37 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/05/25 20:08:07 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_finish(t_philo *philo, int order)
{
	pthread_mutex_lock(&philo->table->finish_padlock);
	if (order || philo->table->finish)
	{
		if (order)
			philo->table->finish = 1;
		pthread_mutex_unlock(&philo->table->finish_padlock);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->finish_padlock);
	return (0);
}

int	is_dead_or_full(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->eat_padlock);
	if (get_time() - philo->last_eat >= philo->table->time_to_die)
	{
		print_status(philo, DIE);
		is_finish(philo, 1);
		pthread_mutex_unlock(&philo->table->eat_padlock);
		return (1);
	}
	else if (philo->table->must_eat > 0
		&& philo->eat_count >= philo->table->must_eat)
	{
		philo->table->full_philos++;
		if (philo->table->full_philos >= philo->table->philosophers)
		{
			is_finish(philo, 1);
			print_status(philo, "f");
			pthread_mutex_unlock(&philo->table->eat_padlock);
			return (1);
		}
	}
	pthread_mutex_unlock(&philo->table->eat_padlock);
	return (0);
}

void	eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_padlock[philo->l_fork]);
	print_status(philo, FORK);
	pthread_mutex_lock(&philo->table->fork_padlock[philo->r_fork]);
	print_status(philo, FORK);
	print_status(philo, EAT);
	sleeping(philo, philo->table->time_to_eat);
	pthread_mutex_lock(&philo->table->eat_padlock);
	philo->eat_count += 1;
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->table->eat_padlock);
	pthread_mutex_unlock(&philo->table->fork_padlock[philo->r_fork]);
	pthread_mutex_unlock(&philo->table->fork_padlock[philo->l_fork]);
}

void	*start_dinner(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(philo->table->time_to_eat * 1000);
	while (1)
	{
		if (is_finish(philo, 0))
			return (0);
		eating(philo);
		print_status(philo, SLEEP);
		sleeping(philo, philo->table->time_to_sleep);
		print_status(philo, THINK);
	}	
	return (0);
}

int	turn_philos_in_threads(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->philosophers)
	{
		if (pthread_create(&table->philo[i].thread, NULL, \
			start_dinner, &table->philo[i]))
			exit_error("Couldn't create thread", table, 3);
	}
	return (0);
}
