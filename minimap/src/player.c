/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 21:30:00 by nkief             #+#    #+#             */
/*   Updated: 2025/09/23 18:07:55 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/**
 * Initialise les valeurs par défaut du joueur
 * @param player Pointeur vers la structure du joueur
 */
void	init_player(t_player *player)
{
    if (!player)
		return ;
	
	// Allouer les coordonnées - OBLIGATOIRE !
	player->position = malloc(sizeof(t_cord));
	player->direction = malloc(sizeof(t_cord));
	
	if (!player->position || !player->direction)
	{
		if (player->position)
			free(player->position);
		if (player->direction)
			free(player->direction);
		return ;
	}
	
    player->position->x = WIDTH / 2; //a modifier plus tard
    player->position->y = HEIGHT / 2;
    player->direction->x = 0;
    player->direction->y = 0;
	player->key_up = false;
    player->key_down = false;
    player->key_left = false;
    player->key_right = false;

    player->left_rotate = false;
    player->right_rotate = false; 
}
int key_press(int keycode, t_cub *cub)
{
    printf("DEBUG: key_press appelée avec keycode=%d\n", keycode);
    if (cub && cub->player)
    {
        printf("DEBUG: cub et cub->player sont valides\n");
        if (keycode == W)
        {
            cub->player->key_up = true;
            printf("DEBUG: key_up set to true\n");
        }
        else if (keycode == S)
        {
            cub->player->key_down = true;
            printf("DEBUG: key_down set to true\n");
        }
        else if (keycode == A)
        {
            cub->player->key_left = true;
            printf("DEBUG: key_left set to true\n");
        }
        else if (keycode == D)
        {
            cub->player->key_right = true;
            printf("DEBUG: key_right set to true\n");
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
    printf("DEBUG: key_drop appelée avec keycode=%d\n", keycode);
    if (cub && cub->player)
    {
        if (keycode == W)
        {
            cub->player->key_up = false;
            printf("DEBUG: key_up set to false\n");
        }
        if (keycode == S)
        {
            cub->player->key_down = false;
            printf("DEBUG: key_down set to false\n");
        }
        if (keycode == A)
        {
            cub->player->key_left = false;
            printf("DEBUG: key_left set to false\n");
        }
        if (keycode == D)
        {
            cub->player->key_right = false;
            printf("DEBUG: key_right set to false\n");
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

	
	free(player);
}

void    move_player(t_player *player)
{
    //printf("DEBUG move_player: key_right=%d, position avant=%.0f\n", player->key_right, player->position->x);
    
    int walking;
    double angle_speed = 0.05;
    float cos_angle = cos(player->angle);
    float sin_angle = sin(player->angle);
    
    if (player->left_rotate)
        player->angle -= angle_speed;
    if (player->right_rotate)
        player->angle += angle_speed;
    if (player->angle < 0)
        player->angle += 2 * PI;
    if (player->angle >= 2 * PI)
        player->angle -= 2 * PI;

    walking = 5;
    if (player->key_up)
    {
        player->position->x += cos_angle * walking;
        player->position->y += sin_angle * walking; 
    }
    if (player->key_down)
    {
        player->position->x -= cos_angle * walking;
        player->position->y -= sin_angle * walking; 
    }
    // Strafe left/right (perpendicular to direction)
    if (player->key_left)
    {
        player->position->x += -sin_angle * walking;
        player->position->y += cos_angle * walking; 
    }
    if (player->key_right)
    {
        player->position->x += sin_angle * walking;
        player->position->y += -cos_angle * walking; 
    }
    
    //printf("DEBUG move_player: position après=%.0f\n", player->position->x);

// Limites pour éviter de sortir de la fenêtre
    if (player->position->x < 0) player->position->x = 0;
    if (player->position->y < 0) player->position->y = 0;
    if (player->position->x > WIDTH - 10) player->position->x = WIDTH - 10;  // -10 pour la taille du triangle
    if (player->position->y > HEIGHT - 10) player->position->y = HEIGHT - 10;
}