/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_init_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 13:03:07 by adi-marc          #+#    #+#             */
/*   Updated: 2025/09/22 13:39:04 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

double  deg_to_radiant(double deg)
{
    return (deg * M_PI / 180.0);
}

void    player_cleanup(t_cub *cub)
{
    if (!cub || !cub->player)
        return;
    
    if (cub->player->position)
        free(cub->player->position);
    if (cub->player->direction)
        free(cub->player->direction);
    if (cub->player->plane)
        free(cub->player->plane);
    free(cub->player);
    cub->player = NULL;
}

int  player_allocate(t_cub *cub)
{
    cub->player = malloc(sizeof(t_player));
    if (!cub->player)
        return (0);
    cub->player->position = malloc(sizeof(t_cord));
    if (!cub->player->position)
    {
        free(cub->player);
        cub->player = NULL;
        return (0);
    }
    cub->player->direction = malloc(sizeof(t_cord));
    if (!cub->player->direction)
    {
        free(cub->player->position);
        free(cub->player);
        cub->player = NULL;
        return (0);
    }
    cub->player->plane = malloc(sizeof(t_cord));
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
