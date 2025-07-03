/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/07/03 15:36:54 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_utils.h"

bool	is_player_char(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

void	record_player_position(t_config *config, int i, int j)
{
	config->map.player_pos_x = j;
	config->map.player_pos_y = i;
	config->map.player_dir = config->map.board[i][j];
}

bool	find_player_position_row(t_config *config, int i, int *count)
{
	int	j;

	j = 0;
	while (config->map.board[i][j])
	{
		if (is_player_char(config->map.board[i][j]))
		{
			record_player_position(config, i, j);
			(*count)++;
		}
		j++;
	}
	return (true);
}

bool	find_player_position_p(t_config *config)
{
	int	i;
	int	player_count;

	player_count = 0;
	i = 0;
	while (i < config->map.rows)
	{
		find_player_position_row(config, i, &player_count);
		i++;
	}
	return (player_count == 1);
}

void	print_config(t_config *config)
{
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
}
