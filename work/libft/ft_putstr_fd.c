/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:16:35 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/25 05:58:11 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The putstr_fd function takes an array of chars and display it to the
selected output in the parameters.
*/

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	while (*s)
	{
		ft_putchar_fd(*s, fd);
		s++;
	}
	return ;
}
