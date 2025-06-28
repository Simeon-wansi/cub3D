/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:26:17 by hmensah-          #+#    #+#             */
/*   Updated: 2025/06/28 18:23:28 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_UTILS_H
# define PARSER_UTILS_H

# include "../includes/cub3d.h"

void	print_error(char *message);
char	*skip_whitespace(char *str);
char	*trim_newline(char *str);
bool	is_valid_map_char(char c);
bool	parse_color(char *line, int *r, int *g, int *b);
bool	parse_texture(char *line, char **texture_path, t_arena *arena);
bool	parse_element(char *line, t_config *config, t_arena *arena);
bool	validate_map_line(char *line);
bool	find_player_position(t_config *config);
void	set_map_dimensions(t_config *config);
bool	is_row_valid(char *row);
bool	are_side_columns_valid(t_config *config);
bool	is_valid_map_char(char c);
bool	is_walkable(char c);
bool	is_position_valid(t_config *config, int i, int j);
void	print_config(t_config *config);
bool	are_internal_positions_valid(t_config *config);
bool	is_map_closed(t_config *config);
bool	parse_map(t_list *map_start, t_config *config, t_arena *arena);
t_list	*build_map_line_list(char *filename);
bool	parse_config_elements(t_list *lines, t_config *config,
			t_arena *arena, t_list **map_start);
char	*prepare_content(char *line, int offset);

#endif