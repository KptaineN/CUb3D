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
    cub->mlx_data = NULL;
    cub->mlx_bpp = 0;
    cub->mlx_line_size = 0;
    cub->mlx_endian = 0;
    cub->player = NULL;
}