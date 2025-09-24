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

static bool can_walk(t_cub *cub, double next_x, double next_y)
{
    double  radius;

    radius = cub->player->radius;
    if (touch(next_x - radius, next_y - radius, cub))
        return (false);
    if (touch(next_x + radius, next_y - radius, cub))
        return (false);
    if (touch(next_x - radius, next_y + radius, cub))
        return (false);
    if (touch(next_x + radius, next_y + radius, cub))
        return (false);
    return (true);
}

void    init_player(t_player *player)
{
    if (!player)
        return ;
    player->angle = 0.0;
    player->cos_angle = 1.0;
    player->sin_angle = 0.0;
    player->fov = 60.0;
    player->move_speed = 6;
    player->rotation_speed = 0.05;
    player->radius = 12.0;
    player->position->x = 0.0;
    player->position->y = 0.0;
    player->direction->x = 0.0;
    player->direction->y = 0.0;
    player->plane->x = 0.0;
    player->plane->y = 0.0;
    player->key_up = false;
    player->key_down = false;
    player->key_left = false;
    player->key_right = false;
    player->left_rotate = false;
    player->right_rotate = false;
}

void    update_player_trig(t_player *player)
{
    double  plane_factor;

    if (!player)
        return ;
    player->cos_angle = cos(player->angle);
    player->sin_angle = sin(player->angle);
    player->direction->x = player->cos_angle;
    player->direction->y = player->sin_angle;
    plane_factor = tan(deg_to_radiant(player->fov) / 2.0);
    player->plane->x = -player->sin_angle * plane_factor;
    player->plane->y = player->cos_angle * plane_factor;
}

void    move_player(t_cub *cub)
{
    t_player    *player;
    double      step;
    double      next_x;
    double      next_y;
    bool        rotated;

    if (!cub || !cub->player)
        return ;
    player = cub->player;
    rotated = false;
    if (player->left_rotate)
    {
        player->angle -= player->rotation_speed;
        rotated = true;
    }
    if (player->right_rotate)
    {
        player->angle += player->rotation_speed;
        rotated = true;
    }
    if (player->angle < 0.0)
        player->angle += 2.0 * PI;
    if (player->angle >= 2.0 * PI)
        player->angle -= 2.0 * PI;
    if (rotated)
        update_player_trig(player);
    step = (double)player->move_speed;
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
    if (player->key_left)
    {
        next_x += -player->sin_angle * step;
        next_y += player->cos_angle * step;
    }
    if (player->key_right)
    {
        next_x += player->sin_angle * step;
        next_y += -player->cos_angle * step;
    }
    if (can_walk(cub, next_x, player->position->y))
        player->position->x = next_x;
    if (can_walk(cub, player->position->x, next_y))
        player->position->y = next_y;
}

int key_press(int keycode, t_cub *cub)
{
    if (!cub || !cub->player)
        return (0);
    if (keycode == W)
        cub->player->key_up = true;
    else if (keycode == S)
        cub->player->key_down = true;
    else if (keycode == A)
        cub->player->key_left = true;
    else if (keycode == D)
        cub->player->key_right = true;
    else if (keycode == LEFT)
        cub->player->left_rotate = true;
    else if (keycode == RIGHT)
        cub->player->right_rotate = true;
    else if (keycode == KEY_M)
        toggle_minimap(cub);
    else if (keycode == ESC_KEY)
    {
        cleanup_resources(cub);
        exit(0);
    }
    /* Anciennes traces de debug (printf) supprimées pour fluidifier la boucle
     * d'évènements. */
    return (0);
}

int key_drop(int keycode, t_cub *cub)
{
    if (!cub || !cub->player)
        return (0);
    if (keycode == W)
        cub->player->key_up = false;
    if (keycode == S)
        cub->player->key_down = false;
    if (keycode == A)
        cub->player->key_left = false;
    if (keycode == D)
        cub->player->key_right = false;
    if (keycode == LEFT)
        cub->player->left_rotate = false;
    if (keycode == RIGHT)
        cub->player->right_rotate = false;
    return (0);
}

void    free_player(t_player *player)
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
