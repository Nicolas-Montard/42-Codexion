#include "dongle.h"
#include "thread_function.h"
#include "thread_info.h"
#include <pthread.h>
#include <stdio.h>

static void	add_to_queue(coder_state_t *coder, dongle_t *dongle)
{
	dongle->queue[dongle->queue_size] = coder;
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
	dongle->available = 1;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
}

void	take_dongle(thread_info_t *thread_info, dongle_t *dongle)
{
	coder_state_t *coder;

	coder = get_coder(thread_info);
	pthread_mutex_lock(&dongle->lock);
	add_to_queue(coder, dongle);
	while (dongle->queue[0] != coder || dongle->available == 0)
	{
		if (thread_info->shared_info->simulation_ended == 1)
		{
			pop_queue(dongle);
			pthread_mutex_unlock(&dongle->lock);
			return ;
		}
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	}
	dongle->available = 0;
	pop_queue(dongle);
	pthread_mutex_unlock(&dongle->lock);
}