/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_game.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 21:10:00 by nkief             #+#    #+#             */
/*   Updated: 2025/09/22 16:38:40 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/**
 * Initialise MiniLibX et crée la fenêtre
 */
void	init_mlx(t_cub *cub)
{
	cub->mlx = mlx_init();
	if (!cub->mlx)
	{
		printf("Erreur: mlx_init échoué\n");
		return ;
	}
	cub->window = mlx_new_window(cub->mlx, WIDTH, HEIGHT, "Cub3D");
	if (!cub->window)
	{
		printf("Erreur: mlx_new_window échoué\n");
		return ;
	}
}
/*
pour close la window
*/

/**
 * Initialise les composants du jeu
 */
int16_t	initialisation(t_cub *cubing)
{
    // Allouer et initialiser le joueur
    if (!cubing->player)
    {
        cubing->player = malloc(sizeof(t_player));
        if (!cubing->player)
            return (-1);
    }
    init_player(cubing->player);  // Initialise la struct joueur
    init_mlx(cubing);
    cubing->image = mlx_new_image(cubing->mlx, WIDTH, HEIGHT);
    cubing->data = mlx_get_data_addr(cubing->image, &cubing->bpp, &cubing->size_line, &cubing->endian);
    mlx_put_image_to_window(cubing->mlx, cubing->window, cubing->image, 0, 0);
    
	return (0);
}

int draw_loop(t_cub *cubing)
{
   // printf("DEBUG draw_loop: position x=%.0f, y=%.0f\n", cubing->player->position->x, cubing->player->position->y);  // Print pour voir la position

	t_player *player = cubing->player;
	move_player(player);
	close_image(cubing);
	draw_y_triangle(player->position->x, player->position->y, 10 ,0xFFFF00, cubing);
	draw_map(cubing);
    mlx_put_image_to_window(cubing->mlx, cubing->window, cubing->image, 0, 0);
	return (0);
}


/**
 * Lance le jeu
 * @param cubing Structure principale contenant toutes les données
 * @return 0 en cas de succès
 */
int16_t	start_game(t_cub *cubing)
{
    if (initialisation(cubing) != 0)
    {
        printf("Erreur lors de l'initialisation\n");
        return (-1);
    }
    printf("initialisation fini\n");
    
    // Rafraîchir l'affichage
    mlx_put_image_to_window(cubing->mlx, cubing->window, cubing->image, 0, 0);
    
    mlx_hook(cubing->window, 2, 1L<<0, key_press, cubing);
    mlx_hook(cubing->window, 3, 1L<<1, key_drop, cubing);
    mlx_loop_hook(cubing->mlx, draw_loop, cubing);
    mlx_loop(cubing->mlx);
    return (0);
}