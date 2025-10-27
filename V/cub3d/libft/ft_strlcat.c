/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:41:45 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/28 15:22:42 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The strlcat function concatenates a src array of chars into a dest
array of chars and null terminates it as long as size isn't reached.
It then returns the sum of the src lenght and the destination lenght.
*/

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_l;
	size_t	src_l;
	size_t	i;

	i = 0;
	src_l = ft_strlen(src);
	while (dst[i] != '\0' && i < size)
		i++;
	dst_l = i;
	if (size == 0)
		return (src_l);
	while (src[i - dst_l] != '\0' && i < size - 1)
	{
		dst[i] = src[i - dst_l];
		i++;
	}
	if (dst_l < size)
		dst[i] = '\0';
	return (dst_l + src_l);
}
