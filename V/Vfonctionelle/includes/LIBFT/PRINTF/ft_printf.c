/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 08:05:46 by nkiefer           #+#    #+#             */
/*   Updated: 2024/11/14 13:20:42 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../libft.h"

int	ft_print_arg(va_list *args, char format)
{
	int	count;

	count = 0;
	if (format == 'c')
		count += (ft_putchar(va_arg(*args, int)));
	else if (format == 's')
		count = (ft_print_string((va_arg(*args, char *))));
	else if (format == 'p')
		count = (ft_put_pointer(va_arg(*args, void *)));
	else if (format == 'd' || format == 'i')
		count = (ft_put_nbr(va_arg(*args, int)));
	else if (format == 'u')
		count = (ft_put_unsigned(va_arg(*args, unsigned int)));
	else if (format == 'x' || format == 'X')
		count = (ft_put_hex(va_arg(*args, unsigned int), format));
	else if (format == '%')
		count = (ft_putchar('%'));
	else
		count = ft_putchar(format);
	return (count);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	size_t	i;
	int		lstr;

	va_start(args, format);
	lstr = 0;
	i = 0;
	if (!format)
		return (-1);
	while (format[i])
	{
		if (format[i] == '%')
		{
			i++;
			lstr += ft_print_arg(&args, format[i]);
		}
		else
		{
			lstr += ft_putchar(format[i]);
		}
		i++;
	}
	va_end(args);
	return (lstr);
}
