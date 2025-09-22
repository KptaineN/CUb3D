/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bubble_str_sort.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:59:11 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 15:07:29 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static size_t	get_tab_len(char **tab)
{
	size_t	len;

	len = 0;
	while (tab[len])
		len++;
	return (len);
}

void	ft_bubble_str_sort(char **tab)
{
	size_t	i;
	size_t	j;
	size_t	len;

	i = 0;
	j = 0;
	len = 0;
	if (!tab)
		return ;
	len = get_tab_len(tab);
	while (i < len)
	{
		j = 0;
		while (j + 1 < len - i)
		{
			if (ft_strcmp(tab[j], tab[j + 1]) > 0)
				ft_swap_str(&tab[j], &tab[j + 1]);
			j++;
		}
		i++;
	}
}
