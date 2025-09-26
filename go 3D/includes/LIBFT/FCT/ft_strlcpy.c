/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:40:14 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:01:44 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

//size_t	ft_strlen(const char *str);

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size > 0)
	{
		while (src[i] && i < (size - 1))
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = 0;
	}
	while (src[i])
		i++;
	return (i);
}
/*#include "libft.h"
#include <stdio.h>
#include <string.h> // Pour la fonction standard strlcpy

int main(void)
{
    char src[] = "Hello, world!";
    char dest_ft[20];
    char dest_std[20];
    size_t size = 20;
    size_t result_ft, result_std;

    // Utilisation de ft_strlcpy
    result_ft = ft_strlcpy(dest_ft, src, size);

    // Utilisation de strlcpy
    result_std = strlcpy(dest_std, src, size);

    // Affichage des résultats
    printf("Source string: \"%s\"\n", src);
    printf("Destination string (ft_strlcpy): \"%s\"\n", dest_ft);
    printf("Destination string (strlcpy): \"%s\"\n", dest_std);
    printf("Return value (ft_strlcpy): %zu\n", result_ft);
    printf("Return value (strlcpy): %zu\n", result_std);

    // Comparaison des résultats
    if (result_ft == result_std && strcmp(dest_ft, dest_std) == 0)
    {
        printf("ft_strlcpy works correctly.\n");
    }
    else
    {
        printf("ft_strlcpy does not work correctly.\n");
    }

    return 0;
}*/
