/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:50:22 by adi-marc          #+#    #+#             */
/*   Updated: 2024/11/06 13:47:01 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The lstnew function creates a new node into the linked list stated in the
libft.h file, in this case it creates a node with the content passed to the
parameter and the next pointing to NULL.
*/

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	struct s_list	*new_node;

	new_node = malloc(sizeof(struct s_list));
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}
