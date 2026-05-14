#ifndef CODER_STATE_H
# define CODER_STATE_H
# include <pthread.h>
# include <sys/time.h>
typedef struct coder_state
{
	int				id;
	int				nb_compile;
	struct timeval	last_compile_start;
	pthread_mutex_t	lock_compile_start;
}					coder_state_t;

#endif