/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_info.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 14:29:01 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/26 15:04:53 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_INFO_H
# define THREAD_INFO_H
# include "coder_state.h"
# include "config.h"
# include "shared_info.h"
# include <pthread.h>

typedef struct thread_info
{
	t_shared_info	*shared_info;
	pthread_mutex_t	lock;
	int				thread_ended;
	int				id;
	int				has_started;
}					t_thread_info;

#endif