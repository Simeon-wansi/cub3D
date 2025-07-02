/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_util2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:07:27 by hmensah-          #+#    #+#             */
/*   Updated: 2025/07/02 19:33:22 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_utils.h"

bool	find_player_position_p(t_config *config)
{
	int	i;
	int	j;
	int	player_count;

	player_count = 0;
	i = 0;
	while (i < config->map.rows)
	{
		j = 0;
		while (config->map.board[i][j])
		{
			if (config->map.board[i][j] == 'N' || config->map.board[i][j] == 'S'
				|| config->map.board[i][j] == 'E'
				|| config->map.board[i][j] == 'W')
			{
				config->map.player_pos_x = j;
				config->map.player_pos_y = i;
				config->map.player_dir = config->map.board[i][j];
				player_count++;
			}
			j++;
		}
		i++;
	}
	return (player_count == 1);
}

// Helper function to set maximum width and column count
void	set_map_dimensions(t_config *config)
{
	int	i;
	int	max_width;
	int	len;

	max_width = 0;
	i = 0;
	while (i < config->map.rows)
	{
		len = ft_strlen(config->map.board[i]);
		if (len > max_width)
			max_width = len;
		i++;
	}
	config->map.cols = max_width;
}

// Helper function to check if a row contains only walls and spaces
bool	is_row_valid(char *row)
{
	int		j;
	char	c;

	j = 0;
	while (j < (int)ft_strlen(row))
	{
		c = row[j];
		if (c != '1' && c != ' ')
			return (false);
		j++;
	}
	return (true);
}

// Helper function to check if side columns are valid
bool	are_side_columns_valid(t_config *config)
{
	int		i;
	char	first;
	char	last;
	int		len;

	i = 0;
	while (i < config->map.rows)
	{
		len = ft_strlen(config->map.board[i]);
		if (len > 0)
		{
			first = config->map.board[i][0];
			last = config->map.board[i][len - 1];
			if (first != '1' && first != ' ')
				return (false);
			if (last != '1' && last != ' ')
				return (false);
		}
		i++;
	}
	return (true);
}

bool validate_map_line(char *line)
{
	int i;
	
	i = 0;
	while (line[i] && line[i] != '\n')
	{
		if (!is_valid_map_char(line[i]))
			return (false);
		i++;
	}
	return (true);
}
