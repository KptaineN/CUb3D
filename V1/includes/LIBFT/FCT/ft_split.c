/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 10:18:26 by gpochon           #+#    #+#             */
/*   Updated: 2025/08/13 14:00:47 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	word_count(const char *s, char c)
{
	int	count;
	int	is_word;

	count = 0;
	is_word = 0;
	if (s == NULL)
		return (0);
	while (*s)
	{
		if (*s != c && !is_word)
		{
			is_word = 1;
			count++;
		}
		if (*s == c)
			is_word = 0;
		s++;
	}
	return (count);
}

static char	*malloc_words(const char *s, char c)
{
	int		len;
	char	*word;
	int		i;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	word = (char *)malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[len] = '\0';
	return (word);
}

static void	free_malloc(char **split, int i)
{
	while (i >= 0)
	{
		free(split[i]);
		i--;
	}
	free(split);
}

char	**ft_split(char const *str, char c)
{
	int		word;
	char	**split;
	int		i;

	word = word_count(str, c);
	split = (char **)malloc(sizeof(char *) * (word + 1));
	if (!str || !split)
		return (NULL);
	i = 0;
	while (*str)
	{
		if (*str != c)
		{
			split[i] = malloc_words(str, c);
			if (!split[i++])
				return (free_malloc(split, i - 1), NULL);
			while (*str && *str != c)
				str++;
		}
		else
			str++;
	}
	split[i] = NULL;
	return (split);
}
/*#include "libft.h"
#include <stdio.h>

void print_split(char **split)
{
    if (!split)
    {
        printf("NULL\n");
        return;
    }

    for (int i = 0; split[i] != NULL; i++)
    {
        printf("split[%d]: \"%s\"\n", i, split[i]);
    }
}

int main(void)
{
    char **result;

    // Cas de test 1 : Chaîne normale avec un délimiteur
    result = ft_split("Hello world this is a test", ' ');
    printf("Test 1:\n");
    print_split(result);

    // Libérer la mémoire allouée
    for (int i = 0; result[i] != NULL; i++)
        free(result[i]);
    free(result);

    // Cas de test 2 : Chaîne avec plusieurs délimiteurs consécutifs
    result = ft_split("Hello  world   this is  a test", ' ');
    printf("Test 2:\n");
    print_split(result);

    // Libérer la mémoire allouée
    for (int i = 0; result[i] != NULL; i++)
        free(result[i]);
    free(result);

    // Cas de test 3 : Chaîne avec délimiteur au début et à la fin
    result = ft_split("  Hello world this is a test  ", ' ');
    printf("Test 3:\n");
    print_split(result);

    // Libérer la mémoire allouée
    for (int i = 0; result[i] != NULL; i++)
        free(result[i]);
    free(result);

    // Cas de test 4 : Chaîne vide
    result = ft_split("", ' ');
    printf("Test 4:\n");
    print_split(result);

    // Libérer la mémoire allouée
    for (int i = 0; result[i] != NULL; i++)
        free(result[i]);
    free(result);

    // Cas de test 5 : Chaîne sans délimiteur
    result = ft_split("HelloWorld", ' ');
    printf("Test 5:\n");
    print_split(result);

    // Libérer la mémoire allouée
    for (int i = 0; result[i] != NULL; i++)
        free(result[i]);
    free(result);

    // Cas de test 6 : Délimiteur qui n'existe pas dans la chaîne
    result = ft_split("HelloWorld", ',');
    printf("Test 6:\n");
    print_split(result);

    // Libérer la mémoire allouée
    for (int i = 0; result[i] != NULL; i++)
        free(result[i]);
    free(result);

    return 0;
}*/
