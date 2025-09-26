#include "../includes/cub3d.h"


void	free_player(t_player *player)
{
	if (!player)
		return ;
	
	if (player->position)
		free(player->position);
	if (player->direction)
		free(player->direction);
	if (player->plane)
		free(player->plane);
	free(player);
}

void free_cub(t_cub *cub)
{
    if (!cub)
        return;
    if (cub->no_path)
        free(cub->no_path);
    if (cub->so_path)
        free(cub->so_path);
    if (cub->we_path)
        free(cub->we_path);
    if (cub->ea_path)
        free(cub->ea_path);
    if (cub->f_color)
        free(cub->f_color);
    if (cub->c_color)
        free(cub->c_color);
    if (cub->file)
        free(cub->file);
    if (cub->map_start)
        free(cub->map_start);
    if (cub->map)
    {
        int i = 0;
        while (cub->map[i])
        {
            free(cub->map[i]);
            i++;
        }
        free(cub->map);
    }
    if (cub->player)
    player_cleanup(cub);
    
    free(cub);
}

/**
 * Nettoie toutes les ressources allouées par MiniLibX
 * param cub Structure principale contenant toutes les données
 */
void	cleanup_resources(t_cub *cub)
{
	if (!cub)
		return ;
	
	if (cub->image && cub->mlx)
		mlx_destroy_image(cub->mlx, cub->image);
	
	if (cub->window && cub->mlx)
		mlx_destroy_window(cub->mlx, cub->window);
	
	if (cub->mlx)
	{
		mlx_destroy_display(cub->mlx);
		free(cub->mlx);
	}
}

/**
 * Fonction appelée lors de la fermeture de la fenêtre (clic sur X)
 * param cub Structure principale contenant toutes les données
 * return Toujours 0
 */
int	close_window(t_cub *cub)
{
	cleanup_resources(cub);
	exit(0);
	return (0);
}

/**
 * Gère les événements de touches du clavier
 * param keycode Code de la touche pressée
 * param cub Structure principale contenant toutes les données
 * return Toujours 0
 */
int	handle_keypress(int keycode, t_cub *cub)
{
	if (keycode == ESC_KEY)
		close_window(cub);
	return (0);
}