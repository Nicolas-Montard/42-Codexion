#include "thread_function.h"
#include "thread_info.h"
#include <unistd.h>

static void	compile(thread_info_t *thread_info)
{
	pthread_mutex_t	first_dongle;
	pthread_mutex_t	second_dongle;
	coder_state_t	*coder;

	coder = get_coder(thread_info);
	first_dongle = get_first_dongle(thread_info);
	second_dongle = get_second_dongle(thread_info);
	pthread_mutex_lock(&first_dongle);
	thread_print("has taken a dongle", thread_info);
	pthread_mutex_lock(&second_dongle);
	thread_print("has taken a dongle", thread_info);
	change_compile_start(thread_info);
	thread_print("is compiling", thread_info);
	usleep(thread_info->config->time_to_compile * 1000);
	pthread_mutex_unlock(&first_dongle);
	pthread_mutex_unlock(&second_dongle);
}

static void	debug(thread_info_t *thread_info)
{
	usleep(thread_info->config->time_to_debug * 1000);
	thread_print("is debugging", thread_info);
}

static void	refactoring(thread_info_t *thread_info)
{
	usleep(thread_info->config->time_to_refactor * 1000);
	thread_print("is refactoring", thread_info);
}

void	*thread_function(void *thread_info_void)
{
	thread_info_t	*thread_info;

	thread_info = (thread_info_t *)thread_info_void;
	compile(thread_info);
	debug(thread_info);
	refactoring(thread_info);
	return (NULL);
}
