#include "../../includes/cub3d.h"

static void door_open_all(t_cub *cub)
{
    int i;

    if (!cub || cub->door_count <= 0 || !cub->doors || !cub->map)
        return;
    for (i = 0; i < cub->door_count; ++i)
    {
        t_door_segment *door = &cub->doors[i];
        if (!door->open)
        {
            door->open = 1;
            if (door->map_y >= 0 && cub->map[door->map_y])
            {
                size_t row_len = ft_strlen(cub->map[door->map_y]);
                if (door->map_x >= 0 && (size_t)door->map_x < row_len
                    && cub->map[door->map_y][door->map_x] == 'D')
                {
                    cub->map[door->map_y][door->map_x] = '0';
                }
            }
        }
    }
}

static void door_activate_panel(t_cub *cub, int map_x, int map_y)
{
    int i;

    if (!cub || cub->panel_count <= 0 || !cub->panels)
        return;
    for (i = 0; i < cub->panel_count; ++i)
    {
        t_panel_trigger *panel = &cub->panels[i];
        if (panel->map_x == map_x && panel->map_y == map_y)
        {
            if (!panel->activated)
            {
                panel->activated = 1;
                cub->panels_activated++;
                if (cub->panel_count > 0
                    && cub->panels_activated >= cub->panel_count)
                    door_open_all(cub);
            }
            return;
        }
    }
}

static int map_tile_at(t_cub *cub, int map_x, int map_y, char *tile_out)
{
    size_t row_len;

    if (tile_out)
        *tile_out = '1';
    if (!cub || !cub->map || map_y < 0 || map_x < 0)
        return (-1);
    if (!cub->map[map_y])
        return (-1);
    row_len = ft_strlen(cub->map[map_y]);
    if ((size_t)map_x >= row_len)
        return (-1);
    if (tile_out)
        *tile_out = cub->map[map_y][map_x];
    return (0);
}

static int tile_is_blocking(char tile)
{
    return (tile == '1' || tile == 'P' || tile == 'D' || tile == ' '
        || tile == 'B');
}

int door_system_init(t_cub *cub)
{
    int y;
    int x;
    int panel_idx;
    int door_idx;
    int panel_total;
    int door_total;

    if (!cub || !cub->map)
        return (0);
    panel_total = 0;
    door_total = 0;
    y = 0;
    while (cub->map[y])
    {
        x = 0;
        while (cub->map[y][x])
        {
            if (cub->map[y][x] == 'P')
                panel_total++;
            else if (cub->map[y][x] == 'D')
                door_total++;
            x++;
        }
        y++;
    }
    cub->panels = NULL;
    cub->doors = NULL;
    cub->panel_count = 0;
    cub->panels_activated = 0;
    cub->door_count = 0;
    if (panel_total > 0)
    {
        cub->panels = (t_panel_trigger *)ft_calloc(panel_total,
                sizeof(t_panel_trigger));
        if (!cub->panels)
            return (ft_putendl_fd("Error: failed to allocate panels", 2), 1);
        cub->panel_count = panel_total;
    }
    if (door_total > 0)
    {
        cub->doors = (t_door_segment *)ft_calloc(door_total,
                sizeof(t_door_segment));
        if (!cub->doors)
        {
            free(cub->panels);
            cub->panels = NULL;
            cub->panel_count = 0;
            return (ft_putendl_fd("Error: failed to allocate doors", 2), 1);
        }
        cub->door_count = door_total;
    }
    panel_idx = 0;
    door_idx = 0;
    y = 0;
    while (cub->map[y])
    {
        x = 0;
        while (cub->map[y][x])
        {
            if (cub->map[y][x] == 'P' && cub->panels)
            {
                cub->panels[panel_idx].map_x = x;
                cub->panels[panel_idx].map_y = y;
                cub->panels[panel_idx].activated = 0;
                panel_idx++;
            }
            else if (cub->map[y][x] == 'D' && cub->doors)
            {
                cub->doors[door_idx].map_x = x;
                cub->doors[door_idx].map_y = y;
                cub->doors[door_idx].open = 0;
                door_idx++;
            }
            x++;
        }
        y++;
    }
    return (0);
}

void door_system_destroy(t_cub *cub)
{
    if (!cub)
        return;
    free(cub->panels);
    cub->panels = NULL;
    cub->panel_count = 0;
    cub->panels_activated = 0;
    free(cub->doors);
    cub->doors = NULL;
    cub->door_count = 0;
}

void door_try_activate(t_cub *cub)
{
    double px;
    double py;
    double dir_x;
    double dir_y;
    double dist;
    double max_dist;
    double step;

    if (!cub || !cub->player || cub->panel_count <= 0)
        return;
    px = cub->player->position->x;
    py = cub->player->position->y;
    dir_x = cub->player->cos_angle;
    dir_y = cub->player->sin_angle;
    max_dist = (double)BLOCK * 1.5;
    step = (double)BLOCK / 10.0;
    dist = cub->player->radius;
    while (dist <= max_dist)
    {
        double sample_x = px + dir_x * dist;
        double sample_y = py + dir_y * dist;
        int map_x = (int)floor(sample_x / (double)BLOCK);
        int map_y = (int)floor(sample_y / (double)BLOCK);
        char tile;

        if (map_tile_at(cub, map_x, map_y, &tile) != 0)
            return;
        if (tile_is_blocking(tile))
        {
            if (tile == 'P')
            {
                if (cub->player && cub->player->current_weapon == WEAPON_HAMMER
                    && cub->player->has_hammer)
                    hammer_spawn_panel_spark(cub, sample_x, sample_y);
                door_activate_panel(cub, map_x, map_y);
            }
            return;
        }
        dist += step;
    }
}
