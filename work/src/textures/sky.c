#include "../../includes/cub3d.h"

static inline double normalize_column(int col, int w)
{
    return ( (col - ((w - 1) / 2.0)) / ((w - 1) / 2.0) ); // [-1, +1]
}

static inline double wrap01(double a) { return a - floor(a); }

void draw_skybox(t_cub *cub)
{
    // direction et plan (comme dans ray_rendu)
    double dx = cub->player->cos_angle;
    double dy = cub->player->sin_angle;
    double px = cub->player->plane->x;
    double py = cub->player->plane->y;

    for (int col = 0; col < WIDTH; ++col)
    {
        double camx = normalize_column(col, WIDTH);
        double rx = dx + px * camx;
        double ry = dy + py * camx;

        double ang = atan2(ry, rx);                     // [-pi, +pi]
        double u   = wrap01(ang / (2.0 * M_PI));        // [0,1)
        int tex_x  = (int)(u * (double)cub->sky.w);
        if (tex_x < 0) tex_x = 0;
        if (tex_x >= cub->sky.w) tex_x = cub->sky.w - 1;

        int bytes = cub->sky.bpp / 8;
        for (int y = 0; y < HEIGHT/2; ++y)
        {
            double v = (double)y / (double)(HEIGHT/2 - 1); // 0=zenith, 1=horizon
            int tex_y = (int)(v * (double)cub->sky.h);
            if (tex_y >= cub->sky.h) tex_y = cub->sky.h - 1;

            int off = tex_y * cub->sky.sl + tex_x * bytes;
            int color = *(int *)(cub->sky.data + off);
            put_pixel(col, y, color, cub);
        }
    }
}

/* Removed duplicate wrap01 and draw_skybox definitions */