/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_stock.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:48:47 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/26 16:01:04 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADS_STOCK_H
# define THREADS_STOCK_H
# include <pthread.h>

typedef struct threads_stock
{
	pthread_t	*threads;
	pthread_t	monitor;
}	t_threads_stock;

#endif