/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 04:31:35 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/14 03:07:17 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLEAN_H
# define CLEAN_H
# include "coder_state.h"
# include "thread_info.h"
# include <pthread.h>

void	free_coders_state(coder_state_t *coders_state, int size);
void	free_main(thread_info_t *thread_info, pthread_t *threads,
			shared_info_t *shared, config_t *config, pthread_t monitor,
			int monitor_created);
void	free_dongles(pthread_mutex_t *dongles, int size);
void	free_shared_info(shared_info_t *shared, int nb_coder);
#endif