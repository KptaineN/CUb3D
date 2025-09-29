/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_pointer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 16:31:35 by xoppy             #+#    #+#             */
/*   Updated: 2024/11/04 05:36:34 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
ft_print_pointer_recursive -> prints a pointer in hexadecimal format with a 
defined base
@param num: number to print
*/
static int	ft_print_pointer_recursive(unsigned long num)
{
	int		count;
	char	*base;

	count = 0;
	base = "0123456789abcdef";
	if (num >= 16)
		count += ft_print_pointer_recursive(num / 16);
	count += write(1, &base[num % 16], 1);
	return (count);
}

/*
ft_print_pointer -> prints a pointer in hexadecimal format
@param ptr: pointer to print
*/
int	ft_print_pointer(void *ptr)
{
	unsigned long	address;
	int				count;

	count = 0;
	if (!ptr)
	{
		count += write(1, "(nil)", 5);
		return (count);
	}
	address = (unsigned long)ptr;
	count = write(1, "0x", 2);
	count += ft_print_pointer_recursive(address);
	return (count);
}
