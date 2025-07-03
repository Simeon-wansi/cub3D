/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/07/03 19:00:00 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_utils.h"

bool	allocate_texture_path(char *path, char **texture_path, t_arena *arena)
{
	*texture_path = arena_alloc(arena, ft_strlen(path) + 1);
	if (!*texture_path)
		return (false);
	ft_strcpy(*texture_path, path);
	return (true);
}

bool	parse_texture(char *line, char **texture_path, t_arena *arena)
{
	char	*path;

	path = skip_whitespace(line);
	if (!*path)
		return (false);
	if (!verify_texture_file(path))
		return (false);
	return (allocate_texture_path(path, texture_path, arena));
}

bool	handle_north_texture(char *content, t_config *config, t_arena *arena)
{
	if (config->parsed_no)
		return (false);
	config->parsed_no = parse_texture(content, &config->textures.north, arena);
	return (config->parsed_no);
}

bool	handle_south_texture(char *content, t_config *config, t_arena *arena)
{
	if (config->parsed_so)
		return (false);
	config->parsed_so = parse_texture(content, &config->textures.south, arena);
	return (config->parsed_so);
}

bool	handle_west_texture(char *content, t_config *config, t_arena *arena)
{
	if (config->parsed_we)
		return (false);
	config->parsed_we = parse_texture(content, &config->textures.west, arena);
	return (config->parsed_we);
}
