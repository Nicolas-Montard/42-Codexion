/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_threads_struct_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:21:13 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/28 05:24:45 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clean.h"
#include "coder_state.h"
#include "create_threads.h"
#include "dongle.h"
#include "utils.h"

int	init_coder_state(t_coder_state *coder_state, int id)
{
	if (pthread_mutex_init(&(coder_state->lock_compile_start), NULL) != 0)
		return (6);
	coder_state->id = id;
	coder_state->nb_compile = 0;
	gettimeofday(&(coder_state->last_compile_start), NULL);
	return (0);
}

t_coder_state	*create_coders(int nb_coder, int *error)
{
	t_coder_state	*coders;
	int				i;

	coders = ft_calloc(nb_coder, sizeof(t_coder_state));
	if (coders == NULL)
	{
		*error = 2;
		return (NULL);
	}
	i = 0;
	while (i < nb_coder)
	{
		if (init_coder_state(&coders[i], i) != 0)
		{
			*error = 6;
			free_coders_state(coders, i);
			return (NULL);
		}
		i++;
	}
	return (coders);
}

int	init_dongle(t_dongle *dongles, int i, int *error)
{
	if (pthread_mutex_init(&dongles[i].lock, NULL) != 0)
	{
		*error = 6;
		return (-1);
	}
	if (pthread_cond_init(&dongles[i].cond, NULL) != 0)
	{
		pthread_mutex_destroy(&dongles[i].lock);
		*error = 7;
		return (-1);
	}
	dongles[i].available = 1;
	dongles[i].queue_size = 0;
	dongles[i].id = i;
	dongles[i].has_been_released = 0;
	return (0);
}

t_dongle	*create_dongles(int nb_coder, int *error)
{
	t_dongle	*dongles;
	int			i;

	dongles = ft_calloc(nb_coder, sizeof(t_dongle));
	if (dongles == NULL)
	{
		*error = 2;
		return (NULL);
	}
	i = 0;
	while (i < nb_coder)
	{
		if (init_dongle(dongles, i, error) < 0)
		{
			free_dongles(dongles, i);
			return (NULL);
		}
		i++;
	}
	return (dongles);
}
