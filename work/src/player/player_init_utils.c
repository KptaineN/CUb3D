/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_init_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 13:03:07 by adi-marc          #+#    #+#             */
/*   Updated: 2025/10/01 11:01:56 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

double  deg_to_radiant(double deg)
{
    return (deg * M_PI / 180.0);
}

int  player_allocate(t_cub *cub)
{
    cub->player = ft_calloc(1, sizeof(t_player));
    if (!cub->player)
        return (0);
    cub->player->position = ft_calloc(1, sizeof(t_cord));
    if (!cub->player->position)
    {
        free(cub->player);
        cub->player = NULL;
        return (0);
    }
    cub->player->direction = ft_calloc(1, sizeof(t_cord));
    if (!cub->player->direction)
    {
        free(cub->player->position);
        free(cub->player);
        cub->player = NULL;
        return (0);
    }
    cub->player->plane = ft_calloc(1, sizeof(t_cord));
    if (!cub->player->plane)
    {
        free(cub->player->direction);
        free(cub->player->position);
        free(cub->player);
        cub->player = NULL;
        return (0);
    }
    return (1);
}
