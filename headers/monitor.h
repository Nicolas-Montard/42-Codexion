/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 15:53:36 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/26 15:06:31 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MONITOR_H
# define MONITOR_H

pthread_t	create_monitor(int *error, t_thread_info *threads_info);
void		print_burnout(t_thread_info *thread_info, int i);
int			is_burned_out(t_thread_info *thread_info, int i, long now_ms);
int			check_burnout(t_thread_info *thread_info, int nb_coder);
void		release_dongle_neighbors(t_dongle *dongles, int i, int nb_dongles);
void		check_dongle_cooldown(t_dongle *dongles, t_config *config);
#endif