/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_num.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:07:56 by nkiefer           #+#    #+#             */
/*   Updated: 2025/07/03 02:20:50 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_string(char *str)
{
	int	count;

	if (str == NULL)
		return (write(1, "(null)", 6));
	count = 0;
	while (*str)
	{
		count += ft_putchar(*str);
		str++;
	}
	return (count);
}

int	ft_put_nbr(long long n)
{
	int	num;

	num = 0;
	if (n == -2147483648)
		return (write(1, "-2147483648", 11));
	if (n < 0)
	{
		num += ft_putchar('-');
		n = -n;
	}
	if (n >= 10)
	{
		num += ft_put_nbr(n / 10);
	}
	num += ft_putchar(n % 10 + '0');
	return (num);
}

int	ft_put_unsigned(unsigned int n)
{
	int	len;

	len = 0;
	if (n >= 10)
	{
		len += ft_put_unsigned(n / 10);
		len += ft_putchar(n % 10 + '0');
	}
	else
		len += ft_putchar(n + '0');
	return (len);
}
