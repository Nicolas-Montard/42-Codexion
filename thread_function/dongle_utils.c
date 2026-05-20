#include "dongle.h"
#include "thread_function.h"
#include "thread_info.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static void	add_to_queue(coder_state_t *coder, dongle_t *dongle,
		char *scheduler)
{
	if (strcmp("fifo", scheduler))
		dongle->queue[dongle->queue_size] = coder;
	else
	{
		if (dongle->queue_size == 0)
			dongle->queue[0] = coder;
		else
		{
			if (timeval_to_ms(dongle->queue[0]->last_compile_start) > timeval_to_ms(coder->last_compile_start))
			{
				dongle->queue[1] = dongle->queue[0];
				dongle->queue[0] = coder;
			}
			else
			{
				dongle->queue[1] = coder;
			}
		}
	}
	dongle->queue_size += 1;
}

static void	pop_queue(dongle_t *dongle)
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

void	release_dongle(dongle_t *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	gettimeofday(&dongle->last_release, NULL);
	dongle->has_been_released = 1;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
}

void	take_dongle(thread_info_t *thread_info, dongle_t *dongle)
{
	coder_state_t	*coder;

	coder = get_coder(thread_info);
	pthread_mutex_lock(&dongle->lock);
	add_to_queue(coder, dongle, thread_info->shared_info->config->scheduler);
	while ((dongle->queue[0] != coder || dongle->available == 0)
		&& thread_info->shared_info->simulation_ended == 0)
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	if (thread_info->shared_info->simulation_ended == 1)
	{
		pthread_mutex_unlock(&dongle->lock);
		dongle->queue_size -= 1;
		return ;
	}
	dongle->available = 0;
	pthread_mutex_unlock(&dongle->lock);
	pop_queue(dongle);
}
