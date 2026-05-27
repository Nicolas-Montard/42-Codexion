/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif_number.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:20:59 by nmontard          #+#    #+#             */
/*   Updated: 2026/05/26 14:03:47 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

static int	number_is_int_positive(char *number)
{
	if (strlen(number) > 10)
		return (0);
	else if (strlen(number) == 10)
	{
		if (strncmp(number, "2147483647", 11) > 0)
			return (0);
	}
	return (1);
}

static int	is_number(char *number)
{
	int	i;

	i = 0;
	while (number[i] != '\0')
	{
		if (number[i] < '0' || number[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	verif_number(char *number, int is_zero_accepted)
{
	if (strlen(number) != 1 && number[0] == '0')
		return (0);
	if (!is_zero_accepted && number[0] == '0')
		return (0);
	if (!is_number(number))
		return (0);
	if (!number_is_int_positive(number))
		return (0);
	return (1);
}
