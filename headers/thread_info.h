/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_info.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 14:29:01 by nmontard          #+#    #+#             */
/*   Updated: 2026/04/30 06:02:08 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_INFO_H
# define THREAD_INFO_H
# include "coder_state.h"
# include "config.h"
# include <pthread.h>
typedef struct thread_info
{
	config_t		*config;
	coder_state_t	**coders_states;
	pthread_mutex_t	*dongles;
	pthread_mutex_t	print_lock;
	int				id;
	struct timeval	program_start;
}					thread_info_t;

#endif