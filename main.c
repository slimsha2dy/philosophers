/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfrusciante <marvin@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 23:23:33 by jfruscia          #+#    #+#             */
/*   Updated: 2022/12/05 16:19:11 by hwichoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_desk	*desk;

	if (error_arg_check(ac, av) == 0)
		return (0);
	desk = init_desk(ac, (const char **)av);
	if (!desk)
		return (0);
	desk -> phils = init_phils(desk);
	if (!desk -> phils)
		return (0);
	hello_philosophers(desk);
	return (0);
}

t_desk	*init_desk(int ac, const char **av)
{
	t_desk	*desk;
	int		i;

	desk = malloc(sizeof(t_desk));
	if (!desk)
		return (NULL);
	desk -> phils_num = ft_atoi(av[1]);
	desk -> time_to_die = ft_atoi(av[2]);
	desk -> time_to_eat = ft_atoi(av[3]);
	desk -> time_to_sleep = ft_atoi(av[4]);
	desk -> number_of_must_eat = 2147483647;
	if (ac == 6)
		desk -> number_of_must_eat = ft_atoi(av[5]);
	desk -> finished = 0;
	desk -> forks = malloc(sizeof(pthread_mutex_t) * desk -> phils_num);
	if (!desk -> forks)
		return (NULL);
	i = 0;
	while (i < desk -> phils_num)
		pthread_mutex_init(&(desk -> forks[i++]), NULL);
	pthread_mutex_init(&(desk -> desk_die_mutex), NULL);
	pthread_mutex_init(&(desk -> print_mutex), NULL);
	return (desk);
}

t_philosopher	*init_phils(t_desk *desk)
{
	t_philosopher		*phils;
	long long			birth_ms;
	int					phils_num;
	int					i;

	phils_num = desk -> phils_num;
	birth_ms = get_time_ms();
	phils = malloc(sizeof(t_philosopher) * phils_num);
	if (!phils)
		return (NULL);
	i = 0;
	while (i < phils_num)
	{
		phils[i].desk = desk;
		phils[i].left_fork = &desk -> forks[i];
		phils[i].right_fork = &desk -> forks[(i + 1) % phils_num];
		phils[i].time_to_die = desk -> time_to_die;
		phils[i].time_to_eat = desk -> time_to_eat;
		phils[i].time_to_sleep = desk -> time_to_sleep;
		phils[i].num_eat = 0;
		phils[i].birth_ms = birth_ms;
		phils[i].last_time = phils[i].birth_ms;
		phils[i++].max_eat_num = desk -> number_of_must_eat;
	}
	return (phils);
}

int	error_arg_check(int ac, char **av)
{
	int	i;
	int	av_idx;

	if (ac != 5 && ac != 6)
	{
		printf("Error : unvalid argument count\n");
		return (0);
	}
	av_idx = 1;
	i = 0;
	while (av_idx < ac)
	{
		i = 0;
		while (av[av_idx][i])
		{
			if (!ft_isdigit((const char)av[av_idx][i]))
			{
				printf("Error : unvalid argument value");
				return (0);
			}
			i++;
		}
		av_idx++;
	}
	return (1);
}

int	single_phil_case(t_desk *desk)
{
	print_state(desk, 0, FORK);
	time_passing(desk -> time_to_die);
	print_state(desk, 0, DIED);
	pthread_mutex_destroy(&desk -> forks[0]);
	free(desk -> phils);
	pthread_mutex_destroy(&desk -> desk_die_mutex);
	pthread_mutex_destroy(&desk -> print_mutex);
	return (1);
}
