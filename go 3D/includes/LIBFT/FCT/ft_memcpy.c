/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 11:24:11 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:00:29 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memcpy(void *dst, const char *src, size_t n)
{
	unsigned char	*tmp_dst;
	unsigned char	*tmp_src;

	if (dst == ((void *)0) && src == (void *)0)
		return (dst);
	tmp_dst = (unsigned char *) dst;
	tmp_src = (unsigned char *) src;
	while (n > 0)
	{
		*tmp_dst = *tmp_src;
		tmp_dst++;
		tmp_src++;
		n--;
	}
	return (dst);
}
/*#include "libft.h"
#include <stdio.h>
#include <string.h> // Pour la fonction standard memcpy

int main(void)
{
    char src[] = "Hello, world!";
    char dst_ft[20];
    char dst_std[20];

    // Utilisation de ft_memcpy et memcpy
    ft_memcpy(dst_ft, src, sizeof(src));
    memcpy(dst_std, src, sizeof(src));

    // Affichage des résultats
    printf("Source: \"%s\"\n", src);
    printf("ft_memcpy: \"%s\"\n", dst_ft);
    printf("memcpy: \"%s\"\n", dst_std);

    // Comparaison des résultats
    if (memcmp(dst_ft, dst_std, sizeof(src)) == 0)
    {
        printf("ft_memcpy works correctly.\n");
    }
    else
    {
        printf("ft_memcpy does not work correctly.\n");
    }

    return 0;
}*/
