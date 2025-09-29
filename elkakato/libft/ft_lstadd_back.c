/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:32:29 by adi-marc          #+#    #+#             */
/*   Updated: 2024/11/07 13:37:17 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The lstadd_back function adds a node to the end of the list, it does so by first
checking if the list already exist, if it doesn't it creates one.
If the list exists it uses lstlast to iterate the list untill the last node and
add the new node to the last node->next.
*/

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		last = ft_lstlast(*lst);
		last->next = new;
	}
}
