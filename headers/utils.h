#ifndef UTILS_H
# define UTILS_H
# include "stdlib.h"

void	*ft_calloc(size_t nmemb, size_t size);
void	join_threads(pthread_t *threads, int nb_threads);

#endif