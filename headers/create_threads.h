#ifndef CREATE_THREADS_H
# define CREATE_THREADS_H
# include "pthread.h"
# include "thread_info.h"

thread_info_t	**create_threads_struct(config_t *config, int *error);
pthread_t		*create_threads(config_t *config, int *error,
					thread_info_t **thread_info);

#endif