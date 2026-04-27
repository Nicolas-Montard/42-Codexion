/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_threads_struct.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 23:19:33 by nmontard          #+#    #+#             */
/*   Updated: 2026/04/27 17:08:48 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clean.h"
#include "coder_state.h"
#include "config.h"
#include "thread_info.h"
#include "utils.h"
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

static coder_state_t	*create_coder_state(int id, int *error)
{
	coder_state_t	*coder_state;

	coder_state = ft_calloc(1, sizeof(coder_state_t));
	if (coder_state == NULL)
	{
		*error = 2;
		return (NULL);
	}
	coder_state->id = id;
	if (gettimeofday(&(coder_state->last_compile_start), NULL) == -1)
	{
		*error = 4;
		free(coder_state);
		return (NULL);
	}
	return (coder_state);
}

static coder_state_t	**create_coders(config_t *config, int *error)
{
	int				i;
	coder_state_t	**coders_state;

	coders_state = ft_calloc(config->nb_coder + 1, sizeof(coder_state_t *));
	if (coders_state == NULL)
	{
		*error = 2;
		return (NULL);
	}
	i = 0;
	while (i < config->nb_coder)
	{
		coders_state[i] = create_coder_state(i + 1, error);
		if (coders_state[i] == NULL)
		{
			free_coders_state(coders_state);
			return (NULL);
		}
		i++;
	}
	return (coders_state);
}

static pthread_mutex_t	*create_dongles(config_t *config, int *error)
{
	int				i;
	pthread_mutex_t	*dongles;

	i = 0;
	dongles = ft_calloc(config->nb_coder + 1, sizeof(pthread_mutex_t *));
	if (dongles == NULL)
	{
		*error = 2;
		return (NULL);
	}
	while (i < config->nb_coder)
	{
		if (pthread_mutex_init(&(dongles[i]), NULL) != 0)
		{
			*error = 6;
			free_dongles(dongles, i);
			return (NULL);
		}
		i++;
	}
	return (dongles);
}

static thread_info_t	*create_thread(config_t *config,
		coder_state_t **coders_state, int *dongles, int *error)
{
	thread_info_t	*thread;

	thread = ft_calloc(1, sizeof(thread_info_t));
	if (thread == NULL)
	{
		*error = 2;
		return (NULL);
	}
	thread->coders_states = coders_state;
	thread->config = config;
	thread->dongles = dongles;
	return (thread);
}

thread_info_t	**create_threads_struct(config_t *config, int *error)
{
	thread_info_t	**threads;
	int				i;
	coder_state_t	**coders_state;
	int				*dongles;

	i = 0;
	threads = ft_calloc(config->nb_coder + 1, sizeof(thread_info_t *));
	if (threads == NULL)
	{
		*error = 2;
		return (NULL);
	}
	dongles = create_dongles(config, error);
	if (dongles == NULL)
	{
		free(threads);
		return (NULL);
	}
	coders_state = create_coders(config, error);
	if (coders_state == NULL)
	{
		free(dongles);
		free(threads);
		return (NULL);
	}
	while (i < config->nb_coder)
	{
		threads[i] = create_thread(config, coders_state, dongles, error);
		if (threads[i] == NULL)
		{
			free_thread_struct(threads);
			return (NULL);
		}
		threads[i]->id = i + 1;
		i++;
	}
	return (threads);
}
