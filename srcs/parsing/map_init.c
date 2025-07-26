/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/07/26 21:58:22 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"
#include "parser_utils.h"

/**
 * Initializes the map using the parser and adapter
 */
bool	init_map_using_parser(t_map *map, char *map_path, t_arena *arena)
{
	t_config	*config;

	config = parse_cub_file(map_path, arena);
	if (!config)
	{
		ft_printf("Error: Failed to parse map file: %s\n", map_path);
		return (false);
	}
	if (!config_to_map(config, map, arena))
	{
		ft_printf("Error: Failed to convert config to map\n");
		return (false);
	}
	printf("Debug: Map loaded with %d rows and %d columns\n",
		map->height, map->width);
	return (true);
}
