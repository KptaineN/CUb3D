/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 18:43:52 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 13:58:41 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_calloc(size_t num, size_t size)
{
	size_t	total_size;
	void	*ptr;

	total_size = num * size;
	if (num != 0 && total_size / num != size)
		return (NULL);
	ptr = malloc(total_size);
	if (ptr == NULL)
		return (NULL);
	ft_bzero(ptr, total_size);
	return (ptr);
}
/*int main()
{
	int* array = (int*)my_calloc(5, sizeof(int)); 
    if (array == NULL)
    {
    fprintf(stderr, "Échec de l'allocation mémoire\n");
    return 1;
    }
    for (int i = 0; i < 5; i++)
    {
    printf("array[%d] = %d\n", i, array[i]);
    }
    free(array);
    return 0;
}*/
