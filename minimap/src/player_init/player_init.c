/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 11:53:33 by adi-marc          #+#    #+#             */
/*   Updated: 2025/09/22 13:38:50 by adi-marc         ###   ########.fr       */
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
                cub->player->position->x = j;
                cub->player->position->y = i;
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
    
    dir = cub->map[(int)cub->player->position->y][(int)cub->player->position->x];
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
