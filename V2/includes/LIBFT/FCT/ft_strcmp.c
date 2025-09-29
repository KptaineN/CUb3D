/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:18:37 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:00:52 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned char	c1;
	unsigned char	c2;

	while (*s1 && *s2)
	{
		c1 = (unsigned char)ft_tolower((unsigned char)*s1);
		c2 = (unsigned char)ft_tolower((unsigned char)*s2);
		if (c1 != c2)
			return ((int)(c1 - c2));
		s1++;
		s2++;
	}
	c1 = (unsigned char)ft_tolower((unsigned char)*s1);
	c2 = (unsigned char)ft_tolower((unsigned char)*s2);
	return ((int)(c1 - c2));
}
/**
 * @brief Compare deux chaînes de caractères sans tenir compte de la casse.
 *
 * Utilise ft_tolower pour normaliser chaque caractère.
 *
 * @param s1 Première chaîne (non-NULL)
 * @param s2 Seconde chaîne (non-NULL)
 * @return  0 si s1 == s2 (à casse près)
 *         <0 si s1 < s2
 *         >0 si s1 > s2
 *c1 & c2 // Si l'une des chaînes s'arrête, on compare le caractère nul
 *
 */
