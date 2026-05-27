/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:20:55 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/27 14:04:20 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clean.h"
#include "config.h"
#include "create_threads.h"
#include "error_manager.h"
#include "monitor.h"
#include "parsing.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include "threads_stock.h"

int	main(int argc, char *argv[])
{
	t_config		*config;
	t_threads_stock	th_stock;
	t_thread_info	*threads_info;
	t_shared_info	shared;
	int				error;

	error = 0;
	threads_info = NULL;
	config = get_config(argc - 1, argv + 1, &error);
	if (config == NULL)
		return (error_manager(error, NULL, NULL, NULL));
	threads_info = create_threads_struct(config, &shared, &error);
	if (threads_info == NULL)
		return (error_manager(error, NULL, NULL, config));
	th_stock.threads = create_threads(&error, threads_info);
	if (error != 0)
		return (error_manager(error, threads_info, th_stock.threads, config));
	th_stock.monitor = create_monitor(&error, threads_info);
	if (error != 0)
		return (error_manager(error, threads_info, th_stock.threads, config));
	join_threads(th_stock.threads, config->nb_coder);
	pthread_join(th_stock.monitor, NULL);
	free_main(threads_info, th_stock.threads, config);
	return (0);
}
