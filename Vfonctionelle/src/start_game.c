/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_game.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 21:10:00 by nkief             #+#    #+#             */
/*   Updated: 2025/09/24 13:44:56 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static bool is_wall(t_cub *cub, int32_t x, int32_t y)
{
    if (x < 0 || y < 0 || x >= cub->map_width || y >= cub->map_height)
        return (true);
    if ((int32_t)ft_strlen(cub->map[y]) <= x)
        return (true);
    return (cub->map[y][x] == '1');
}

void    init_mlx(t_cub *cub)
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

int16_t initialisation(t_cub *cub)
{
    if (cub->player)
        update_player_trig(cub->player);
    init_mlx(cub);
    cub->image = mlx_new_image(cub->mlx, WIDTH, HEIGHT);
    cub->data = mlx_get_data_addr(cub->image, &cub->bpp, &cub->size_line,
            &cub->endian);
    mlx_put_image_to_window(cub->mlx, cub->window, cub->image, 0, 0);
    return (0);
}

bool    touch(double px, double py, t_cub *game)
{
    int32_t cell_x;
    int32_t cell_y;
    size_t  line_len;

    if (!game || !game->map)
        return (true);
    if (px < 0 || py < 0)
        return (true);
    cell_x = (int32_t)(px / BLOCK);
    cell_y = (int32_t)(py / BLOCK);
    if (cell_x < 0 || cell_y < 0)
        return (true);
    if (cell_x >= game->map_width || cell_y >= game->map_height)
        return (true);
    line_len = ft_strlen(game->map[cell_y]);
    if ((size_t)cell_x >= line_len)
        return (true);
    /* Ancien code: strnlen() et compte de lignes à chaque appel (beaucoup
     * plus lourd) est conservé dans l'historique Git pour référence. */
    return (game->map[cell_y][cell_x] == '1');
}

static double perform_dda(t_cub *cub, double ray_dir_x, double ray_dir_y,
        int *side)
{
    double  pos_x;
    double  pos_y;
    int32_t map_x;
    int32_t map_y;
    double  delta_x;
    double  delta_y;
    double  side_x;
    double  side_y;
    int32_t step_x;
    int32_t step_y;

    pos_x = cub->player->position->x / BLOCK;
    pos_y = cub->player->position->y / BLOCK;
    map_x = (int32_t)pos_x;
    map_y = (int32_t)pos_y;
    delta_x = (ray_dir_x == 0) ? 1e30 : fabs(1.0 / ray_dir_x);
    delta_y = (ray_dir_y == 0) ? 1e30 : fabs(1.0 / ray_dir_y);
    if (ray_dir_x < 0)
    {
        step_x = -1;
        side_x = (pos_x - map_x) * delta_x;
    }
    else
    {
        step_x = 1;
        side_x = (map_x + 1.0 - pos_x) * delta_x;
    }
    if (ray_dir_y < 0)
    {
        step_y = -1;
        side_y = (pos_y - map_y) * delta_y;
    }
    else
    {
        step_y = 1;
        side_y = (map_y + 1.0 - pos_y) * delta_y;
    }
    while (1)
    {
        if (side_x < side_y)
        {
            side_x += delta_x;
            map_x += step_x;
            *side = 0;
        }
        else
        {
            side_y += delta_y;
            map_y += step_y;
            *side = 1;
        }
        if (is_wall(cub, map_x, map_y))
            break ;
    }
    if (*side == 0)
        return ((side_x - delta_x) * BLOCK);
    return ((side_y - delta_y) * BLOCK);
}

static void draw_column(t_cub *cub, int column, double ray_dir_x,
        double ray_dir_y)
{
    double  distance;
    int     line_height;
    int     draw_start;
    int     draw_end;
    int     y;
    int     color;
    int     side;

    distance = perform_dda(cub, ray_dir_x, ray_dir_y, &side);
    if (distance < 1e-6)
        distance = 1e-6;
    line_height = (int)((double)HEIGHT / (distance / BLOCK));
    draw_start = -line_height / 2 + HEIGHT / 2;
    if (draw_start < 0)
        draw_start = 0;
    draw_end = line_height / 2 + HEIGHT / 2;
    if (draw_end >= HEIGHT)
        draw_end = HEIGHT - 1;
    color = 0xCCCCCC;
    if (side == 1)
        color = 0x777777;
    y = draw_start;
    while (y <= draw_end)
    {
        put_pixel(column, y, color, cub);
        y++;
    }
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
        camera_x = 2.0 * column / (double)WIDTH - 1.0;
        ray_dir_x = cub->player->cos_angle + cub->player->plane->x * camera_x;
        ray_dir_y = cub->player->sin_angle + cub->player->plane->y * camera_x;
        draw_column(cub, column, ray_dir_x, ray_dir_y);
        column++;
    }
}

int draw_loop(t_cub *cub)
{
    move_player(cub);
    close_image(cub);
    /* Ancien rendu (draw_map + rayons incrémentés pixel par pixel) conservé en
     * commentaire dans l'historique Git pour se souvenir de l'évolution. */
    render_scene(cub);
    draw_minimap(cub);
    mlx_put_image_to_window(cub->mlx, cub->window, cub->image, 0, 0);
    return (0);
}

int16_t start_game(t_cub *cub)
{
    if (initialisation(cub) != 0)
    {
        printf("Erreur lors de l'initialisation\n");
        return (-1);
    }
    printf("initialisation fini\n");
    mlx_put_image_to_window(cub->mlx, cub->window, cub->image, 0, 0);
    mlx_hook(cub->window, 2, 1L << 0, key_press, cub);
    mlx_hook(cub->window, 3, 1L << 1, key_drop, cub);
    mlx_loop_hook(cub->mlx, draw_loop, cub);
    mlx_loop(cub->mlx);
    return (0);
}
