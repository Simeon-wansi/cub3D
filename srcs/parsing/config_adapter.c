/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_adapter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/07/28 15:00:30 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"
#include "parser_utils.h"

void	set_player_info(t_config *config, t_map *map);

/*
 * if src is non‑NULL, allocate dst via arena and copy src into it.
 * Returns false on allocation failure, true otherwise.
 */
static bool	copy_one_texture(const char *src, char **dst, t_arena *arena)
{
	size_t	len;

	if (!src)
		return (true);
	len = ft_strlen(src);
	*dst = arena_alloc(arena, len + 1);
	if (!*dst)
		return (false);
	ft_strcpy(*dst, src);
	return (true);
}

/*
 * copy each of the 4 texture paths, using the helper.
 */
bool	copy_texture_paths(t_config *config, t_map *map, t_arena *arena)
{
	if (!copy_one_texture(config->textures.north, &map->no_texture_path, arena))
		return (false);
	if (!copy_one_texture(config->textures.south, &map->so_texture_path, arena))
		return (false);
	if (!copy_one_texture(config->textures.west, &map->we_texture_path, arena))
		return (false);
	if (!copy_one_texture(config->textures.east, &map->ea_texture_path, arena))
		return (false);
	return (true);
}

/**
 * Sets the color values in the map
 */
void	set_map_colors(t_config *config, t_map *map)
{
	map->floor_color.red = config->colors.floor_r;
	map->floor_color.green = config->colors.floor_g;
	map->floor_color.blue = config->colors.floor_b;
	map->ceiling_color.red = config->colors.ceiling_r;
	map->ceiling_color.green = config->colors.ceiling_g;
	map->ceiling_color.blue = config->colors.ceiling_b;
}

/**
 * Copies map grid data from config to map
 */
bool	copy_map_grid(t_config *config, t_map *map, t_arena *arena)
{
	int	i;

	map->width = config->map.cols;
	map->height = config->map.rows;
	map->grid = arena_alloc(arena, sizeof(char *) * (map->height + 1));
	if (!map->grid)
		return (false);
	i = 0;
	while (i < map->height)
	{
		map->grid[i] = arena_alloc(arena, map->width + 1);
		if (!map->grid[i])
			return (false);
		ft_strcpy(map->grid[i], config->map.board[i]);
		i++;
	}
	map->grid[i] = NULL;
	return (true);
}

/**
 * Transforms a t_config structure into a t_map structure
 * Returns true if successful, false otherwise
 */
bool	config_to_map(t_config *config, t_map *map, t_arena *arena)
{
	if (!config || !map || !arena)
		return (false);
	ft_memset(map, 0, sizeof(t_map));
	copy_texture_paths(config, map, arena);
	set_map_colors(config, map);
	if (!copy_map_grid(config, map, arena))
		return (false);
	set_player_info(config, map);
	return (true);
}
