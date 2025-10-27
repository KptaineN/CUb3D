#include "../includes/cub3d.h"
/*
void    ft_game_init(int argc, char argv, t_list cubinit)
{
    initialisation du minibitex et ses foncfions
    init des variables du prog et donc du jeux
    init des positions de la map?

}*/

void init_values(t_cub *cub)
{
    // Utilisation de ft_memset pour s'assurer que toute la structure est à zéro
    ft_memset(cub, 0, sizeof(t_cub));
    
    // Initialisation explicite des pointeurs à NULL (redondant mais plus clair)
    cub->no_path = NULL;
    cub->so_path = NULL;
    cub->we_path = NULL;
    cub->ea_path = NULL;
    cub->f_color = NULL;
    cub->c_color = NULL;
    cub->file = NULL;
    cub->map_start = NULL;
    cub->map = NULL;
    cub->mlx = NULL;
    cub->mlx_window = NULL;
    cub->mlx_image = NULL;
    cub->player = NULL;
    cub->alien = NULL;
    /* init alien spawn position (will be set when parsing map) */
    cub->alien_spawn_x = -1;
    cub->alien_spawn_y = -1;
    /* player spawn saved flag and default values */
    cub->player_spawn_x = 0.0;
    cub->player_spawn_y = 0.0;
    cub->player_spawn_angle = 0.0;
    cub->player_spawn_saved = 0;
    cub->game_over_alpha = 0.0;
    /* init AI entities - will be spawned properly after map parsing */
    
    // Initialisation sécurisée des données MLX
    init_mlx_data(cub);

    /* ensure zbuffer has sane values to avoid touching uninitialised memory */
    for (int i = 0; i < WIDTH; ++i)
        cub->zbuffer[i] = 1e9;
    
    /* initialize bullet impacts array - déjà fait par memset au début, mais on reset le compteur */
    cub->bullet_impact_count = 0;
    
    /* default show minimap: enabled for debugging/dev by default */
    cub->show_minimap = 1;
    /* minimap follow defaults: start centered on player when set later; smoothing alpha tuned for responsiveness */
    cub->minimap_view_x = 0.0;
    cub->minimap_view_y = 0.0;
    cub->minimap_smooth_alpha = 0.18; /* per-frame lerp factor ~ responsive but smooth */
    cub->minimap_deadzone = 0.18; /* cells */
    /* door/panel system */
    cub->panels = NULL;
    cub->panel_count = 0;
    cub->panels_activated = 0;
    cub->doors = NULL;
    cub->door_count = 0;
    cub->barrels = NULL;
    cub->barrel_count = 0;
    cub->pickups = NULL;
    cub->pickup_count = 0;
}
