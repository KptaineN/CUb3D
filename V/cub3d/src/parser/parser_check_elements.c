/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_check_elements.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 20:12:38 by adi-marc          #+#    #+#             */
/*   Updated: 2025/09/22 14:16:31 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

const char *skip_spaces(const char *s)
{
	while (*s && ft_isspace(*s))
		s++;
	return (s);
}

static int key_with_payload(const char *s, const char *key)
{
	const char *p;

	s = skip_spaces(s);
	p = strstr(s, key);
	if (p != s)
		return (0);
	p += ft_strlen(key);
	if (*p == '\0' || !ft_isspace(*p))
		return (0);
	p = skip_spaces(p);
	if (*p == '\0')
		return (0);
	return (1);
}

static int key_with_single_token(const char *s, const char *key)
{
	const char *p;

	s = skip_spaces(s);
	p = ft_strstr(s, key);
	if (p != s)
		return (0);
	p += ft_strlen(key);
	if (*p == '\0' || !ft_isspace(*p))
		return (0);
	p = skip_spaces(p);
	if (*p == '\0')
		return (0);
	while (*p && !ft_isspace(*p))
		p++;
	p = skip_spaces(p);
	return (*p == '\0');
}

static int key_without_payload(const char *s, const char *key)
{
	const char *p;

	s = skip_spaces(s);
	p = strstr(s, key);
	if (p != s)
		return (0);
	p += ft_strlen(key);
	p = skip_spaces(p);
	if (*p == '\0')
		return (1);
	return (0);
}

static int is_blank_line(const char *s)
{
	while (*s)
	{
		if (!ft_isspace(*s))
			return (0);
		s++;
	}
	return (1);
}

static int looks_like_map_line(const char *s)
{
	s = skip_spaces(s);
	if (*s == '0' || *s == '1')
		return (1);
	return (0);
}

int	check_elements(t_cub *cub)
{
	int		i;
	int		seen_no;
	int		seen_so;
	int		seen_we;
	int		seen_ea;
	int		seen_f;
	int		seen_c;
	char	*line;

	i = 0;
	// Skip all empty lines at the beginning
	while (cub->file[i] && line_is_empty(cub->file[i]))
		i++;
	if (!cub->file[i])
		return (printf("Error: file contains only empty lines\n"), 1);
	seen_no = 0;
	seen_so = 0;
	seen_we = 0;
	seen_ea = 0;
	seen_f = 0;
	seen_c = 0;
	while (cub->file[i])
	{
		line = cub->file[i];
		if (is_blank_line(line))
		{
			i++;
			continue;
		}
		if (looks_like_map_line(line))
		{
			cub->map_start = &cub->file[i];
			break;
		}
		if (key_with_single_token(line, "NO"))
		{
			if (seen_no)
				return (printf("Error: duplicate NO\n"), 1);
			seen_no = 1;
		}
		else if (key_with_single_token(line, "SO"))
		{
			if (seen_so)
				return (printf("Error: duplicate SO\n"), 1);
			seen_so = 1;
		}
		else if (key_with_single_token(line, "WE"))
		{
			if (seen_we)
				return (printf("Error: duplicate WE\n"), 1);
			seen_we = 1;
		}
		else if (key_with_single_token(line, "EA"))
		{
			if (seen_ea)
				return (printf("Error: duplicate EA\n"), 1);
			seen_ea = 1;
		}
		else if (key_with_payload(line, "F"))
		{
			if (seen_f)
				return (printf("Error: duplicate F\n"), 1);
			seen_f = 1;
		}
		else if (key_with_payload(line, "C"))
		{
			if (seen_c)
				return (printf("Error: duplicate C\n"), 1);
			seen_c = 1;
		}
		else if (key_without_payload(line, "NO") || key_without_payload(line, "SO")
			|| key_without_payload(line, "WE") || key_without_payload(line, "EA")
			|| key_without_payload(line, "F") || key_without_payload(line, "C"))
		{
			return (printf("Error: header without value\n"), 1);
		}
		i++;
	}
	if (!seen_no || !seen_so || !seen_we || !seen_ea || !seen_f || !seen_c)
		return (printf("Error: missing header(s). Need NO, SO, WE, EA, F, C exactly once\n"), 1);
	return (0);
}
