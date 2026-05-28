/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_function.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:47:17 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/28 06:53:46 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread_function.h"
#include "thread_info.h"
#include <unistd.h>
#include <stdio.h>

static void	wait_for_start(t_thread_info *thread_info)
{
	struct timespec	start_timer;
	int				nb_pairs;

	nb_pairs = (thread_info->shared_info->config->nb_coder + 1) / 2;
	start_timer = make_timespec(thread_info->shared_info->config->time_to_burnout);
	pthread_mutex_lock(&thread_info->shared_info->simulation_lock);
	while (thread_info->shared_info->pairs_ready < nb_pairs
		&& thread_info->shared_info->simulation_ended == 0)
	{
		pthread_cond_timedwait(&thread_info->shared_info->pairs_ready_cond,
			&thread_info->shared_info->simulation_lock, &start_timer);
	}
	thread_info->shared_info->impairs_ready += 1;
	pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
}

static void	run_loop(t_thread_info *th_info, t_coder_state *coder)
{
	pthread_mutex_lock(&th_info->shared_info->simulation_lock);
	while (th_info->shared_info->simulation_ended != 1)
	{
		pthread_mutex_unlock(&th_info->shared_info->simulation_lock);
		if (coder->nb_compile < th_info->shared_info->config->nb_compile_req)
			compile(th_info);
		else
		{
			pthread_mutex_lock(&th_info->lock);
			th_info->thread_ended = 1;
			pthread_mutex_unlock(&th_info->lock);
			pthread_mutex_lock(&th_info->shared_info->simulation_lock);
			break ;
		}
		debug(th_info);
		refactoring(th_info);
		pthread_mutex_lock(&th_info->shared_info->simulation_lock);
	}
	pthread_mutex_unlock(&th_info->shared_info->simulation_lock);
}

void	*thread_function(void *thread_info_void)
{
	t_thread_info	*thread_info;
	t_coder_state	*coder;
	t_dongle		*dongles;
	struct timespec	timer;
	
	thread_info = (t_thread_info *)thread_info_void;
	dongles = thread_info->shared_info->dongles;
	coder = get_coder(thread_info);
	if (coder->id % 2 == 1)
		wait_for_start(thread_info);
	else
	{
		pthread_mutex_lock(&thread_info->shared_info->simulation_lock);
		thread_info->shared_info->pairs_ready += 1;
		pthread_cond_broadcast(&thread_info->shared_info->pairs_ready_cond);
		pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
	}
	queue_start(coder, get_first_dongle(thread_info),
	thread_info->shared_info->config->scheduler);
	queue_start(coder, get_second_dongle(thread_info),
	thread_info->shared_info->config->scheduler);
	timer = make_timespec(20000);
	pthread_mutex_lock(&thread_info->shared_info->simulation_lock);
	while (thread_info->shared_info->can_start != 1)
	{
		pthread_cond_timedwait(&thread_info->shared_info->pairs_ready_cond, 
			&thread_info->shared_info->simulation_lock, &timer);
	}
	pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
	pthread_mutex_lock(&coder->lock_compile_start);
	gettimeofday(&coder->last_compile_start, NULL);
	pthread_mutex_unlock(&coder->lock_compile_start);
	run_loop(thread_info, coder);
	return (NULL);
}
