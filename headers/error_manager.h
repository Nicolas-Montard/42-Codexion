#ifndef ERROR_MANAGER_H
# define ERROR_MANAGER_H
# include "config.h"
# include "thread_info.h"
# include <pthread.h>

int	error_manager(int error, thread_info_t *thread_info, pthread_t *threads,
		config_t *config, pthread_t monitor, int monitor_created);

#endif