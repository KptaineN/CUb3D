#include "../includes/cub3d.h"

bool is_wall(t_cub *cub, int32_t x, int32_t y)
{
    if (x < 0 || y < 0 || x >= cub->map_width || y >= cub->map_height)
        return (true);
    if ((int32_t)ft_strlen(cub->map[y]) <= x)
        return (true);
    return (cub->map[y][x] == '1');
}

double  perform_dda(t_cub *cub, double ray_dir_x, double ray_dir_y,
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