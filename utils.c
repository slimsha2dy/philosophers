/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfrusciante <marvin@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 00:32:32 by jfruscia          #+#    #+#             */
/*   Updated: 2022/12/05 15:15:24 by hwichoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time_ms(void)
{
	struct timeval	start_time;
	long long		time_ms;

	gettimeofday(&start_time, NULL);
	time_ms = (start_time.tv_sec * 1000) + (start_time.tv_usec / 1000);
	return (time_ms);
}

long long	get_time_interval(long long start, long long end)
{
	return (end - start);
}

long long	get_timestamp(t_philosopher *phil)
{
	long long	timestamp;

	timestamp = get_time_ms() - phil -> birth_ms;
	return (timestamp);
}

void	time_passing(long long time)
{
	long long	end_time;

	end_time = time + get_time_ms();
	while (end_time > get_time_ms())
		usleep(100);
}

void	print_state(t_desk *desk, int phil_idx, int STATUS)
{
	t_philosopher	*phil;

	phil = &(desk -> phils[phil_idx]);
	pthread_mutex_lock(&(desk -> print_mutex));
	if (STATUS == DIED \
			&& desk -> finished == 0)
	{
		check_die_desk(desk, phil_idx, UPDATE);
		printf("%lld %d died\n", get_timestamp(phil), phil_idx + 1);
	}
	else if (STATUS == SLEEPING \
			&& desk -> finished == 0)
		printf("%lld %d is sleeping\n", get_timestamp(phil), phil_idx + 1);
	else if (STATUS == THINKING \
			&& desk -> finished == 0)
		printf("%lld %d is thinking\n", get_timestamp(phil), phil_idx + 1);
	else if (STATUS == FORK && desk -> finished == 0)
		printf("%lld %d has taken a fork\n", get_timestamp(phil), phil_idx + 1);
	else if (STATUS == EATING && desk -> finished == 0)
		printf("%lld %d is eating\n", get_timestamp(phil), phil_idx + 1);
	pthread_mutex_unlock(&(desk -> print_mutex));
}
