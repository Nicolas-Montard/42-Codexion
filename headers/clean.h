/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 04:31:35 by nmontard          #+#    #+#             */
/*   Updated: 2026/04/30 05:18:18 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLEAN_H
# define CLEAN_H
# include "coder_state.h"
# include "thread_info.h"
# include <pthread.h>

void	free_coders_state(coder_state_t **coders_state);
void	free_thread_struct(thread_info_t **threads);
void	free_main(thread_info_t **thread_info, pthread_t *threads,
			config_t *config);
void	free_dongles(pthread_mutex_t **dongles, int size);
#endif