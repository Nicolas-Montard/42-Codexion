/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:20:55 by nmontard          #+#    #+#             */
/*   Updated: 2026/04/20 16:11:28 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"
#include "error_manager.h"
#include "parsing.h"
#include <stdio.h>
#include <string.h>

int	main(int argc, char *argv[])
{
	config_t *config;
	int error;

	error = 0;
	config = get_config(argc - 1, (argv + 1), &error);
	if (error != 0)
		return (error_manager(error));
	return (0);
}