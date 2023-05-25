/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 11:27:19 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/05/25 19:15:18 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	clean_table(t_table *table)
{
	free (table->philo);
	free (table->fork_padlock);
	return (0);
}

int	destroy_padlocks(t_table *table)
{
	int i;

	i = -1;
	while (++i < table->philosophers)
		pthread_mutex_destroy(&table->fork_padlock[i]);
	clean_table(table);
	pthread_mutex_destroy(&table->print_padlock);
	pthread_mutex_destroy(&table->eat_padlock);
	pthread_mutex_destroy(&table->finish_padlock);
	return (0);
}

void	clean_threads(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->philosophers)
		pthread_join(table->philo[i].thread, NULL);
	destroy_padlocks(table);	
}

void	clean_exit(t_table *table)
{
	int i;
	int flag;

	flag = 1;
	while (flag)
	{
		i = -1;
		table->full_philos = 0;
		while (++i < table->philosophers)
		{
			if (flag && is_dead_or_full(&table->philo[i]))
				flag = 0;
		}
		usleep(10);
	}
	clean_threads(table);
}

int	print_error(char *msg)
{
	write(2, "Error: ", 7);
	while (*msg)
	{
		write(2, &*msg, 1);
		msg++;	
	}
	write(2, "\n", 1);
	return (1);
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

long long	get_time(void)
{
	struct timeval	timeval;

	gettimeofday(&timeval, NULL);
	return ((timeval.tv_sec * 1000) + (timeval.tv_usec / 1000));
}

void	print_status(t_philo *philo, const char *status)
{
	long long	now;
	
	pthread_mutex_lock(&philo->table->print_padlock);
	if (!is_finish(philo, 0))
	{
		now = get_time() - philo->table->start;
		printf("%lld %d %s\n", now, philo->id, status);
	}
	pthread_mutex_unlock(&philo->table->print_padlock);
	if (status[0] == 'f')
		printf("%s\n", END);
}

void	sleeping(t_philo *philo, long long ms)
{
	long long	now;

	now = get_time();
	while (!is_finish(philo, 0) && (get_time() - now) < ms)
		usleep(100);
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

static void	eating(t_philo *philo)
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

int	is_finish(t_philo *philo, int order)
{
	pthread_mutex_lock(&philo->table->finish_padlock);
	if (order)
	{
		philo->table->finish = 1;
		pthread_mutex_unlock(&philo->table->finish_padlock);
		return (1);
	}
	if (philo->table->finish)
	{
		pthread_mutex_unlock(&philo->table->finish_padlock);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->finish_padlock);
	return (0);
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

int	set_table(t_table *table, int ac, char **av)
{
	if (ac < 5 || ac > 6)
		return (print_error("Wrong number of arguments"));
	table->philosophers = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		table->must_eat = ft_atoi(av[5]);
	else
		table->must_eat = -1;
	if (table->philosophers < 1 || table->time_to_die < 1
		|| table->time_to_eat < 1 || table->time_to_sleep < 1
		|| (ac == 6 && table->must_eat < 1))
		return (print_error("Invalid arguments"));
	table->finish = 0;
	return (0);
}

int	call_philosophers(t_table *table)
{
	int i;
	
	i = -1;
	table->philo = malloc(sizeof(t_philo) * table->philosophers);
	table->fork_padlock = malloc(sizeof(pthread_mutex_t) * table->philosophers);
	table->start = get_time();
	if (!table->philo || !table->fork_padlock)
		return (print_error("Couldn't create the philosophers and forks"));
	while (++i < table->philosophers)
	{
		table->philo[i].id = i + 1;
		table->philo[i].l_fork = i;
		table->philo[i].r_fork = (i + 1) % table->philosophers;
		table->philo[i].eat_count = 0;
		table->philo[i].last_eat = table->start;
		table->philo[i].table = table;
	}
	return (0);
}

int	start_padlocks(t_table *table)
{
	int i;

	i = -1;
	while (++i < table->philosophers)
	{
		if (pthread_mutex_init(&table->fork_padlock[i], NULL))
			return (print_error("Couldn't init forks mutex"));
	}
	if (pthread_mutex_init(&table->print_padlock, NULL))
		return (print_error("Couldn't init print mutex"));
	if (pthread_mutex_init(&table->eat_padlock, NULL))
		return (print_error("Couldn't init eat mutex"));
	if (pthread_mutex_init(&table->finish_padlock, NULL))
		return (print_error("Couldn't init finish mutex"));
	return (0);
}

int	turn_philos_in_threads(t_table *table)
{
	int			i;

	i = -1;
	if (table->philosophers == 1)
	{
		print_status(&table->philo[0], FORK);
		sleeping(&table->philo[0], table->time_to_die);
		print_status(&table->philo[0], DIE);
		is_finish(&table->philo[0], 1);
		return (0);
	}
	while (++i < table->philosophers)
	{
		if (pthread_create(&table->philo[i].thread, NULL, \
			start_dinner, &(table->philo[i])))
			return (print_error("Couldn't create thread"));
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_table table;
	
	if (set_table(&table, ac, av))
		return (0);
	if (call_philosophers(&table))
		return (clean_table(&table));
	if (start_padlocks(&table))
		return (destroy_padlocks(&table));
	if (turn_philos_in_threads(&table))
		return (0);
	clean_exit(&table);
}