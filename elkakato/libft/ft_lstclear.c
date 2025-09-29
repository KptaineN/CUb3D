/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:47:12 by adi-marc          #+#    #+#             */
/*   Updated: 2024/11/07 14:06:34 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The lstclear function deletes all the nodes following the node given in the 
parameters, it uses a temporary node to store the next node of the one being
deleted to continue the iteration once it disappears. There is no need to free
since we are using the lstdelone function that takes care of it.
*/

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;

	if (lst == NULL || *lst == NULL || del == NULL)
		return ;
	while (*lst != NULL)
	{
		tmp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = tmp;
	}
	*lst = NULL;
}
