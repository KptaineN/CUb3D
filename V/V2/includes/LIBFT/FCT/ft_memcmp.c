/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:21:46 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:00:27 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;
	size_t			i;

	str1 = ((unsigned char *) s1);
	str2 = ((unsigned char *) s2);
	i = 0;
	while (i < n)
	{
		if (((unsigned char)str1[i]) != ((unsigned char)str2[i]))
			return (((unsigned char) str1[i]) - ((unsigned char) str2[i]));
		i++;
	}
	return (0);
}
/*#include "libft.h"
#include <stdio.h>
#include <string.h> // Pour la fonction standard memcmp

int main(void)
{
    const char str1[] = "Hello, world!";
    const char str2[] = "Hello, World!";
    size_t n = 13;

    int result_ft = ft_memcmp(str1, str2, n);
    int result_std = memcmp(str1, str2, n);

    printf("Input strings: \"%s\" and \"%s\"\n", str1, str2);
    printf("Number of bytes to compare: %zu\n", n);

    printf("ft_memcmp: %d\n", result_ft);
    printf("memcmp: %d\n", result_std);

    return 0;
}*/