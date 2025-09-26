/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:33:57 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:01:42 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	ft_strlen(const char *str);

void	*ft_memcpy(void *dst, const char *src, size_t n);

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	len_src;
	size_t	len_dst;
	size_t	rest_space_dst;

	if (!dst && dstsize == 0)
		return (ft_strlen((char *)src));
	len_src = ft_strlen(src);
	len_dst = ft_strlen(dst);
	rest_space_dst = (dstsize - len_dst - 1);
	if (len_dst >= dstsize)
		return (len_src + dstsize);
	if (len_src <= rest_space_dst)
		ft_memcpy(dst + len_dst, src, len_src + 1);
	else
	{
		ft_memcpy(dst + len_dst, src, rest_space_dst);
		dst[len_dst + rest_space_dst] = '\0';
	}
	return (len_dst + len_src);
}
/*#include "libft.h"
#include <stdio.h>
#include <string.h> // Pour la fonction standard strlcat

int main(void)
{
    char dst_ft[50] = "Hello, ";
    char dst_std[50] = "Hello, ";
    const char *src = "world!";
    size_t dstsize = 50;
    size_t result_ft, result_std;

    // Utilisation de ft_strlcat
    result_ft = ft_strlcat(dst_ft, src, dstsize);

    // Utilisation de strlcat
    result_std = strlcat(dst_std, src, dstsize);

    // Affichage des résultats
    printf("Source string: \"%s\"\n", src);
    printf("Destination string (ft_strlcat): \"%s\"\n", dst_ft);
    printf("Destination string (strlcat): \"%s\"\n", dst_std);
    printf("Return value (ft_strlcat): %zu\n", result_ft);
    printf("Return value (strlcat): %zu\n", result_std);

    // Comparaison des résultats
    if (result_ft == result_std && strcmp(dst_ft, dst_std) == 0)
    {
        printf("ft_strlcat works correctly.\n");
    }
    else
    {
        printf("ft_strlcat does not work correctly.\n");
    }

    return 0;
}*/
