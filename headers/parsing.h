/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:21:17 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/26 15:06:47 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H
# include <config.h>

int			verif_number(char *number, int is_zero_accepted);
t_config	*get_config(int nb_parameters, char *parameters[], int *error);
#endif