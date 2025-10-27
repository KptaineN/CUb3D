/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:51:22 by adi-marc          #+#    #+#             */
/*   Updated: 2025/10/07 12:33:24 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/**
 * Clean up color resources (Floor and Ceiling colors)
 * Frees the color strings and the color structures
 */
static void	cleanup_colors(t_cub *cub)
{
	if (cub->f_color)
	{
		free(cub->f_color->string);
		free(cub->f_color);
		cub->f_color = NULL;
	}
	if (cub->c_color)
	{
		free(cub->c_color->string);
		free(cub->c_color);
		cub->c_color = NULL;
	}
}

/**
 * Clean up texture paths (North, South, West, East)
 * Frees all texture path strings
 */
static void	cleanup_paths(t_cub *cub)
{
	free(cub->no_path);
	cub->no_path = NULL;
	free(cub->so_path);
	cub->so_path = NULL;
	free(cub->we_path);
	cub->we_path = NULL;
	free(cub->ea_path);
	cub->ea_path = NULL;
}

/**
 * Clean up map and file resources
 * Frees the file content and map arrays
 */
static void	cleanup_files_and_map(t_cub *cub)
{
	free_full_map(cub->file);
	cub->file = NULL;
	free_full_map(cub->map);
	cub->map = NULL;
	cub->map_start = NULL;
}

/**
 * Clean up player resources
 * Frees player position, direction, plane vectors and player structure
 */
static void	cleanup_player(t_cub *cub)
{
	if (cub->player)
	{
		free(cub->player->position);
		free(cub->player->direction);
		free(cub->player->plane);
		free(cub->player);
		cub->player = NULL;
	}
}

/**
 * Clean up MLX resources in the correct order
 * 1. Destroy image first
 * 2. Destroy window
 * 3. Destroy display and free MLX pointer
 */
