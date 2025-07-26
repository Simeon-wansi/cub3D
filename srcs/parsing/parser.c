/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/07/26 22:04:36 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_utils.h"

bool	validate_input_file(char *filename)
{
	if (!validate_file_extension(filename))
	{
		print_error("File must have .cub extension");
		return (false);
	}
	return (true);
}

t_config	*parse_cub_file(char *filename, t_arena *arena)
{
	t_list		*lines;
	t_list		*map_start;
	t_config	*config;

	map_start = NULL;
	if (!validate_input_file(filename))
		return (NULL);
	lines = build_map_line_list(filename);
	if (!lines)
		return (print_error("Cannot read file or file is empty"), NULL);
	config = init_config(arena);
	if (!config)
	{
		ft_lstclear(&lines, free);
		return (NULL);
	}
	if (!parse_config_elements(lines, config, arena, &map_start))
		return (ft_lstclear(&lines, free), NULL);
	if (!parse_map(map_start, config, arena))
	{
		ft_lstclear(&lines, free);
		return (NULL);
	}
	ft_lstclear(&lines, free);
	return (config);
}
