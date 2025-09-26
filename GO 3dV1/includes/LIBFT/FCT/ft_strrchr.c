/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:30:11 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:02:28 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	ft_strlen(const char *str);

char	*ft_strrchr(const char *str, int c)
{
	int	i;

	i = ft_strlen(str) - 1;
	while (i >= 0)
	{
		if (str[i] == ((char )c))
		{
			return ((char *)&str[i]);
		}
		i--;
	}
	if (c == '\0')
	{
		return ((char *)&str[ft_strlen(str)]);
	}
	return (NULL);
}
/*#include "libft.h"
#include <stdio.h>
#include <string.h> // Pour la fonction standard strrchr

int main(void)
{
    const char *str = "Hello, world!";
    int c = 'o';
    int c_not_found = 'x';
    int c_null = '\0';

    // Test 1 : Caractère trouvé
    char *result_ft = ft_strrchr(str, c);
    char *result_std = strrchr(str, c);
    printf("Test 1 - Character found:\n");
    printf("Input string: \"%s\"\n", str);
    printf("Character to find: '%c'\n", c);
    printf("ft_strrchr: %s\n", result_ft ? result_ft : "NULL");
    printf("strrchr: %s\n", result_std ? result_std : "NULL");
    printf("\n");

    // Test 2 : Caractère non trouvé
    result_ft = ft_strrchr(str, c_not_found);
    result_std = strrchr(str, c_not_found);
    printf("Test 2 - Character not found:\n");
    printf("Input string: \"%s\"\n", str);
    printf("Character to find: '%c'\n", c_not_found);
    printf("ft_strrchr: %s\n", result_ft ? result_ft : "NULL");
    printf("strrchr: %s\n", result_std ? result_std : "NULL");
    printf("\n");

    // Test 3 : Caractère nul
    result_ft = ft_strrchr(str, c_null);
    result_std = strrchr(str, c_null);
    printf("Test 3 - Null character:\n");
    printf("Input string: \"%s\"\n", str);
    printf("Character to find: '\\0'\n");
    printf("ft_strrchr: %s\n", result_ft ? result_ft : "NULL");
    printf("strrchr: %s\n", result_std ? result_std : "NULL");
    printf("\n");

    return 0;
}*/
