/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 06:39:07 by adi-marc          #+#    #+#             */
/*   Updated: 2025/02/06 10:26:32 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Read_buffer -> loops over the file descriptor untill it finds a \n character
or the end of the file. It reads the buffer and joins it with the static buffer
and returns it as soon as it finds a \n character.
@param fd: file descriptor
@param buffer: buffer used to read the file
@param static_buffer: static buffer that stores the rest of the line
*/
static char	*read_buffer(int fd, char *buffer, char *static_buffer)
{
	int		bytes_r;
	char	*temp;

	bytes_r = 1;
	while (bytes_r > 0)
	{
		bytes_r = read(fd, buffer, BUFFER_SIZE);
		if (bytes_r == -1)
			return (NULL);
		else if (bytes_r == 0)
			break ;
		buffer[bytes_r] = '\0';
		if (!static_buffer)
			static_buffer = ft_strdup("");
		temp = static_buffer;
		static_buffer = ft_strjoin(static_buffer, buffer);
		free(temp);
		temp = NULL;
		if (!static_buffer)
			return (NULL);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (static_buffer);
}

/*
Line_extraction -> extracts the line from the static buffer and returns it.
@param static_buffer: static buffer given by the read_buffer function
*/
char	*line_extraction(char *static_buffer)
{
	size_t	i;
	char	*backup_line;

	i = 0;
	while (static_buffer[i] != '\n' && static_buffer[i] != '\0')
		i++;
	if (static_buffer[i] == '\0' || static_buffer[1] == '\0')
		return (NULL);
	backup_line = ft_substr(static_buffer, i + 1, ft_strlen(static_buffer) - i);
	if (!*backup_line)
	{
		free(backup_line);
		backup_line = NULL;
	}
	static_buffer[i + 1] = '\0';
	return (backup_line);
}

/*
get_next_line -> reads a file line by line and returns it.
Checks for NULLS errors and malloc the initial buffer.
@param fd: file descriptor
*/
char	*get_next_line(int fd)
{
	char		*line;
	char		*buffer;
	static char	*static_buffer[5000];

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	line = read_buffer(fd, buffer, static_buffer[fd]);
	free(buffer);
	buffer = NULL;
	if (!line)
	{
		free(static_buffer[fd]);
		static_buffer[fd] = NULL;
		return (NULL);
	}
	static_buffer[fd] = line_extraction(line);
	return (line);
}
