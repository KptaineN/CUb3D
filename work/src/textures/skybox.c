/*
** draw_skybox : Rendu d'une skybox panoramique (texture cylindrique)
** 
** Principe : 
** - L'angle de vue du joueur détermine quelle partie de la texture voir
** - La texture wrap en X (boucle à 360°)
** - Remplit la moitié haute de l'écran (y = 0 à HEIGHT/2)
*/
#include "../../includes/cub3d.h"

static double wrap01(double x)
{
    return x - floor(x);
}

void draw_skybox(t_cub *cub)
{
    if (!cub->sky.data || !cub->sky.img)
        return; // pas de texture de ciel chargée
        
    // angle de base du joueur (direction centrale)
    double base_angle = atan2(cub->player->sin_angle, cub->player->cos_angle);
    
    // largeur d'angle couverte par l'écran (FOV)
    double fov_rad = cub->player->fov * M_PI / 180.0;
    double half_fov = fov_rad * 0.5;
    
    int bytes = cub->sky.bpp / 8;
    
    for (int x = 0; x < WIDTH; ++x)
    {
        // angle de ce pixel par rapport au centre
        double pixel_ratio = (double)(x - WIDTH/2) / (double)(WIDTH/2);
        double pixel_angle = base_angle + pixel_ratio * half_fov;
        
        // normalise l'angle dans [0, 2π] puis [0, 1]
        double angle_norm = wrap01(pixel_angle / (2.0 * M_PI));
        
        // coordonnée X dans la texture skybox
        int tex_x = (int)(angle_norm * (double)cub->sky.w);
        if (tex_x < 0) tex_x = 0;
        if (tex_x >= cub->sky.w) tex_x = cub->sky.w - 1;
        
        // dessine la colonne verticale du ciel
        for (int y = 0; y < HEIGHT/2; ++y)
        {
            // coordonnée Y dans la texture (de haut en bas)
            int tex_y = (y * cub->sky.h) / (HEIGHT/2);
            if (tex_y < 0) tex_y = 0;
            if (tex_y >= cub->sky.h) tex_y = cub->sky.h - 1;
            
            int offset = tex_y * cub->sky.sl + tex_x * bytes;
            int color = *(int *)(cub->sky.data + offset);
            
            put_pixel(x, y, color, cub);
        }
    }
}
