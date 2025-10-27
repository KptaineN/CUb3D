/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 21:30:00 by nkief             #+#    #+#             */
/*   Updated: 2025/09/26 12:12:19 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/**
 * Initialise les valeurs par défaut du joueur
 * @param player Pointeur vers la structure du joueur
 */
void	init_player(t_player *player)
{
    if (!player)
		return ;
	
	// Allouer les coordonnées - OBLIGATOIRE !
	/*player->position = malloc(sizeof(t_cord));
	player->direction = malloc(sizeof(t_cord));
	
	if (!player->position || !player->direction)
	{
		if (player->position)
			free(player->position);
		if (player->direction)
			free(player->direction);
		return ;
	}*/
    player->angle = 0.0;
    player->cos_angle = 1.0;
    player->sin_angle = 0.0;
    player->fov = 66.0;
	player->move_frame = 1;
    player->rotation_frame = 0.05;
    player->radius = 12;
    player->position->x = WIDTH / 2; //a modifier plus tard
    player->position->y = HEIGHT / 2;
    player->direction->x = 0;
    player->direction->y = 0;
    player->plane->x = 0.0;
    player->plane->y = 0.0;
	player->key_up = false;
    player->key_down = false;
    player->key_left = false;
    player->key_right = false;
    player->left_rotate = false;
    player->right_rotate = false; 
}

int key_press(int keycode, t_cub *cub)
{
    //printf("DEBUG: key_press appelée avec keycode=%d\n", keycode);
    if (cub && cub->player)
    {
        if (keycode == W || keycode == UP || keycode == Z)
        {
            cub->player->key_up = true;
        }
        else if (keycode == S || keycode == DOWN)
        {
            cub->player->key_down = true;
        }
        else if (keycode == Q || keycode == A)
        {
            cub->player->key_left = true;
        }
        else if (keycode == D)
        {
            cub->player->key_right = true;
        }
        else if (keycode == LEFT)
        {
            cub->player->left_rotate = true;
        }
        else if (keycode == RIGHT)
        {
            cub->player->right_rotate = true;
        }
        else if (keycode == ESC_KEY)
        {
            cleanup_resources(cub);
            exit(0);
        }
    }
    else
    {
        printf("DEBUG: cub ou cub->game est NULL\n");
    }
    return (0);
}

int key_drop(int keycode, t_cub *cub)
{
   // printf("DEBUG: key_drop appelée avec keycode=%d\n", keycode);
    if (cub && cub->player)
    {
        if (keycode == W  || keycode == Z || keycode == UP)
        {
            cub->player->key_up = false;
        }
        if (keycode == S || keycode == DOWN)
        {
            cub->player->key_down = false;
        }
        if (keycode == A || keycode == Q)
        {
            cub->player->key_left = false;
        }
        if (keycode == D)
        {
            cub->player->key_right = false;
        }
        else if (keycode == LEFT)
        {
            cub->player->left_rotate = false;
        }
        else if (keycode == RIGHT)
        {
            cub->player->right_rotate = false;
        }
    }
    return (0);
}

/**
 * Libère la mémoire allouée pour le joueur
 * @param player Pointeur vers la structure du joueur
 */
void	free_player(t_player *player)
{
	if (!player)
		return ;
	
	if (player->position)
		free(player->position);
	if (player->direction)
		free(player->direction);
	if (player->plane)
		free(player->plane);
	free(player);
}

void    move_player(t_cub *cub)
{
    //printf("DEBUG move_player: key_right=%d, position avant=%.0f\n", player->key_right, player->position->x);
    t_player       *player;
    double      step;
    double      next_x;
    double      next_y;
    bool            rotated;

    if (!cub || !cub->player)
        return ;
    player = cub->player;
    rotated = false;
    if (player->left_rotate)
    {
        player->angle -= player->rotation_frame;
        rotated = true;
    }
    if (player->right_rotate)
    {
        player->angle += player->rotation_frame;
        rotated = true;
    }
    if (player->angle < 0.0)
        player->angle += 2.0 * PI;
    if (player->angle >= 2.0 * PI)
        player->angle -= 2.0 * PI;

    if (rotated)
        update_player_trig(player);
    step = (double)player->move_frame;
    next_x = player->position->x;
    next_y = player->position->y;
    if (player->key_up)
    {
        next_x += player->cos_angle * step;
        next_y += player->sin_angle * step;
    }
    if (player->key_down)
    {
        next_x -= player->cos_angle * step;
        next_y -= player->sin_angle * step;
    }
    if (player->key_right)
    {
        next_x += -player->sin_angle * step;
        next_y += player->cos_angle * step;
    }
    if (player->key_left)
    {
        next_x += player->sin_angle * step;
        next_y += -player->cos_angle * step;
    }

    // Vérification des collisions avant d'appliquer les nouvelles coordonnées
    if (!touch(next_x, player->position->y, cub))
        player->position->x = next_x;
    if (!touch(player->position->x, next_y, cub))
        player->position->y = next_y;

    //printf("DEBUG move_player: position après=%.0f\n", player->position->x);

// Limites pour éviter de sortir de la fenêtre
    if (player->position->x < 0) player->position->x = 0;
    if (player->position->y < 0) player->position->y = 0;
    if (player->position->x > WIDTH - 10) player->position->x = WIDTH - 10;  // -10 pour la taille du triangle
    if (player->position->y > HEIGHT - 10) player->position->y = HEIGHT - 10;
}