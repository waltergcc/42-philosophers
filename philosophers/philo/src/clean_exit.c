/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 19:46:42 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/05/27 19:15:21 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_table(t_table *table)
{
	free (table->philo);
	free (table->fork_padlock);
}

void	destroy_padlocks(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->philosophers)
		pthread_mutex_destroy(&table->fork_padlock[i]);
	clean_table(table);
	pthread_mutex_destroy(&table->print_padlock);
	pthread_mutex_destroy(&table->eat_padlock);
	pthread_mutex_destroy(&table->finish_padlock);
}

void	finish_dinner(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->philosophers)
		pthread_join(table->philo[i].thread, NULL);
	destroy_padlocks(table);
}

void	start_dinner_monitor(t_table *table)
{
	int	i;
	int	keep_dinner;

	keep_dinner = 1;
	while (keep_dinner)
	{
		i = -1;
		table->full_philos = 0;
		while (++i < table->philosophers)
		{
			if (keep_dinner && is_someone_dead_or_full(&table->philo[i]))
				keep_dinner = 0;
		}
		usleep(10);
	}
	finish_dinner(table);
}
