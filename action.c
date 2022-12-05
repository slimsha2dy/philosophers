/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfrusciante <marvin@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 00:41:46 by jfruscia          #+#    #+#             */
/*   Updated: 2022/12/05 20:23:23 by hwichoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	eating(t_desk *desk, int phil_idx)
{
	t_philosopher	*phil;

	phil = &(desk -> phils[phil_idx]);
	if (pthread_mutex_lock(phil -> left_fork) == 0)
		print_state(desk, phil_idx, FORK);
	if (pthread_mutex_lock(phil -> right_fork) == 0)
		print_state(desk, phil_idx, FORK);
	if (check_die(desk, phil_idx) == 1 || check_full(desk, phil_idx) == 1)
	{
		pthread_mutex_unlock(phil -> left_fork);
		pthread_mutex_unlock(phil -> right_fork);
		return (0);
	}
	if (check_die_desk(desk, phil_idx, CHECK) == 0)
	{
		print_state(desk, phil_idx, EATING);
		phil -> num_eat++;
		phil -> last_time = get_time_ms();
		time_passing(phil -> time_to_eat);
	}
	pthread_mutex_unlock(phil -> left_fork);
	pthread_mutex_unlock(phil -> right_fork);
	return (1);
}

int	thinking(t_desk *desk, int phil_idx)
{
	t_philosopher	*phil;

	phil = &(desk -> phils[phil_idx]);
	print_state(desk, phil_idx, THINKING);
	return (0);
}

int	sleeping(t_desk *desk, int phil_idx)
{
	t_philosopher	*phil;
	long long		sleep_time;

	phil = &(desk -> phils[phil_idx]);
	sleep_time = phil -> time_to_sleep;
	if (phil -> time_to_die < sleep_time)
		sleep_time = phil -> time_to_die;
	print_state(desk, phil_idx, SLEEPING);
	time_passing(sleep_time);
	check_die(desk, phil_idx);
	return (1);
}

int	check_die(t_desk *desk, int phil_idx)
{
	t_philosopher	*phil;

	phil = &(desk -> phils[phil_idx]);
	if (phil -> time_to_die \
			< get_time_interval(phil -> last_time, get_time_ms()))
	{
		phil -> status = FINISHED;
		print_state(desk, phil_idx, DIED);
		return (1);
	}
	return (0);
}

int	check_full(t_desk *desk, int phil_idx)
{
	t_philosopher	*phil;

	phil = &(desk -> phils[phil_idx]);
	if (phil -> max_eat_num == phil -> num_eat)
	{
		phil -> status = FINISHED;
		return (1);
	}
	return (0);
}
