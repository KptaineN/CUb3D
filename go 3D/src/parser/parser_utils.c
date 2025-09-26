/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 12:58:37 by adi-marc          #+#    #+#             */
/*   Updated: 2025/09/19 13:01:04 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

//line_is_empty

//count_ma_lines

int ft_lastchars(const char *s, const char *suffix, size_t i)
{
    char    *last;
    int result;
    size_t  len;

    if (!s || !suffix)
        return (1);
    len = ft_strlen(s);
    if (len < i)
        return (1);
    last = ft_substr(s, len - i, i);
    if (!last)
        return (1);
    result = ft_strncmp(last, suffix, i);
    free(last);
    return (result);
}