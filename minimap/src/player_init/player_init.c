/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 11:53:33 by adi-marc          #+#    #+#             */
/*   Updated: 2025/09/23 18:21:58 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void find_player_pos(t_cub *cub)
{
    int x;
    int y;

    y = 0;
    while (cub->map[y])
    {
        x = 0;
        while (cub->map[y][x])
        {
            if (cub->map[y][x] == 'N' || cub->map[y][x] == 'S'
                || cub->map[y][x] == 'W' || cub->map[y][x] == 'E')
            {
                cub->player->position->x = (double)x * (double)BLOCK
                    + (double)BLOCK / 2.0;
                cub->player->position->y = (double)y * (double)BLOCK
                    + (double)BLOCK / 2.0;
                return ;
            }
            x++;
        }
        y++;
    }
}

static void find_player_dir(t_cub *cub)
{
    char    dir;
    int     cell_x;
    int     cell_y;

    cell_x = (int)(cub->player->position->x / (double)BLOCK);
    cell_y = (int)(cub->player->position->y / (double)BLOCK);
    if (!cub->map || cell_y < 0 || cell_x < 0)
        return ;
    dir = cub->map[cell_y][cell_x];
    if (dir == 'N')
    {
        cub->player->direction->x = 0.0;
        cub->player->direction->y = -1.0;
    }
    else if (dir == 'S')
    {
        cub->player->direction->x = 0.0;
        cub->player->direction->y = 1.0;
    }
    else if (dir == 'E')
    {
        cub->player->direction->x = 1.0;
        cub->player->direction->y = 0.0;
    }
    else if (dir == 'W')
    {
        cub->player->direction->x = -1.0;
        cub->player->direction->y = 0.0;
    }
    cub->player->angle = atan2(cub->player->direction->y,
            cub->player->direction->x);
    if (cub->map && cell_y >= 0 && cell_x >= 0 && cub->map[cell_y][cell_x])
        cub->map[cell_y][cell_x] = '0';
}

int player_init(t_cub *cub)
{
    if (!player_allocate(cub))
        return (0);
    init_player(cub->player);
    find_player_pos(cub);
    find_player_dir(cub);
    update_player_trig(cub->player);
    return (1);
}
