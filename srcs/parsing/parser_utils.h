/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:26:17 by hmensah-          #+#    #+#             */
/*   Updated: 2025/07/02 19:39:20 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_UTILS_H
# define PARSER_UTILS_H
# include "../../includes/libft/libft.h"
# include "../../includes/libft/ft_list.h"
# include "../../includes/libft/arena.h"
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

// Parser functions
t_config *parse_cub_file(char *filename, t_arena *arena);
void print_error(char *message);
void print_config(t_config *config);

void	print_error(char *message);
char	*skip_whitespace(char *str);
char	*trim_newline(char *str);
bool	is_valid_map_char(char c);
bool	parse_color_p(char *line, int *r, int *g, int *b);
bool	parse_texture(char *line, char **texture_path, t_arena *arena);
bool	parse_element(char *line, t_config *config, t_arena *arena);
bool	validate_map_line(char *line);
bool	find_player_position_p(t_config *config);
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