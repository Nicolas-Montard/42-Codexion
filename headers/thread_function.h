#ifndef THREAD_FUNCTION_H
# define THREAD_FUNCTION_H
# include "thread_info.h"
# include <pthread.h>
pthread_mutex_t	get_first_dongle(thread_info_t *thread_info);
pthread_mutex_t	get_second_dongle(thread_info_t *thread_info);
coder_state_t	*get_coder(thread_info_t *thread_info);
void			thread_print(char *str, size_t time, int coder_id,
					pthread_mutex_t print_lock);
#endif