/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/07/03 15:29:55 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_utils.h"

int	count_map_rows_p(t_list *map_start)
{
	t_list	*current;
	int		row_count;
	char	*trimmed;

	current = map_start;
	row_count = 0;
	while (current)
	{
		trimmed = skip_whitespace((char *)current->content);
		trim_newline(trimmed);
		if (*trimmed)
			row_count++;
		current = current->next;
	}
	return (row_count);
}

bool	allocate_map_board(t_config *config, int row_count, t_arena *arena)
{
	config->map.rows = row_count;
	config->map.board = arena_alloc(arena, sizeof(char *) * (row_count + 1));
	if (!config->map.board)
		return (false);
	return (true);
}

bool	copy_map_line(t_config *config, char *line, int index, t_arena *arena)
{
	char	*trimmed;

	trimmed = skip_whitespace(line);
	trim_newline(trimmed);
	
	if (!validate_map_line(trimmed))
	{
		print_error("Invalid character in map");
		return (false);
	}
	
	config->map.board[index] = arena_alloc(arena, ft_strlen(trimmed) + 1);
	if (!config->map.board[index])
		return (false);
	ft_strcpy(config->map.board[index], trimmed);
	return (true);
}

bool	populate_map_board(t_list *map_start, t_config *config, t_arena *arena)
{
	t_list	*current;
	int		i;
	
	current = map_start;
	i = 0;
	while (current && i < config->map.rows)
	{
		char *line = (char *)current->content;
		char *trimmed = skip_whitespace(line);
		
		if (*trimmed)
		{
			if (!copy_map_line(config, line, i, arena))
				return (false);
			i++;
		}
		current = current->next;
	}
	config->map.board[i] = NULL;
	return (true);
}
