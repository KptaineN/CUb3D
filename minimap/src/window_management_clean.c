/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window_management_clean.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 21:10:00 by nkief             #+#    #+#             */
/*   Updated: 2025/09/22 15:45:31 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/**
 * Nettoie toutes les ressources allouées par MiniLibX
 * @param cub Structure principale contenant toutes les données
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
 * @param cub Structure principale contenant toutes les données
 * @return Toujours 0
 */
int	close_window(t_cub *cub)
{
	cleanup_resources(cub);
	exit(0);
	return (0);
}

/**
 * Gère les événements de touches du clavier
 * @param keycode Code de la touche pressée
 * @param cub Structure principale contenant toutes les données
 * @return Toujours 0
 */
int	handle_keypress(int keycode, t_cub *cub)
{
	if (keycode == ESC_KEY)
		close_window(cub);
	return (0);
}