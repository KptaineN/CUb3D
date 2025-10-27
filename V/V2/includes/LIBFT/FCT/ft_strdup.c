/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 18:13:24 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:01:35 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strdup(const char *s)
{
	char	*dest;
	size_t	i;

	if (s == NULL)
		return (NULL);
	dest = (char *)malloc(ft_strlen(s) + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (s[i])
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
/*#include "libft.h"
#include <stdio.h>
#include <string.h> // Pour la fonction standard strdup

int	main(void)
{
	const char *str = "Hello, world!";

	// Utilisation de ft_strdup
	char *dup_ft = ft_strdup(str);
	if (!dup_ft)
	{
		perror("ft_strdup");
		return (1);
	}

	// Utilisation de strdup
	char *dup_std = strdup(str);
	if (!dup_std)
	{
		perror("strdup");
		free(dup_ft);
		return (1);
	}

	// Affichage des résultats
	printf("Original string: \"%s\"\n", str);
	printf("ft_strdup: \"%s\"\n", dup_ft);
	printf("strdup: \"%s\"\n", dup_std);

	// Comparaison des résultats
	if (strcmp(dup_ft, dup_std) == 0)
	{
		printf("ft_strdup works correctly.\n");
	}
	else
	{
		printf("ft_strdup does not work correctly.\n");
	}

	// Libération de la mémoire allouée
	free(dup_ft);
	free(dup_std);

	return (0);
}*/
