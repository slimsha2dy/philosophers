/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfrusciante <marvin@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 23:17:22 by jfruscia          #+#    #+#             */
/*   Updated: 2022/12/05 15:53:39 by hwichoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define SLEEPING 0
# define THINKING 1
# define FORK 2
# define EATING 3
# define FINISHED 4
# define DIED 4
# define CHECK 0
# define UPDATE 1

# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_philosopher
{
	int				status;
	long long		birth_ms;
	long long		last_time;
	int				phils_id;
	int				num_eat;
	int				max_eat_num;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	struct s_desk	*desk;
}	t_philosopher;

typedef struct s_desk
{
	t_philosopher	*phils;
	pthread_mutex_t	*forks;
	pthread_mutex_t	desk_die_mutex;
	pthread_mutex_t	print_mutex;
	int				phils_num;
	int				phils_idx;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				number_of_must_eat;
	int				finished;
}	t_desk;

/***********************/
/*****   main.c   ******/
/***********************/
t_desk			*init_desk(int ac, const char **av);
t_philosopher	*init_phils(t_desk *desk);
int				init_argv(t_desk *desk, int arc, const char **av);
int				error_arg_check(int ac, char **av);
int				single_phil_case(t_desk *desk);

/***********************/
/*****   utils.c   *****/
/***********************/
long long		get_time_ms(void);
long long		get_time_interval(long long start, long long end);
long long		get_timestamp(t_philosopher *phil);
void			time_passing(long long time);
void			print_state(t_desk *desk, int phil_idx, int STATUS);

/***********************/
/*****    ft_*.c   *****/
/***********************/
int				ft_atoi(const char *s);
int				ft_isdigit(const char s);

/***********************/
/**   philosopher.c  ***/
/***********************/
void			hello_philosophers(t_desk *desk);
void			bye_philosophers(t_desk *desk, pthread_t *threads);
void			*philosophers_action(void *_NULLABLE);
int				philosophers_action_2(t_desk *desk, int phil_idx);
int				check_die_desk(t_desk *desk, int phil_idx, int option);

/***********************/
/*****   action.c  *****/
/***********************/
int				eating(t_desk *desk, int phil_idx);
int				thinking(t_desk *desk, int phil_idx);
int				sleeping(t_desk *desk, int phil_idx);
int				check_die(t_desk *desk, int phil_idx);
int				check_full(t_desk *desk, int phil_idx);

#endif
