/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_game.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 21:10:00 by nkief             #+#    #+#             */
/*   Updated: 2025/09/26 14:46:19 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"


// fonction pour la collision
bool    touch(double px, double py, t_cub *game)
{
    int32_t cell_x;
    int32_t cell_y;
    size_t  line_len;

    if (!game || !game->map)
        return (true);

    // Hors monde négatif => collision
    if (px < 0 || py < 0)
        return (true);

    // Convertit pixels -> cases
    cell_x = (int32_t)(px / BLOCK);
    cell_y = (int32_t)(py / BLOCK);

    // Hors bornes ou ligne trop courte => on considère “mur”
    if (cell_x < 0 || cell_y < 0)
        return (true);
    if (cell_x >= game->map_width || cell_y >= game->map_height)
        return (true);
    line_len = ft_strlen(game->map[cell_y]);
    if ((size_t)cell_x >= line_len)
        return (true);

    // Collision si la case vaut '1'
    return (game->map[cell_y][cell_x] == '1');
}

static void render_scene(t_cub *cub)
{
    int     column;
    double  camera_x;
    double  ray_dir_x;
    double  ray_dir_y;

    column = 0;
    while (column < WIDTH)
    {
         // camera_x ∈ [-1, +1] : -1 = bord gauche, 0 = centre, +1 = bord droit.
        // C’est la coordonnée horizontale sur le plan de projection.
        camera_x = 2.0 * column / (double)WIDTH - 1.0;
        // Rayon dans le monde :
        // Rayon central = dir ; on l’étale en ajoutant “plane * camera_x”.
        ray_dir_x = cub->player->cos_angle + cub->player->plane->x * camera_x;
        ray_dir_y = cub->player->sin_angle + cub->player->plane->y * camera_x;
        // On calcule et dessine la tranche de mur interceptée par ce rayon.
        draw_column(cub, column, ray_dir_x, ray_dir_y);
        column++;
    }
}

int draw_loop(t_cub *cubing)
{
   // printf("DEBUG draw_loop: position x=%.0f, y=%.0f\n", cubing->player->position->x, cubing->player->position->y);  // Print pour voir la position
	move_player(cubing);
	close_image(cubing);
    render_scene(cubing); 
    mlx_put_image_to_window(cubing->mlx, cubing->window, cubing->image, 0, 0);
    
    return (0);
}


/**
 * Lance le jeu
 * param cubing Structure principale contenant toutes les données
 * return 0 en cas de succès
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
    mlx_hook(cubing->window, 17, 1L<<17, close_window, cubing); // Gestion de l'événement de fermeture
    mlx_loop_hook(cubing->mlx, draw_loop, cubing);
    mlx_loop(cubing->mlx);
    return (0);
}