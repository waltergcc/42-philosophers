/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 19:51:37 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/05/27 21:57:40 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_time_to_finish(t_philo *philo, int finish_order)
{
	pthread_mutex_lock(&philo->table->finish_padlock);
	if (finish_order || philo->table->finish)
	{
		if (finish_order)
			philo->table->finish = 1;
		pthread_mutex_unlock(&philo->table->finish_padlock);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->finish_padlock);
	return (0);
}

int	is_someone_dead_or_full(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->eat_padlock);
	if (get_time() - philo->last_eat >= philo->table->time_to_die)
	{
		print_action(philo, DIE);
		is_time_to_finish(philo, YES);
		pthread_mutex_unlock(&philo->table->eat_padlock);
		return (1);
	}
	else if (philo->table->must_eat > 0
		&& philo->eat_count >= philo->table->must_eat)
	{
		philo->table->full_philos++;
		if (philo->table->full_philos >= philo->table->philosophers)
		{
			is_time_to_finish(philo, YES);
			print_action(philo, FINISH);
			pthread_mutex_unlock(&philo->table->eat_padlock);
			return (1);
		}
	}
	pthread_mutex_unlock(&philo->table->eat_padlock);
	return (0);
}

void	time_to_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_padlock[philo->l_fork]);
	pthread_mutex_lock(&philo->table->fork_padlock[philo->r_fork]);
	print_action(philo, TAKE);
	print_action(philo, TAKE);
	print_action(philo, EAT);
	advance_time(philo, philo->table->time_to_eat);
	pthread_mutex_lock(&philo->table->eat_padlock);
	philo->eat_count++;
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->table->eat_padlock);
	pthread_mutex_unlock(&philo->table->fork_padlock[philo->r_fork]);
	pthread_mutex_unlock(&philo->table->fork_padlock[philo->l_fork]);
}

int	lone_philosopher(t_table *table)
{
	print_action(&table->philo[0], TAKE);
	advance_time(&table->philo[0], table->time_to_die);
	print_action(&table->philo[0], DIE);
	is_time_to_finish(&table->philo[0], YES);
	return (0);
}

void	*start_dinner(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	if (philo->id % 2 == 0)
		usleep(philo->table->time_to_eat * 1000);
	while (1)
	{
		if (philo->table->philosophers == 1)
		{
			lone_philosopher(philo->table);
			return (0);
		}
		if (is_time_to_finish(philo, NO))
			return (0);
		time_to_eat(philo);
		print_action(philo, SLEEP);
		advance_time(philo, philo->table->time_to_sleep);
		print_action(philo, THINK);
	}	
	return (0);
}
