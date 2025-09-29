/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 16:29:50 by xoppy             #+#    #+#             */
/*   Updated: 2024/11/04 05:38:46 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
ft_parse_percent -> prints a percent sign in case of "%%" 
in the format string
*/
static int	ft_parse_percent(void)
{
	write(1, "%", 1);
	return (1);
}

// ft_parse_unknown -> prints an unknown character in case of an unknown format
static int	ft_parse_unknown(char format)
{
	write(1, "%", 1);
	write(1, &format, 1);
	return (2);
}

/*
ft_parser -> parses the format string and calls the corresponding function
to print the argument.
@param format: format character
@param args: arguments list
*/
int	ft_parser(char format, va_list args)
{
	int	chars;

	chars = 0;
	if (format == 'c')
		chars += ft_print_char(va_arg(args, int));
	else if (format == 's')
		chars += ft_print_str(va_arg(args, char *));
	else if (format == 'p')
		chars += ft_print_pointer(va_arg(args, void *));
	else if (format == 'd' || format == 'i')
		chars += ft_print_int(va_arg(args, int));
	else if (format == 'u')
		chars += ft_print_unsigned_int(va_arg(args, unsigned int));
	else if (format == 'x')
		chars += ft_print_hex(va_arg(args, unsigned int), 0);
	else if (format == 'X')
		chars += ft_print_hex(va_arg(args, unsigned int), 1);
	else if (format == '%')
		chars += ft_parse_percent();
	else
		chars += ft_parse_unknown(format);
	return (chars);
}
