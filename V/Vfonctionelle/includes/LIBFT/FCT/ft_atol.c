/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 16:48:34 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 13:58:34 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

long	ft_atol(const char *nptr)
{
	long	rslt;
	long	i;
	long	sign;

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
