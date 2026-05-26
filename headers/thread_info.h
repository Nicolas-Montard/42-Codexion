/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_info.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 14:29:01 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/24 16:22:47 by nmontard         ###   ########.fr       */
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
	shared_info_t	*shared_info;
	pthread_mutex_t	lock;
	int				thread_ended;
	int				id;
	int				has_started;
}					thread_info_t;

#endif