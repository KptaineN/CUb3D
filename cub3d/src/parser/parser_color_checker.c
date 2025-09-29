/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_color_checker.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-marc <adi-marc@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 13:56:50 by adi-marc          #+#    #+#             */
/*   Updated: 2025/09/26 15:27:10 by adi-marc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static t_color *allocate_color(void)
{
	t_color *color;

	color = malloc(sizeof(t_color));
	if (!color)
		return (NULL);
	color->string = NULL;
	color->red = 0;
	color->green = 0;
	color->blue = 0;
	return (color);
}

static int is_valid_rgb_value(int value)
{
	return (value >= 0 && value <= 255);
}

static int is_valid_number_string(const char *str)
{
	int i;
	
	if (!str || !str[0])
		return (0);
	
	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int count_commas(const char *str)
{
	int count;
	int i;
	
	if (!str)
		return (0);
	
	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == ',')
			count++;
		i++;
	}
	return (count);
}

static int has_consecutive_commas(const char *str)
{
	int i;
	
	if (!str)
		return (0);
	
	i = 0;
	while (str[i])
	{
		if (str[i] == ',' && str[i + 1] == ',')
			return (1);
		i++;
	}
	return (0);
}

static int is_valid_rgb_format(const char *str)
{
	int i;
	int comma_count;
	int state; // 0 = expect number start, 1 = in number, 2 = after number before comma/end
	
	if (!str || !str[0])
		return (0);
	
	// Check basic comma requirements first
	comma_count = count_commas(str);
	if (comma_count != 2 || has_consecutive_commas(str))
		return (0);
	
	// Skip leading whitespace
	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	
	state = 0; // Expecting number start
	comma_count = 0; // Track processed commas
	
	while (str[i])
	{
		if (state == 0) // Expecting start of number
		{
			if (ft_isdigit(str[i]))
				state = 1; // Now in number
			else if (str[i] == ' ' || str[i] == '\t')
				; // Skip whitespace before number
			else
				return (0); // Invalid character
		}
		else if (state == 1) // In number
		{
			if (ft_isdigit(str[i]))
				; // Continue in number
			else if (str[i] == ' ' || str[i] == '\t')
				state = 2; // After number, expecting comma or end
			else if (str[i] == ',')
			{
				comma_count++;
				if (comma_count > 2)
					return (0);
				state = 0; // Expect next number
			}
			else
				return (0); // Invalid character in number
		}
		else if (state == 2) // After number, before comma/end
		{
			if (str[i] == ' ' || str[i] == '\t')
				; // Skip whitespace
			else if (str[i] == ',')
			{
				comma_count++;
				if (comma_count > 2)
					return (0);
				state = 0; // Expect next number
			}
			else
				return (0); // Invalid character after number
		}
		i++;
	}
	
	// Must end in state 1 (in number) or 2 (after number) and have processed 2 commas
	return ((state == 1 || state == 2) && comma_count == 2);
}

static int parse_rgb_values(const char *rgb_str, unsigned char *r, unsigned char *g, unsigned char *b)
{
	char **components;
	char *trimmed_comp;
	int red, green, blue;
	int i;

	if (!rgb_str)
		return (0);
	
	// First, validate the overall format
	if (!is_valid_rgb_format(rgb_str))
		return (0);
	
	components = ft_split(rgb_str, ',');
	if (!components)
		return (0);
	
	i = 0;
	while (components[i])
		i++;
	
	if (i != 3)
	{
		i = 0;
		while (components[i])
			free(components[i++]);
		free(components);
		return (0);
	}
	
	trimmed_comp = ft_strtrim(components[0], " \t");
	if (!trimmed_comp || !is_valid_number_string(trimmed_comp))
	{
		if (trimmed_comp)
			free(trimmed_comp);
		i = 0;
		while (components[i])
			free(components[i++]);
		free(components);
		return (0);
	}
	red = ft_atoi(trimmed_comp);
	free(trimmed_comp);
	
	trimmed_comp = ft_strtrim(components[1], " \t");
	if (!trimmed_comp || !is_valid_number_string(trimmed_comp))
	{
		if (trimmed_comp)
			free(trimmed_comp);
		i = 0;
		while (components[i])
			free(components[i++]);
		free(components);
		return (0);
	}
	green = ft_atoi(trimmed_comp);
	free(trimmed_comp);
	
	trimmed_comp = ft_strtrim(components[2], " \t");
	if (!trimmed_comp || !is_valid_number_string(trimmed_comp))
	{
		if (trimmed_comp)
			free(trimmed_comp);
		i = 0;
		while (components[i])
			free(components[i++]);
		free(components);
		return (0);
	}
	blue = ft_atoi(trimmed_comp);
	free(trimmed_comp);
	
	// Clean up components array
	i = 0;
	while (components[i])
		free(components[i++]);
	free(components);
	
	// Validate RGB ranges
	if (!is_valid_rgb_value(red) || !is_valid_rgb_value(green) || !is_valid_rgb_value(blue))
		return (0);
	
	*r = (unsigned char)red;
	*g = (unsigned char)green;
	*b = (unsigned char)blue;
	return (1);
}

static void store_color(t_cub *cub, const char *line, char color_type)
{
	const char *payload;
	t_color *color;

	if (!cub || !line)
		return;
	
	payload = skip_spaces(line + 1);
	color = allocate_color();
	if (!color)
		return;
	
	color->string = ft_strdup(payload);
	if (!color->string)
	{
		free(color);
		return;
	}
	
	if (color_type == 'F' && !cub->f_color)
		cub->f_color = color;
	else if (color_type == 'C' && !cub->c_color)
		cub->c_color = color;
	else
	{
		free(color->string);
		free(color);
	}
}

int valid_colors(t_cub *cub) 
{ 
	if (!cub || !cub->f_color || !cub->c_color)
		return (0);
	
	if (!cub->f_color->string || !cub->c_color->string)
		return (0);
	
	if (!parse_rgb_values(cub->f_color->string, &cub->f_color->red, 
						  &cub->f_color->green, &cub->f_color->blue))
		return (0);
	
	if (!parse_rgb_values(cub->c_color->string, &cub->c_color->red, 
						  &cub->c_color->green, &cub->c_color->blue))
		return (0);
	
	return (1);
}

void retrieve_colors(t_cub *cub) 
{
	int i;
	char *trimmed;

	if (!cub || !cub->file)
		return;
	
	i = 0;
	while (cub->file[i])
	{
		trimmed = ft_strtrim(cub->file[i], " \t\r\n");
		if (!trimmed)
			return;
		
		if (ft_strncmp(trimmed, "F", 1) == 0 && (trimmed[1] == ' ' || trimmed[1] == '\t'))
			store_color(cub, trimmed, 'F');
		else if (ft_strncmp(trimmed, "C", 1) == 0 && (trimmed[1] == ' ' || trimmed[1] == '\t'))
			store_color(cub, trimmed, 'C');
		
		free(trimmed);
		i++;
	}
}