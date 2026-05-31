/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 02:25:34 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/31 02:25:36 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread_function.h"
#include <string.h>

void	queue_start(t_coder_state *coder, t_dongle *dongle, char *scheduler)
{
	pthread_mutex_lock(&dongle->lock);
	if (strcmp("fifo", scheduler) == 0)
		dongle->queue[dongle->queue_size] = coder;
	else
	{
		if (coder->id % 2 == 0)
		{
			if (dongle->queue[0] != NULL && dongle->queue[0]->id % 2 == 0)
				dongle->queue[1] = coder;
			else
				dongle->queue[0] = coder;
		}
		else
			dongle->queue[1] = coder;
	}
	dongle->queue_size += 1;
	pthread_mutex_unlock(&dongle->lock);
}

void	add_to_queue(t_coder_state *coder, t_dongle *dongle,
		char *scheduler)
{
	if (strcmp("fifo", scheduler) == 0)
		dongle->queue[dongle->queue_size] = coder;
	else
	{
		if (dongle->queue_size == 0)
			dongle->queue[0] = coder;
		else
		{
			if (timeval_to_ms(dongle->queue[0]->last_compile_start)
				> timeval_to_ms(coder->last_compile_start))
			{
				dongle->queue[1] = dongle->queue[0];
				dongle->queue[0] = coder;
			}
			else
				dongle->queue[1] = coder;
		}
	}
	dongle->queue_size += 1;
}

void	pop_queue(t_dongle *dongle)
{
	if (dongle->queue_size == 1)
		dongle->queue[0] = NULL;
	else
	{
		dongle->queue[0] = dongle->queue[1];
		dongle->queue[1] = NULL;
	}
	dongle->queue_size -= 1;
}
