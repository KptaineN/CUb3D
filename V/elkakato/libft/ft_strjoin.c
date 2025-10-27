/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 15:08:55 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/28 15:27:41 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The strjoin function takes two arrays of chars and joins them in a new array
that needs to have enough memory to accepts both strings. The function returns
NULL if one of the string is empty of if the allocation fails.
*/

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*final_string;
	size_t	total_size;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	total_size = ft_strlen(s1) + ft_strlen(s2);
	final_string = (char *)malloc(sizeof(char) * (total_size + 1));
	if (!final_string)
		return (NULL);
	while (s1[i] != '\0')
	{
		final_string[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		final_string[i++] = s2[j++];
	}
	final_string[i] = '\0';
	return (final_string);
}
