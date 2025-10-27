/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:41:39 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/28 18:28:02 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The strlcpy functions copies a src array of chars into a dest array of chars
respecting the size given to dest, including a space for the null terminator.
*/

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	src_l;
	size_t	i;

	src_l = ft_strlen(src);
	i = 0;
	if (size == 0)
		return (src_l);
	while (src[i] != '\0' && i < size - 1)
	{
		dest[i] = src[i];
		i++;
	}
	if (size > 0)
		dest[i] = '\0';
	return (src_l);
}
