#include "../includes/cub3d.h"
#include <fcntl.h>
#include <string.h>

// void init_player_pos(t_game *game, int i, int j)
// {
//     game->player.x = j * BLOCK_SIZE + BLOCK_SIZE / 2;
//     game->player.y = i * BLOCK_SIZE + BLOCK_SIZE / 2;
//     if (game->map.map_data[i][j] == 'N')
//         game->player.angle = 3 * PI / 2;
//     else if (game->map.map_data[i][j] == 'S')
//         game->player.angle = PI / 2;
//     else if (game->map.map_data[i][j] == 'E')
//         game->player.angle = 0;
//     else if (game->map.map_data[i][j] == 'W')
//         game->player.angle = PI;
    
// }




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
}

t_color parse_color(char *color_str)
{
    t_color color;
    char **components;
    int i;

    components = ft_split(color_str, ',');
    if (!components || !components[0] || !components[1] || !components[2])
    {
        ft_printf(" Error parsing color: %s\n", color_str);
        exit(EXIT_FAILURE);
    }

    color.red = atoi(components[0]);
    color.green = atoi(components[1]);
    color.blue = atoi(components[2]);

    // Free the components array
    i = 0;
    while (components[i])
    {
        free(components[i]);
        i++;
    }
    free(components);

    return color;
}


char *trim_whitespace(char *str)
{
    char *end;

    // Trim leading whitespace
    while (isspace((unsigned char)*str))
        str++;

    if (*str == '\0')  // If the string is empty after trimming
        return str;
    
    // Trim trailing whitespace
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    // Null-terminate the trimmed string
    end[1] = '\0';

    return str;
}

