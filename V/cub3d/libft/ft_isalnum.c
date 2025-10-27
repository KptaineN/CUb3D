/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 10:46:33 by xoppy             #+#    #+#             */
/*   Updated: 2024/10/23 16:39:00 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The isalnum function returns a 1 if the ascii character
passed as a parameter is an alphanumerical character,
if not, it returns a 0
*/

#include "libft.h"

int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}
