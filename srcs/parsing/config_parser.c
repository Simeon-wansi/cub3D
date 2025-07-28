/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/07/28 14:52:26 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_utils.h"

/**
 * Sets player information in the map
 */
void	set_player_info(t_config *config, t_map *map)
{
	map->player_dir = config->map.player_dir;
	map->player_x = config->map.player_pos_x;
	map->player_y = config->map.player_pos_y;
}

bool	process_config_line(t_list *current, t_config *config,
		t_arena *arena, t_list **map_start)
{
	char	*trimmed;

	trimmed = skip_whitespace((char *)current->content);
	trim_newline(trimmed);
	if (!*trimmed)
		return (true);
	if (!ft_strncmp(trimmed, "NO ", 3) || !ft_strncmp(trimmed, "SO ", 3)
		|| !ft_strncmp(trimmed, "WE ", 3) || !ft_strncmp(trimmed, "EA ", 3)
		|| !ft_strncmp(trimmed, "F ", 2) || !ft_strncmp(trimmed, "C ", 2))
	{
		if (!parse_element((char *)current->content, config, arena))
			return (false);
	}
	else if (validate_map_line(trimmed))
	{
		*map_start = current;
		return (false);
	}
	else
	{
		print_error("Invalid line in configuration");
		return (false);
	}
	return (true);
}

bool	parse_config_elements(t_list *lines, t_config *config,
		t_arena *arena, t_list **map_start)
{
	t_list	*current;

	current = lines;
	*map_start = NULL;
	while (current)
	{
		if (!process_config_line(current, config, arena, map_start))
			break ;
		current = current->next;
	}
	if (!validate_required_elements(config))
	{
		print_error("Missing required configuration elements");
		return (false);
	}
	if (!*map_start)
	{
		print_error("No map found in file");
		return (false);
	}
	return (true);
}

t_config	*init_config(t_arena *arena)
{
	t_config	*config;

	config = arena_alloc(arena, sizeof(t_config));
	if (!config)
		return (NULL);
	ft_memset(config, 0, sizeof(t_config));
	return (config);
}
