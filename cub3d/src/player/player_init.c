/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 11:53:33 by adi-marc          #+#    #+#             */
/*   Updated: 2025/09/30 00:08:59 by nkief            ###   ########.fr       */
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
               // cub->player->position->x = j;
               // cub->player->position->y = i;

                cub->player->position->x = ((double)j + 0.5) * BLOCK;
                cub->player->position->y = ((double)i + 0.5) * BLOCK;
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
    
   // dir = cub->map[(int)cub->player->position->y][(int)cub->player->position->x];
   dir = cub->map[(int)(cub->player->position->y / BLOCK)]
        [(int)(cub->player->position->x / BLOCK)]; 
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

/*
Dans ce code, k représente la moitié de l’angle de champ de vision (FOV)
du joueur, converti en radians, puis passé à la fonction tangente.
Plus précisément :

k = tan(deg_to_radiant(cub->player->fov) / 2.0);
Explication :

cub->player->fov : le champ de vision du joueur, en degrés.
deg_to_radiant(...) : convertit cet angle en radians.
Division par 2 : on prend la moitié de l’angle
(car le FOV s’étend de chaque côté de la direction du joueur).
tan(...) : la tangente de cet angle donne le rapport entre la largeur du plan
de projection et la distance à l’écran dans un moteur 3D de type raycasting
En résumé :
k sert à calculer la largeur du plan de caméra (ou "camera plane")
en fonction du FOV, ce qui permet de déterminer comment les rayons
sont projetés pour le rendu 3D. C’est un facteur d’échelle pour positionner
le plan perpendiculaire à la direction du joueur.
*/
void  find_player_plane(t_cub *cub)
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

void    set_player_angles(t_cub *cub)
{
    cub->player->move_frame = 0.05;
    cub->player->rotation_frame = 0.5 * BLOCK;
    cub->player->radius = 0.05  * BLOCK;
    //cub->player->angle = 0.0;
   // cub->player->cos_angle = 1.0;
   // cub->player->sin_angle = 0.0;
   // cub->player->fov = 60;
    if (cub->player->fov <= 0.0)
        cub->player->fov = 60;
    update_player_trig(cub);
}


static void set_player_keys(t_cub *cub)
{
	cub->player->key_up = false;
    cub->player->key_down = false;
    cub->player->key_left = false;
    cub->player->key_right = false;
    cub->player->left_rotate = false;
    cub->player->right_rotate = false; 
}

int    player_init(t_cub *cub)
{
    if (!player_allocate(cub))
        return (0) ;
    find_player_pos(cub);
    find_player_dir(cub);
    set_player_angles(cub);
    if (cub->player->fov <= 0.0)
        cub->player->fov = 60;
    cub->player->angle = atan2(cub->player->direction->y, cub->player->direction->x);
    update_player_trig(cub);
   // find_player_plane(cub);
   // cub->player->move_frame = 0.1;
 //   cub->player->rotation_frame = 0.05;
    set_player_keys(cub);
    // cub->player->radius = 0.05;
    return (1);
}
