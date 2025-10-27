/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_keys.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:53:52 by adi-marc          #+#    #+#             */
/*   Updated: 2025/09/26 17:08:03 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"


int key_press(int keycode, t_cub *cub)
{
    //printf("DEBUG: key_press appelée avec keycode=%d\n", keycode);
    if (cub && cub->player)
    {
        if (keycode == W || keycode == UP || keycode == Z)
            cub->player->key_up = true;
        else if (keycode == S || keycode == DOWN)
            cub->player->key_down = true;
        else if (keycode == Q || keycode == A)
            cub->player->key_left = true;
        else if (keycode == D)
            cub->player->key_right = true;
        else if (keycode == LEFT)
            cub->player->left_rotate = true;
        else if (keycode == RIGHT)
            cub->player->right_rotate = true;
        else if (keycode == ESC_KEY)
        {
            cleanup_resources(cub);
            exit(0);
        }
    }
    else
        printf("DEBUG: cub ou cub->game est NULL\n");
    return (0);
}

int key_drop(int keycode, t_cub *cub)
{
   // printf("DEBUG: key_drop appelée avec keycode=%d\n", keycode);
    if (cub && cub->player)
    {
        if (keycode == W  || keycode == Z || keycode == UP)
            cub->player->key_up = false;
        if (keycode == S || keycode == DOWN)
            cub->player->key_down = false;
        if (keycode == A || keycode == Q)
            cub->player->key_left = false;
        if (keycode == D)
            cub->player->key_right = false;
        else if (keycode == LEFT)
            cub->player->left_rotate = false;
        else if (keycode == RIGHT)
            cub->player->right_rotate = false;
    }
    return (0);
}
