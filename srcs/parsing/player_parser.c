/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/07/26 22:05:08 by hmensah-         ###   ########.fr       */
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
