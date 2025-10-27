/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 12:25:02 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:01:58 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		if (((unsigned char) s1[i]) != ((unsigned char) s2[i]))
		{
			return (((unsigned char) s1[i]) - ((unsigned char) s2[i]));
		}
		i++;
	}
	return (0);
}
/*#include "libft.h"
#include <stdio.h>
#include <string.h> // Pour la fonction standard strncmp

int main(void)
{
    const char *s1 = "Hello, world!";
    const char *s2 = "Hello, World!";
    size_t n = 5;
    int result_ft, result_std;

    // Utilisation de ft_strncmp
    result_ft = ft_strncmp(s1, s2, n);

    // Utilisation de strncmp
    result_std = strncmp(s1, s2, n);

    // Affichage des résultats
    printf("s1: \"%s\"\n", s1);
    printf("s2: \"%s\"\n", s2);
    printf("Number of characters to compare: %zu\n", n);
    printf("ft_strncmp: %d\n", result_ft);
    printf("strncmp: %d\n", result_std);

    // Comparaison des résultats
    if (result_ft == result_std)
    {
        printf("ft_strncmp works correctly.\n");
    }
    else
    {
        printf("ft_strncmp does not work correctly.\n");
    }

    return 0;
}*/
