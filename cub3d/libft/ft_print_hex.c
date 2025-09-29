/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 16:31:08 by xoppy             #+#    #+#             */
/*   Updated: 2024/11/04 05:36:25 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
ft_print_hex_recursive -> prints an unsigned integer in hexadecimal format
@param num: number to print
@param base: base to print the hexadecimal (uppercase or lowercase)
*/
static int	ft_print_hex_recursive(unsigned int num, char *base)
{
	int	count;

	count = 0;
	if (num >= 16)
		count += ft_print_hex_recursive(num / 16, base);
	count += write(1, &base[num % 16], 1);
	return (count);
}

/*
ft_print_hex -> prints an unsigned integer in hexadecimal format
@param num: number to print
@param uppercase: flag to print the hexadecimal in uppercase or lowercase
*/
int	ft_print_hex(unsigned int num, int uppercase)
{
	char	*base;
	int		count;

	if (uppercase == 1)
		base = "0123456789ABCDEF";
	else if (uppercase == 0)
		base = "0123456789abcdef";
	count = ft_print_hex_recursive(num, base);
	return (count);
}
