#include "thread_function.h"
#include "thread_info.h"
#include <stdio.h>
#include <unistd.h>

static void	compile(thread_info_t *thread_info)
{
	dongle_t		*first_dongle;
	dongle_t		*second_dongle;
	coder_state_t	*coder;

	coder = get_coder(thread_info);
	first_dongle = get_first_dongle(thread_info);
	second_dongle = get_second_dongle(thread_info);
	if (thread_info->shared_info->simulation_ended == 1)
		return ;
	take_dongle(thread_info, first_dongle);
	if (thread_info->shared_info->simulation_ended == 1)
	{
		release_dongle(first_dongle);
		return ;
	}
	thread_print("has taken a dongle", thread_info);
	if (first_dongle == second_dongle)
		usleep((thread_info->shared_info->config->time_to_burnout + 20) * 1000);
	take_dongle(thread_info, second_dongle);
	if (thread_info->shared_info->simulation_ended == 1)
	{
		release_dongle(first_dongle);
		release_dongle(second_dongle);
		return ;
	}
	thread_print("has taken a dongle", thread_info);
	change_compile_start(thread_info);
	thread_print("is compiling", thread_info);
	usleep(thread_info->shared_info->config->time_to_compile * 1000);
	coder->nb_compile += 1;
	release_dongle(first_dongle);
	release_dongle(second_dongle);
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
	if (coder->id % 2 == 0)
		usleep(500);
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
			thread_info->thread_ended = 1;
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
