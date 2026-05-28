/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_threads_struct.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 23:19:33 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/28 06:15:09 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clean.h"
#include "coder_state.h"
#include "config.h"
#include "create_threads.h"
#include "dongle.h"
#include "thread_info.h"
#include "utils.h"
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

static int	init_shared_mutexes(t_shared_info *shared_info, int *error)
{
	if (pthread_mutex_init(&shared_info->print_lock, NULL) != 0)
	{
		*error = 6;
		return (6);
	}
	if (pthread_mutex_init(&shared_info->simulation_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&shared_info->print_lock);
		*error = 6;
		return (6);
	}
	if (pthread_cond_init(&shared_info->pairs_ready_cond, NULL) != 0)
	{
		pthread_mutex_destroy(&shared_info->print_lock);
		pthread_mutex_destroy(&shared_info->simulation_lock);
		*error = 7;
		return (7);
	}
	shared_info->pairs_ready = 0;
	shared_info->impairs_ready = 0;
	return (0);
}

static int	init_shared_info(t_shared_info *shared_info, t_config *config,
		int *error)
{
	if (init_shared_mutexes(shared_info, error) != 0)
		return (*error);
	shared_info->simulation_ended = 0;
	shared_info->can_start = 0;
	shared_info->config = config;
	shared_info->coders_states = create_coders(config->nb_coder, error);
	if (shared_info->coders_states == NULL)
	{
		free_shared_info(shared_info, 0);
		return (*error);
	}
	shared_info->dongles = create_dongles(config->nb_coder, error);
	if (shared_info->dongles == NULL)
	{
		free_shared_info(shared_info, config->nb_coder);
		return (*error);
	}
	return (0);
}

static int	init_threads(t_thread_info *threads, t_shared_info *shared_info,
		int nb_coder, int *error)
{
	int	i;

	i = 0;
	while (i < nb_coder)
	{
		if (pthread_mutex_init(&threads[i].lock, NULL) != 0)
		{
			*error = 6;
			while (--i >= 0)
				pthread_mutex_destroy(&threads[i].lock);
			return (-1);
		}
		threads[i].shared_info = shared_info;
		threads[i].thread_ended = 0;
		threads[i].id = i;
		i++;
	}
	return (0);
}

t_thread_info	*create_threads_struct(t_config *config,
		t_shared_info *shared_info, int *error)
{
	t_thread_info	*threads;

	if (init_shared_info(shared_info, config, error) != 0)
		return (NULL);
	threads = ft_calloc(config->nb_coder, sizeof(t_thread_info));
	if (threads == NULL)
	{
		*error = 2;
		free_shared_info(shared_info, config->nb_coder);
		return (NULL);
	}
	if (init_threads(threads, shared_info, config->nb_coder, error) < 0)
	{
		free(threads);
		free_shared_info(shared_info, config->nb_coder);
		return (NULL);
	}
	return (threads);
}
