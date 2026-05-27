/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 04:31:35 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/26 15:05:46 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLEAN_H
# define CLEAN_H
# include "coder_state.h"
# include "thread_info.h"
# include <pthread.h>

void	free_coders_state(t_coder_state *coders_state, int size);
void	free_main(t_thread_info *thread_info, pthread_t *threads,
			t_config *config);
void	free_dongles(t_dongle *dongles, int size);
void	free_shared_info(t_shared_info *shared, int nb_coder);
#endif