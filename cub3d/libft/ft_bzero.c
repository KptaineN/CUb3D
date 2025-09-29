/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 09:40:19 by xoppy             #+#    #+#             */
/*   Updated: 2024/10/23 16:38:53 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The bzero function takes a number n and fills the s pointer with null bytes
In order to achieve this, we need to cast a char ptr to s
since it's a void pointer
*/

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*ptr;

	i = 0;
	ptr = (char *)s;
	while (i < n)
	{
		ptr[i] = '\0';
		i++;
	}
}
