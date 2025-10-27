/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:08:29 by adi-marc          #+#    #+#             */
/*   Updated: 2025/09/29 12:22:16 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void    init_mlx(t_cub *cub)
{
    cub->mlx = mlx_init();
    if (!cub->mlx)
    {
        printf("Error: mlx_init failed...\n");
        free(cub->mlx);
        return ;
    }
    cub->mlx_window = mlx_new_window(cub->mlx, WIDTH, HEIGHT, "cub3D");
    if (!cub->mlx_window)
    {
        printf("Error: mlx_new_window failed...\n");
        free(cub->mlx);
        free(cub->mlx_window);
        return ;
    }
}

/**
 * Initialise les composants du jeu *
 
 * * Create a new MLX compatible image.
  mlx_new_image(cubing->mlx, WIDTH, HEIGHT);
** @param	void *mlx_ptr	the mlx instance pointer;
** @param	int  width		the width of the image to be created;
** @param	int  height		the height of the image to be created;
** @return	void*			the image instance reference.

** Gets the data address of the current image.
  mlx_get_data_addr(cubing->image, &cubing->bpp, &cubing->size_line,
        &cubing->endian);
** @param	void *img_ptr			the image instance;
** @param	int  *bits_per_pixel	a pointer to where the bpp is written;
** @param	int  *size_line		a pointer to where the line is written;
** @param	int  *endian		a pointer to where the endian is written;
** @return	char*				the memory address of the image.

** Put an image to the given window.
 mlx_put_image_to_window(cubing->mlx, cubing->window, cubing->image, 0, 0);
** @param	void *mlx_ptr	the mlx instance;
** @param	void *win_ptr	the window instance;
** @param	int  x			the x location of where the image ought to be placed;
** @param	int  y			the y location of where the image ought to be placed;
** @return	int				has no return value (bc).
*/
int window_init(t_cub *cub)
{
    init_mlx(cub);
    cub->mlx_image = mlx_new_image(cub->mlx, WIDTH, HEIGHT);
    if (!cub->mlx_image)
    {
        printf("Error: mlx_new_image failed...\n");
        return (0);
    }
    cub->mlx_data = mlx_get_data_addr(cub->mlx_image, &cub->mlx_bpp, &cub->mlx_line_size, &cub->mlx_endian);
    if (!cub->mlx_data)
    {
        printf("Error: mlx_get_data_addr failed...\n");
        return (0);
    }
    mlx_put_image_to_window(cub->mlx, cub->mlx_window, cub->mlx_image, 0, 0);
    return (1);
}
