
/*
Conventions Y : on est en MLX (Y augmente vers le bas) → +Y = SUD.
Wrap angle : wrap01(ang / (2π)) garantit que la skybox boucle bien à 360°.
BLOCK : on travaille en cases pour dir/plane/ray ; 
pense à diviser la position joueur par BLOCK là où c’est nécessaire.
Perf : si put_pixel écrit directement dans une image MLX (pas la fenêtre),
ça ira bien. Évite d’appeler mlx_pixel_put en boucle (lent).
*/
#include "../../includes/cub3d.h"

void draw_floor(t_cub *cub)
{
    // 1) vecteurs
    double dx = cub->player->cos_angle;
    double dy = cub->player->sin_angle;
    double px = cub->player->plane->x;
    double py = cub->player->plane->y;

    // position joueur en unités "cases" (pas pixels)
    double pmap_x = cub->player->position->x / (double)BLOCK;
    double pmap_y = cub->player->position->y / (double)BLOCK;

    // rayons gauche/droite du frustum
    double rLx = dx - px, rLy = dy - py;
    double rRx = dx + px, rRy = dy + py;

    if (!cub->floor.data || cub->floor.w <= 0 || cub->floor.h <= 0) return;
    int bytes = cub->floor.bpp / 8;
    /* hauteur caméra utilisée pour le calcul du floor casting (runtime) */
    double posZ = cub->player->eye_height_current; // en pixels écran

    for (int y = HEIGHT/2; y < HEIGHT; ++y)
    {
        double p = (double)y - (double)HEIGHT / 2.0;
        if (p == 0.0) continue; // évite /0 à l'horizon exact
        double rowDist = posZ / p;

        // point sol au début de la ligne + pas par pixel
        double stepX = rowDist * (rRx - rLx) / (double)WIDTH;
        double stepY = rowDist * (rRy - rLy) / (double)WIDTH;
        double floorX = pmap_x + rowDist * rLx;
        double floorY = pmap_y + rowDist * rLy;

        for (int x = 0; x < WIDTH; ++x)
        {
            double fracX = floorX - floor(floorX);
            double fracY = floorY - floor(floorY);

            int tex_x = (int)(fracX * (double)cub->floor.w);
            int tex_y = (int)(fracY * (double)cub->floor.h);
            if (tex_x < 0) tex_x = 0;
            if (tex_x >= cub->floor.w) tex_x = cub->floor.w - 1;
            if (tex_y < 0) tex_y = 0;
            if (tex_y >= cub->floor.h) tex_y = cub->floor.h - 1;

            int off = tex_y * cub->floor.sl + tex_x * bytes;
            if ((size_t)off + sizeof(int) > (size_t)(cub->floor.h * cub->floor.sl)) continue;
            int color = *(int *)(cub->floor.data + off);

            // léger assombrissement avec la distance (optionnel, look ++)
            double shade = 1.0 / (1.0 + 0.02 * rowDist * rowDist);
            unsigned char *c = (unsigned char *)&color;
            c[0] = (unsigned char)(c[0] * shade);
            c[1] = (unsigned char)(c[1] * shade);
            c[2] = (unsigned char)(c[2] * shade);

            put_pixel(x, y, color, cub);

            floorX += stepX;
            floorY += stepY;
        }
    }
}
