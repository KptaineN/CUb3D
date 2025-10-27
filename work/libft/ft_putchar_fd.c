/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:15:18 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/25 05:58:03 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The putchar_fd function simply writes a character given as
parameter in the output given in the second parameter
*/

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
	return ;
}
