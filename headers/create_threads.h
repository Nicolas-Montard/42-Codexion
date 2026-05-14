#ifndef CREATE_THREADS_H
# define CREATE_THREADS_H
# include "pthread.h"
# include "thread_info.h"

thread_info_t	*create_threads_struct(config_t *config,
					shared_info_t *shared_info, int *error);
pthread_t		*create_threads(int *error, thread_info_t *threads_info);

#endif