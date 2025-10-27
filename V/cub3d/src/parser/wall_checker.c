/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_checker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 22:28:57 by adi-marc          #+#    #+#             */
/*   Updated: 2025/09/19 12:35:19 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

#include <stdio.h>

static int	check_zero(char **buffer_map, int i, int j, int rows)
{
	if (i == 0 || i == rows - 1)
		return (1);
	if (j == 0 || buffer_map[i][j + 1] == '\0')
		return (1);
	if (buffer_map[i - 1][j] == ' ' ||
		buffer_map[i + 1][j] == ' ' ||
		buffer_map[i][j - 1] == ' ' ||
		buffer_map[i][j + 1] == ' ')
		return (1);
	return (0);
}

int	wall_check(char **buffer_map)
{
	int	i;
	int	j;
	int	rows;

	rows = 0;
	while (buffer_map[rows])
		rows++;
	i = 0;
	while (i < rows)
	{
		j = 0;
		while (buffer_map[i][j])
		{
			if (buffer_map[i][j] == '0' && check_zero(buffer_map, i, j, rows))
				return (1);
			j++;
		}
		i++;
	}
	printf("La map est bien entourée de murs\n");
	return (0);
}

