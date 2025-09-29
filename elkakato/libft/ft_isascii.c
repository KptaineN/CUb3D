/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 10:49:55 by xoppy             #+#    #+#             */
/*   Updated: 2024/10/23 16:39:15 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The isascii function returns a 1 if the ascii character
passed as paramenter is included in the ascii table,
if not, it returns a 0
*/

#include "libft.h"

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	else
		return (0);
}
