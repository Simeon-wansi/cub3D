/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:07:27 by hmensah-          #+#    #+#             */
/*   Updated: 2025/07/02 19:32:31 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_utils.h"

bool	check_direction(char *trimmed, t_list *current, t_config *config,
		t_arena *arena)
{
	if (!ft_strncmp(trimmed, "NO ", 3) || !ft_strncmp(trimmed, "SO ", 3)
			|| !ft_strncmp(trimmed, "WE ", 3) || !ft_strncmp(trimmed, "EA ", 3)
			|| !ft_strncmp(trimmed, "F ", 2) || !ft_strncmp(trimmed, "C ", 2))
	{
		if (!parse_element((char *)current->content, config, arena))
			return (false);
	}
	return (true);
}

bool parse_config_elements(t_list *lines, t_config *config,
		t_arena *arena, t_list **map_start)
{
	t_list	*current;
	char	*trimmed;
	
	current = lines;
	while (current)
	{
		trimmed = skip_whitespace((char *)current->content);
		trim_newline(trimmed);
		if (!*trimmed)
		{
			current = current->next;
			continue;
		}
		if (!ft_strncmp(trimmed, "NO ", 3) || !ft_strncmp(trimmed, "SO ", 3)
			|| !ft_strncmp(trimmed, "WE ", 3) || !ft_strncmp(trimmed, "EA ", 3)
			|| !ft_strncmp(trimmed, "F ", 2) || !ft_strncmp(trimmed, "C ", 2))
		{
			if (!parse_element((char *)current->content, config, arena))
				return (false);
		}
		else if (validate_map_line(trimmed))
		{
			*map_start = current;
			break;
		}
		else
			return (print_error("Invalid line in configuration"), false);
		current = current->next;
	}
	if (!config->parsed_no || !config->parsed_so || !config->parsed_we
		|| !config->parsed_ea || !config->parsed_f || !config->parsed_c)
		return (print_error("Missing required configuration elements"), false);	
	if (!*map_start)
		return (print_error("No map found in file"), false);	
	return (true);
}

t_config *parse_cub_file(char *filename, t_arena *arena)
{
	t_list *lines;
	t_list *map_start;
	t_config *config;

	map_start = NULL;
	if (ft_strlen(filename) < 4
		|| ft_strcmp(filename + ft_strlen(filename) - 4, ".cub") != 0)
		return (print_error("File must have .cub extension"), NULL);
	lines = build_map_line_list(filename);
	if (!lines)
		return (print_error("Cannot read file or file is empty"), NULL);
	config = arena_alloc(arena, sizeof(t_config));
	if (!config)
		return (ft_lstclear(&lines, free), NULL);
	ft_memset(config, 0, sizeof(t_config));
	if (!parse_config_elements(lines, config, arena, &map_start))
		return (ft_lstclear(&lines, free), NULL);
	if (!parse_map(map_start, config, arena))
		return (ft_lstclear(&lines, free), NULL);
	ft_lstclear(&lines, free);
	return (config);
}

// int main(int argc, char **argv)
// {
// 	t_arena *arena;
// 	t_config *config;

// 	if (argc != 2 || !argv[1])
// 	{
// 		print_error("Usage: ./cub3D <map_file.cub>");
// 		return (1);
// 	}
// 	arena = arena_create(1024 * 1024);
// 	if (!arena)
// 	{
// 		print_error("Failed to create memory arena");
// 		return (1);
// 	}
// 	config = parse_cub_file(argv[1], arena);
// 	if (!config)
// 	{
// 		arena_destroy(arena);
// 		return (1);
// 	}
// 	print_config(config);
// 	arena_destroy(arena);
// 	return (0);
// }
