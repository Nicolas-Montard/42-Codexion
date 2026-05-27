/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:47:21 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/26 15:39:15 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder_state.h"
#include "pthread.h"
#include "thread_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "thread_function.h"

void	thread_print(char *str, t_thread_info *thread_info)
{
	pthread_mutex_lock(&thread_info->shared_info->simulation_lock);
	if (thread_info->shared_info->simulation_ended == 1)
	{
		pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
		return ;
	}
	pthread_mutex_lock(&thread_info->shared_info->print_lock);
	printf("%ld %d %s\n",
		get_time_since_start(thread_info->shared_info->config->started_at),
		thread_info->id + 1, str);
	pthread_mutex_unlock(&thread_info->shared_info->print_lock);
	pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
}

void	change_compile_start(t_thread_info *thread_info)
{
	struct timeval	now;
	t_coder_state	*coder;

	coder = get_coder(thread_info);
	gettimeofday(&now, NULL);
	pthread_mutex_lock(&coder->lock_compile_start);
	coder->last_compile_start = now;
	pthread_mutex_unlock(&coder->lock_compile_start);
}

struct timespec	make_timespec(int timeout_ms)
{
	struct timeval	tv;
	struct timespec	time_spec;

	gettimeofday(&tv, NULL);
	tv.tv_sec += timeout_ms / 1000;
	tv.tv_usec += (timeout_ms % 1000) * 1000;
	if (tv.tv_usec >= 1000000)
	{
		tv.tv_sec += 1;
		tv.tv_usec -= 1000000;
	}
	time_spec.tv_sec = tv.tv_sec;
	time_spec.tv_nsec = tv.tv_usec * 1000;
	return (time_spec);
}
