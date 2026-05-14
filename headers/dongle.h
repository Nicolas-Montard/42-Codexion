#ifndef DONGLE_H
# define DONGLE_H
# include <pthread.h>
typedef struct dongle
{
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	int				queue[2];
	int				queue_size;
	int				available;
}					dongle_t;

#endif