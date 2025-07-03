/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   element_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/07/03 15:34:30 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_utils.h"

bool	handle_east_texture(char *content, t_config *config, t_arena *arena)
{
	if (config->parsed_ea)
		return (false);
	config->parsed_ea = parse_texture(content, &config->textures.east, arena);
	return (config->parsed_ea);
}

bool	handle_floor_color(char *content, t_config *config)
{
	if (config->parsed_f)
		return (false);
	config->parsed_f = parse_color_p(content, &config->colors.floor_r,
			&config->colors.floor_g, &config->colors.floor_b);
	return (config->parsed_f);
}

bool	handle_ceiling_color(char *content, t_config *config)
{
	if (config->parsed_c)
		return (false);
	config->parsed_c = parse_color_p(content, &config->colors.ceiling_r,
			&config->colors.ceiling_g, &config->colors.ceiling_b);
	return (config->parsed_c);
}

bool	parse_element(char *line, t_config *config, t_arena *arn)
{
	char	*trimmed;

	trimmed = skip_whitespace(line);
	if (is_empty_line(trimmed))
		return (true);
	if (!ft_strncmp(trimmed, "NO ", 3))
		return (handle_north_texture(prepare_content(trimmed, 3), config, arn));
	else if (!ft_strncmp(trimmed, "SO ", 3))
		return (handle_south_texture(prepare_content(trimmed, 3), config, arn));
	else if (!ft_strncmp(trimmed, "WE ", 3))
		return (handle_west_texture(prepare_content(trimmed, 3), config, arn));
	else if (!ft_strncmp(trimmed, "EA ", 3))
		return (handle_east_texture(prepare_content(trimmed, 3), config, arn));
	else if (!ft_strncmp(trimmed, "F ", 2))
		return (handle_floor_color(prepare_content(trimmed, 2), config));
	else if (!ft_strncmp(trimmed, "C ", 2))
		return (handle_ceiling_color(prepare_content(trimmed, 2), config));
	return (false);
}

bool	validate_required_elements(t_config *config)
{
	if (!config->parsed_no || !config->parsed_so)
		return (false);
	if (!config->parsed_we || !config->parsed_ea)
		return (false);
	if (!config->parsed_f || !config->parsed_c)
		return (false);
	return (true);
}
