/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:40:52 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/27 20:33:59 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The memmove function copies n bytes from an array to another, ensuring that
there is no overlapping between the two. It does that with the first if statement
that checks if there is an overlapping, in which case it copies the bytes
backwards instead of foreword.
*/

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;

	d = dest;
	s = (unsigned char *) src;
	if (dest < src)
		return (ft_memcpy(dest, src, n));
	if (dest > src)
	{
		while (n > 0)
		{
			n--;
			d[n] = s[n];
		}
	}
	return (dest);
}
