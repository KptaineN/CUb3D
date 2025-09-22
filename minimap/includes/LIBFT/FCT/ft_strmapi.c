/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 09:58:31 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:01:53 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*ms;

	if (!s || !f)
		return (NULL);
	ms = malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!ms)
		return (NULL);
	i = 0;
	while (s[i])
	{
		ms[i] = f(i, s[i]);
		i++;
	}
	ms[i] = '\0';
	return (ms);
}
/*#include "libft.h"
#include <stdio.h>

char example_function(unsigned int i, char c)
{
    if (i % 2 == 0)
    {
        return ft_toupper(c);
    }
    else
    {
        return ft_tolower(c);
    }
}

int main(void)
{
    char const *str = "Hello, World!";
    char *result;

    // Utilisation de ft_strmapi
    result = ft_strmapi(str, example_function);
    if (!result)
    {
        perror("ft_strmapi");
        return 1;
    }

    // Affichage des résultats
    printf("Original string: \"%s\"\n", str);
    printf("Modified string: \"%s\"\n", result);

    // Libération de la mémoire allouée
    free(result);

    return 0;
}*/
