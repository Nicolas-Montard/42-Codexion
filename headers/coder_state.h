/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_state.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:46:36 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/26 15:00:18 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODER_STATE_H
# define CODER_STATE_H
# include <pthread.h>
# include <sys/time.h>

typedef struct coder_state
{
	int				id;
	int				nb_compile;
	struct timeval	last_compile_start;
	pthread_mutex_t	lock_compile_start;
}					t_coder_state;

#endif