int count_map_rows(char *map_str)
{
    int fd;
    char *line;
    int count;
    char *trimmed_line;
    bool in_map_section;

    in_map_section = false;
    count = 0;
    fd = open(map_str, O_RDONLY);
    if (fd < 0)
    {
        ft_printf(" Error opening map file: %s\n", map_str);
        return -1;
    }

    line = get_next_line(fd);
    while (line)
    {
       trimmed_line = trim_whitespace(line);
       // I check if I recehd the map sectiomn
       if (!in_map_section && (trimmed_line[0] == '1' || trimmed_line[0] == '0'))
           in_map_section = true;

       // I count only non-empty lines in the map section
       if (in_map_section && strlen(trimmed_line) > 0)
            count++;
    
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
    return count;
}

int find_max_width(char *map_str)
{
    int fd;
    char *line;
    int max_width;
    char *trimmed_line;

    max_width = 0;
    fd = open(map_str, O_RDONLY);
    if (fd < 0)
    {
        ft_printf("Error opening map file: %s\n", map_str);
        return -1;
    }
    line = get_next_line(fd);
    while (line)
    {
        trimmed_line = trim_whitespace(line);
        if ((int)strlen(trimmed_line) > max_width)
            max_width = strlen(trimmed_line);
        
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
    return max_width;
}

void init_map(t_map *map, char *map_path)
{
   int fd;
   char *line;
   int map_row;
   int in_map_section;
   char *trimmed_line;
   int i;

   map_row = 0;
   in_map_section = false;

   //I initialize the map structure
    map->height = count_map_rows(map_path);
    map->width = find_max_width(map_path);
    map->grid = malloc(sizeof(char *) * (map->height + 1));
    if (!map->grid)
    {
         ft_printf(" Error allocating memory for map grid\n");
         exit(EXIT_FAILURE);
    }
    
    fd = open(map_path, O_RDONLY);
    if (fd < 0)
    {
        ft_printf(" Error opening map file: %s\n", map_path);
        exit(EXIT_FAILURE);
    }
    line = get_next_line(fd);
    while(line)
    {
        trimmed_line = trim_whitespace(line);
        if (strncmp(trimmed_line, "NO", 3) == 0)
            map->no_texture_path = strdup(trim_whitespace(trimmed_line + 3));
        else if (strncmp(trimmed_line, "SO", 3) == 0)
            map->so_texture_path = strdup(trim_whitespace(trimmed_line + 3));
        else if (strncmp(trimmed_line, "WE", 3) == 0)
            map->we_texture_path = strdup(trim_whitespace(trimmed_line + 3));
        else if (strncmp(trimmed_line, "EA", 3) == 0)
            map->ea_texture_path = strdup(trim_whitespace(trimmed_line + 3));
        else if (strncmp(trimmed_line, "F", 2) == 0)
            map->floor_color = parse_color(trim_whitespace(trimmed_line + 2));
        else if (strncmp(trimmed_line, "C", 2) == 0)
            map->ceiling_color = parse_color(trim_whitespace(trimmed_line + 2));
        else if (trimmed_line[0] == '1' || trimmed_line[0] == '0')
        {
            in_map_section = true;

            // Allocate memory for this row, pad to max width
            map->grid[map_row] = malloc(sizeof(char) * (map->width + 1));
            if (!map->grid[map_row])
            {
                ft_printf(" Error allocating memory for map row\n");
                exit(EXIT_FAILURE);
            }
            // Copy the line and pad with spaces if necessary
            i = 0;
            while(i < ((int)strlen(line) - 1) && i < map->width)
            {
                map->grid[map_row][i] = line[i];
                i++;
            }
            map->grid[map_row][map->width] = '\0'; // Null-terminate the string
            map_row++;
        }
        free(line);
        line = get_next_line(fd);
    }

    map->grid[map->height] = NULL; // Null-terminate the array
    close(fd);
}

void init_player_from_map(t_game *game)
{
    int i;
    int j;
    char c;

    i = 0;
    while(i < game->map.height)
    {
        j = 0;
        while(j < game->map.width)
        {
            c = game->map.grid[i][j];
            if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
            {
                game->player.x = j * BLOCK_SIZE + BLOCK_SIZE / 2;
                game->player.y = i * BLOCK_SIZE + BLOCK_SIZE / 2;
                if (c == 'N')
                {
                    game->player.direction = NORTH;
                    game->player.angle = 3 * M_PI_2; // Facing North
                }
                else if (c == 'S')
                {
                    game->player.direction = SOUTH;
                    game->player.angle = M_PI_2; // Facing South
                }
                else if (c == 'E')
                {
                    game->player.direction = EAST;
                    game->player.angle = 0; // Facing East
                }
                else if (c == 'W')
                {
                    game->player.direction = WEST;
                    game->player.angle = M_PI; // Facing West
                }
                game->player.dx = cos(game->player.angle) ;
                game->player.dy = sin(game->player.angle) ;
                game->player.fov = M_PI / 3; // Set the field of view angle
                game->map.grid[i][j] = '0'; // Replace player character with empty space

                // initialisation of movement flags to false
                game->player.move_up = false;
                game->player.move_down = false;
                game->player.move_left = false;
                game->player.move_right = false;
                game->player.rotate_left = false;
                game->player.rotate_right = false;

                return;
            }
        }
    }
    ft_printf(" Error: Player position not found in map\n");
    exit(EXIT_FAILURE);
}




void draw_walls(t_game *game)
{
    int x;
    t_ray *ray;
    int line_height;
    double corrected_dist;
    int draw_start;
    int draw_end;
    int color;
    int y;

    x  = 0;
    while (x < WINDOW_WIDTH)
    {
        ray = &game->rays[x];
        // to avoid division by zero
        if (ray->distance == 0)
        {
            x++;
            continue;
        }
        // Step 1: Project wall height
        corrected_dist = ray->distance * cos(ray->angle - game->player.angle);
        line_height = (int)((BLOCK_SIZE * WINDOW_HEIGHT) / corrected_dist);
        // Step 2: Determine top/bottom Y coords
        draw_start = -line_height / 2 + WINDOW_HEIGHT / 2;
        if (draw_start < 0)
            draw_start = 0;

        draw_end = line_height / 2 + WINDOW_HEIGHT / 2;
        if (draw_end >= WINDOW_HEIGHT)
            draw_end = WINDOW_HEIGHT - 1;
        // Step 3: Choose wall color (basic shading)
        if (ray->wall_side == 1) // EW wall
            color = COLOR_RED;
        else // NS wall
            color = COLOR_GREEN;
        // Step 4: Draw vertical line
        y = draw_start;
        while (y < draw_end)
        {
            put_pixel(&game->win_img, x, y, color);
            y++;
        }
        x++;

    }
}

int main(int ac, char **av)
{
    t_game game;

    if (ac != 2)
    {
        ft_printf(" Error Usage: %s <map_file>\n", av[0]);
        return EXIT_FAILURE;
    }

    init_map(&game.map, av[1]);
    
    game.mlx_ptr = mlx_init();
    game.win_ptr = mlx_new_window(game.mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT, "Cub3D");

    if (init_images_and_textures(&game) != 0)
        return EXIT_FAILURE;

    init_player_from_map(&game);
    init_colors(&game);
    cast_rays(&game);






    // Load map and textures here
    // load_map(av[1], &game.map);
    // load_textures(&game);

    mlx_loop(game.mlx_ptr);
    
    return EXIT_SUCCESS;
}