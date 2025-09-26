/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parse_map.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 19:48:36 by adi-marc          #+#    #+#             */
/*   Updated: 2025/09/16 20:42:244 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* ---------- print map buffer ---------- */
static void print_map_buffer(char **map_buffer)
{
	int i = 0;

	while (map_buffer && map_buffer[i])
	{
		printf("%s\n", map_buffer[i]);
		i++;
	}
}

//copie la map dans un buffer
int	is_map_valid(t_cub *cub)
{
	char	**map_buffer;
	if (allocate_map_buffer(cub, &map_buffer, 2))
		return (map_fail(cub, "Error: could not allocate map buffer"));
	
	/* Show map_buffer */
	print_map_buffer(map_buffer);

	// Check si le spawn est present et est unique
	if (check_spawn(map_buffer, cub))
		return (map_fail(cub, "error du au spawn\n"));
	// Check si la map est bien entourée de murs
	if (wall_check(map_buffer))
		return (map_fail(cub, "error du aux murs"));

	// Free la map_buffer utilisée pour le checks du spawn et des murs
	free_full_map(map_buffer);
	return (0);
}

/*
 * allocate_map_buffer:
 * - allocates *map_buffer and fills it with copies of cub->map
 * - reserves 'pad' extra empty rows (NULL pointers) at the end for later padding
 * - returns 0 on success, 1 on error (and leaves *map_buffer NULL)
 *
 * Uses copy_map and free_partial_map to avoid duplication.
 */
static void free_partial_rows(char **rows, int n)
{
	int i = 0;
	if (!rows)
		return;
	while (i < n) 
	{
		free(rows[i++]); 
		free(rows);
	}
}

/* ---------- helpers dims ---------- */
static int arr_len(char **a)
{
	int i;
	
	i = 0;
	while(a && a[i])
			i++;
	return i;
}
static int max_len(char **a)
{
	int i;
	int m;
	int k;

	i = 0;
	m = 0;
	k = 0;
	while (a && a[i])
	{
		k=0;
		while (a[i][k])
				k++;
		if (k>m)
			m=k;
		i++;
	}
	return m;
}

/* ---------- allocation du padded (respecte pad partout) ---------- */
int allocate_map_buffer(t_cub *cub, char ***out, int pad)
{
	int h, w, ph, pw, i, j;

	if (!cub || !cub->map || !out || pad < 1)
		return (1);

	h = arr_len(cub->map);
	w = max_len(cub->map);
	ph = h + 2 * pad;
	pw = w + 2 * pad;

	*out = (char **)malloc(sizeof(char *) * (ph + 1));
	if (!*out) return (1);

	/* alloue toutes les lignes + remplit en ' ' */
	i = 0;
	while (i < ph) {
		(*out)[i] = (char *)malloc(pw + 1);
		if (!(*out)[i]) return (free_partial_rows(*out, i), *out=NULL, 1);
		j = 0;
		while (j < pw) {
			(*out)[i][j] = ' ';
			j++;
		}
		(*out)[i][pw] = '\0';
		i++;
	}
	(*out)[ph] = NULL;

	/* copie la map au centre (offset pad, pad) */
	i = 0;
	while (i < h) {
		j = 0;
		while (cub->map[i][j]) {
			(*out)[i + pad][j + pad] = cub->map[i][j];
			j++;
		}
		i++;
	}
	return (0);
}


