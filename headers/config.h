/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 13:08:01 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/06 14:09:21 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H
# include <sys/time.h>
typedef struct config
{
	int				nb_coder;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				nb_compile_req;
	int				dongle_cooldown;
	char			*scheduler;
	struct timeval	started_at;
}					config_t;
#endif