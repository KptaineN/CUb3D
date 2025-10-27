/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:42:37 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/23 16:41:56 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The strchr function looks for the last occurence of a
specific character inside an array of characters,
if it finds it, it returns a pointer to the address of
it, if not it returns NULL
*/

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	to_find;
	char	*result;

	i = 0;
	to_find = (char)c;
	result = NULL;
	while (s[i] != '\0')
	{
		if (s[i] == to_find)
			result = (char *)&s[i];
		i++;
	}
	if (s[i] == to_find)
		return ((char *)&s[i]);
	return (result);
}
