/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_util1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:07:27 by hmensah-          #+#    #+#             */
/*   Updated: 2025/06/28 21:22:16 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_utils.h"

bool	parse_texture(char *line, char **texture_path, t_arena *arena)
{
	char	*path;
	int		fd;

	path = skip_whitespace(line);
	if (!*path)
		return (false);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (false);
	close(fd);
	*texture_path = arena_alloc(arena, ft_strlen(path) + 1);
	if (!*texture_path)
		return (false);
	ft_strcpy(*texture_path, path);
	return (true);
}

bool	parse_element(char *line, t_config *config, t_arena *arena)
{
	char	*trimmed;
	char	*content;

	trimmed = skip_whitespace(line);
	if (!*trimmed || *trimmed == '\n')
		return (true);
	if (!ft_strncmp(trimmed, "NO ", 3))
	{
		if (config->parsed_no)
			return (false);
		content = skip_whitespace(trimmed + 3);
		trim_newline(content);
		config->parsed_no = parse_texture(content, &config->textures.north, arena);
		return (config->parsed_no);
	}
	else if (!ft_strncmp(trimmed, "SO ", 3))
	{
		if (config->parsed_so)
			return (false);
		content = skip_whitespace(trimmed + 3);
		trim_newline(content);
		config->parsed_so = parse_texture(content, &config->textures.south, arena);
		return (config->parsed_so);
	}
	else if (!ft_strncmp(trimmed, "WE ", 3))
	{
		if (config->parsed_we)
			return (false);
		content = skip_whitespace(trimmed + 3);
		trim_newline(content);
		config->parsed_we = parse_texture(content, &config->textures.west, arena);
		return (config->parsed_we);
	}
	else if (!ft_strncmp(trimmed, "EA ", 3))
	{
		if (config->parsed_ea)
			return (false);
		content = skip_whitespace(trimmed + 3);
		trim_newline(content);
		config->parsed_ea = parse_texture(content, &config->textures.east, arena);
		return (config->parsed_ea);
	}
	else if (!ft_strncmp(trimmed, "F ", 2))
	{
		if (config->parsed_f)
			return (false);
		content = skip_whitespace(trimmed + 2);
		trim_newline(content);
		config->parsed_f = parse_color(content, &config->colors.floor_r, 
									 &config->colors.floor_g, &config->colors.floor_b);
		return (config->parsed_f);
	}
	else if (!ft_strncmp(trimmed, "C ", 2))
	{
		if (config->parsed_c)
			return (false);
		content = skip_whitespace(trimmed + 2);
		trim_newline(content);
		config->parsed_c = parse_color(content, &config->colors.ceiling_r, 
									 &config->colors.ceiling_g, &config->colors.ceiling_b);
		return (config->parsed_c);
	}
	return (false);
}
