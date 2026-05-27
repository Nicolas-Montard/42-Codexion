/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_manager.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:46:46 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/26 15:36:28 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_MANAGER_H
# define ERROR_MANAGER_H
# include "config.h"
# include "thread_info.h"
# include <pthread.h>

int	error_manager(int error, t_thread_info *thread_info, pthread_t *threads,
		t_config *config);

#endif