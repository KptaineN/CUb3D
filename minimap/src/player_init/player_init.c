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

static void  find_player_pos(t_cub *cub)
{
    int i;
    int j;

    i = 0;
    while (cub->map[i])
    {
        j = 0;
        while(cub->map[i][j])
        {
            if (cub->map[i][j] == 'N' || cub->map[i][j] == 'S' 
                || cub->map[i][j] == 'W' || cub->map[i][j] == 'E')
            {
                /* placer le joueur au centre de la case (en pixels) */
                cub->player->position->x = (double)j * (double)BLOCK + (double)BLOCK / 2.0;
                cub->player->position->y = (double)i * (double)BLOCK + (double)BLOCK / 2.0;
                return ;
            }
            j++;
        }
        i++;
    }
}

static void    find_player_dir(t_cub *cub)
{
    char    dir;
    int     cell_x;
    int     cell_y;
    
    /* retrouver les indices de case à partir de la position en pixels */
    cell_x = (int)(cub->player->position->x / (double)BLOCK);
    cell_y = (int)(cub->player->position->y / (double)BLOCK);
    if (!cub->map || cell_y < 0 || cell_x < 0)
        return ;
    dir = cub->map[cell_y][cell_x];
    if (dir == 'N')
    {
        cub->player->direction->x = (double)0;
        cub->player->direction->y = (double)-1;
    }
    else if (dir == 'S')
    {
        cub->player->direction->x = (double)0;
        cub->player->direction->y = (double)1;
    }
    else if (dir == 'E')
    {
        cub->player->direction->x = (double)1;
        cub->player->direction->y = (double)0;
    }
    else if (dir == 'W')
    {
        cub->player->direction->x = (double)-1;
        cub->player->direction->y = (double)0;
    }

    /* initialiser l'angle du joueur à partir du vecteur direction */
    cub->player->angle = atan2(cub->player->direction->y, cub->player->direction->x);

    /* remplacer le caractère de spawn par '0' pour que la case soit considérée libre */
    if (cub->map && cell_y >= 0 && cell_x >= 0 && cub->map[cell_y][cell_x])
        cub->map[cell_y][cell_x] = '0';
}


static void  find_player_plane(t_cub *cub)
{
    double  dir_x;
    double  dir_y;
    double  k;

    dir_x = cub->player->direction->x;
    dir_y = cub->player->direction->y;
    k = tan(deg_to_radiant(cub->player->fov) / 2.0);
    cub->player->plane->x = -dir_y * k;
    cub->player->plane->y = dir_x * k;
}

int    player_init(t_cub *cub)
{
    if (!player_allocate(cub))
        return (0) ;
    find_player_pos(cub);
    find_player_dir(cub);
    cub->player->fov = (double)60;
    find_player_plane(cub);
    cub->player->move_frame = 0.1;
    cub->player->rotation_frame = 0.05;
    cub->player->radius = 0.05;
    return (1);
}
