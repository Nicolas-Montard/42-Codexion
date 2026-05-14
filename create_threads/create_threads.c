/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 01:38:56 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/14 02:59:59 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clean.h"
#include "create_threads.h"
#include "monitor.h"
#include "thread_function.h"
#include "thread_info.h"
#include "utils.h"
#include <pthread.h>
#include <stdio.h>

pthread_t	*create_threads(int *error, thread_info_t *threads_info)
{
	pthread_t	*threads;
	int			i;
	int			nb_coder;

	i = 0;
	nb_coder = threads_info[0].shared_info->config->nb_coder;
	threads = ft_calloc(nb_coder, sizeof(pthread_t));
	if (threads == NULL)
	{
		*error = 2;
		return (NULL);
	}
	while (i < nb_coder)
	{
		if (pthread_create(&threads[i], NULL, thread_function,
				&threads_info[i]) != 0)
		{
			*error = 5;
			threads_info[0].shared_info->simulation_ended = 1;
			while (--i >= 0)
				pthread_join(threads[i], NULL);
			free(threads);
			return (NULL);
		}
		i++;
	}
	return (threads);
}
