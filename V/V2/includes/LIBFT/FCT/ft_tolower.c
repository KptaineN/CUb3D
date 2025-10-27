/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:12:07 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:03:11 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		c += 32;
	return (c);
}
/*#include "libft.h"
#include <stdio.h>
#include <ctype.h> // Pour la fonction standard tolower

int main(void)
{
    int test_cases[] = {
        'A', 'Z', 'a', 'z', '0', '9', '!', ' ', '\n', 'B', 'Y', 0
    };

    for (int i = 0; test_cases[i] != 0; i++)
    {
        int c = test_cases[i];
        int result_ft = ft_tolower(c);
        int result_std = tolower(c);

        printf("Input: '%c' (ASCII: %d)\n", c, c);
        printf("ft_tolower: %c (ASCII: %d)\n", result_ft, result_ft);
        printf("tolower: %c (ASCII: %d)\n", result_std, result_std);

        if (result_ft == result_std)
        {
            printf("Result: PASS\n");
        }
        else
        {
            printf("Result: FAIL\n");
        }
        printf("\n");
    }

    return 0;
}*/
