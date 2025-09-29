/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:55:15 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/28 18:37:00 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	count_words(char const *s, char c)
{
	size_t	word_count;
	size_t	i;

	word_count = 0;
	i = 0;
	while (s[i] != '\0')
	{
		while (s[i] != '\0' && s[i] == c)
			i++;
		while (s[i] != '\0' && s[i] != c)
		{
			word_count++;
			while (s[i] != '\0' && s[i] != c)
				i++;
		}
	}
	return (word_count);
}

int	safe_m(char **splitted, int pos, size_t len)
{
	int	i;

	i = 0;
	splitted[pos] = malloc(len);
	if (!splitted[pos])
	{
		while (i < pos)
			free(splitted[i++]);
		free(splitted);
		return (1);
	}
	return (0);
}

int	filler(char **splitted, char const *s, char c)
{
	size_t	i;
	size_t	len;
	int		j;

	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		len = 0;
		while (s[i] != '\0' && s[i] == c)
			i++;
		while (s[i] != '\0' && s[i] != c)
		{
			len++;
			i++;
		}
		if (len > 0)
		{
			if (safe_m(splitted, j, len + 1))
				return (1);
			ft_strlcpy(splitted[j], s + i - len, len + 1);
			j++;
		}
	}
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char	**splitted;
	size_t	word_count;

	if (!s)
		return (NULL);
	word_count = 0;
	word_count = count_words(s, c);
	splitted = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!splitted)
		return (NULL);
	splitted[word_count] = NULL;
	if (filler(splitted, s, c))
		return (NULL);
	return (splitted);
}
