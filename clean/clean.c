/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:46:28 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/30 17:50:28 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder_state.h"
#include "thread_info.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void	free_dongles(t_dongle *dongles, int size)
{
	int	i;

	i = 0;
	if (dongles != NULL)
	{
		while (i < size)
		{
			pthread_cond_destroy(&dongles[i].cond);
			pthread_mutex_destroy(&dongles[i].lock);
			i++;
		}
		free(dongles);
	}
}

void	free_coders_state(t_coder_state *coders_state, int size)
{
	int	i;

	i = 0;
	if (coders_state != NULL)
	{
		while (i < size)
		{
			pthread_mutex_destroy(&coders_state[i].lock_compile_start);
			i++;
		}
		free(coders_state);
	}
}

void	free_shared_info(t_shared_info *shared, int nb_coder)
{
	pthread_mutex_destroy(&shared->print_lock);
	pthread_mutex_destroy(&shared->simulation_lock);
	pthread_cond_destroy(&shared->can_start_cond);
	if (shared->dongles != NULL)
		free_dongles(shared->dongles, nb_coder);
	if (shared->coders_states != NULL)
		free_coders_state(shared->coders_states, nb_coder);
}

void	free_threads_info(t_thread_info *threads, int nb_coder)
{
	int	i;

	i = 0;
	while (i < nb_coder)
	{
		pthread_mutex_destroy(&threads[i].lock);
		i++;
	}
	free(threads);
}

void	free_main(t_thread_info *thread_info, pthread_t *threads,
		t_config *config)
{
	int				nb_coder;
	t_shared_info	*shared;

	shared = NULL;
	nb_coder = 0;
	if (config != NULL)
		nb_coder = config->nb_coder;
	if (threads != NULL)
		free(threads);
	if (thread_info != NULL)
	{
		shared = thread_info->shared_info;
		free_threads_info(thread_info, nb_coder);
	}
	if (shared != NULL)
		free_shared_info(shared, nb_coder);
	if (config != NULL)
		free(config);
}
