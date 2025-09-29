/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 14:44:09 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/27 20:37:32 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The toupper function takes an ascii character as
parameter and returns the uppercase version of it if
possible, if not, it just returns the initial character
*/

#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 'a' + 'A');
	return (c);
}
