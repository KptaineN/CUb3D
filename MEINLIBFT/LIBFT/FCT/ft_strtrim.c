/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 16:34:03 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:02:43 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);

static int	ft_check_set(char const c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	fin;
	size_t	front;
	size_t	len_trimmed;
	char	*trimar;

	if (!s1 || !set)
		return (NULL);
	front = 0;
	fin = ft_strlen(s1);
	while (s1[front] && ft_check_set(s1[front], set))
		front++;
	while (fin > front && ft_check_set(s1[fin - 1], set))
		fin--;
	len_trimmed = fin - front;
	trimar = (char *)malloc(sizeof(char) * (len_trimmed + 1));
	if (!trimar)
		return (NULL);
	ft_strlcpy(trimar, &s1[front], len_trimmed + 1);
	trimar[len_trimmed] = '\0';
	return (trimar);
}
/*#include "libft.h"
#include <stdio.h>

int main(void)
{
    char const *s1 = "  Hello, world!  ";
    char const *set = " ";
    char *result;

    // Utilisation de ft_strtrim
    result = ft_strtrim(s1, set);
    if (!result)
    {
        perror("ft_strtrim");
        return 1;
    }

    // Affichage des résultats
    printf("Original string: \"%s\"\n", s1);
    printf("Set of characters to trim: \"%s\"\n", set);
    printf("Trimmed string: \"%s\"\n", result);

    // Libération de la mémoire allouée
    free(result);

    // Autres cas de test
    s1 = "xxHello, world!xx";
    set = "x";
    result = ft_strtrim(s1, set);
    if (!result)
    {
        perror("ft_strtrim");
        return 1;
    }
    printf("\nOriginal string: \"%s\"\n", s1);
    printf("Set of characters to trim: \"%s\"\n", set);
    printf("Trimmed string: \"%s\"\n", result);
    free(result);

    s1 = "Hello, world!";
    set = " ";
    result = ft_strtrim(s1, set);
    if (!result)
    {
        perror("ft_strtrim");
        return 1;
    }
    printf("\nOriginal string: \"%s\"\n", s1);
    printf("Set of characters to trim: \"%s\"\n", set);
    printf("Trimmed string: \"%s\"\n", result);
    free(result);

    s1 = "  ";
    set = " ";
    result = ft_strtrim(s1, set);
    if (!result)
    {
        perror("ft_strtrim");
        return 1;
    }
    printf("\nOriginal string: \"%s\"\n", s1);
    printf("Set of characters to trim: \"%s\"\n", set);
    printf("Trimmed string: \"%s\"\n", result);
    free(result);

    return 0;
}*/