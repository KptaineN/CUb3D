/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 13:11:23 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:01:49 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}
/*
#include "libft.h"
#include <stdio.h>
#include <string.h> // Pour la fonction standard strlen

int	main(void)
{
		const char *str = test_cases[i];
	size_t	len_ft;
	size_t	len_std;

	const char *test_cases[] = {
		"Hello, world!",
		"",
		"42",
		"This is a longer string to test the length function.",
	"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
	aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
	aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
	"f i n"
	};
	for (int i = 0; test_cases[i] != NULL; i++)
	{
		len_ft = ft_strlen(str);
		len_std = strlen(str);
		printf("Test case %d:\n", i + 1);
		printf("Input string: \"%s\"\n", str);
		printf("ft_strlen: %zu\n", len_ft);
		printf("strlen: %zu\n", len_std);
		if (len_ft == len_std)
		{
			printf("Result: PASS\n");
		}
		else
		{
			printf("Result: FAIL\n");
		}
		printf("\n");
		}
}
*/
