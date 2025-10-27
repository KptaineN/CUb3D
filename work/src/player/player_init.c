/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 11:53:33 by adi-marc          #+#    #+#             */
/*   Updated: 2025/10/20 14:22:23 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void  find_player_pos(t_cub *cub)
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

void    find_player_dir(t_cub *cub)
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
void find_player_plane(t_player *p)
{
    double vecteur_perpendiculaire;
    // Direction du joueur: (dir_x, dir_y) = (cos, sin)
    double dir_x;
    double dir_y;

    dir_x = p->direction->x;
    dir_y = p->direction->y;
    // Vect_perpe = tan(FOV/2). Plus FOV est grand, plus le plane est "long".
     /* Utiliser la FOV courante (current_fov) pour que les changements dynamiques
         (ex: course) se reflètent directement sur le plane caméra. */
     vecteur_perpendiculaire = tan(deg_to_radiant(p->current_fov) / 2.0);
    // Un vecteur perpendiculaire à (dir_x, dir_y) est (-dir_y, dir_x)
    // On le met à l'échelle par Vect_perpe -> c'est notre "plane".
    p->plane->x = -dir_y * vecteur_perpendiculaire;
    p->plane->y =  dir_x * vecteur_perpendiculaire;
}

void    set_player_angles(t_cub *cub)
{
    cub->player->walk_speed = 3.5;         // Vitesse de marche (ajustée)
    cub->player->run_speed = 7.5;          // Vitesse en courant (plus nerveuse)
    cub->player->crouch_speed = 1.8;       // Vitesse en accroupi
    cub->player->move_frame = cub->player->walk_speed; // vitesse actuelle
    cub->player->rotation_frame = 0.07;    // Rotation légèrement plus douce
    cub->player->radius = 8.0;             // Rayon de collision normal
    cub->player->base_fov = 60.0;          // FOV de base
    cub->player->run_fov_delta = 8.0;     // augmentation de FOV quand on court (plus marquée)
    cub->player->fov = cub->player->base_fov;
    cub->player->current_fov = cub->player->fov;
    cub->player->is_running = false;
    cub->player->is_crouching = false;
    cub->player->eye_height_base = (double)HEIGHT * 0.5; /* hauteur caméra normale en pixels */
    cub->player->eye_height_current = cub->player->eye_height_base; /* valeur runtime initiale */
    cub->player->crouch_eye_height_pixels = cub->player->eye_height_base * 0.85; /* moins bas, effet réduit */
    /* système de combat */
    cub->player->health = 4;              /* 4 cœurs au départ */
    cub->player->max_health = 4;          /* maximum 4 cœurs */
    cub->player->hitbox_radius = 12.0;    /* rayon hitbox joueur (pixels) */
    cub->player->invincibility_timer = 0.0;
    cub->player->is_invincible = 0;
    /* effets visuels */
    cub->player->damage_flash_timer = 0.0;
    cub->player->damage_level = 0;
    cub->player->is_dead = 0;
    cub->player->death_timer = 0.0;
}

static void set_player_keys(t_cub *cub)
{
    cub->player->key_up = false;
    cub->player->key_down = false;
    cub->player->key_left = false;
    cub->player->key_right = false;
    cub->player->left_rotate = false;
    cub->player->right_rotate = false; 
    /* minimap toggled via cub->show_minimap */
    /* smooth half-turn animation state */
    cub->player->is_turning = false;
    cub->player->turn_target_angle = 0.0;
    cub->player->turn_speed = 0.25; /* vitesse rotation rapide (rad/frame) - environ 14° par frame = très rapide */
}

int    player_init(t_cub *cub)
{
    if (!player_allocate(cub))          // 1. Alloue mémoire
        return (0) ;
    find_player_pos(cub);             // 2. Trouve position de spawn
    find_player_dir(cub);            // 3. Définit direction selon N/S/E/W
    set_player_angles(cub);         // 4. Configure vitesses (sans écraser direction)
    
    // Calcule angle et trigonométrie BASÉS sur la direction
    if (cub->player->fov <= 0.0)
        cub->player->fov = 60;
    // Calculer l'angle BASÉ sur la direction définie par le spawn
    cub->player->angle = atan2(cub->player->direction->y, cub->player->direction->x);
    // Calculer les valeurs trigonométriques sans écraser la direction
    cub->player->cos_angle = cos(cub->player->angle);
    cub->player->sin_angle = sin(cub->player->angle);
    find_player_plane(cub->player);                      // 5. Calcule le plan caméra
    set_player_keys(cub);                               // 6. Init état des touches
    weapon_init(cub->player);                           // 7. Init weapon system
    /* store initial spawn position & angle into cub for later respawns */
    if (cub && cub->player && cub->player->position)
    {
        cub->player_spawn_x = cub->player->position->x;
        cub->player_spawn_y = cub->player->position->y;
        cub->player_spawn_angle = cub->player->angle;
    cub->player_spawn_saved = 1;
    }
    return (1);
}
