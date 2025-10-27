/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:25:36 by adi-marc          #+#    #+#             */
/*   Updated: 2025/09/29 10:49:38 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

// 1) Check que argv[1] est bien un path
// Changer printf()
int	check_file_name(const char *path)
{
	char	*ext;

	if (!path || path[0] == '\0')
    {
        printf("Invalid file name rien dans la map");
        return (1);        
    }
	if (path[0] == '.')
	{
        printf("Invalid file name");
        return (1);        
    }
	ext = ft_strrchr(path, '.');
	if (!ext || ft_strncmp(ext, ".cub", 4) != 0)
    {
        printf("Invalid file extension, the file must be a .cub file\n");
        return (1);        
    }
	return (0);
}

// 1 si fichier vide (seulement espaces), 0 sinon
static int check_space_empty_fd(int fd)
{
    char *line;

    while ((line = get_next_line(fd)))
    {
        int i = 0;
        while (line[i] && ft_isspace((unsigned char)line[i]))
            i++;
        if (line[i] != '\0')
        {
            free(line);
            return 0; // a du contenu
        }
        free(line);
    }
    return 1; // vide
}

// Fonction qui check que le fichier n'est pas vide
int check_if_file_empty(const char *path)
{
    int fd = open(path, O_RDONLY);
    if (fd < 0)
        return (fprintf(stderr, "Error: cannot open '%s'\n", path), 1);

    if (check_space_empty_fd(fd))
    {
        close(fd);
        return (fprintf(stderr, "Error: the map file is empty (only whitespace)\n"), 1);
    }
    close(fd);
    return 0;
}

static int	count_lines(const char *path)
{
	int		fd;
	int		n;
	char	*line;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (-1);
	n = 0;
	line = get_next_line(fd);
	while (line)
	{
		n++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (n);
}

/* 2) Allouer et remplir cub->file (chaque ligne du fichier, \n inclus) */
int	file_populate(const char *path, t_cub *cub)
{
	int		fd;
	int		i;
	int		n;
	char	*line;

	n = count_lines(path);
	if (n <= 0)              /* 0 = vide ; -1 = erreur open/read */
		return (n);
	cub->file = (char **)malloc(sizeof(char *) * (n + 1));
	if (!cub->file)
		return (-1);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (-1);
	i = 0;
	line = get_next_line(fd);
	while (line)
	{
		cub->file[i++] = ft_strdup(line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	cub->file[i] = NULL;
	return (i); /* nb de lignes remplies */
}

int ft_cub_parser(int argc, char **argv, t_cub *cub)
{
    if (argc != 2)
		return (printf("need map\n"), 1);
    if (check_file_name(argv[1]))
        return (1);
    if (check_if_file_empty(argv[1]))
        return (1);
    if (file_populate(argv[1], cub) <= 0)
        return (1);
    if(check_elements(cub))
        return (1);
    if (is_path_valid(cub))
        return (1);
    if (check_map(cub))
        return (1); 
    return (0);
}


/*int	check_if_file_empty(char *path)
{
	int		fd;
	char	*line;

	fd = open(path, O_RDONLY);
	if (fd < 0)
    {
        printf("Unable to open the map file");
        close(fd);
        return (1);
    }
	line = get_next_line(fd);
	if (!line || line[0] == '\0')
	{
		free(line);
		close(fd);
		printf("The map file is empty");
        return (1);
	}
	check_space_empty(line, fd);
	// Check if the entire file is empty (only whitespace)
	while (line)
	{
		int i = 0;
		int has_content = 0;
		while (line[i])
		{
			if (!ft_isspace((unsigned char)line[i]))
			{
				has_content = 1;
				break;
			}
			i++;
		}
		if (has_content)
		{
			free(line);
			close(fd);
			return (0);  // File has content
		}
		free(line);
		line = get_next_line(fd);
	}
	
	// If we reach here, the file contains only whitespace
	close(fd);
	printf("The map file is empty");
	return (1);
	close(fd);
	return (0);
}*/