/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_path_checker.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 12:38:47 by adi-marc          #+#    #+#             */
/*   Updated: 2025/09/27 14:17:35 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int existing_maps(t_cub *cub)
{
    int fd;

    if (!cub || !cub->no_path || !cub->so_path || !cub->we_path || !cub->ea_path)
        return (0);
    fd = open(cub->no_path, O_RDONLY);
    if (fd < 0) 
        return (0);
    close(fd);
    fd = open(cub->so_path, O_RDONLY);
    if (fd < 0) 
        return (0);
    close(fd);
    fd = open(cub->we_path, O_RDONLY);
    if (fd < 0) 
        return (0);
    close(fd);
    fd = open(cub->ea_path, O_RDONLY);
    if (fd < 0) 
        return (0);
    close(fd);
    return (1);
}

static int valid_prefix(t_cub *cub)
{
    if (!cub || !cub->no_path || !cub->so_path || !cub->ea_path || !cub->we_path)
        return (0);
    if ((ft_strncmp(cub->no_path, "./", 2) != 0)
    || (ft_strncmp(cub->so_path, "./", 2) != 0)
    || (ft_strncmp(cub->ea_path, "./", 2) != 0)
    || (ft_strncmp(cub->we_path, "./", 2) != 0))
        return (0);
    return (1);
}

static int valid_extension(t_cub *cub)
{
    if (!valid_prefix(cub)) {
        printf("The path of the textures must start with ./\n");
        return (0);
    }
    if ((ft_lastchars(cub->no_path, ".xpm", 4) != 0 && ft_lastchars(cub->no_path, ".XPM", 4) != 0)
    || (ft_lastchars(cub->so_path, ".xpm", 4) != 0 && ft_lastchars(cub->so_path, ".XPM", 4) != 0)
    || (ft_lastchars(cub->ea_path, ".xpm", 4) != 0 && ft_lastchars(cub->ea_path, ".XPM", 4) != 0)
    || (ft_lastchars(cub->we_path, ".xpm", 4) != 0 && ft_lastchars(cub->we_path, ".XPM", 4) != 0)) 
    {
        printf("One of the textures has the wrong file type (must be .xpm or .XPM)\n");
        return (0);
    }
    return (1);
}

static void store_texture_path(t_cub *cub, const char *line)
{
    const char *payload;
    char *trimmed;

    if (!cub || !line)
        return;
    if (ft_strncmp(line, "NO", 2) == 0 && !cub->no_path)
    {
        payload = skip_spaces(line + 2);
        trimmed = ft_strtrim(payload, " \t\r\n");
        cub->no_path = trimmed;
    }
    else if (ft_strncmp(line, "SO", 2) == 0 && !cub->so_path) 
    {
        payload = skip_spaces(line + 2);
        trimmed = ft_strtrim(payload, " \t\r\n");
        cub->so_path = trimmed;
    }
    else if (ft_strncmp(line, "WE", 2) == 0 && !cub->we_path)
    {
        payload = skip_spaces(line + 2);
        trimmed = ft_strtrim(payload, " \t\r\n");
        cub->we_path = trimmed;
    }
    else if (ft_strncmp(line, "EA", 2) == 0 && !cub->ea_path)
    {
        payload = skip_spaces(line + 2);
        trimmed = ft_strtrim(payload, " \t\r\n");
        cub->ea_path = trimmed;
    }
}

static int retrieve_map_path(t_cub *cub)
{
    int   i;
    char *trimmed;

    if (!cub || !cub->file) 
        return (0);
    i = 0;
    while (cub->file[i]) 
    {
        trimmed = ft_strtrim(cub->file[i], " \t\r\n");
        if (!trimmed) 
            return (0);
        if (ft_strncmp(trimmed, "NO", 2) == 0
         || ft_strncmp(trimmed, "SO", 2) == 0
         || ft_strncmp(trimmed, "WE", 2) == 0
         || ft_strncmp(trimmed, "EA", 2) == 0)
            store_texture_path(cub, trimmed);
        free(trimmed);
        i++;
    }
    return (1);
}

int is_path_valid(t_cub *cub)
{
    if (!retrieve_map_path(cub)) 
        return (1);
    retrieve_colors(cub);
    if (!valid_prefix(cub) || !valid_extension(cub)
        || !existing_maps(cub))
    {
        printf("Error with the texture path\n");
        return (1);
    }
    if (!valid_colors(cub))
    {
        printf("Error with the colors: Invalid RGB format detected\n");
        return (1);
    }
    return (0);
}