/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:51:22 by adi-marc          #+#    #+#             */
/*   Updated: 2025/09/26 16:51:43 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int on_destroy(t_cub *cub)
{
    if (!cub) 
        return (0);
    if (cub->mlx_window && cub->mlx)
        mlx_destroy_window(cub->mlx, cub->mlx_window);
    cub->mlx_window = NULL;
    if (cub->mlx)
        mlx_loop_end(cub->mlx);
    return (0);
}

int cleanup_resources(t_cub *cub)
{
    if (!cub) 
        return (0);
    if (cub->mlx_image && cub->mlx) 
    {
        mlx_destroy_image(cub->mlx, cub->mlx_image);
        cub->mlx_image = NULL;
    }
    if (cub->mlx_window && cub->mlx)
    {
        mlx_destroy_window(cub->mlx, cub->mlx_window);
        cub->mlx_window = NULL;
    }
    if (cub->mlx)
    {
        mlx_destroy_display(cub->mlx);
        free(cub->mlx);
        cub->mlx = NULL;
    }
    return (0);
}