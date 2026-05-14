/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:20:55 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/14 03:25:06 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clean.h"
#include "config.h"
#include "create_threads.h"
#include "error_manager.h"
#include "monitor.h"
#include "parsing.h"
#include <stdio.h>
#include <string.h>

int	main(int argc, char *argv[])
{
	config_t		*config;
	pthread_t		*threads;
	thread_info_t	*threads_info;
	shared_info_t	shared;
	pthread_t		monitor;
	int				error;

	error = 0;
	threads_info = NULL;
	threads = NULL;
	config = get_config(argc - 1, argv + 1, &error);
	if (config == NULL)
		return (error_manager(error, NULL, NULL, NULL, 0, 0));
	threads_info = create_threads_struct(config, &shared, &error);
	if (threads_info == NULL)
		return (error_manager(error, NULL, NULL, config, 0, 0));
	threads = create_threads(&error, threads_info);
	if (error != 0)
		return (error_manager(error, threads_info, threads, config, 0, 0));
	monitor = create_monitor(&error, threads_info);
	if (error != 0)
		return (error_manager(error, threads_info, threads, config, monitor,
				0));
	free_main(threads_info, threads, &shared, config, monitor, 1);
	return (0);
}
