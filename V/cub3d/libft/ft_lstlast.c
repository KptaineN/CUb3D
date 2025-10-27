/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:28:52 by adi-marc          #+#    #+#             */
/*   Updated: 2024/11/07 13:31:54 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The lstlast function returns the last node of a linked list, it finds it by 
iterating through the whole list untill it finds the node that has next = NULL
*/

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}
