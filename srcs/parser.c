/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:07:27 by hmensah-          #+#    #+#             */
/*   Updated: 2025/06/28 00:00:00 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void print_error(char *message)
{
    ft_putstr_fd("Error\n", 2);
    ft_putstr_fd(message, 2);
    ft_putchar_fd('\n', 2);
}

char *skip_whitespace(char *str)
{
    while (*str && (*str == ' ' || *str == '\t'))
        str++;
    return (str);
}

char *trim_newline(char *str)
{
    int len;
    
    if (!str)
        return (str);
    len = ft_strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
    return (str);
}

bool is_valid_map_char(char c)
{
    return (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == ' ');
}

bool parse_color(char *line, int *r, int *g, int *b)
{
    char **rgb_parts;
    int i;
    
    rgb_parts = ft_split(line, ',');
    if (!rgb_parts)
        return (false);
    i = 0;
    while (rgb_parts[i])
        i++;
    if (i != 3)
    {
        i = 0;
        while (rgb_parts[i])
            free(rgb_parts[i++]);
        free(rgb_parts);
        return (false);
    }
    *r = ft_atoi(rgb_parts[0]);
    *g = ft_atoi(rgb_parts[1]);
    *b = ft_atoi(rgb_parts[2]);
    i = 0;
    while (rgb_parts[i])
        free(rgb_parts[i++]);
    free(rgb_parts);
    
    return (*r >= 0 && *r <= 255 && *g >= 0 && *g <= 255 && *b >= 0 && *b <= 255);
}

bool parse_texture(char *line, char **texture_path, t_arena *arena)
{
    char *path;
    int fd;
    
    path = skip_whitespace(line);
    if (!*path)
        return (false);
    fd = open(path, O_RDONLY);
    if (fd < 0)
        return (false);
    close(fd);
    *texture_path = arena_alloc(arena, ft_strlen(path) + 1);
    if (!*texture_path)
        return (false);
    ft_strcpy(*texture_path, path);
    return (true);
}

bool parse_element(char *line, t_config *config, t_arena *arena)
{
    char *trimmed;
    char *content;
    
    trimmed = skip_whitespace(line);
    if (!*trimmed || *trimmed == '\n')
        return (true);
    if (!ft_strncmp(trimmed, "NO ", 3))
    {
        if (config->parsed_no)
            return (false);
        content = skip_whitespace(trimmed + 3);
        trim_newline(content);
        config->parsed_no = parse_texture(content, &config->textures.north, arena);
        return (config->parsed_no);
    }
    else if (!ft_strncmp(trimmed, "SO ", 3))
    {
        if (config->parsed_so)
            return (false);
        content = skip_whitespace(trimmed + 3);
        trim_newline(content);
        config->parsed_so = parse_texture(content, &config->textures.south, arena);
        return (config->parsed_so);
    }
    else if (!ft_strncmp(trimmed, "WE ", 3))
    {
        if (config->parsed_we)
            return (false);
        content = skip_whitespace(trimmed + 3);
        trim_newline(content);
        config->parsed_we = parse_texture(content, &config->textures.west, arena);
        return (config->parsed_we);
    }
    else if (!ft_strncmp(trimmed, "EA ", 3))
    {
        if (config->parsed_ea)
            return (false);
        content = skip_whitespace(trimmed + 3);
        trim_newline(content);
        config->parsed_ea = parse_texture(content, &config->textures.east, arena);
        return (config->parsed_ea);
    }
    else if (!ft_strncmp(trimmed, "F ", 2))
    {
        if (config->parsed_f)
            return (false);
        content = skip_whitespace(trimmed + 2);
        trim_newline(content);
        config->parsed_f = parse_color(content, &config->colors.floor_r, 
                                     &config->colors.floor_g, &config->colors.floor_b);
        return (config->parsed_f);
    }
    else if (!ft_strncmp(trimmed, "C ", 2))
    {
        if (config->parsed_c)
            return (false);
        content = skip_whitespace(trimmed + 2);
        trim_newline(content);
        config->parsed_c = parse_color(content, &config->colors.ceiling_r, 
                                     &config->colors.ceiling_g, &config->colors.ceiling_b);
        return (config->parsed_c);
    }
    return (false);
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

bool find_player_position(t_config *config)
{
    int i, j;
    int player_count = 0;
    
    i = 0;
    while (i < config->map.rows)
    {
        j = 0;
        while (config->map.board[i][j])
        {
            if (config->map.board[i][j] == 'N' || config->map.board[i][j] == 'S' ||
                config->map.board[i][j] == 'E' || config->map.board[i][j] == 'W')
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
void set_map_dimensions(t_config *config)
{
    int i;
    int max_width;
    
    max_width = 0;
    i = 0;
    while (i < config->map.rows)
    {
        int len = ft_strlen(config->map.board[i]);
        if (len > max_width)
            max_width = len;
        i++;
    }
    config->map.cols = max_width;
}

// Helper function to check if a row contains only walls and spaces
bool is_row_valid(char *row)
{
    int j;
    
    j = 0;
    while (j < (int)ft_strlen(row))
    {
        char c = row[j];
        if (c != '1' && c != ' ')
            return (false);
        j++;
    }
    return (true);
}

// Helper function to check if side columns are valid
bool are_side_columns_valid(t_config *config)
{
    int i;
    
    i = 0;
    while (i < config->map.rows)
    {
        int len = ft_strlen(config->map.board[i]);
        if (len > 0)
        {
            char first = config->map.board[i][0];
            char last = config->map.board[i][len - 1];
            
            if (first != '1' && first != ' ')
                return (false);
            if (last != '1' && last != ' ')
                return (false);
        }
        i++;
    }
    return (true);
}

// Helper function to check if a character is valid in the map
bool is_valid_map_char(char c)
{
    return (c == '1' || c == ' ' || c == '0' || c == 'N' || 
            c == 'S' || c == 'E' || c == 'W');
}

// Helper function to check if a position is walkable
bool is_walkable(char c)
{
    return (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

// Helper function to check adjacency validity for a single position
bool is_position_valid(t_config *config, int i, int j)
{
    char current;
    char up;
    char down;
    char left;
    char right;

    current = config->map.board[i][j];
    if (!is_walkable(current))
        return (true);
    up = (j < (int)ft_strlen(config->map.board[i-1])) ? 
             config->map.board[i-1][j] : ' ';
    down = (j < (int)ft_strlen(config->map.board[i+1])) ? 
               config->map.board[i+1][j] : ' ';
    left = config->map.board[i][j-1];
    right = config->map.board[i][j+1];
    if (!is_valid_map_char(up) || !is_valid_map_char(down) ||
        !is_valid_map_char(left) || !is_valid_map_char(right))
    {
        return (false);
    }
    return (true);
}

// Helper function to check internal positions for map closure
bool are_internal_positions_valid(t_config *config)
{
    int i;
    int j;
    
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

bool is_map_closed(t_config *config)
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

t_list *build_map_line_list(char *filename)
{
    t_list *head;
    t_list *current;
    int fd;
    char *line;

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

bool parse_config_elements(t_list *lines, t_config *config, t_arena *arena, t_list **map_start)
{
    t_list *current = lines;
    
    while (current)
    {
        char *trimmed = skip_whitespace((char *)current->content);
        trim_newline(trimmed);
        
        if (!*trimmed)
        {
            current = current->next;
            continue;
        }
        if (!ft_strncmp(trimmed, "NO ", 3) || !ft_strncmp(trimmed, "SO ", 3) ||
            !ft_strncmp(trimmed, "WE ", 3) || !ft_strncmp(trimmed, "EA ", 3) ||
            !ft_strncmp(trimmed, "F ", 2) || !ft_strncmp(trimmed, "C ", 2))
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
        {
            print_error("Invalid line in configuration");
            return (false);
        }
        
        current = current->next;
    }
    if (!config->parsed_no || !config->parsed_so || !config->parsed_we || 
        !config->parsed_ea || !config->parsed_f || !config->parsed_c)
    {
        print_error("Missing required configuration elements");
        return (false);
    }
    
    if (!*map_start)
    {
        print_error("No map found in file");
        return (false);
    }
    
    return (true);
}

bool parse_map(t_list *map_start, t_config *config, t_arena *arena)
{
    t_list *current = map_start;
    int row_count = 0;
    int i = 0;
    
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
    if (!find_player_position(config))
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

t_config *parse_cub_file(char *filename, t_arena *arena)
{
    t_list *lines;
    t_list *map_start = NULL;
    t_config *config;

    if (ft_strlen(filename) < 4 || ft_strcmp(filename + ft_strlen(filename) - 4, ".cub") != 0)
    {
        print_error("File must have .cub extension");
        return (NULL);
    }
    lines = build_map_line_list(filename);
    if (!lines)
    {
        print_error("Cannot read file or file is empty");
        return (NULL);
    }
    config = arena_alloc(arena, sizeof(t_config));
    if (!config)
    {
        ft_lstclear(&lines, free);
        return (NULL);
    }
    ft_memset(config, 0, sizeof(t_config));
    if (!parse_config_elements(lines, config, arena, &map_start))
    {
        ft_lstclear(&lines, free);
        return (NULL);
    }
    if (!parse_map(map_start, config, arena))
    {
        ft_lstclear(&lines, free);
        return (NULL);
    }
    ft_lstclear(&lines, free);
    return (config);
}

void print_config(t_config *config)
{
    int i;
    
    if (!config)
        return;
        
    printf("=== Configuration ===\n");
    printf("North texture: %s\n", config->textures.north);
    printf("South texture: %s\n", config->textures.south);
    printf("West texture: %s\n", config->textures.west);
    printf("East texture: %s\n", config->textures.east);
    printf("Floor color: %d,%d,%d\n", config->colors.floor_r, config->colors.floor_g, config->colors.floor_b);
    printf("Ceiling color: %d,%d,%d\n", config->colors.ceiling_r, config->colors.ceiling_g, config->colors.ceiling_b);
    printf("Player position: (%d,%d) facing %c\n", config->map.player_pos_x, config->map.player_pos_y, config->map.player_dir);
    printf("Map dimensions: %dx%d\n", config->map.cols, config->map.rows);
    printf("=== Map ===\n");
    
    i = 0;
    while (i < config->map.rows && config->map.board[i])
    {
        printf("%s\n", config->map.board[i]);
        i++;
    }
}

int main(int argc, char **argv)
{
    t_arena *arena;
    t_config *config;

    if (argc != 2 || !argv[1])
    {
        print_error("Usage: ./cub3D <map_file.cub>");
        return (1);
    }
    arena = arena_create(1024 * 1024);
    if (!arena)
    {
        print_error("Failed to create memory arena");
        return (1);
    }
    config = parse_cub_file(argv[1], arena);
    if (!config)
    {
        arena_destroy(arena);
        return (1);
    }
    print_config(config);
    arena_destroy(arena);
    return (0);
}
