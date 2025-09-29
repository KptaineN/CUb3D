/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 14:22:32 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:02:45 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	size_t			len_str;
	char			*str;

	if (!s)
		return (NULL);
	len_str = ft_strlen(s);
	if (start >= len_str)
		return (ft_strdup("\0"));
	if (len > len_str - start)
		len = len_str - start;
	str = ft_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
/*#include "libft.h"
#include <stdio.h>

int main(void)
{
    char const *s = "Hello, world!";
    unsigned int start = 7;
    size_t len = 5;
    char *result;

    // Utilisation de ft_substr
    result = ft_substr(s, start, len);
    if (!result)
    {
        perror("ft_substr");
        return 1;
    }

    // Affichage des résultats
    printf("Original string: \"%s\"\n", s);
    printf("Start index: %u\n", start);
    printf("Length: %zu\n", len);
    printf("Substring: \"%s\"\n", result);

    // Libération de la mémoire allouée
    free(result);

    // Autres cas de test
    s = "Hello, world!";
    start = 0;
    len = 5;
    result = ft_substr(s, start, len);
    if (!result)
    {
        perror("ft_substr");
        return 1;
    }
    printf("\nOriginal string: \"%s\"\n", s);
    printf("Start index: %u\n", start);
    printf("Length: %zu\n", len);
    printf("Substring: \"%s\"\n", result);
    free(result);

    s = "Hello, world!";
    start = 13;
    len = 5;
    result = ft_substr(s, start, len);
    if (!result)
    {
        perror("ft_substr");
        return 1;
    }
    printf("\nOriginal string: \"%s\"\n", s);
    printf("Start index: %u\n", start);
    printf("Length: %zu\n", len);
    printf("Substring: \"%s\"\n", result);
    free(result);

    s = "Hello, world!";
    start = 7;
    len = 20;
    result = ft_substr(s, start, len);
    if (!result)
    {
        perror("ft_substr");
        return 1;
    }
    printf("\nOriginal string: \"%s\"\n", s);
    printf("Start index: %u\n", start);
    printf("Length: %zu\n", len);
    printf("Substring: \"%s\"\n", result);
    free(result);

    return 0;
}*/
