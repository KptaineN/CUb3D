/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_spawn.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 21:33:43 by adi-marc          #+#    #+#             */
/*   Updated: 2025/09/26 11:03:06 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"
// Make sure t_cub is defined in cub3d.h

static int	is_allowed_map_char(char c)
{
	return (c == '0' || c == '1' || c == ' '
		|| c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

static int	get_map_height(char **padded)
{
	int	h;

	h = 0;
	while (padded && padded[h])
		h++;
	return (h);
}

static int	get_map_width(char **padded)
{
	int	w;

	w = 0;
	if (!padded || !padded[0])
		return (0);
	while (padded[0][w])
		w++;
	return (w);
}

static int	is_spawn_char(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

static int	process_char_and_count_spawns(char **padded, int height, int width)
{
	int	i;
	int	j;
	int	spawns;
	
	spawns = 0;
	i = 0;
	while (i < height)
	{
		j = 0;
		while (j < width)
		{
			if (!is_allowed_map_char(padded[i][j]))
				return (-1);
			if (is_spawn_char(padded[i][j]))
			{
				spawns++;
				padded[i][j] = '0';
			}
			j++;
		}
		i++;
	}
	return (spawns);
}

int	check_spawn(char **padded, t_cub *cub)
{
	int height;
	int width;
	int spawn_count;

	if (!padded)
		return (1);
	height = get_map_height(padded);
	if (height == 0)
		return (1);
	width = get_map_width(padded);
	if (width == 0)
		return (1);
	cub->map_height = height;
	cub->map_width = width;
	spawn_count = process_char_and_count_spawns(padded, height, width);
	if (spawn_count == -1)
		return (1);
	return (spawn_count != 1);
}