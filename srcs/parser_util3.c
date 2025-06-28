/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_util3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:07:27 by hmensah-          #+#    #+#             */
/*   Updated: 2025/06/28 18:22:24 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_utils.h"

// Helper function to check if a position is walkable
bool	is_walkable(char c)
{
	return (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

// Helper function to check adjacency validity for a single position
bool	is_position_valid(t_config *config, int i, int j)
{
	char	current;
	char	up;
	char	down;
	char	left;
	char	right;

	current = config->map.board[i][j];
	if (!is_walkable(current))
		return (true);
	if (j < (int)ft_strlen(config->map.board[i - 1]))
		up = config->map.board[i - 1][j];
	else
		up = ' ';
	if (j < (int)ft_strlen(config->map.board[i + 1]))
		down = config->map.board[i + 1][j];
	else
		down = ' ';
	left = config->map.board[i][j - 1];
	right = config->map.board[i][j + 1];
	if (!is_valid_map_char(up) || !is_valid_map_char(down)
		|| !is_valid_map_char(left) || !is_valid_map_char(right))
	{
		return (false);
	}
	return (true);
}

// Helper function to check internal positions for map closure
bool	are_internal_positions_valid(t_config *config)
{
	int	i;
	int	j;

	i = 1;
	while (i < config->map.rows - 1)
	{
		j = 1;
		while (j < (int)ft_strlen(config->map.board[i]) - 1)
		{
			if (!is_position_valid(config, i, j))
				return (false);
			j++;
		}
		i++;
	}
	return (true);
}

void	print_config(t_config *config)
{
	int	i;

	if (!config)
		return ;
	printf("=== Configuration ===\n");
	printf("North texture: %s\n", config->textures.north);
	printf("South texture: %s\n", config->textures.south);
	printf("West texture: %s\n", config->textures.west);
	printf("East texture: %s\n", config->textures.east);
	printf("Floor color: %d,%d,%d\n", config->colors.floor_r,
		config->colors.floor_g, config->colors.floor_b);
	printf("Ceiling color: %d,%d,%d\n", config->colors.ceiling_r,
		config->colors.ceiling_g, config->colors.ceiling_b);
	printf("Player position: (%d,%d) facing %c\n", config->map.player_pos_x,
		config->map.player_pos_y, config->map.player_dir);
	printf("Map dimensions: %dx%d\n", config->map.cols, config->map.rows);
	printf("=== Map ===\n");
	i = 0;
	while (i < config->map.rows && config->map.board[i])
	{
		printf("%s\n", config->map.board[i]);
		i++;
	}
}

char	*prepare_content(char *line, int offset)
{
	char	*content;

	content = skip_whitespace(line + offset);
	trim_newline(content);
	return (content);
}

bool	is_map_closed(t_config *config)
{
	set_map_dimensions(config);
	if (!is_row_valid(config->map.board[0]))
		return (false);
	if (!is_row_valid(config->map.board[config->map.rows - 1]))
		return (false);
	if (!are_side_columns_valid(config))
		return (false);
	if (!are_internal_positions_valid(config))
		return (false);
	return (true);
}
