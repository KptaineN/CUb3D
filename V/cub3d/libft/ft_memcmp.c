/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:40:43 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/25 05:54:11 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The memcmp function iterates n times through two array of bytes comparing them,
if it finds a difference it returns the difference between the two using
unsigned chars. If the two arrays are the same or if n = 0, it returns 0.
*/

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*first;
	const unsigned char	*second;
	size_t				i;

	first = s1;
	second = s2;
	i = 0;
	while (i < n)
	{
		if (first[i] != second[i])
			return (first[i] - second[i]);
		i++;
	}
	return (0);
}
