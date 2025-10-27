/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:05:30 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/23 16:39:27 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The isprint function returns a 1 if the ascii character
passed as parameter is a printable character, if not,
it returns a 0
*/

#include "libft.h"

int	ft_isprint(char c)
{
	if (c >= 32 && c <= 126)
		return (1);
	else
		return (0);
}
