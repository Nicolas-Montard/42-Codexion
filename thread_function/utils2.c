/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:38:24 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/26 15:40:43 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread_function.h"

long	timeval_to_ms(struct timeval tv)
{
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

// get left if even, right if odd, reverse if even and last
t_dongle	*get_first_dongle(t_thread_info *thread_info)
{
	if (thread_info->id % 2 == 0)
	{
		if (thread_info->id == thread_info->shared_info->config->nb_coder - 1)
			return (&thread_info->shared_info->dongles[0]);
		return (&thread_info->shared_info->dongles[thread_info->id]);
	}
	if (thread_info->id == thread_info->shared_info->config->nb_coder - 1)
		return (&thread_info->shared_info->dongles[0]);
	return (&thread_info->shared_info->dongles[thread_info->id + 1]);
}

// get right if even, left if odd, reverse if even and last
t_dongle	*get_second_dongle(t_thread_info *thread_info)
{
	if (thread_info->id % 2 == 0)
	{
		if (thread_info->id == thread_info->shared_info->config->nb_coder - 1)
			return (&thread_info->shared_info->dongles[thread_info->id]);
		return (&thread_info->shared_info->dongles[thread_info->id + 1]);
	}
	return (&thread_info->shared_info->dongles[thread_info->id]);
}

t_coder_state	*get_coder(t_thread_info *thread_info)
{
	return (&thread_info->shared_info->coders_states[thread_info->id]);
}

long	get_time_since_start(struct timeval tv)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (timeval_to_ms(now) - timeval_to_ms(tv));
}
