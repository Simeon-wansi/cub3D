/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   position_validator.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/07/03 19:00:00 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_utils.h"

char	get_cell_at_position(t_config *config, int i, int j)
{
	int	row_len;
	
	if (i < 0 || i >= config->map.rows)
		return (' ');
	row_len = ft_strlen(config->map.board[i]);
	if (j < 0 || j >= row_len)
		return (' ');
	return (config->map.board[i][j]);
}

bool	check_adjacent_cells(t_config *config, int i, int j)
{
	char	up;
	char	down;
	char	left;
	char	right;

	up = get_cell_at_position(config, i - 1, j);
	down = get_cell_at_position(config, i + 1, j);
	left = get_cell_at_position(config, i, j - 1);
	right = get_cell_at_position(config, i, j + 1);
	
	if (!is_valid_map_char(up) || !is_valid_map_char(down))
		return (false);
	if (!is_valid_map_char(left) || !is_valid_map_char(right))
		return (false);
	if (up == ' ' || down == ' ' || left == ' ' || right == ' ')
		return (false);
	return (true);
}

bool	is_position_valid(t_config *config, int i, int j)
{
	char	current;

	current = config->map.board[i][j];
	if (!is_walkable(current))
		return (true);
	return (check_adjacent_cells(config, i, j));
}

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
