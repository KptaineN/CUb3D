/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 13:07:29 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:00:17 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_isprint(int c)
{
	if (c < 32 || c > 126)
	{
		return (0);
	}
	return (1);
}
/*#include "libft.h"
#include <stdio.h>
#include <ctype.h> // Pour la fonction standard isprint

int main(void)
{
    int test_cases[] = {
        'a', 'Z', '0', '9', '!', ' ', '\n', 31, 32, 126, 127, 0
    };

    for (int i = 0; test_cases[i] != 0; i++)
    {
        int c = test_cases[i];
        printf("Input: '%c' (ASCII: %d)\n", c, c);
        printf("ft_isprint: %d\n", ft_isprint(c));
        printf("isprint: %d\n", isprint(c));
        printf("\n");
    }

    return 0;
}*/