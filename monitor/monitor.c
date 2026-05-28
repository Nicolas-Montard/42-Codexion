/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 05:31:39 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/28 06:34:25 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread_function.h"
#include "thread_info.h"
#include <pthread.h>
#include <unistd.h>
#include "monitor.h"

static void	check_can_start(t_thread_info *thread_info)
{
	struct timeval now;

	pthread_mutex_lock(&thread_info->shared_info->simulation_lock);
	while (thread_info->shared_info->can_start != 1)
	{
		if ((thread_info->shared_info->pairs_ready
			+ thread_info->shared_info->impairs_ready)
			== thread_info->shared_info->config->nb_coder)
		{
			thread_info->shared_info->can_start = 1;
			gettimeofday(&now, NULL);
			thread_info->shared_info->config->started_at = now;
			pthread_cond_broadcast(&thread_info->shared_info->pairs_ready_cond);
			pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
			break;
		}
		pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
		usleep(100);
		pthread_mutex_lock(&thread_info->shared_info->simulation_lock);
	}
}

static int	check_all_ended(t_thread_info *thread_info, int nb_coder)
{
	int	i;

	i = 0;
	while (i < nb_coder)
	{
		pthread_mutex_lock(&thread_info[i].lock);
		if (thread_info[i].thread_ended == 0)
		{
			pthread_mutex_unlock(&thread_info[i].lock);
			return (0);
		}
		pthread_mutex_unlock(&thread_info[i].lock);
		i++;
	}
	return (1);
}

static void	*monitor(void *thread_info_void)
{
	t_thread_info	*thread_info;
	int				nb_coder;

	thread_info = (t_thread_info *)thread_info_void;
	nb_coder = thread_info->shared_info->config->nb_coder;
	check_can_start(thread_info);
	while (thread_info->shared_info->simulation_ended != 1)
	{
		if (check_burnout(thread_info, nb_coder))
			break ;
		check_dongle_cooldown(thread_info->shared_info->dongles,
			thread_info->shared_info->config);
		if (check_all_ended(thread_info, nb_coder))
		{
			thread_info->shared_info->simulation_ended = 1;
			break ;
		}
	}
	return (NULL);
}

pthread_t	create_monitor(int *error, t_thread_info *threads_info)
{
	pthread_t	monitor_thread;

	if (pthread_create(&monitor_thread, NULL, monitor, threads_info) != 0)
	{
		*error = 5;
		return (monitor_thread);
	}
	return (monitor_thread);
}
