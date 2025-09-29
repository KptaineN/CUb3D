/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:19:02 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/28 16:07:00 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The strtrim function takes a set of chars and removes them from the start and end
of the given array of chars. It checks for empty strings and failes memory
allocations.
*/

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	size_t	len;
	char	*trimmed_s;

	start = 0;
	if (!s1 || !set)
		return (NULL);
	end = ft_strlen(s1);
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	while (end > start && ft_strchr(set, s1[end - 1]))
		end--;
	len = end - start;
	trimmed_s = (char *)malloc(sizeof(char) * (len + 1));
	if (!trimmed_s)
		return (NULL);
	ft_strlcpy(trimmed_s, (s1 + start), (len + 1));
	return (trimmed_s);
}
