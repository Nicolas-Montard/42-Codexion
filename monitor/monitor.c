/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 05:31:39 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/24 17:56:25 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread_function.h"
#include "thread_info.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

static int	check_burnout(thread_info_t *thread_info, int nb_coder)
{
	struct timeval	now;
	long			now_ms;
	long			last_compile_ms;
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
		pthread_mutex_lock(&thread_info->shared_info->coders_states[i].lock_compile_start);
		last_compile_ms = timeval_to_ms(thread_info->shared_info->coders_states[i].last_compile_start);
		pthread_mutex_unlock(&thread_info->shared_info->coders_states[i].lock_compile_start);
		if (now_ms
			- last_compile_ms >= thread_info->shared_info->config->time_to_burnout
			&& thread_info->thread_ended == 0)
		{
			pthread_mutex_lock(&thread_info->shared_info->simulation_lock);
			thread_info->shared_info->simulation_ended = 1;
			pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
			pthread_mutex_lock(&thread_info->shared_info->print_lock);
			printf("%ld %d %s\n",
				get_time_since_start(thread_info->shared_info->config->started_at),
				thread_info[i].id + 1, "burned out");
			pthread_mutex_unlock(&thread_info->shared_info->print_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

static void	check_dongle_cooldown(thread_info_t *thread_info)
{
	int				i;
	int				nb_dongles;
	dongle_t		*dongles;
	config_t		*config;
	struct timeval	now;

	config = thread_info->shared_info->config;
	dongles = thread_info->shared_info->dongles;
	nb_dongles = thread_info->shared_info->config->nb_coder;
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
			pthread_cond_broadcast(&dongles[(i + 1) % nb_dongles].cond);
			pthread_cond_broadcast(&dongles[(i - 1 + nb_dongles)
				% nb_dongles].cond);
		}
		pthread_mutex_unlock(&dongles[i].lock);
		i++;
	}
}

static int	check_started(thread_info_t *thread_info, int nb_coder)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&thread_info->shared_info->simulation_lock);
	if (thread_info->shared_info->can_start = 0)
	{
	}
	pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
	while (i < nb_coder)
	{
		pthread_mutex_lock(&thread_info[i].lock);
		if (thread_info[i].id % 2 == 0 && thread_info[i].has_started == 0)
		{
			pthread_mutex_unlock(&thread_info[i].lock);
			return (0);
		}
		pthread_mutex_unlock(&thread_info[i].lock);
		i++;
	}
	return (1);
}

static int	check_all_ended(thread_info_t *thread_info, int nb_coder)
{
	int	i;

	i = 0;
	while (i < nb_coder)
	{
		if (thread_info[i].thread_ended == 0)
			return (0);
		i++;
	}
	return (1);
}

static void	broadcast_all(thread_info_t *thread_info)
{
	int	i;

	i = 0;
	while (i < thread_info->shared_info->config->nb_coder)
	{
		pthread_cond_broadcast(&thread_info->shared_info->dongles[i].cond);
		i++;
	}
}
// TODO set the variable can start
static void	*monitor(void *thread_info_void)
{
	thread_info_t	*thread_info;
	int				nb_coder;

	thread_info = (thread_info_t *)thread_info_void;
	nb_coder = thread_info->shared_info->config->nb_coder;
	while (thread_info->shared_info->simulation_ended != 1)
	{
		if (check_burnout(thread_info, nb_coder))
			break ;
		check_dongle_cooldown(thread_info);
		if (check_all_ended(thread_info, nb_coder))
		{
			thread_info->shared_info->simulation_ended = 1;
			broadcast_all(thread_info);
			break ;
		}
	}
	return (NULL);
}

pthread_t	create_monitor(int *error, thread_info_t *threads_info)
{
	pthread_t	monitor_thread;

	if (pthread_create(&monitor_thread, NULL, monitor, threads_info) != 0)
	{
		*error = 5;
		return (monitor_thread);
	}
	return (monitor_thread);
}
