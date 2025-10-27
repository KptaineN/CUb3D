/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:15:26 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/25 05:58:16 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The putendl_fd function writes an array of chars inside the given
output and adds a newline at end of it.
*/

#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	while (*s)
	{
		ft_putchar_fd(*s, fd);
		s++;
	}
	ft_putchar_fd('\n', fd);
	return ;
}
