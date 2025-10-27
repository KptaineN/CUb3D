/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:40:55 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/28 15:22:06 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The memset function takes an array of bytes and fills it
n times with a character, in order to do so we use unsigned
chars since they take a single byte in memory.
*/

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*ptr;
	int				i;

	ptr = s;
	i = 0;
	while (n > 0)
	{
		ptr[i] = (unsigned char)c;
		i++;
		n--;
	}
	return (s);
}
