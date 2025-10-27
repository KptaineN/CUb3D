/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 12:49:27 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 13:59:28 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_isalnum(int c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
	{
		return (1);
	}
	return (0);
}
/*#include "libft.h"
#include <stdio.h>
#include <ctype.h> // Pour la fonction standard isalnum

int main(void)
{
    int test_cases[] = {
        'a', 'Z', '0', '9', '!', ' ', '\n', '1', 'b', 'G', 0
    };

    for (int i = 0; test_cases[i] != 0; i++)
    {
        int c = test_cases[i];
        printf("Input: '%c' (ASCII: %d)\n", c, c);
        printf("ft_isalnum: %d\n", ft_isalnum(c));
        printf("isalnum: %d\n", isalnum(c));
        printf("\n");
    }

    return 0;
}*/
