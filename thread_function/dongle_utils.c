/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:47:12 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/31 02:23:10 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dongle.h"
#include "thread_function.h"
#include "thread_info.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	gettimeofday(&dongle->last_release, NULL);
	dongle->has_been_released = 1;
	pthread_mutex_unlock(&dongle->lock);
}

static void	wait_for_dongles(t_thread_info *thread_info, t_dongle *dongles[2],
		t_coder_state *coder)
{
	struct timespec	end_timer;

	pthread_mutex_lock(&thread_info->shared_info->simulation_lock);
	while (((dongles[0]->queue[0] != coder || dongles[1]->queue[0] != coder)
			|| (dongles[0]->available == 0 || dongles[1]->available == 0))
		&& thread_info->shared_info->simulation_ended == 0)
	{
		end_timer = make_timespec(
				thread_info->shared_info->config->time_to_burnout + 10);
		pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
		pthread_mutex_unlock(&(dongles[1]->lock));
		pthread_cond_timedwait(&(dongles[0]->cond), &(dongles[0]->lock),
			&end_timer);
		pthread_mutex_lock(&(dongles[1]->lock));
		pthread_mutex_lock(&thread_info->shared_info->simulation_lock);
	}
}

void	take_dongles(t_thread_info *thread_info, t_dongle *dongles[2])
{
	t_coder_state	*coder;

	coder = get_coder(thread_info);
	pthread_mutex_lock(&(dongles[0]->lock));
	pthread_mutex_lock(&(dongles[1]->lock));
	wait_for_dongles(thread_info, dongles, coder);
	if (thread_info->shared_info->simulation_ended == 1)
	{
		pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
		pthread_mutex_unlock(&(dongles[0]->lock));
		pthread_mutex_unlock(&(dongles[1]->lock));
		return ;
	}
	pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
	dongles[0]->available = 0;
	dongles[1]->available = 0;
	pop_queue(dongles[0]);
	pop_queue(dongles[1]);
	change_compile_start(thread_info);
	add_to_queue(
		coder, dongles[0], thread_info->shared_info->config->scheduler);
	add_to_queue(
		coder, dongles[1], thread_info->shared_info->config->scheduler);
	pthread_mutex_unlock(&(dongles[0]->lock));
	pthread_mutex_unlock(&(dongles[1]->lock));
}
