/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:55:36 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/23 16:39:22 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The isdigit function returns a 1 if the ascii character
passed as a parameter is a digit, if not, it returns a 0
*/

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}
