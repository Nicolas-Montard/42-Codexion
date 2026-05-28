/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_info.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:46:52 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/28 06:13:52 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHARED_INFO_H
# define SHARED_INFO_H
# include "coder_state.h"
# include "config.h"
# include "dongle.h"
# include <pthread.h>

typedef struct shared_info
{
	pthread_mutex_t	print_lock;
	pthread_mutex_t	simulation_lock;
	int				simulation_ended;
	t_dongle		*dongles;
	t_config		*config;
	t_coder_state	*coders_states;
	pthread_cond_t	pairs_ready_cond;
	int				pairs_ready;
	int				impairs_ready;
	int				can_start;
}					t_shared_info;

#endif