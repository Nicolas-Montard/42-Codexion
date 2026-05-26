#ifndef SHARED_INFO_H
# define SHARED_INFO_H
# include "coder_state.h"
# include "config.h"
# include "dongle.h"
# include <pthread.h>

typedef struct shared_info
{
	pthread_mutex_t	print_lock;
	pthread_mutex_t	simulation_lock;
	int				simulation_ended;
	dongle_t		*dongles;
	config_t		*config;
	coder_state_t	*coders_states;
	pthread_cond_t	can_start_cond;
	int				can_start;
}					shared_info_t;

#endif