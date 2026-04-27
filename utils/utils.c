/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 15:01:38 by nmontard          #+#    #+#             */
/*   Updated: 2026/04/22 15:15:05 by nmontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*result;
	char	*ptr;
	size_t	i;
	size_t	total;

	total = nmemb * size;
	result = malloc(total);
	if (!result)
		return (NULL);
	ptr = (char *)result;
	i = 0;
	while (i < total)
	{
		ptr[i] = 0;
		i++;
	}
	return (result);
}
