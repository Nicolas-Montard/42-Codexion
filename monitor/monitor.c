/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 05:31:39 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/07 06:26:51 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread_function.h"
#include "thread_info.h"
#include <pthread.h>

void	*monitor(void **thread_info_void)
{
	thread_info_t	**thread_info;
	int				i;

	thread_info = (thread_info_t **)thread_info_void;
	while ((*thread_info)->simulation_ended != 1)
	{
		i = 0;
		while (i < (*thread_info)->config->nb_coder)
		{
			pthread_mutex_lock(&(*thread_info)->coders_states[i]->lock_compile_start);
			if (((*thread_info)->coders_states[i]->last_compile_start.tv_usec
					/ 1000 - (*thread_info)->config->started_at.tv_usec
					/ 1000) >= (*thread_info)->config->time_to_burnout)
			{
				(*thread_info)->simulation_ended = 1;
				thread_print("burned out", thread_info[i]);
			}
			pthread_mutex_unlock(&(*thread_info)->coders_states[i]->lock_compile_start);
		}
	}
	return (NULL);
}
