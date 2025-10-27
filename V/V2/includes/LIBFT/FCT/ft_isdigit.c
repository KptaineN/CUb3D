/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 12:41:36 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:00:07 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
	{
		return (1);
	}
	return (0);
}
/*#include "libft.h"
#include <stdio.h>
#include <ctype.h> // Pour la fonction standard isdigit

int main(void)
{
    int test_cases[] = {
        '0', '9', 'a', 'Z', '!', ' ', '\n', '5', '3', 0
    };

    for (int i = 0; test_cases[i] != 0; i++)
    {
        int c = test_cases[i];
        printf("Input: '%c' (ASCII: %d)\n", c, c);
        printf("ft_isdigit: %d\n", ft_isdigit(c));
        printf("isdigit: %d\n", isdigit(c));
        printf("\n");
    }

    return 0;
}*/