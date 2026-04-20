/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:29:01 by nmontard          #+#    #+#             */
/*   Updated: 2026/04/20 16:06:29 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

/*
error value indicate the type of error:
1: wrong number of parameters
2: a malloc has failed
3: invalid value of parameters
*/
void	free_all(void)
{
	// TODO
}

int	error_manager(int error)
{
	fprintf(stderr, "Error: ");
	if (error == 1)
		fprintf(stderr, "There isn´t the right number of parameters\n");
	if (error == 2)
		fprintf(stderr, "A malloc has failed\n");
	if (error == 3)
		fprintf(stderr, "The parameters have invalid value\n");
	free_all();
	return (1);
}