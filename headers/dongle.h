#ifndef DONGLE_H
# define DONGLE_H
# include "coder_state.h"
# include <pthread.h>
typedef struct dongle
{
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	coder_state_t	*queue[2];
	int				available;
	int				queue_size;
}					dongle_t;

#endif