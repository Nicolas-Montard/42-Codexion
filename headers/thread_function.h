#ifndef THREAD_FUNCTION_H
# define THREAD_FUNCTION_H
# include "thread_info.h"
# include <pthread.h>
dongle_t		*get_first_dongle(thread_info_t *thread_info);
dongle_t		*get_second_dongle(thread_info_t *thread_info);
coder_state_t	*get_coder(thread_info_t *thread_info);
void			thread_print(char *str, thread_info_t *thread_info);
void			change_compile_start(thread_info_t *thread_info);
void			*thread_function(void *thread_info_void);
long			timeval_to_ms(struct timeval tv);
long			get_time_since_start(struct timeval tv);
void			take_dongle(thread_info_t *thread_info, dongle_t *dongle);
void			release_dongle(dongle_t *dongle);
#endif
