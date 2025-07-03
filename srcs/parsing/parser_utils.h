/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/07/03 15:36:40 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_UTILS_H
# define PARSER_UTILS_H
# include "../../includes/libft/libft.h"
# include "../../includes/libft/ft_list.h"
# include "../../includes/libft/arena.h"
# include "../../includes/cub3d.h"
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <stdbool.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>

typedef struct s_textures
{
    char    *north;
    char    *south;
    char    *west;
    char    *east;
}   t_textures;

typedef struct s_colors
{
    int floor_r;
    int floor_g;
    int floor_b;
    int ceiling_r;
    int ceiling_g;
    int ceiling_b;
}   t_colors;

typedef struct s_map2
{
    char    **board;
    int     cols;
    int     rows;
    int     player_pos_x;
    int     player_pos_y;
    char    player_dir;
}   t_map2;

typedef struct s_config
{
    t_textures  textures;
    t_colors    colors;
    t_map2      map;
    bool        parsed_no;
    bool        parsed_so;
    bool        parsed_we;
    bool        parsed_ea;
    bool        parsed_f;
    bool        parsed_c;
}   t_config;

/* file_handler.c */
bool	validate_file_extension(char *filename);
bool	can_open_file(char *filename);
t_list	*read_file_to_list(char *filename);
t_list	*read_remaining_lines(int fd, t_list *current, t_list *head);
bool	verify_texture_file(char *path);

/* string_utils.c */
void	print_error(char *message);
char	*skip_whitespace(char *str);
char	*trim_newline(char *str);
char	*prepare_content(char *line, int offset);
bool	is_empty_line(char *line);

/* color_parser.c */
bool	count_rgb_parts(char **rgb_parts);
void	free_rgb_parts(char **rgb_parts);
bool	validate_rgb_values(int r, int g, int b);
bool	parse_color_values(char **rgb_parts, int *r, int *g, int *b);
bool	parse_color_p(char *line, int *r, int *g, int *b);

/* texture_parser.c */
bool	allocate_texture_path(char *path, char **texture_path, t_arena *arena);
bool	parse_texture(char *line, char **texture_path, t_arena *arena);
bool	handle_north_texture(char *content, t_config *config, t_arena *arena);
bool	handle_south_texture(char *content, t_config *config, t_arena *arena);
bool	handle_west_texture(char *content, t_config *config, t_arena *arena);

/* element_parser.c */
bool	handle_east_texture(char *content, t_config *config, t_arena *arena);
bool	handle_floor_color(char *content, t_config *config);
bool	handle_ceiling_color(char *content, t_config *config);
bool	parse_element(char *line, t_config *config, t_arena *arena);
bool	validate_required_elements(t_config *config);

/* map_validator.c */
bool	is_valid_map_char(char c);
bool	is_walkable(char c);
bool	validate_map_line(char *line);
bool	is_row_valid(char *row);
bool	check_side_column_char(char c);

/* map_validator2.c */
bool	check_side_columns(t_config *config, int i);
bool	are_side_columns_valid(t_config *config);
void	set_map_dimensions(t_config *config);
bool	is_map_closed(t_config *config);

/* position_validator.c */
char	get_cell_at_position(t_config *config, int i, int j);
bool	check_adjacent_cells(t_config *config, int i, int j);
bool	is_position_valid(t_config *config, int i, int j);
bool	are_internal_positions_valid(t_config *config);

/* player_parser.c */
bool	is_player_char(char c);
void	record_player_position(t_config *config, int i, int j);
bool	find_player_position_row(t_config *config, int i, int *count);
bool	find_player_position_p(t_config *config);
void	print_config(t_config *config);

/* map_parser.c */
int		count_map_rows_p(t_list *map_start);
bool	allocate_map_board(t_config *config, int row_count, t_arena *arena);
bool	copy_map_line(t_config *config, char *line, int index, t_arena *arena);
bool	populate_map_board(t_list *map_start, t_config *config, t_arena *arena);

/* map_parser2.c */
bool	validate_map_structure(t_config *config);
bool	parse_map(t_list *map_start, t_config *config, t_arena *arena);
t_list	*build_map_line_list(char *filename);
bool	check_direction(char *trimmed, t_list *current, t_config *config,
		t_arena *arena);

/* config_parser.c */
bool	process_config_line(t_list *current, t_config *config,
		t_arena *arena, t_list **map_start);
bool	parse_config_elements(t_list *lines, t_config *config,
		t_arena *arena, t_list **map_start);
t_config	*init_config(t_arena *arena);

/* config_adapter.c */
bool	copy_texture_paths(t_config *config, t_map *map, t_arena *arena);
void	set_map_colors(t_config *config, t_map *map);
bool	copy_map_grid(t_config *config, t_map *map, t_arena *arena);
void	set_player_info(t_config *config, t_map *map);
bool	config_to_map(t_config *config, t_map *map, t_arena *arena);

/* parser.c */
bool	validate_input_file(char *filename);
t_config	*parse_cub_file(char *filename, t_arena *arena);

/* map_init.c */
bool	init_map_using_parser(t_map *map, char *map_path, t_arena *arena);

#endif