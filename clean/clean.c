#include "coder_state.h"
#include "thread_info.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void	free_dongles(dongle_t *dongles, int size)
{
	int	i;

	i = 0;
	if (dongles != NULL)
	{
		while (i < size)
		{
			pthread_cond_destroy(&dongles[i].cond);
			pthread_mutex_destroy(&dongles[i].lock);
			i++;
		}
		free(dongles);
	}
}

void	free_coders_state(coder_state_t *coders_state, int size)
{
	int	i;

	i = 0;
	if (coders_state != NULL)
	{
		while (i < size)
		{
			pthread_mutex_destroy(&coders_state[i].lock_compile_start);
			i++;
		}
		free(coders_state);
	}
}

void	free_shared_info(shared_info_t *shared, int nb_coder)
{
	pthread_mutex_destroy(&shared->print_lock);
	pthread_mutex_destroy(&shared->simulation_lock);
	if (shared->dongles != NULL)
		free_dongles(shared->dongles, nb_coder);
	if (shared->coders_states != NULL)
		free_coders_state(shared->coders_states, nb_coder);
}

void	free_threads_info(thread_info_t *threads, int nb_coder)
{
	int	i;

	i = 0;
	while (i < nb_coder)
	{
		pthread_mutex_destroy(&threads[i].lock);
		i++;
	}
	free(threads);
}

void	free_main(thread_info_t *thread_info, pthread_t *threads,
		shared_info_t *shared, config_t *config)
{
	int	i;
	int	nb_coder;

	nb_coder = 0;
	if (config != NULL)
		nb_coder = config->nb_coder;
	i = 0;
	if (threads != NULL)
		free(threads);
	if (thread_info != NULL)
		free_threads_info(thread_info, nb_coder);
	if (shared != NULL)
		free_shared_info(shared, nb_coder);
	if (config != NULL)
		free(config);
}
