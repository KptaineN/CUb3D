/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 16:28:25 by xoppy             #+#    #+#             */
/*   Updated: 2024/11/04 05:36:21 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
ft_print_char -> prints a single character
@param c: character to print
*/
int	ft_print_char(int c)
{
	write(1, &c, 1);
	return (1);
}
