/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_threads_struct.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 23:19:33 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/26 11:36:53 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clean.h"
#include "coder_state.h"
#include "config.h"
#include "dongle.h"
#include "thread_info.h"
#include "utils.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

static int	init_coder_state(coder_state_t *coder_state, int id)
{
	if (pthread_mutex_init(&(coder_state->lock_compile_start), NULL) != 0)
		return (6);
	coder_state->id = id;
	coder_state->nb_compile = 0;
	gettimeofday(&(coder_state->last_compile_start), NULL);
	return (0);
}

static coder_state_t	*create_coders(int nb_coder, int *error)
{
	coder_state_t	*coders;
	int				i;

	coders = ft_calloc(nb_coder, sizeof(coder_state_t));
	if (coders == NULL)
		return (*error = 2, NULL);
	i = 0;
	while (i < nb_coder)
	{
		if (init_coder_state(&coders[i], i) != 0)
		{
			*error = 6;
			while (--i >= 0)
				pthread_mutex_destroy(&coders[i].lock_compile_start);
			free(coders);
			return (NULL);
		}
		i++;
	}
	return (coders);
}

static dongle_t	*create_dongles(int nb_coder, int *error)
{
	dongle_t	*dongles;
	int			i;

	dongles = ft_calloc(nb_coder, sizeof(dongle_t));
	if (dongles == NULL)
	{
		*error = 2;
		return (NULL);
	}
	i = 0;
	while (i < nb_coder)
	{
		if (pthread_mutex_init(&dongles[i].lock, NULL) != 0)
		{
			*error = 6;
			while (--i >= 0)
			{
				pthread_mutex_destroy(&dongles[i].lock);
				pthread_cond_destroy(&dongles[i].cond);
			}
			free(dongles);
			return (NULL);
		}
		if (pthread_cond_init(&dongles[i].cond, NULL) != 0)
		{
			pthread_mutex_destroy(&dongles[i].lock);
			*error = 7;
			while (--i >= 0)
			{
				pthread_mutex_destroy(&dongles[i].lock);
				pthread_cond_destroy(&dongles[i].cond);
			}
			free(dongles);
			return (NULL);
		}
		dongles[i].available = 1;
		dongles[i].queue_size = 0;
		dongles[i].id = i;
		dongles[i].has_been_released = 0;
		i++;
	}
	return (dongles);
}

static int	init_shared_info(shared_info_t *shared_info, config_t *config,
		int *error)
{
	if (pthread_mutex_init(&shared_info->print_lock, NULL) != 0)
		return (*error = 6);
	if (pthread_mutex_init(&shared_info->simulation_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&shared_info->print_lock);
		return (*error = 6);
	}
	if (pthread_cond_init(&shared_info->can_start_cond, NULL) != 0)
	{
		pthread_mutex_destroy(&shared_info->print_lock);
		pthread_mutex_destroy(&shared_info->simulation_lock);
		return (*error = 7);
	}
	shared_info->simulation_ended = 0;
	shared_info->can_start = 0;
	shared_info->config = config;
	shared_info->coders_states = create_coders(config->nb_coder, error);
	if (shared_info->coders_states == NULL)
	{
		pthread_mutex_destroy(&shared_info->print_lock);
		pthread_mutex_destroy(&shared_info->simulation_lock);
		pthread_cond_destroy(&shared_info->can_start_cond);
		return (*error);
	}
	shared_info->dongles = create_dongles(config->nb_coder, error);
	if (shared_info->dongles == NULL)
	{
		pthread_mutex_destroy(&shared_info->print_lock);
		pthread_mutex_destroy(&shared_info->simulation_lock);
		pthread_cond_destroy(&shared_info->can_start_cond);
		free(shared_info->coders_states);
		return (*error);
	}
	return (0);
}

thread_info_t	*create_threads_struct(config_t *config,
		shared_info_t *shared_info, int *error)
{
	thread_info_t	*threads;
	int				i;

	if (init_shared_info(shared_info, config, error) != 0)
		return (NULL);
	threads = ft_calloc(config->nb_coder, sizeof(thread_info_t));
	if (threads == NULL)
	{
		*error = 2;
		free_shared_info(shared_info, config->nb_coder);
		return (NULL);
	}
	i = 0;
	while (i < config->nb_coder)
	{
		if (pthread_mutex_init(&threads[i].lock, NULL) != 0)
		{
			*error = 6;
			while (--i >= 0)
				pthread_mutex_destroy(&threads[i].lock);
			free(threads);
			free_shared_info(shared_info, config->nb_coder);
			return (NULL);
		}
		threads[i].has_started = 0;
		threads[i].shared_info = shared_info;
		threads[i].thread_ended = 0;
		threads[i].id = i;
		i++;
	}
	return (threads);
}
