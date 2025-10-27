/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:00:39 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/23 16:39:06 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The isalpha function returns a 1 if the ascii character
passed as a parameter is an alphabetic character, if not,
it returns a 0
*/

#include "libft.h"

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else
		return (0);
}
