/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:40:47 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/25 05:54:19 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The memcpy function takes a source array of bytes and copies the first n bytes
of it into the dest pointer. In order to do so we use unsigned chars
wich only take 1 byte.
*/

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;
	size_t			i;

	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}
