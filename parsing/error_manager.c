/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:29:01 by nmontard          #+#    #+#             */
/*   Updated: 2026/04/27 16:35:20 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clean.h"
#include "pthread.h"
#include "thread_info.h"
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
*/

int	error_manager(int error, thread_info_t **thread_info, pthread_t *threads,
		config_t *config)
{
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
	if (thread_info != NULL)
		free_thread_struct(thread_info);
	if (threads != NULL)
		free_threads(threads);
	if (config != NULL)
		free(config);
	return (1);
}
