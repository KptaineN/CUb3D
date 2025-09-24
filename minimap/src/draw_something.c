#include "../includes/cub3d.h"

#define YELLOW 0xFFFF00 // RGB en hexadécimal (rouge+vert)

void    put_pixel(int x, int y, int color, t_cub *cub)
{
    int32_t i;

    if (!cub || !cub->data)
        return ;
    if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
        return ;
    i = (int32_t)y * cub->size_line + (int32_t)x * (cub->bpp / 8);
    cub->data[i] = color & 0xFF;
    cub->data[i + 1] = (color >> 8) & 0xFF;
    cub->data[i + 2] = (color >> 16) & 0xFF;
}

void draw_y_triangle(int x, int y, int size, int color, t_cub *cub)
{
    int row, start_x, end_x;
    t_cord pixel;

    // Pour chaque ligne du triangle (de haut en bas)
    row = 0;
    while (row < size)
    {
        // Calcul du décalage horizontal (triangle centré)
        start_x = x + (size / 2) - (row / 2);
        end_x = x + (size / 2) + (row / 2);

        // Tracer une ligne horizontale remplie
        while (start_x <= end_x)
        {
            pixel.x = start_x;
            pixel.y = y + row;
            put_pixel(pixel.x, pixel.y, color, cub);
            start_x++;
        }
        row++;
    }
}


void    close_image(t_cub *cub)
{
    size_t  buffer_size;

    if (!cub || !cub->data)
        return ;
    buffer_size = (size_t)cub->size_line * HEIGHT;
    /* Ancienne approche: double boucle qui appelait put_pixel pour effacer
     * chaque pixel un par un (très coûteux). */
    /*
    y = 0;
    while (y < HEIGHT)
    {
        x = 0;
        while (x < WIDTH)
        {
            put_pixel(x, y, 0, cub);
            x++;
        }
        y++;
    }
    */
    ft_bzero(cub->data, buffer_size);
}

void draw_square(int x, int y, int size, int color, t_cub *cub)
{
    int i;
    t_cord pixel;

    // Ligne horizontale en haut
    i = 0;
    while (i < size)
    {
        pixel.x = x + i;
        pixel.y = y;
        put_pixel(pixel.x, pixel.y, color, cub);
        i++;
    }
    // Ligne verticale à gauche
    i = 0;
    while (i < size)
    {
        pixel.x = x;
        pixel.y = y + i;
        put_pixel(pixel.x, pixel.y, color, cub);
        i++;
    }
    // Ligne horizontale en bas
    i = 0;
    while (i < size)
    {
        pixel.x = x + i;
        pixel.y = y + size - 1;
        put_pixel(pixel.x, pixel.y, color, cub);
        i++;
    }
    // Ligne verticale à droite
    i = 0;
    while (i < size)
    {
        pixel.x = x + size - 1;
        pixel.y = y + i;
        put_pixel(pixel.x, pixel.y, color, cub);
        i++;
    }
}

void draw_map(t_cub *cub)
{
    char **map;
    map = cub->map;
    int color = 0x0000FF; //texture mur minimap
    int x = 0;
    int y = 0;
    while (map[y])
    {
        x = 0;
        while(map[y][x])
        {
            if (map[y][x] == '1')
                draw_square(x * BLOCK, y * BLOCK, BLOCK, color, cub); 
            x++;
        }
        y++;
    }
}

static void draw_minimap_cell(int32_t sx, int32_t sy, int32_t size, int color,
        t_cub *cub)
{
    int32_t x;
    int32_t y;

    y = 0;
    while (y < size)
    {
        x = 0;
        while (x < size)
        {
            put_pixel((int)(sx + x), (int)(sy + y), color, cub);
            x++;
        }
        y++;
    }
}

static void draw_minimap_player(t_cub *cub, int32_t origin_x, int32_t origin_y)
{
    int32_t px;
    int32_t py;
    int32_t half;
    int32_t size;

    size = cub->minimap_scale / 2 + 2;
    half = size / 2;
    px = origin_x + (int32_t)cub->minimap_scale * 5 + cub->minimap_scale / 2;
    py = origin_y + (int32_t)cub->minimap_scale * 5 + cub->minimap_scale / 2;
    draw_minimap_cell(px - half, py - half, size, 0xFFFF00, cub);
}

void    draw_minimap(t_cub *cub)
{
    int32_t cell_x;
    int32_t cell_y;
    int32_t start_x;
    int32_t start_y;
    int32_t offset_x;
    int32_t offset_y;
    int32_t screen_x;
    int32_t screen_y;
    int32_t view_size;
    int32_t margin;

    if (!cub || !cub->minimap_visible || !cub->player)
        return ;
    view_size = 11;
    margin = 16;
    cell_x = (int32_t)(cub->player->position->x / BLOCK);
    cell_y = (int32_t)(cub->player->position->y / BLOCK);
    start_x = cell_x - view_size / 2;
    start_y = cell_y - view_size / 2;
    offset_y = 0;
    while (offset_y < view_size)
    {
        offset_x = 0;
        while (offset_x < view_size)
        {
            screen_x = margin + offset_x * cub->minimap_scale;
            screen_y = margin + offset_y * cub->minimap_scale;
            if (start_y + offset_y >= 0 && start_y + offset_y < cub->map_height
                && start_x + offset_x >= 0
                && start_x + offset_x < cub->map_width)
            {
                if ((int32_t)ft_strlen(cub->map[start_y + offset_y])
                    > start_x + offset_x
                    && cub->map[start_y + offset_y][start_x + offset_x] == '1')
                    draw_minimap_cell(screen_x, screen_y, cub->minimap_scale,
                        0x2A4B8D, cub);
            }
            offset_x++;
        }
        offset_y++;
    }
    draw_minimap_player(cub, margin, margin);
}

void    toggle_minimap(t_cub *cub)
{
    if (!cub)
        return ;
    cub->minimap_visible = !cub->minimap_visible;
}
