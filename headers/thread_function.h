/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_function.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:46:55 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/31 02:20:57 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_FUNCTION_H
# define THREAD_FUNCTION_H
# include "thread_info.h"
# include <pthread.h>

t_dongle		*get_first_dongle(t_thread_info *thread_info);
t_dongle		*get_second_dongle(t_thread_info *thread_info);
t_coder_state	*get_coder(t_thread_info *thread_info);
void			thread_print(char *str, t_thread_info *thread_info);
void			change_compile_start(t_thread_info *thread_info);
void			*thread_function(void *thread_info_void);
long			timeval_to_ms(struct timeval tv);
long			get_time_since_start(struct timeval tv);
void			take_dongles(t_thread_info *thread_info, t_dongle *dongles[2]);
void			release_dongle(t_dongle *dongle);
struct timespec	make_timespec(int timeout_ms);
void			compile(t_thread_info *thread_info);
void			debug(t_thread_info *thread_info);
void			refactoring(t_thread_info *thread_info);
void			queue_start(t_coder_state *coder,
					t_dongle *dongle, char *scheduler);
void			add_to_queue(t_coder_state *coder, t_dongle *dongle,
				char *scheduler);
void			pop_queue(t_dongle *dongle);
#endif
