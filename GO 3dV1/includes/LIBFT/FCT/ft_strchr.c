/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:59:51 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:00:59 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strchr(const char *str, int c)
{
	while (*str)
	{
		if (*str == (char)c)
		{
			return ((char *)str);
		}
		str++;
	}
	if (c == '\0')
	{
		return ((char *)str);
	}
	return (NULL);
}
/*#include "libft.h"
#include <stdio.h>
#include <string.h> // Pour la fonction standard strchr

int main(void)
{
    const char str[] = "Hello, world!";
    int c = 'o';
    int c_not_found = 'x';
    int c_null = '\0';

    // Test 1 : Caractère trouvé
    char *result_ft = ft_strchr(str, c);
    char *result_std = strchr(str, c);
    printf("Test 1 - Character found:\n");
    printf("Input string: \"%s\"\n", str);
    printf("Character to find: '%c'\n", c);
    printf("ft_strchr: %s\n", result_ft);
    printf("strchr: %s\n", result_std);
    printf("\n");

    // Test 2 : Caractère non trouvé
    result_ft = ft_strchr(str, c_not_found);
    result_std = strchr(str, c_not_found);
    printf("Test 2 - Character not found:\n");
    printf("Input string: \"%s\"\n", str);
    printf("Character to find: '%c'\n", c_not_found);
    printf("ft_strchr: %s\n", result_ft ? result_ft : "NULL");
    printf("strchr: %s\n", result_std ? result_std : "NULL");
    printf("\n");

    // Test 3 : Caractère nul
    result_ft = ft_strchr(str, c_null);
    result_std = strchr(str, c_null);
    printf("Test 3 - Null character:\n");
    printf("Input string: \"%s\"\n", str);
    printf("Character to find: '\\0'\n");
    printf("ft_strchr: %s\n", result_ft);
    printf("strchr: %s\n", result_std);
    printf("\n");

    return 0;
}*/
