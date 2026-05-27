/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:40:24 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/26 15:24:57 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread_info.h"
#include "dongle.h"
#include "thread_function.h"
#include "stdio.h"

void	print_burnout(t_thread_info *thread_info, int i)
{
	pthread_mutex_lock(&thread_info->shared_info->simulation_lock);
	thread_info->shared_info->simulation_ended = 1;
	pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
	pthread_mutex_lock(&thread_info->shared_info->print_lock);
	printf("%ld %d %s\n",
		get_time_since_start(thread_info->shared_info->config->started_at),
		thread_info[i].id + 1, "burned out");
	pthread_mutex_unlock(&thread_info->shared_info->print_lock);
}

int	is_burned_out(t_thread_info *thread_info, int i, long now_ms)
{
	long	last_compile_ms;
	long	time_to_burnout;

	pthread_mutex_lock(
		&thread_info->shared_info->coders_states[i].lock_compile_start);
	last_compile_ms = timeval_to_ms(
			thread_info->shared_info->coders_states[i].last_compile_start);
	pthread_mutex_unlock(
		&thread_info->shared_info->coders_states[i].lock_compile_start);
	time_to_burnout = thread_info->shared_info->config->time_to_burnout;
	return (now_ms - last_compile_ms >= time_to_burnout);
}

int	check_burnout(t_thread_info *thread_info, int nb_coder)
{
	struct timeval	now;
	long			now_ms;
	int				i;

	gettimeofday(&now, NULL);
	now_ms = timeval_to_ms(now);
	i = 0;
	while (i < nb_coder)
	{
		pthread_mutex_lock(&thread_info[i].lock);
		if (thread_info[i].thread_ended == 1)
		{
			pthread_mutex_unlock(&thread_info[i].lock);
			i++;
			continue ;
		}
		pthread_mutex_unlock(&thread_info[i].lock);
		if (is_burned_out(thread_info, i, now_ms))
		{
			print_burnout(thread_info, i);
			return (1);
		}
		i++;
	}
	return (0);
}

void	release_dongle_neighbors(t_dongle *dongles, int i, int nb_dongles)
{
	pthread_mutex_lock(&dongles[(i + 1) % nb_dongles].lock);
	pthread_cond_broadcast(&dongles[(i + 1) % nb_dongles].cond);
	pthread_mutex_unlock(&dongles[(i + 1) % nb_dongles].lock);
	pthread_mutex_lock(&dongles[(i - 1 + nb_dongles) % nb_dongles].lock);
	pthread_cond_broadcast(&dongles[(i - 1 + nb_dongles) % nb_dongles].cond);
	pthread_mutex_unlock(&dongles[(i - 1 + nb_dongles) % nb_dongles].lock);
}

void	check_dongle_cooldown(t_dongle *dongles, t_config *config)
{
	int				i;
	int				nb_dongles;
	struct timeval	now;

	nb_dongles = config->nb_coder;
	gettimeofday(&now, NULL);
	i = 0;
	while (i < nb_dongles)
	{
		pthread_mutex_lock(&dongles[i].lock);
		if (dongles[i].has_been_released == 1 && (timeval_to_ms(now)
				- (timeval_to_ms(dongles[i].last_release)
					+ config->dongle_cooldown)) > 0)
		{
			dongles[i].available = 1;
			dongles[i].has_been_released = 0;
			pthread_cond_broadcast(&dongles[i].cond);
			pthread_mutex_unlock(&dongles[i].lock);
			release_dongle_neighbors(dongles, i, nb_dongles);
		}
		else
			pthread_mutex_unlock(&dongles[i].lock);
		i++;
	}
}
