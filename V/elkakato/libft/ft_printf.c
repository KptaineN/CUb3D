/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:24:26 by xoppy             #+#    #+#             */
/*   Updated: 2024/11/04 05:36:41 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
ft_print -> main function to print the format string, it loops over the 
format and calls the parser function to print the arguments when it finds 
a '%' character.
@param format: format string
@param args: arguments list
*/
int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		chars;
	int		i;

	chars = 0;
	i = 0;
	va_start(args, format);
	while (format[i] != '\0')
	{
		if (format[i] == '%')
		{
			i++;
			chars += ft_parser(format[i], args);
		}
		else
		{
			write(1, &format[i], 1);
			chars++;
		}
		i++;
	}
	va_end(args);
	return (chars);
}
