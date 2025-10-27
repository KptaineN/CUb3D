/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 11:14:21 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:00:23 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	int_len(long nbr)
{
	int	count;

	count = 0;
	if (nbr <= 0)
		count++;
	while (nbr != 0)
	{
		nbr = nbr / 10;
		count++;
	}
	return (count);
}

static char	*cas_spe(int n)
{
	if (n == 0)
		return (ft_strdup("0"));
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	return (NULL);
}

char	*ft_itoa(int n)
{
	long	nbr;
	int		len;
	char	*result;
	char	*special;

	special = cas_spe(n);
	if (special)
		return (special);
	len = int_len(n);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	result[len] = '\0';
	nbr = n;
	if (nbr < 0)
	{
		result[0] = '-';
		nbr = -nbr;
	}
	while (nbr > 0)
	{
		result[--len] = (nbr % 10) + '0';
		nbr = nbr / 10;
	}
	return (result);
}
/*#include "libft.h"
#include <stdio.h>

int main(void)
{
    int test_cases[] = {
        0, 123, -123, 2147483647, -2147483648, 42, -42, 1000000, -1000000, 0
    };

    for (int i = 0; test_cases[i] != 0; i++)
    {
        int n = test_cases[i];
        char *result = ft_itoa(n);
        if (result)
        {
            printf("Input: %d\n", n);
            printf("ft_itoa: %s\n", result);
            free(result);
        }
        else
        {
            printf("Input: %d\n", n);
            printf("ft_itoa: NULL (allocation failed)\n");
        }
        printf("\n");
    }

    return 0;
}*/
