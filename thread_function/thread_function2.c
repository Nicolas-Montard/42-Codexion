/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_function2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 13:50:20 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/28 03:53:11 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread_function.h"
#include "thread_info.h"
#include <unistd.h>

static void	do_compile(t_thread_info *thread_info, t_dongle *dongles[2],
		t_coder_state *coder)
{
	thread_print("has taken a dongle", thread_info);
	thread_print("has taken a dongle", thread_info);
	change_compile_start(thread_info);
	thread_print("is compiling", thread_info);
	usleep(thread_info->shared_info->config->time_to_compile * 1000);
	coder->nb_compile += 1;
	release_dongle(dongles[0]);
	release_dongle(dongles[1]);
}

void	compile(t_thread_info *thread_info)
{
	t_dongle		*dongles[2];
	t_coder_state	*coder;

	coder = get_coder(thread_info);
	dongles[0] = get_first_dongle(thread_info);
	dongles[1] = get_second_dongle(thread_info);
	if (dongles[0] == dongles[1])
		usleep((thread_info->shared_info->config->time_to_burnout + 20) * 1000);
	if (thread_info->shared_info->simulation_ended == 1)
		return ;
	take_dongles(thread_info, dongles);
	pthread_mutex_lock(&thread_info->shared_info->simulation_lock);
	if (thread_info->shared_info->simulation_ended == 1)
	{
		pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
		release_dongle(dongles[0]);
		release_dongle(dongles[1]);
		return ;
	}
	pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
	do_compile(thread_info, dongles, coder);
}

void	debug(t_thread_info *thread_info)
{
	pthread_mutex_lock(&thread_info->shared_info->simulation_lock);
	if (thread_info->shared_info->simulation_ended != 1)
	{
		pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
		thread_print("is debugging", thread_info);
		usleep(thread_info->shared_info->config->time_to_debug * 1000);
	}
	else
		pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
}

void	refactoring(t_thread_info *thread_info)
{
	pthread_mutex_lock(&thread_info->shared_info->simulation_lock);
	if (thread_info->shared_info->simulation_ended != 1)
	{
		pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
		thread_print("is refactoring", thread_info);
		usleep(thread_info->shared_info->config->time_to_refactor * 1000);
	}
	else
		pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
}
