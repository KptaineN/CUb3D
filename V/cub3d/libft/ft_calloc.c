/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:41:38 by adi-marc          #+#    #+#             */
/*   Updated: 2024/10/28 15:10:36 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The calloc functions allocates memory for nmeb items and fills it with zeroes,
the size of each element is defines by the size variable. It checks if either
is NULL before starting and it also checks the total size calculation has been
done correctly.
*/

#include "libft.h"

void	*ft_calloc(size_t nmeb, size_t size)
{
	void	*ptr;

	if (nmeb == 0 || size == 0)
		return (malloc(0));
	if (nmeb > (SIZE_MAX / size))
		return (NULL);
	ptr = malloc(nmeb * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, nmeb * size);
	return (ptr);
}
