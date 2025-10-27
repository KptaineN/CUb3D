/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsigned_int.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 16:30:17 by xoppy             #+#    #+#             */
/*   Updated: 2024/11/04 05:36:38 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
ft_print_unsigned_int -> prints an unsigned integer
@param n: unsigned integer to print
*/
int	ft_print_unsigned_int(unsigned int n)
{
	int	count;

	count = 0;
	if (n >= 10)
		count += ft_print_unsigned_int(n / 10);
	count += ft_print_char((n % 10) + '0');
	return (count);
}
