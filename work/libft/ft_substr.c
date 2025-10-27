/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:50:44 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/28 18:37:06 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The substr function takes an array of char as a parameter,
	an index and e lenght.
It returns a substring of the main string that start at the index and that has
a lenght of len. It has to take in consideration the various exceptions
such as an empty string, an index that exceeds the string lenght and so on.
*/

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*final;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		len = 0;
	else if (len > s_len - start)
		len = s_len - start;
	final = (char *)malloc(sizeof(char) * (len + 1));
	if (!final)
		return (NULL);
	if (len > 0)
		ft_strlcpy(final, s + start, len + 1);
	else
		final[0] = '\0';
	return (final);
}
