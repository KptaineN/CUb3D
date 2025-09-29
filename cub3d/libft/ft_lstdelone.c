/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:42:07 by adi-marc          #+#    #+#             */
/*   Updated: 2024/11/07 14:04:57 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The lstdelone function deletes a single node using the del() function passed
as parameter. It then proceeds to free the memory that the node used.
*/

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (lst == NULL || del == NULL)
		return ;
	(*del)(lst->content);
	free(lst);
}
