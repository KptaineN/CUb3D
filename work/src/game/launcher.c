#include "../../includes/cub3d.h"

// Forward declaration for window_init if not included in cub3d.h
//int window_init(t_cub *cub);

// Forward declaration for move_player if not included in cub3d.h
//void move_player(t_cub *cub);


int draw_loop(t_cub *cub)
{
    static struct timeval last = {0,0};
    struct timeval now;
    double dt;

    gettimeofday(&now, NULL);
    if (last.tv_sec == 0 && last.tv_usec == 0)
        dt = 1.0 / 60.0; /* fallback */
    else
        dt = (now.tv_sec - last.tv_sec) + (now.tv_usec - last.tv_usec) / 1e6;
    last = now;

    move_player(cub);
    pickup_update(cub);
    /* update HUD reload (textures that are rotated every 25s) */
    hud_update_reload(cub, dt);
    /* update HUD per-frame (fade etc.) */
    hud_update(cub, dt);
    /* update weapon system */
    weapon_update(cub->player, dt);
    weapon_auto_fire(cub);
    barrel_update(cub, dt);
    weapon_update_sparks(cub, dt);
    /* update bullet impacts (fade out over 15s) */
    weapon_update_impacts(cub, dt);
    /* update AI with real dt */
    alien_update(cub, dt);
    clear_image(cub);
    render_frame(cub);
    mlx_put_image_to_window(cub->mlx, cub->mlx_window, cub->mlx_image, 0, 0);
    return (0);
}

int start_game(t_cub *cub)
{
    if (!window_init(cub))
    {
        printf("Erreur lors de l'initialisation de la fenetre\n");
        return (1);
    }
    /* ensure alien AI is initialized after player allocation (player_init is called before start_game) */
    if (!cub->alien && cub->player)
        alien_init(cub);
    mlx_hook(cub->mlx_window, 2, 1L<<0, key_press, cub);
    mlx_hook(cub->mlx_window, 3, 1L<<1, key_drop, cub);
    mlx_hook(cub->mlx_window, 17, 0, on_destroy, cub);
    mlx_loop_hook(cub->mlx, draw_loop, cub);
    mlx_loop(cub->mlx);
    // Le cleanup sera géré par on_destroy ou cleanup_and_exit
    return (0);
}
