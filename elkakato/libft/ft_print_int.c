/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_int.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 16:29:38 by xoppy             #+#    #+#             */
/*   Updated: 2024/11/04 05:36:29 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
ft_print_int -> prints an integer
@param n: integer to print
*/
int	ft_print_int(int n)
{
	int		count;
	long	num;

	count = 0;
	num = n;
	if (num < 0)
	{
		count += ft_print_char('-');
		num = -num;
	}
	if (num >= 10)
		count += ft_print_int(num / 10);
	count += ft_print_char((num % 10) + '0');
	return (count);
}
