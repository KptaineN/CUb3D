/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 14:34:42 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/27 20:38:21 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The tolower function takes an ascii character as
parameter and returns the lowercase version of it if
possible, if not, it just returns the initial character
*/

#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 'a' - 'A');
	return (c);
}
