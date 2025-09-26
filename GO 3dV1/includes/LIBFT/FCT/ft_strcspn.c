/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcspn.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:01:19 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 15:01:25 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	ft_strcspn(const char *s, const char *reject)
{
	size_t		count;
	const char	*p;
	const char	*r;

	count = 0;
	p = s;
	while (*p != '\0')
	{
		r = reject;
		while (*r != '\0')
		{
			if (*p == *r)
			{
				return (count);
			}
			r++;
		}
		count++;
		p++;
	}
	return (count);
}
