/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:03:32 by nmontard          #+#    #+#             */
/*   Updated: 2026/04/30 02:13:14 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"
#include "parsing.h"
#include <stdlib.h>
#include <string.h>

static int	verif_parameters(int nb_parameters, char *parameters[])
{
	int		i;
	char	*parameter;

	i = 0;
	while (i < nb_parameters)
	{
		parameter = parameters[i];
		if (i < nb_parameters - 1)
		{
			if (!verif_number(parameter))
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

static void	assign_parameters(char **parameters, config_t *config)
{
	config->nb_coder = atoi(parameters[0]);
	config->time_to_burnout = atoi(parameters[1]);
	config->time_to_compile = atoi(parameters[2]);
	config->time_to_debug = atoi(parameters[3]);
	config->time_to_refactor = atoi(parameters[4]);
	config->nb_compile_req = atoi(parameters[5]);
	config->dongle_cooldown = atoi(parameters[6]);
	config->scheduler = parameters[7];
}

config_t	*get_config(int nb_parameters, char *parameters[], int *error)
{
	config_t	*config;

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
	config = malloc(sizeof(config_t));
	if (config == NULL)
	{
		*error = 2;
		return (NULL);
	}
	assign_parameters(parameters, config);
	return (config);
}
