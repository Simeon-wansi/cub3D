/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validator2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/07/03 19:00:00 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_utils.h"

bool	check_side_columns(t_config *config, int i)
{
	char	first;
	char	last;
	int		len;

	len = ft_strlen(config->map.board[i]);
	if (len == 0)
		return (true);
	first = config->map.board[i][0];
	last = config->map.board[i][len - 1];
	if (!check_side_column_char(first))
		return (false);
	if (!check_side_column_char(last))
		return (false);
	return (true);
}

bool	are_side_columns_valid(t_config *config)
{
	int	i;

	i = 0;
	while (i < config->map.rows)
	{
		if (!check_side_columns(config, i))
			return (false);
		i++;
	}
	return (true);
}

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
