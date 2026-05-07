/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 01:38:56 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/07 05:22:42 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clean.h"
#include "create_threads.h"
#include "thread_function.h"
#include "thread_info.h"
#include "utils.h"
#include <pthread.h>
#include <stdio.h>

pthread_t	*create_threads(config_t *config, int *error,
		thread_info_t **threads_info)
{
	pthread_t *threads;
	int i;

	i = 0;
	threads = ft_calloc(config->nb_coder + 1, sizeof(pthread_t));
	if (threads == NULL)
	{
		*error = 2;
		return (NULL);
	}
	while (threads_info[i] != NULL)
	{
		if (pthread_create(&(threads[i]), NULL, thread_function,
				threads_info[i]) != 0)
		{
			*error = 5;
			return (threads);
		}
		i++;
	}
	return (threads);
}