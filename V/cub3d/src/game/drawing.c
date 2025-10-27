#include "../../includes/cub3d.h"


void    put_pixel(int x, int y, int color, t_cub *cub)
{
    int32_t i; //index du premier octet du pixel (x,y)

    if (!cub || !cub->mlx_data)
        return ;
    if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
        return ;
    i = (int32_t)y * cub->mlx_line_size + (int32_t)x * (cub->mlx_bpp / 8);
    cub->mlx_data[i] = color & 0xFF;                    //BLEU
    cub->mlx_data[i + 1] = (color >> 8) & 0xFF;         //VERT
    cub->mlx_data[i + 2] = (color >> 16) & 0xFF;        //ROUGE
}

void draw_column(t_cub *cub, int column, double ray_dir_x, double ray_dir_y)
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

   // line_height = (int)((double)HEIGHT / (distance / BLOCK));
    line_height = (int)((double)HEIGHT / distance); 
    draw_start = -line_height / 2 + HEIGHT / 2;
    if (draw_start < 0)
        draw_start = 0;
    draw_end = line_height / 2 + HEIGHT / 2;
    if (draw_end >= HEIGHT)
        draw_end = HEIGHT - 1;

    color = (side == 1) ? 0x777777 : 0xCCCCCC;

    y = draw_start;
    while (y <= draw_end)
        put_pixel(column, y++, color, cub);
}