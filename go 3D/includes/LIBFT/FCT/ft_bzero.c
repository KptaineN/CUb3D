/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 11:03:52 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 13:58:39 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memset(void *s, int c, size_t len);

void	ft_bzero(void *s, size_t n)
{
	(ft_memset(s, 0, n));
}
/*#include "libft.h"
#include <stdio.h>
#include <strings.h> // Pour la fonction standard bzero
#include <stdlib.h>  // Pour malloc

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
    size_t len = 10;
    void *mem1 = malloc(len);
    void *mem2 = malloc(len);

    if (!mem1 || !mem2)
    {
        perror("malloc");
        return 1;
    }

    // Initialiser les deux blocs de mémoire avec des valeurs non nulles
    ft_memset(mem1, 0xFF, len);
    ft_memset(mem2, 0xFF, len);

    // Utiliser ft_bzero et bzero
    ft_bzero(mem1, len);
    bzero(mem2, len);

    // Afficher les résultats
    printf("ft_bzero result:\n");
    print_memory(mem1, len);

    printf("bzero result:\n");
    print_memory(mem2, len);

    // Comparer les résultats
    if (memcmp(mem1, mem2, len) == 0)
    {
        printf("ft_bzero works correctly.\n");
    }
    else
    {
        printf("ft_bzero does not work correctly.\n");
    }

    free(mem1);
    free(mem2);

    return 0;
}*/