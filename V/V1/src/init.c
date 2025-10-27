
#include "../includes/cub3d.h"

//void    ft_game_init(int argc, char argv, t_list cubinit)
//{
    /* initialisation du minibitex et ses foncfions
    init des variables du prog et donc du jeux
    init des positions de la map?
    */

/**
 * Initialise MiniLibX et crée la fenêtre
 */
void	init_mlx(t_cub *cub)
{
	cub->mlx = mlx_init();
	if (!cub->mlx)
	{
		printf("Erreur: mlx_init échoué\n");
		return ;
	}
	cub->window = mlx_new_window(cub->mlx, WIDTH, HEIGHT, "Cub3D");
	if (!cub->window)
	{
		printf("Erreur: mlx_new_window échoué\n");
		return ;
	}
}

void	init_player(t_player *player)
{
    if (!player)
		return ;
    player->angle = 0.0;
    player->cos_angle = 1.0;
    player->sin_angle = 0.0;
    player->fov = 66.0;
	player->move_frame = 1;
    player->rotation_frame = 0.05;
    player->radius = 12;
    player->position->x = WIDTH / 2; //a modifier plus tard
    player->position->y = HEIGHT / 2;
    player->direction->x = 0;
    player->direction->y = 0;
    player->plane->x = 0.0;
    player->plane->y = 0.0;
	player->key_up = false;
    player->key_down = false;
    player->key_left = false;
    player->key_right = false;
    player->left_rotate = false;
    player->right_rotate = false; 
}

/**
 * Initialise les composants du jeu
 */
int16_t	initialisation(t_cub *cubing)
{
    // Allouer et initialiser le joueur
    if (!cubing->player)
        update_player_trig(cubing->player);
    init_mlx(cubing);
    cubing->image = mlx_new_image(cubing->mlx, WIDTH, HEIGHT);
    cubing->data = mlx_get_data_addr(cubing->image, &cubing->bpp, &cubing->size_line,
        &cubing->endian);
    mlx_put_image_to_window(cubing->mlx, cubing->window, cubing->image, 0, 0);
	return (0);
}

void    init_values(t_cub *cub)
{
    cub->no_path = NULL;
    cub->so_path = NULL;
    cub->we_path = NULL;
    cub->ea_path = NULL;
    cub->f_color = NULL;
    cub->c_color = NULL;
    cub->file = NULL;
    cub->map_start = NULL;
    cub->map = NULL;
    cub->player = NULL;
    cub->map_width = 0;
    cub->map_height = 0;
    // Initialiser les nouveaux champs MLX
    cub->mlx = NULL;
    cub->window = NULL;
    cub->image = NULL;
    cub->data = NULL;
    cub->bpp = 0;
    cub->size_line = 0;
    cub->endian = 0;
}