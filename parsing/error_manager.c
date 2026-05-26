/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:29:01 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/26 13:05:04 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clean.h"
#include "pthread.h"
#include "thread_info.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/*
error value indicate the type of error:
1: wrong number of parameters
2: a malloc has failed
3: invalid value of parameters
4: error with gettimeofdays
5: error in thread creation
6: error in mutex creation
7: error in cond creation
*/

int	error_manager(int error, thread_info_t *thread_info, pthread_t *threads,
		config_t *config, pthread_t monitor, int monitor_created)
{
	if (thread_info != NULL)
		thread_info->shared_info->simulation_ended = 1;
	if (config != NULL)
		join_threads(threads, config->nb_coder);
	if (monitor_created == 1)
		pthread_join(monitor, NULL);
	free_main(thread_info, threads, config);
	fprintf(stderr, "Error: ");
	if (error == 1)
		fprintf(stderr, "There isn´t the right number of parameters\n");
	if (error == 2)
		fprintf(stderr, "A malloc has failed\n");
	if (error == 3)
		fprintf(stderr, "The parameters have invalid value\n");
	if (error == 4)
		fprintf(stderr, "The current time cannot be obtained");
	if (error == 5)
		fprintf(stderr, "A thread hasn't been able to create itself");
	if (error == 6)
		fprintf(stderr, "A mutex hasn't been able to create itself");
	if (error == 7)
		fprintf(stderr, "A cond hasn't been able to create itself");
	return (1);
}
