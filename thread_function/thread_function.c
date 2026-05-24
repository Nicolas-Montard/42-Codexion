#include "thread_function.h"
#include "thread_info.h"
#include <stdio.h>
#include <unistd.h>

static void	compile(thread_info_t *thread_info)
{
	dongle_t		*dongles[2];
	coder_state_t	*coder;

	coder = get_coder(thread_info);
	dongles[0] = get_first_dongle(thread_info);
	dongles[1] = get_second_dongle(thread_info);
	if (dongles[0] == dongles[1])
		usleep((thread_info->shared_info->config->time_to_burnout + 20) * 1000);
	if (thread_info->shared_info->simulation_ended == 1)
		return ;
	take_dongles(thread_info, dongles);
	if (thread_info->shared_info->simulation_ended == 1)
	{
		release_dongle(dongles[0]);
		release_dongle(dongles[1]);
		return ;
	}
	thread_print("has taken a dongle", thread_info);
	thread_print("has taken a dongle", thread_info);
	change_compile_start(thread_info);
	thread_print("is compiling", thread_info);
	usleep(thread_info->shared_info->config->time_to_compile * 1000);
	coder->nb_compile += 1;
	release_dongle(dongles[0]);
	release_dongle(dongles[1]);
}

static void	debug(thread_info_t *thread_info)
{
	pthread_mutex_lock(&thread_info->shared_info->simulation_lock);
	if (thread_info->shared_info->simulation_ended != 1)
	{
		pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
		thread_print("is debugging", thread_info);
		usleep(thread_info->shared_info->config->time_to_debug * 1000);
	}
	else
		pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
}

static void	refactoring(thread_info_t *thread_info)
{
	pthread_mutex_lock(&thread_info->shared_info->simulation_lock);
	if (thread_info->shared_info->simulation_ended != 1)
	{
		pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
		thread_print("is refactoring", thread_info);
		usleep(thread_info->shared_info->config->time_to_refactor * 1000);
	}
	else
		pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
}

void	*thread_function(void *thread_info_void)
{
	thread_info_t	*thread_info;
	coder_state_t	*coder;

	thread_info = (thread_info_t *)thread_info_void;
	coder = get_coder(thread_info);
	if (coder->id % 2 == 1)
		usleep(800);
	pthread_mutex_lock(&thread_info->lock);
	thread_info->has_started = 1;
	pthread_mutex_unlock(&thread_info->lock);
	pthread_mutex_lock(&coder->lock_compile_start);
	gettimeofday(&coder->last_compile_start, NULL);
	pthread_mutex_unlock(&coder->lock_compile_start);
	pthread_mutex_lock(&thread_info->shared_info->simulation_lock);
	while (thread_info->shared_info->simulation_ended != 1)
	{
		pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
		if (coder->nb_compile < thread_info->shared_info->config->nb_compile_req)
			compile(thread_info);
		else
		{
			pthread_mutex_lock(&thread_info->lock);
			thread_info->thread_ended = 1;
			pthread_mutex_unlock(&thread_info->lock);
			pthread_mutex_lock(&thread_info->shared_info->simulation_lock);
			break ;
		}
		debug(thread_info);
		refactoring(thread_info);
		pthread_mutex_lock(&thread_info->shared_info->simulation_lock);
	}
	pthread_mutex_unlock(&thread_info->shared_info->simulation_lock);
	return (NULL);
}
