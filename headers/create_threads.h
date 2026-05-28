/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_threads.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:46:40 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/28 05:12:42 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CREATE_THREADS_H
# define CREATE_THREADS_H
# include "pthread.h"
# include "thread_info.h"

t_thread_info	*create_threads_struct(t_config *config,
					t_shared_info *shared_info, int *error);
int				init_coder_state(t_coder_state *coder_state, int id);
t_coder_state	*create_coders(int nb_coder, int *error);
int				init_dongle(t_dongle *dongles, int i, int *error);
t_dongle		*create_dongles(int nb_coder, int *error);
pthread_t		*create_threads(int *error, t_thread_info *threads_info);

#endif