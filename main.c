/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:20:55 by nmontard          #+#    #+#             */
/*   Updated: 2026/04/27 13:55:55 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clean.h"
#include "config.h"
#include "create_threads.h"
#include "error_manager.h"
#include "parsing.h"
#include <stdio.h>
#include <string.h>

int	main(int argc, char *argv[])
{
	config_t		*config;
	pthread_t		*threads;
	thread_info_t	**thread_info;
	int				error;
	int				i;

	i = 0;
	error = 0;
	thread_info = NULL;
	threads = NULL;
	config = get_config(argc - 1, argv + 1, &error);
	if (config == NULL)
		return (error_manager(error, thread_info, threads, config));
	thread_info = create_threads_struct(config, &error);
	if (thread_info == NULL)
		return (error_manager(error, thread_info, threads, config));
	threads = create_threads(config, &error, thread_info);
	if (threads == NULL)
		return (error_manager(error, thread_info, threads, config));
	while (threads[i] != 0)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	free_main(thread_info, threads, config);
	return (0);
}
