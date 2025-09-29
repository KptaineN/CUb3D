/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:49:33 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 13:58:27 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_isspace(int c)
{
	if (c == 9 || c == 10 || c == 11 || c == 12 || c == 13 || c == 32)
	{
		return (1);
	}
	return (0);
}

int	ft_atoi(const char *nptr)
{
	int	rslt;
	int	i;
	int	sign;

	rslt = 0;
	i = 0;
	sign = 1;
	while (ft_isspace(nptr[i]))
	{
		i++;
	}
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
		{
			sign = -1;
		}
		i++;
	}
	while (nptr[i] && ft_isdigit(nptr[i]))
	{
		rslt = rslt * 10 + (nptr[i] - '0');
		i++;
	}
	return (rslt * sign);
}
/*#include "libft.h"
#include <stdio.h>

int	main(void)
{
	const char *test_cases[] = {
		"42",
		"   -42",
		"4193 with words",
		"words and 987",
		"-91283472332",
		"+123",
		"   +0",
		"   -0",
		"2147483647",
		"-2147483648",
		"   000123",
		"   -000123",
		NULL
	};

	for (int i = 0; test_cases[i] != NULL; i++)
	{
		printf("Input: \"%s\"\n", test_cases[i]);
		printf("ft_atoi: %d\n", ft_atoi(test_cases[i]));
		printf("atoi: %d\n", atoi(test_cases[i]));
		printf("\n");
	}

	return (0);
}*/
