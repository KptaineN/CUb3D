/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 15:11:19 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:03:22 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		c -= 32;
	return (c);
}
/*#include "libft.h"
#include <ctype.h> // Pour la fonction standard toupper
#include <stdio.h>

int	main(void)
{
	int test_cases[] = {
		'a', 'z', 'A', 'Z', '0', '9', '!', ' ', '\n', 'b', 'y', 0
	};

	for (int i = 0; test_cases[i] != 0; i++)
	{
		int c = test_cases[i];
		int result_ft = ft_toupper(c);
		int result_std = toupper(c);

		printf("Input: '%c' (ASCII: %d)\n", c, c);
		printf("ft_toupper: %c (ASCII: %d)\n", result_ft, result_ft);
		printf("toupper: %c (ASCII: %d)\n", result_std, result_std);

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

	return (0);
}*/
