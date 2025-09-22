/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 13:06:55 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:00:33 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memmove(void *dest, const void *src, size_t len)
{
	char		*d;
	const char	*s;
	const char	*end_s;
	char		*end_d;

	if (dest == NULL && src == NULL && len > 0)
		return (NULL);
	d = dest;
	s = src;
	if (d < s)
	{
		while (len--)
		{
			*d++ = *s++;
		}
	}
	else
	{
		end_s = s + (len - 1);
		end_d = d + (len - 1);
		while (len--)
			*end_d-- = *end_s--;
	}
	return (dest);
}
