#include "coder_state.h"
#include "pthread.h"
#include "thread_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

long	timeval_to_ms(struct timeval tv)
{
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

// get left if even, right if odd, reverse if even and last
dongle_t	*get_first_dongle(thread_info_t *thread_info)
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
dongle_t	*get_second_dongle(thread_info_t *thread_info)
{
	if (thread_info->id % 2 == 0)
	{
		if (thread_info->id == thread_info->shared_info->config->nb_coder - 1)
			return (&thread_info->shared_info->dongles[thread_info->id]);
		return (&thread_info->shared_info->dongles[thread_info->id + 1]);
	}
	return (&thread_info->shared_info->dongles[thread_info->id]);
}

coder_state_t	*get_coder(thread_info_t *thread_info)
{
	return (&thread_info->shared_info->coders_states[thread_info->id]);
}

long	get_time_since_start(struct timeval tv)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (timeval_to_ms(now) - timeval_to_ms(tv));
}

void	thread_print(char *str, thread_info_t *thread_info)
{
	pthread_mutex_lock(&thread_info->shared_info->simulation_lock);
	if (thread_info->shared_info->simulation_ended == 1)
	{
		pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
		return ;
	}
	pthread_mutex_lock(&thread_info->shared_info->print_lock);
	printf("%ld %d %s\n",
		get_time_since_start(thread_info->shared_info->config->started_at),
		thread_info->id + 1, str);
	pthread_mutex_unlock(&thread_info->shared_info->print_lock);
	pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
}

void	change_compile_start(thread_info_t *thread_info)
{
	struct timeval	now;
	coder_state_t	*coder;

	coder = get_coder(thread_info);
	gettimeofday(&now, NULL);
	pthread_mutex_lock(&coder->lock_compile_start);
	coder->last_compile_start = now;
	pthread_mutex_unlock(&coder->lock_compile_start);
}

struct timespec	make_timespec(int timeout_ms)
{
	struct timeval tv;
	struct timespec time_spec;

	gettimeofday(&tv, NULL);

	tv.tv_sec += timeout_ms / 1000;
	tv.tv_usec += (timeout_ms % 1000) * 1000;

	if (tv.tv_usec >= 1000000)
	{
		tv.tv_sec += 1;
		tv.tv_usec -= 1000000;
	}

	time_spec.tv_sec = tv.tv_sec;
	time_spec.tv_nsec = tv.tv_usec * 1000;

	return (time_spec);
}