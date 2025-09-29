/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 13:00:50 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 13:59:39 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_isascii(int c)
{
	if (c < 0 || c > 127)
	{
		return (0);
	}
	return (1);
}
/*#include "libft.h"
#include <stdio.h>
#include <ctype.h> // Pour la fonction standard isascii

int main(void)
{
    int test_cases[] = {
        'a', 'Z', '0', '9', '!', ' ', '\n', 128, -1, 0, 127, 255, 0
    };

    for (int i = 0; test_cases[i] != 0; i++)
    {
        int c = test_cases[i];
        printf("Input: '%c' (ASCII: %d)\n", c, c);
        printf("ft_isascii: %d\n", ft_isascii(c));
        printf("isascii: %d\n", isascii(c));
        printf("\n");
    }

    return 0;
}*/
