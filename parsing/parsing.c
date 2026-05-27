/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:03:32 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/26 15:12:32 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"
#include "parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

static int	verif_parameters(int nb_parameters, char *parameters[])
{
	int		i;
	char	*parameter;

	i = 0;
	while (i < nb_parameters)
	{
		parameter = parameters[i];
		if (i == 0)
		{
			if (!verif_number(parameter, 0))
				return (0);
		}
		else if (i < nb_parameters - 1)
		{
			if (!verif_number(parameter, 1))
				return (0);
		}
		else
		{
			if (strcmp(parameter, "fifo") != 0 && strcmp(parameter, "edf") != 0)
				return (0);
		}
		i++;
	}
	return (1);
}

static void	assign_parameters(char **parameters, t_config *config)
{
	config->nb_coder = atoi(parameters[0]);
	config->time_to_burnout = atoi(parameters[1]);
	config->time_to_compile = atoi(parameters[2]);
	config->time_to_debug = atoi(parameters[3]);
	config->time_to_refactor = atoi(parameters[4]);
	config->nb_compile_req = atoi(parameters[5]);
	config->dongle_cooldown = atoi(parameters[6]);
	config->scheduler = parameters[7];
	gettimeofday(&config->started_at, NULL);
}

t_config	*get_config(int nb_parameters, char *parameters[], int *error)
{
	t_config	*config;

	if (nb_parameters != 8)
	{
		*error = 1;
		return (NULL);
	}
	if (!verif_parameters(nb_parameters, parameters))
	{
		*error = 3;
		return (NULL);
	}
	config = malloc(sizeof(t_config));
	if (config == NULL)
	{
		*error = 2;
		return (NULL);
	}
	assign_parameters(parameters, config);
	return (config);
}
