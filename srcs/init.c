#include "../includes/cub3d.h"

void init_colors(t_game *game)
{
    int red;
    int green;
    int blue;

    red = game->map.floor_color.red;
    green = game->map.floor_color.green;
    blue = game->map.floor_color.blue;
    game->floor_color = (red << 16) | (green << 8) | blue;
    red = game->map.ceiling_color.red;
    green = game->map.ceiling_color.green;
    blue = game->map.ceiling_color.blue;
    game->ceiling_color = (red << 16) | (green << 8) | blue;
    printf("Debug: Floor color initialized to R: %d, G: %d, B: %d\n", game->map.floor_color.red, game->map.floor_color.green, game->map.floor_color.blue);
}

bool find_player_position(t_map *map)
{
    int i;
    int j;
    int player_count;

    player_count = 0;
    i = 0;

    while(i < map->height)
    {
        j = 0;
        while(j < map->width)
        {
            if (map->grid[i][j] == 'N' || map->grid[i][j] == 'S' || 
                map->grid[i][j] == 'E' || map->grid[i][j] == 'W')
            {
                if (player_count > 0)
                {
                    ft_printf("Error: Multiple player positions found in map\n");
                    return false;
                }
                map->player_dir = map->grid[i][j];
                map->player_x = j;
                map->player_y = i;
                player_count++;
            }
            j++;
        }
        i++;
    }

    return (player_count == 1);
    
}

void init_map(t_map *map, char *map_path)
{
    int fd;
    char *line;
    char *trimmed_line;
    int map_row = 0;
    int in_map_section = false;
    int i;

    // Initialize map dimensions and allocate grid
    map->height = count_map_rows(map_path);
    map->width = find_max_width(map_path);
    map->grid = malloc(sizeof(char *) * (map->height + 1)); // +1 for NULL termination
    if (!map->grid)
    {
        ft_printf("Error allocating memory for map grid\n");
        exit(EXIT_FAILURE);
    }

    // Open map file
    fd = open(map_path, O_RDONLY);
    if (fd < 0)
    {
        ft_printf("Error opening map file: %s\n", map_path);
        exit(EXIT_FAILURE);
    }

    line = get_next_line(fd);
    while (line)
    {
        trimmed_line = trim_whitespace(line);

        // Texture paths and colors
        if (strncmp(trimmed_line, "NO", 2) == 0 && isspace(trimmed_line[2]))
            map->no_texture_path = strdup(trim_whitespace(trimmed_line + 2));
        else if (strncmp(trimmed_line, "SO", 2) == 0 && isspace(trimmed_line[2]))
            map->so_texture_path = strdup(trim_whitespace(trimmed_line + 2));
        else if (strncmp(trimmed_line, "WE", 2) == 0 && isspace(trimmed_line[2]))
            map->we_texture_path = strdup(trim_whitespace(trimmed_line + 2));
        else if (strncmp(trimmed_line, "EA", 2) == 0 && isspace(trimmed_line[2]))
            map->ea_texture_path = strdup(trim_whitespace(trimmed_line + 2));
        else if (strncmp(trimmed_line, "F", 1) == 0 && isspace(trimmed_line[1]))
            map->floor_color = parse_color(trim_whitespace(trimmed_line + 1));
        else if (strncmp(trimmed_line, "C", 1) == 0 && isspace(trimmed_line[1]))
            map->ceiling_color = parse_color(trim_whitespace(trimmed_line + 1));

        // Map grid lines
        else if (trimmed_line[0] == '1' || trimmed_line[0] == '0')
        {
            in_map_section = true;
            map->grid[map_row] = malloc(sizeof(char) * (map->width + 1));
            if (!map->grid[map_row])
            {
                ft_printf("Error allocating memory for map row\n");
                exit(EXIT_FAILURE);
            }

            i = 0;
            while (i < (int)strlen(trimmed_line) && i < map->width)
            {
                map->grid[map_row][i] = trimmed_line[i];
                i++;
            }
            while (i < map->width)
            {
                map->grid[map_row][i] = ' ';
                i++;
            }
            map->grid[map_row][i] = '\0';
            map_row++;
        }

        // free(trimmed_line);
        free(line);
        line = get_next_line(fd);
    }

    map->grid[map_row] = NULL; // Null-terminate the grid
    // debug verification of map value
    if (!find_player_position(map))
    {
        ft_printf("Error: Player position not found in map\n");
        exit(EXIT_FAILURE);
    }
    printf("Debug: Map loaded with %d rows and %d columns\n", map->height, map->width);
    printf("debug: texture paths:\n");
    printf("NO: %s\n", map->no_texture_path ? map->no_texture_path : "None");
    printf("SO: %s\n", map->so_texture_path ? map->so_texture_path : "None");
    printf("WE: %s\n", map->we_texture_path ? map->we_texture_path : "None");   
    close(fd);
}


void load_texture(t_game *game)
{
    int i;
    char *texture_paths[4] = {
        game->map.we_texture_path,
        game->map.ea_texture_path,
        game->map.no_texture_path,
        game->map.so_texture_path
    };


    i = 0;
    while(i < 4)
    {
        game->textures[i].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr, texture_paths[i],
                                                            &game->textures[i].width, &game->textures[i].height);
        if (!game->textures[i].img_ptr)
        {
            ft_printf("Error loading texture from path: %s\n", texture_paths[i]);
            exit(EXIT_FAILURE);
        }
        game->textures[i].addr = mlx_get_data_addr(game->textures[i].img_ptr, 
                                                    &game->textures[i].bpp, 
                                                    &game->textures[i].line_length, 
                                                    &game->textures[i].endian);
        if (!game->textures[i].addr)
        {
            ft_printf("Error getting data address for texture: %s\n", texture_paths[i]);
            exit(EXIT_FAILURE);
        }
        printf("Debug: Texture %d loaded successfully from %s\n", i, texture_paths[i]);
        i++;
    }

}