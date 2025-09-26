#include "../includes/cub3d.h"

#define YELLOW 0xFFFF00 // RGB en hexadécimal (rouge+vert)

void    put_pixel(int x, int y, int color, t_cub *cub)
{
    int i;
    if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
        return ;
    i = y * cub->size_line + x * (cub->bpp / 8);
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

float distance(float x, float y)
{
    return (sqrt(x * x + y * y));
}

void    draw_line(t_player *player, t_cub *game, float start_x)
{
    float cos_angle = cos(start_x);
    float sin_angle = sin(start_x);
    float ray_x = player->position->x;
    float ray_y = player->position->y;
    
    while (!touch(ray_x, ray_y, game))
    {
        put_pixel(ray_x, ray_y, 0xF00000, game);
        ray_x += cos_angle;
        ray_y += sin_angle;
    }
    float dist = distance(ray_x - player->position->x, ray_y - player->position->y );
    float height = (BLOCK / dist) * (WIDTH / 2);
    int start_y = (HEIGHT - height) / 2;
    int end = start_y + height;
    while (start_y < end)
    {
        put_pixel(ray_x, start_y, 255, game);
        start_y++;
    }
}

void    close_image(t_cub *cub)
{
    int x;
    int y;
    
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

void draw_column(t_cub *cub, int column, double ray_dir_x,
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