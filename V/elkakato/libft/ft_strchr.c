/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:41:04 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/23 16:39:33 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The strchr function looks for the first occurence of a specific character inside
an array of characters, if it finds it, it returns a pointer to
the address of it, if not it returns NULL
*/

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	to_find;

	i = 0;
	to_find = (char)c;
	while (s[i] != '\0')
	{
		if (s[i] == to_find)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == to_find)
		return ((char *)&s[i]);
	return (NULL);
}
