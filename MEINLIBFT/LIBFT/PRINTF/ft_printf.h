/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 19:54:04 by nkiefer           #+#    #+#             */
/*   Updated: 2025/09/08 19:54:08 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>
# include "../libft.h"

int	ft_printf(const char *format, ...);
int	ft_print_arg(va_list *args, char format);
int	ft_put_unsigned(unsigned int n);
int	ft_put_pointer(void *ptr);
int	ft_put_nbr(long long n);
int	ft_put_hex(unsigned long long num, char format);
int	ft_print_string(char *str);
//int	ft_strlen_4pf(const char *str);

#endif
