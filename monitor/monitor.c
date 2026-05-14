/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 05:31:39 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/14 03:20:01 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread_function.h"
#include "thread_info.h"
#include <pthread.h>
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
		pthread_mutex_lock(&thread_info->shared_info->coders_states[i].lock_compile_start);
		last_compile_ms = timeval_to_ms(thread_info->shared_info->coders_states[i].last_compile_start);
		pthread_mutex_unlock(&thread_info->shared_info->coders_states[i].lock_compile_start);
		if (now_ms
			- last_compile_ms >= thread_info->shared_info->config->time_to_burnout)
		{
			thread_info->shared_info->simulation_ended = 1;
			thread_print("burned out", &thread_info[i]);
			return (1);
		}
		i++;
	}
	return (0);
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
		if (check_all_ended(thread_info, nb_coder))
		{
			thread_info->shared_info->simulation_ended = 1;
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}

pthread_t	create_monitor(int *error, thread_info_t *threads_info)
{
	pthread_t monitor_thread;

	if (pthread_create(&monitor_thread, NULL, monitor, threads_info) != 0)
	{
		*error = 5;
		return (monitor_thread);
	}
	return (monitor_thread);
}