/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 14:41:51 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:00:25 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memchr(const void *str, int c, size_t n)
{
	const unsigned char	*ptr;
	unsigned char		un_c;

	ptr = (const unsigned char *) str;
	un_c = (unsigned char) c;
	while (n--)
	{
		if (*ptr == un_c)
		{
			return ((void *)ptr);
		}
		ptr++;
	}
	return (NULL);
}
