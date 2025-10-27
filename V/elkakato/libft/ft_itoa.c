/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:14:59 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/25 08:32:04 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The itoa function takes an int as a parameter and returns it inside an array of
chars that needs to have the memory assigned to it with malloc.
*/

#include "libft.h"

int	ft_intlen(int n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	if (n == -2147483648)
		return (10);
	if (n < 0)
		n = -n;
	while (n > 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	int		size;
	long	nb;
	char	*result;

	nb = n;
	size = ft_intlen(n) + (n < 0);
	result = (char *)malloc(sizeof(char) * (size + 1));
	if (result == NULL)
		return (NULL);
	if (n < 0)
	{
		result[0] = '-';
		nb = -nb;
	}
	result[size] = '\0';
	size--;
	while (size >= (n < 0))
	{
		result[size] = (nb % 10) + 48;
		nb = nb / 10;
		size--;
	}
	return (result);
}
