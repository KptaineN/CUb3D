/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc < adi-marc@student.42luxembour    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 02:11:41 by adi-marc          #+#    #+#             */
/*   Updated: 2025/01/16 14:28:01 by xoppy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

long	ft_atol(const char *nptr)
{
	long	sign;
	long	number;

	sign = 1;
	number = 0;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign = -1;
		else
			sign = 1;
		nptr++;
		if (*nptr == '+' || *nptr == '-')
			return (0);
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		number = number * 10 + (*nptr - '0');
		nptr++;
	}
	return (sign * number);
}
