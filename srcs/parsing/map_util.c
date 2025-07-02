/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:25:51 by hmensah-          #+#    #+#             */
/*   Updated: 2025/07/02 19:33:48 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_utils.h"

t_list *build_map_line_list(char *filename)
{
	t_list  *head;
	t_list  *current;
	int     fd;
	char    *line;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (NULL);
	line = get_next_line(fd);
	if (!line)
		return (close(fd), NULL);
	head = ft_lstnew(line);
	if (!head)
		return (close(fd), NULL);
	current = head;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break;
		current->next = ft_lstnew(line);
		if (!current->next)
		{
			ft_lstclear(&head, free);
			return (close(fd), NULL);
		}
		current = current->next;
	}
	return (close(fd), head);
}

bool parse_map(t_list *map_start, t_config *config, t_arena *arena)
{
	t_list  *current;
	int     row_count;
	int     i;

    current = map_start;
    row_count = 0;
    i = 0;
	while (current)
	{
		char *trimmed = skip_whitespace((char *)current->content);
		trim_newline(trimmed);
		if (*trimmed)
			row_count++;
		current = current->next;
	}
	if (row_count == 0)
	{
		print_error("Empty map");
		return (false);
	}
	config->map.rows = row_count;
	config->map.board = arena_alloc(arena, sizeof(char *) * (row_count + 1));
	if (!config->map.board)
		return (false);
	current = map_start;
	i = 0;
	while (current && i < row_count)
	{
		char *line = (char *)current->content;
		char *trimmed = skip_whitespace(line);
		trim_newline(trimmed);
		
		if (*trimmed)
		{
			if (!validate_map_line(trimmed))
			{
				print_error("Invalid character in map");
				return (false);
			}
			
			config->map.board[i] = arena_alloc(arena, ft_strlen(trimmed) + 1);
			if (!config->map.board[i])
				return (false);
			ft_strcpy(config->map.board[i], trimmed);
			i++;
		}
		current = current->next;
	}
	config->map.board[i] = NULL;
	if (!find_player_position_p(config))
	{
		print_error("Map must contain exactly one player (N, S, E, or W)");
		return (false);
	}
	if (!is_map_closed(config))
	{
		print_error("Map must be closed/surrounded by walls");
		return (false);
	}
	return (true);
}
