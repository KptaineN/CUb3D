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