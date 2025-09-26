/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_pointer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 18:19:24 by nkiefer           #+#    #+#             */
/*   Updated: 2024/11/15 09:28:49 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_put_pointer(void *ptr)
{
	int	len;

	len = 0;
	if (!ptr)
		return (write(1, "(nil)", 5));
	len = write(1, "0x", 2);
	len += ft_put_hex((unsigned long long)ptr, 'x');
	return (len);
}

int	ft_put_hex(unsigned long long num, char format)
{
	int		len;
	char	*hex_digits;

	len = 0;
	if (format == 'x')
		hex_digits = "0123456789abcdef";
	else if (format == 'X')
		hex_digits = "0123456789ABCDEF";
	else
		return (0);
	if (num >= 16)
	{
		len += ft_put_hex(num / 16, format);
	}
	len += ft_putchar(hex_digits[num % 16]);
	return (len);
}