static void	cleanup_mlx(t_cub *cub)
{
	/* Destroy textures/images created via mlx before tearing down display */
	if (cub && cub->mlx)
	{
		/* main backbuffer image */
		if (cub->mlx_image)
		{
			mlx_destroy_image(cub->mlx, cub->mlx_image);
			cub->mlx_image = NULL;
		}
		/* wall textures */
		for (int i = 0; i < 4; ++i)
		{
			if (cub->textur[i].img)
			{
				mlx_destroy_image(cub->mlx, cub->textur[i].img);
				cub->textur[i].img = NULL;
				cub->textur[i].data = NULL;
			}
		}
		/* sky / floor / alien textures */
		if (cub->sky.img)
		{
			mlx_destroy_image(cub->mlx, cub->sky.img);
			cub->sky.img = NULL; cub->sky.data = NULL;
		}
		if (cub->floor.img)
		{
			mlx_destroy_image(cub->mlx, cub->floor.img);
			cub->floor.img = NULL; cub->floor.data = NULL;
		}
		if (cub->panel_tex.img)
		{
			mlx_destroy_image(cub->mlx, cub->panel_tex.img);
			cub->panel_tex.img = NULL; cub->panel_tex.data = NULL;
		}
        if (cub->door_tex.img)
        {
            mlx_destroy_image(cub->mlx, cub->door_tex.img);
            cub->door_tex.img = NULL; cub->door_tex.data = NULL;
        }
        if (cub->wood_tex.img)
        {
            mlx_destroy_image(cub->mlx, cub->wood_tex.img);
            cub->wood_tex.img = NULL; cub->wood_tex.data = NULL;
        }
        if (cub->barrel_tex.img)
        {
            mlx_destroy_image(cub->mlx, cub->barrel_tex.img);
            cub->barrel_tex.img = NULL; cub->barrel_tex.data = NULL;
        }
        for (int i = 0; i < 11; ++i)
        {
            if (cub->barrel_explosion[i].img)
            {
                mlx_destroy_image(cub->mlx, cub->barrel_explosion[i].img);
                cub->barrel_explosion[i].img = NULL;
                cub->barrel_explosion[i].data = NULL;
            }
        }
        if (cub->pickup_medkit_tex.img)
        {
            mlx_destroy_image(cub->mlx, cub->pickup_medkit_tex.img);
            cub->pickup_medkit_tex.img = NULL;
            cub->pickup_medkit_tex.data = NULL;
        }
        if (cub->pickup_ammo_tex.img)
        {
            mlx_destroy_image(cub->mlx, cub->pickup_ammo_tex.img);
            cub->pickup_ammo_tex.img = NULL;
            cub->pickup_ammo_tex.data = NULL;
        }
        for (int i = 0; i < 10; ++i)
        {
            if (cub->digit_tex[i].img)
            {
                mlx_destroy_image(cub->mlx, cub->digit_tex[i].img);
                cub->digit_tex[i].img = NULL;
                cub->digit_tex[i].data = NULL;
            }
        }
        if (cub->pistol_ammo_tex.img)
        {
            mlx_destroy_image(cub->mlx, cub->pistol_ammo_tex.img);
            cub->pistol_ammo_tex.img = NULL;
            cub->pistol_ammo_tex.data = NULL;
        }
        if (cub->mp_reload_tex.img)
        {
            mlx_destroy_image(cub->mlx, cub->mp_reload_tex.img);
            cub->mp_reload_tex.img = NULL;
            cub->mp_reload_tex.data = NULL;
        }
        if (cub->pistol_idle_tex.img)
        {
            mlx_destroy_image(cub->mlx, cub->pistol_idle_tex.img);
            cub->pistol_idle_tex.img = NULL;
            cub->pistol_idle_tex.data = NULL;
        }
        if (cub->pistol_fire_tex.img)
        {
            mlx_destroy_image(cub->mlx, cub->pistol_fire_tex.img);
            cub->pistol_fire_tex.img = NULL;
            cub->pistol_fire_tex.data = NULL;
        }
        if (cub->fists_idle_tex.img)
        {
            mlx_destroy_image(cub->mlx, cub->fists_idle_tex.img);
            cub->fists_idle_tex.img = NULL;
            cub->fists_idle_tex.data = NULL;
        }
        if (cub->fists_touch_tex.img)
        {
            mlx_destroy_image(cub->mlx, cub->fists_touch_tex.img);
            cub->fists_touch_tex.img = NULL;
            cub->fists_touch_tex.data = NULL;
        }
        for (int i = 0; i < 2; ++i)
        {
            if (cub->kick_tex[i].img)
            {
                mlx_destroy_image(cub->mlx, cub->kick_tex[i].img);
                cub->kick_tex[i].img = NULL;
                cub->kick_tex[i].data = NULL;
            }
        }
        if (cub->mini_pistol_tex.img)
        {
            mlx_destroy_image(cub->mlx, cub->mini_pistol_tex.img);
            cub->mini_pistol_tex.img = NULL;
            cub->mini_pistol_tex.data = NULL;
        }
        if (cub->mini_mp_tex.img)
        {
            mlx_destroy_image(cub->mlx, cub->mini_mp_tex.img);
            cub->mini_mp_tex.img = NULL;
            cub->mini_mp_tex.data = NULL;
        }
        if (cub->mini_hammer_tex.img)
        {
            mlx_destroy_image(cub->mlx, cub->mini_hammer_tex.img);
            cub->mini_hammer_tex.img = NULL;
            cub->mini_hammer_tex.data = NULL;
        }
        for (int i = 0; i < 6; ++i)
        {
            if (cub->hammer[i].img)
            {
                mlx_destroy_image(cub->mlx, cub->hammer[i].img);
                cub->hammer[i].img = NULL;
                cub->hammer[i].data = NULL;
            }
        }
        for (int i = 0; i < HAMMER_SPARK_FRAMES; ++i)
        {
            if (cub->hammer_spark_tex[i].img)
            {
                mlx_destroy_image(cub->mlx, cub->hammer_spark_tex[i].img);
                cub->hammer_spark_tex[i].img = NULL;
                cub->hammer_spark_tex[i].data = NULL;
            }
        }
        if (cub->alien_tex.img)
        {
			mlx_destroy_image(cub->mlx, cub->alien_tex.img);
			cub->alien_tex.img = NULL; cub->alien_tex.data = NULL;
		}
		/* minimap textures */
		if (cub->minimap_tex.img)
		{
			mlx_destroy_image(cub->mlx, cub->minimap_tex.img);
			cub->minimap_tex.img = NULL; cub->minimap_tex.data = NULL;
		}
		if (cub->minimap_wall_tex.img)
		{
			mlx_destroy_image(cub->mlx, cub->minimap_wall_tex.img);
			cub->minimap_wall_tex.img = NULL; cub->minimap_wall_tex.data = NULL;
		}
        if (cub->minimap_player_tex.img)
        {
            mlx_destroy_image(cub->mlx, cub->minimap_player_tex.img);
            cub->minimap_player_tex.img = NULL;
            cub->minimap_player_tex.data = NULL;
        }
		/* HUD textures */
		cleanup_hud_textures(cub);
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
}

/**
 * Clean up all allocated resources
 * This function cleans up all resources without freeing the main cub structure
 * Use this when you want to clean resources but keep cub for reuse
 */
void	cleanup_resources(t_cub *cub)
{
	if (!cub)
		return ;
    door_system_destroy(cub);
    barrel_system_destroy(cub);
    pickup_system_destroy(cub);
	cleanup_mlx(cub);
	cleanup_player(cub);
	if (cub->alien)
		alien_free(cub);
	cleanup_colors(cub);
	cleanup_paths(cub);
	cleanup_files_and_map(cub);
}

/**
 * Complete cleanup: free all resources and the main cub structure
 * This is the main cleanup function to call before program exit
 */
void	cleanup_all(t_cub *cub)
{
	if (!cub)
		return ;
	cleanup_resources(cub);
	free(cub);
}

/**
 * Window destroy event handler
 * Called when the user closes the window (X button)
 * Ends the MLX loop and performs complete cleanup
 */
int	on_destroy(t_cub *cub)
{
	if (cub && cub->mlx)
		mlx_loop_end(cub->mlx);
	cleanup_all(cub);
	exit(0);
	return (0);
}
