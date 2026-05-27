/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:46:43 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/26 15:06:08 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DONGLE_H
# define DONGLE_H
# include "coder_state.h"
# include <pthread.h>
# include <sys/time.h>

typedef struct t_dongle
{
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	t_coder_state	*queue[2];
	int				available;
	int				queue_size;
	int				id;
	int				has_been_released;
	struct timeval	last_release;
}					t_dongle;

#endif