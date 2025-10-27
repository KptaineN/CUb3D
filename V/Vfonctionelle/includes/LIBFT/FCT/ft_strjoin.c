/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 06:08:49 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:01:40 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*nstr;
	int		len;

	if (!s1 || !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	nstr = (char *)malloc(sizeof(char) * (len + 1));
	if (!nstr)
		return (NULL);
	ft_strlcpy(nstr, s1, ft_strlen(s1) + 1);
	ft_strlcat(nstr, s2, len + 1);
	return (nstr);
}
/*#include "libft.h"
#include <stdio.h>

int main(void)
{
    char *s1 = "Hello, ";
    char *s2 = "world!";
    char *result;

    // Utilisation de ft_strjoin
    result = ft_strjoin(s1, s2);
    if (!result)
    {
        perror("ft_strjoin");
        return 1;
    }

    // Affichage du résultat
    printf("s1: \"%s\"\n", s1);
    printf("s2: \"%s\"\n", s2);
    printf("ft_strjoin: \"%s\"\n", result);

    // Libération de la mémoire allouée
    free(result);

    return 0;
}*/