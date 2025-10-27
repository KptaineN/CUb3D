/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_spawn.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 21:33:43 by adi-marc          #+#    #+#             */
/*   Updated: 2025/10/23 21:00:42 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	is_allowed_map_char(char c)
{
    return (c == '0' || c == '1' || c == ' ' || c == '6'
        || c == 'N' || c == 'S' || c == 'E' || c == 'W'
        || c == 'X' || c == 'P' || c == 'D' || c == 'B'
        || c == 'M' || c == 'I' || c == 'K' || c == 'J' || c == 'T'
        || c == 'H'
        || c == 'm' || c == 'h' || c == 'k' || c == 'j' || c == 't' || c == 'i');
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
	int	found_first_spawn;
	char	c;
	
	spawns = 0;
	found_first_spawn = 0;
	i = 0;
	while (i < height)
	{
		j = 0;
		while (j < width)
		{
            c = padded[i][j];
            if (!is_allowed_map_char(c))
                return (-1);
            if (is_spawn_char(c))
            {
                if (found_first_spawn)
                {
                    /* This is a duplicate spawn - replace with '0' */
                    padded[i][j] = '0';
                }
                else
                {
                    /* This is the first spawn - keep it for player_init to read */
                    found_first_spawn = 1;
					spawns++;
				}
			}
            else if (c == 'H' || c == 'h' || c == 'M' || c == 'm'
                || c == 'K' || c == 'k' || c == 'J' || c == 'j'
                || c == 'T' || c == 't' || c == 'I' || c == 'i' || c == '6')
                padded[i][j] = '0';
            j++;
        }
        i++;
    }
	return (spawns);
}

int	check_spawn(char **padded)
{
	int	height;
	int	width;
	int	spawn_count;

	height = get_map_height(padded);
	if (!padded || height == 0)
		return (1);
	width = get_map_width(padded);
	if (width == 0)
		return (1);
	spawn_count = process_char_and_count_spawns(padded, height, width);
	if (spawn_count == -1)
		return (1);
	return (spawn_count != 1);
}

/* Extract alien spawn position from map (marked with 'X')
   Returns 1 if 'X' found and position stored, 0 otherwise */
int	find_alien_spawn(char **map, int *spawn_x, int *spawn_y)
{
	int	i;
	int	j;

	if (!map || !spawn_x || !spawn_y)
		return (0);
	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == 'X')
			{
				*spawn_x = j;
				*spawn_y = i;
				map[i][j] = '0'; /* replace X with empty space */
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}
