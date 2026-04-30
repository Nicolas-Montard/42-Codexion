#include "coder_state.h"
#include "pthread.h"
#include "thread_info.h"
#include <stdlib.h>

// get left dongle unless last coder
pthread_mutex_t	get_first_dongle(thread_info_t *thread_info)
{
	if (thread_info->id + 1 != thread_info->config->nb_coder)
		return (thread_info->dongles[thread_info->id]);
	else
		return (thread_info->dongles[0]);
}

// get right dongle unless last coder
pthread_mutex_t	get_second_dongle(thread_info_t *thread_info)
{
	if (thread_info->id + 1 != thread_info->config->nb_coder)
		return (thread_info->dongles[thread_info->id + 1]);
	else
		return (thread_info->dongles[thread_info->id]);
}

coder_state_t	*get_coder(thread_info_t *thread_info)
{
	return (thread_info->coders_states[thread_info->id]);
}

void	thread_print(char *str, size_t time, int thread_id,
		pthread_mutex_t print_lock)
{
	pthread_mutex_lock(&print_lock);
	printf("%d %d %s", time, thread_id, str);
	pthread_mutex_unlock(&print_lock);
}