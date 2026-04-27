#ifndef CODER_STATE_H
# define CODER_STATE_H
# include <sys/time.h>
typedef struct coder_state
{
	int				id;
	struct timeval	last_compile_start;
}					coder_state_t;

#endif