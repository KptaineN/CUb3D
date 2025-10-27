#include "../../includes/cub3d.h"

void render_frame(t_cub *cub)
{
    // 1) ciel texturé (moitié haute)
    draw_skybox(cub);

    // 2) sol texturé (moitié basse)
    draw_floor(cub);

    // 3) murs au-dessus (tes colonnes raycast habituelles)
    t_cord dir = { cub->player->cos_angle, cub->player->sin_angle };
    t_cord plane = { cub->player->plane->x, cub->player->plane->y };

    for (int column = 0; column < WIDTH; ++column)
    {
        t_cord ray = make_ray(column, WIDTH, dir, plane);
        draw_column(cub, column, ray.x, ray.y); // ta version texturée de mur
    }

    /* draw pickups and barrels before alien for proper layering */
    pickup_render(cub);
    barrel_render(cub);
    weapon_draw_hammer_sparks(cub);

    /* draw alien sprite on top */
    draw_alien_sprite(cub);

    /* minimap in top-left */
    draw_minimap(cub);

    /* Draw bullet impacts on walls */
    weapon_draw_wall_impacts(cub);
    
    /* Draw weapon sprite BEFORE HUD so HUD is on top */
    weapon_draw(cub);
    
    /* Draw crosshair (style Predator) for MP */
    weapon_draw_crosshair(cub);

    /* Draw ammo counter */
    hud_draw_ammo_counter(cub);
    hud_draw_weapon_icons(cub);

    /* HUD overlays (health bar, damage flash, game over) - drawn on top of weapons */
    draw_health_bar(cub);
    draw_damage_flash(cub);
    draw_game_over(cub);

    // 4) si tu as un backbuffer, pousse l'image ici
    mlx_put_image_to_window(cub->mlx, cub->mlx_window, cub->mlx_image, 0, 0);
}
