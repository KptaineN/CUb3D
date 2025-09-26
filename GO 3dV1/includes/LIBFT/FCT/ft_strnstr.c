/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 08:30:56 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:02:25 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if ((big == NULL || little == NULL) && len == 0)
		return (NULL);
	if (*little == '\0')
		return ((char *)big);
	i = 0;
	while (big[i] && i < len)
	{
		j = 0;
		while (little[j] && big[i + j] == little[j] && (i + j) < len)
		{
			j++;
		}
		if (little[j] == '\0')
			return ((char *)(big + i));
		i++;
	}
	return (NULL);
}
/*#include "libft.h"
#include <stdio.h>
#include <string.h> // Pour la fonction standard strnstr

int main(void)
{
    const char *big = "Hello, world!";
    const char *little = "world";
    size_t len = 12;
    char *result_ft;
    char *result_std;

    // Utilisation de ft_strnstr
    result_ft = ft_strnstr(big, little, len);

    // Utilisation de strnstr
    result_std = strnstr(big, little, len);

    // Affichage des résultats
    printf("Big string: \"%s\"\n", big);
    printf("Little string: \"%s\"\n", little);
    printf("Length to search: %zu\n", len);
    printf("ft_strnstr: %s\n", result_ft ? result_ft : "NULL");
    printf("strnstr: %s\n", result_std ? result_std : "NULL");

    // Comparaison des résultats
    if (result_ft == result_std)
    {
        printf("ft_strnstr works correctly.\n");
    }
    else
    {
        printf("ft_strnstr does not work correctly.\n");
    }

    return 0;
}*/
