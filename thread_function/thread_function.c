#include "thread_function.h"
#include "thread_info.h"

void	*thread_function(void *thread_info_void)
{
	thread_info_t	*thread_info;
	pthread_mutex_t	first_dongle;
	pthread_mutex_t	second_dongle;
	coder_state_t	*coder;

	thread_info = (thread_info_t *)thread_info_void;
	first_dongle = get_first_dongle(thread_info);
	second_dongle = get_second_dongle(thread_info);
	coder = get_coder(thread_info);
	pthread_mutex_lock(&first_dongle);
	// change calcul second
	thread_print("has taken a dongle", coder->last_compile_start.tv_usec / 1000,
		thread_info->id, thread_info->print_lock);
	pthread_mutex_lock(&second_dongle);
	thread_print("has taken a dongle", coder->last_compile_start.tv_usec / 1000,
		thread_info->id, thread_info->print_lock);
}
