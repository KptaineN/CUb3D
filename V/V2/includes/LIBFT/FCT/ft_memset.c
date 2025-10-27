/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 09:59:03 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:00:36 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memset(void *s, int c, size_t len)
{
	unsigned char	*tmp_ps;

	tmp_ps = ((unsigned char *)s);
	while (len > 0)
	{
		*(tmp_ps++) = ((unsigned char ) c);
		len--;
	}
	return (s);
}
/*#include "libft.h"
#include <stdio.h>
#include <string.h> // Pour la fonction standard memset

void print_memory(const void *mem, size_t len)
{
    const unsigned char *bytes = (const unsigned char *)mem;
    for (size_t i = 0; i < len; i++)
    {
        printf("%02x ", bytes[i]);
    }
    printf("\n");
}

int main(void)
{
    char buffer_ft[20];
    char buffer_std[20];

    // Initialiser les deux buffers avec des valeurs non nulles
    memset(buffer_ft, 0xFF, sizeof(buffer_ft));
    memset(buffer_std, 0xFF, sizeof(buffer_std));

    // Utilisation de ft_memset et memset
    ft_memset(buffer_ft, 'A', 10);
    memset(buffer_std, 'A', 10);

    // Affichage des résultats
    printf("ft_memset result:\n");
    print_memory(buffer_ft, sizeof(buffer_ft));

    printf("memset result:\n");
    print_memory(buffer_std, sizeof(buffer_std));

    // Comparaison des résultats
    if (memcmp(buffer_ft, buffer_std, sizeof(buffer_ft)) == 0)
    {
        printf("ft_memset works correctly.\n");
    }
    else
    {
        printf("ft_memset does not work correctly.\n");
    }

    return 0;
}*/