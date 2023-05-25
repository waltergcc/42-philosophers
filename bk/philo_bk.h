/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 11:27:22 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/05/25 19:18:17 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_philo
{
	int				id;
	int				eat_count;
	int				l_fork;
	int				r_fork;
	long long		last_eat;
	struct s_table	*table;
	pthread_t		thread;
}					t_philo;

typedef struct s_table
{
	int				philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	int				full_philos;
	int				finish;
	long long		start;
	t_philo			*philo;
	pthread_mutex_t	*fork_padlock;
	pthread_mutex_t	print_padlock;
	pthread_mutex_t	eat_padlock;
	pthread_mutex_t	finish_padlock;
}					t_table;

# define FORK "has taken a fork"
# define EAT "is eating"
# define SLEEP "is sleeping"
# define THINK "is thinking"
# define DIE "died"
# define END "All philosophers ate enough"

int			print_error(char *msg);
int			ft_atoi(const char *s);
long long	get_time(void);
void		print_status(t_philo *philo, const char *status);
void		sleeping(t_philo *philo, long long ms);

int			set_table(t_table *table, int ac, char **av);
int			call_philosophers(t_table *table);
int			start_padlocks(t_table *table);
int			turn_philos_in_threads(t_table *table);

int			is_finish(t_philo *philo, int order);
int			is_dead_or_full(t_philo *philo);
void		*start_dinner(void *arg);

int			clean_table(t_table *table);
int			destroy_padlocks(t_table *table);
void			clean_threads(t_table *table);
void		clean_exit(t_table *table);

#endif