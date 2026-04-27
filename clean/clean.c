#include "coder_state.h"
#include "thread_info.h"
#include <pthread.h>
#include <stdlib.h>

void	free_dongles(pthread_mutex_t *dongles, int size)
{
	int	i;

	i = 0;
	if (dongles != NULL)
	{
		while (i < size)
		{
			pthread_mutex_destroy(&(dongles[i]));
			i++;
		}
		free(dongles);
	}
}

void	free_coders_state(coder_state_t **coders_state)
{
	int	i;

	i = 0;
	if (coders_state != NULL)
	{
		while (coders_state[i] != NULL)
		{
			free(coders_state[i]);
			i++;
		}
		free(coders_state);
	}
}

void	free_thread_struct(thread_info_t **threads)
{
	int	i;

	i = 0;
	if (threads != NULL)
	{
		if (threads[0]->dongles != NULL)
			free_dongles(threads[0]->dongles, threads[0]->config->nb_coder);
		if (threads[0]->coders_states != NULL)
			free_coders_state(threads[0]->coders_states);
		while (threads[i] != NULL)
		{
			free(threads[i]);
			i++;
		}
		free(threads);
	}
}

void	free_main(thread_info_t **thread_info, pthread_t *threads,
		config_t *config)
{
	if (thread_info != NULL)
		free_thread_struct(thread_info);
	if (threads != NULL)
		free(threads);
	if (config != NULL)
		free(config);
}