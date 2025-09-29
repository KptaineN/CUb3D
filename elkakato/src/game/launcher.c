#include "../../includes/cub3d.h"

// Forward declaration for window_init if not included in cub3d.h
int window_init(t_cub *cub);

// Forward declaration for move_player if not included in cub3d.h
void move_player(t_cub *cub);


int draw_loop(t_cub *cub)
{
    move_player(cub);
    //close_image(cub);
    /* Ancien rendu (draw_map + rayons incrémentés pixel par pixel) conservé en
     * commentaire dans l'historique Git pour se souvenir de l'évolution. */
    ray_rendu(cub);
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
    mlx_hook(cub->mlx_window, 2, 1L<<0, key_press, cub);
    mlx_hook(cub->mlx_window, 3, 1L<<1, key_drop, cub);
    mlx_hook(cub->mlx_window, 17, 0, on_destroy, cub);
    mlx_loop_hook(cub->mlx, draw_loop, cub);
    mlx_loop(cub->mlx);
    cleanup_resources(cub);
    return (0);
}