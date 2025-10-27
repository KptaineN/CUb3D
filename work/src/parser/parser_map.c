/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 16:38:40 by adi-marc          #+#    #+#             */
/*   Updated: 2025/10/09 21:01:14 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* returns 1 if the line is only whitespace or NULL, else 0 */
int	line_is_empty(char *line)
{
	int	i;

	if (!line)
		return (1);
	i = 0;
	while (line[i])
	{
		if (!ft_isspace((unsigned char)line[i]))
			return (0);
		i++;
	}
	return (1);
}

/* duplicate a line; returns NULL on failure */
char	*copy_map(const char *current_line)
{
	char    *ret;
	size_t  len;

	if (!current_line)
		return (NULL);
	len = ft_strlen(current_line);
	/* remove trailing newline if present (lines from get_next_line include '\n') */
	if (len > 0 && current_line[len - 1] == '\n')
		len--;
	ret = (char *)malloc(len + 1);
	if (!ret)
		return (NULL);
	if (len > 0)
		ft_memcpy(ret, current_line, len);
	ret[len] = '\0';
	return (ret);
}

/* count non-empty lines in map_start */
int	count_map_lines(char **start)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (start && start[i])
	{
		if (!line_is_empty(start[i]))
			count++;
		i++;
	}
	return (count);
}

/* free the first n lines of map and the container */
void	free_partial_map(char **map, int n)
{
	int	i;

	if (!map)
		return ;
	i = 0;
	while (i < n)
	{
		free(map[i]);
		i++;
	}
	free(map);
}
void	free_full_map(char **map)
{
	int	i;

	if (!map)
		return ;
	i = 0;
	while (map[i])
	{
		free(map[i]);
		i++;
	}
	free(map);
}

int	map_fail(t_cub *cub, const char *msg)
{
	if (msg)
		ft_putendl_fd((char *)msg, 2);
	if (cub->map)
	{
		free_full_map(cub->map);
		cub->map = NULL;
	}
	/* Si tu veux EXIT ici :
	 *   cub_clean_all(cub);
	 *   exit(EXIT_FAILURE);
	 */
	return (1);
}

/* build cub->map from cub->map_start; returns 0 on success, 1 on error */
int	check_map(t_cub *cub)
{
	int	i;
	int	j;
	int	count;

	if (!cub || !cub->map_start)
		return (1);
	count = count_map_lines(cub->map_start);
	cub->map = (char **)malloc(sizeof(char *) * (count + 1));
	if (!cub->map)
		return (1);
	i = 0;
	j = 0;
	while (cub->map_start[i])
	{
		if (!line_is_empty(cub->map_start[i]))
		{
			cub->map[j] = copy_map(cub->map_start[i]);
			if (!cub->map[j])
				return (free_partial_map(cub->map, j), cub->map = NULL, 1);
			j++;
		}
		i++;
	}
	cub->map[j] = NULL;
	if (is_map_valid(cub))
		return (map_fail(cub,
			"Error: invalid map (must be closed and contain exactly one spawn N/S/E/W)"));
	/* find alien spawn position marked with 'X' in map (optional) */
	find_alien_spawn(cub->map, &cub->alien_spawn_x, &cub->alien_spawn_y);
	return (0);
}

/* Minimal implementation placeholder.
 * Should check that the map is closed and contains exactly one spawn (N/S/E/W).
 * Returning 0 means "valid" for now.
 */
int	map_is_closed_and_single_spawn(char **map)
{
	(void)map;
	return (0);
}

