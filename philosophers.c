/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfrusciante <marvin@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 00:12:46 by jfruscia          #+#    #+#             */
/*   Updated: 2022/12/05 01:29:24 by jfruscia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	hello_philosophers(t_desk *desk)
{
	int				i;
	int				offset;
	int				phil_idx;
	pthread_t		*threads;

	offset = desk -> phils_num + (desk -> phils_num % 2 == 0);
	threads = malloc(sizeof(pthread_t) * desk -> phils_num);
	if (desk -> phils_num == 1)
	{
		single_phil_case(desk);
		return ;
	}
	i = 1;
	while (i < (desk -> phils_num * 2) + 1)
	{
		phil_idx = i % offset;
		desk -> phils[phil_idx].phils_id = phil_idx;
		if (phil_idx == 0)
			usleep(500);
		pthread_create(&threads[phil_idx], NULL, \
				philosophers_action, &(desk -> phils[phil_idx]));
		i += 2;
	}
	bye_philosophers(desk, threads);
}

void	*philosophers_action(void *_NULLABLE)
{
	t_desk			*desk;
	t_philosopher	*phil;
	int				phil_idx;

	phil = (t_philosopher *)_NULLABLE;
	desk = phil -> desk;
	phil_idx = phil -> phils_id;
	philosophers_action_2(desk, phil_idx);
	return (NULL);
}

int	philosophers_action_2(t_desk *desk, int phil_idx)
{
	t_philosopher	*phil;

	phil = &(desk -> phils[phil_idx]);
	while (check_die_desk(desk, phil_idx, CHECK) == 0 && \
			check_full(desk, phil_idx) == 0)
	{
		eating(desk, phil_idx);
		sleeping(desk, phil_idx);
		thinking(desk, phil_idx);
	}
	return (1);
}

int	check_die_desk(t_desk *desk, int phil_idx, int option)
{
	t_philosopher	*phil;

	phil = &(desk -> phils[phil_idx]);
	if (option == UPDATE)
	{
		pthread_mutex_lock(&(desk -> desk_die_mutex));
		desk -> finished = 1;
	}
	else if (option == CHECK)
	{
		pthread_mutex_lock(&(desk -> desk_die_mutex));
		if (desk -> finished == 1)
		{
			pthread_mutex_unlock(&(desk -> desk_die_mutex));
			return (1);
		}
		pthread_mutex_unlock(&(desk -> desk_die_mutex));
		return (0);
	}
	pthread_mutex_unlock(&(desk -> desk_die_mutex));
	return (1);
}

void	bye_philosophers(t_desk *desk, pthread_t *threads)
{
	int				i;
	int				offset;

	offset = desk -> phils_num + (desk -> phils_num % 2 == 0);
	i = 1;
	while (i < (desk -> phils_num * 2) + 1)
	{
		pthread_join(threads[i % offset], NULL);
		i += 2;
	}
	i = 1;
	while (i < (desk -> phils_num * 2) + 1)
	{
		pthread_mutex_destroy(&desk -> forks[i % offset]);
		i += 2;
	}
	free(desk -> phils);
	pthread_mutex_destroy(&desk -> desk_die_mutex);
	pthread_mutex_destroy(&desk -> info_mutex);
	pthread_mutex_destroy(&desk -> print_mutex);
	free(desk);
}
