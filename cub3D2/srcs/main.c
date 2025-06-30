#include "../includes/cub3d.h"
#include <fcntl.h>
#include <string.h>



void move_up(t_game *game)
{
    int x;
    int y;

    x  = (int)(game->player.x + game->player.dx )/BLOCK_SIZE;
    y  = (int)(game->player.y + game->player.dy )/BLOCK_SIZE;
    if (x >= 0 && x < game->map.width && y >= 0 && y < game->map.height && (game->map.grid[y][x] == '0')) // Check for wall
    {
        game->player.x += game->player.dx * MOVE_SPEED;
        game->player.y += game->player.dy * MOVE_SPEED;
        cast_rays(game); // Recast rays after moving
        update_window(game);
    }
  
}

void move_down(t_game *game)
{
    int x;
    int y;

    x  = (int)(game->player.x - game->player.dx)/BLOCK_SIZE;
    y  = (int)(game->player.y - game->player.dy)/BLOCK_SIZE;

    // if (x >= 0 && x < game->map.width && y >= 0 && y < game->map.height && (game->map.grid[y][x] = '0')) // Check for wall
    if (x >= 0 && x < game->map.width && y >= 0 && y < game->map.height && (game->map.grid[y][x] == '0'))
    {
        game->player.x -= game->player.dx * MOVE_SPEED;
        game->player.y -= game->player.dy * MOVE_SPEED;
        cast_rays(game); // Recast rays after moving
        update_window(game);
    }
}


void move_left(t_game *game)
{
    int x;
    int y;
    double sin_angle;
    double cos_angle;

    sin_angle = sin(game->player.angle + M_PI_2) * MOVE_SPEED; // Perpendicular movement
    cos_angle = cos(game->player.angle + M_PI_2) * MOVE_SPEED; // Perpendicular movement
    x  = (int)(game->player.x - cos_angle)/BLOCK_SIZE;
    y  = (int)(game->player.y - sin_angle)/BLOCK_SIZE;
    // if (x >= 0 && x < game->map.width && y >= 0 && y < game->map.height && (game->map.grid[y][x] = '0')) // Check for wall
    if (x >= 0 && x < game->map.width && y >= 0 && y < game->map.height && (game->map.grid[y][x] == '0'))
    {
        game->player.x -= cos_angle;
        game->player.y -= sin_angle;
        cast_rays(game); // Recast rays after moving
        update_window(game);
    }
}

void move_right(t_game *game)
{
    int x;
    int y;
    double sin_angle;
    double cos_angle;

    sin_angle = sin(game->player.angle + M_PI_2) * MOVE_SPEED; // Perpendicular movement
    cos_angle = cos(game->player.angle + M_PI_2) * MOVE_SPEED; // Perpendicular movement
    x  = (int)(game->player.x + cos_angle)/BLOCK_SIZE;
    y  = (int)(game->player.y + sin_angle)/BLOCK_SIZE;
     // Check for wall
    if (x >= 0 && x < game->map.width && y >= 0 && y < game->map.height && (game->map.grid[y][x] == '0'))
    {
        game->player.x += cos_angle;
        game->player.y += sin_angle;
        cast_rays(game); // Recast rays after moving
        update_window(game);
    }
}

void rotate_left(t_game *game)
{
    game->player.angle -= ROTATE_SPEED; // Rotate left
    if (game->player.angle < 0)
        game->player.angle += 2 * M_PI; // Wrap around
    game->player.dx = cos(game->player.angle);
    game->player.dy = sin(game->player.angle);
    cast_rays(game); // Recast rays after rotation
    update_window(game);
}

void rotate_right(t_game *game)
{
    game->player.angle += ROTATE_SPEED; // Rotate right
    if (game->player.angle >= 2 * M_PI)
        game->player.angle -= 2 * M_PI; // Wrap around
    game->player.dx = cos(game->player.angle);
    game->player.dy = sin(game->player.angle);
    cast_rays(game); // Recast rays after rotation
    update_window(game);
}

int key_hook(int key_code, t_game *game)
{
    if (key_code == KEY_ESC)
    {
        // free_and_clean(game);
         exit(0);
    }
    else if (key_code == KEY_W)
        move_up(game);
    else if (key_code == KEY_S)
        move_down(game);
    else if (key_code == KEY_A)
        move_left(game);
    else if (key_code == KEY_D)
        move_right(game);
    else if (key_code == KEY_LEFT)
        rotate_left(game);
    else if (key_code == KEY_RIGHT)
        rotate_right(game);
    return 0;
}

//code of the key release hook
int key_release_hook(int key_code, t_game *game)
{
    if (key_code == KEY_W)
        game->player.move_up = false;
    else if (key_code == KEY_S)
        game->player.move_down = false;
    else if (key_code == KEY_A)
        game->player.move_left = false;
    else if (key_code == KEY_D)
        game->player.move_right = false;
    else if (key_code == KEY_LEFT)
        game->player.rotate_left = false;
    else if (key_code == KEY_RIGHT)
        game->player.rotate_right = false;

    return 0;
}

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
    printf("Debug: Parsed color - R: %d, G: %d, B: %d\n", color.red, color.green, color.blue);
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
    // printf("Debug: Map initialized with %d rows and %d columns\n", map->height, map->width);
    // printf("Debug: Map textures: NO=%s, SO=%s, WE=%s, EA=%s\n", 
    //        map->no_texture_path, map->so_texture_path, map->we_texture_path, map->ea_texture_path);
    // printf("Debug: Map colors: Floor=(%d, %d, %d), Ceiling=(%d, %d, %d)\n",
    //           map->floor_color.red, map->floor_color.green, map->floor_color.blue,
    //           map->ceiling_color.red, map->ceiling_color.green, map->ceiling_color.blue);
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
                printf("Player at: (%.2f, %.2f), grid pos: (%d, %d)\n", 
                    game->player.x, game->player.y, (int)(game->player.x/BLOCK_SIZE), (int)(game->player.y/BLOCK_SIZE));
                // initialisation of movement flags to false
                game->player.move_up = false;
                game->player.move_down = false;
                game->player.move_left = false;
                game->player.move_right = false;
                game->player.rotate_left = false;
                game->player.rotate_right = false;

                printf("Debug: Player initialized at position (%.2f, %.2f) facing angle %.2f radians\n",
                       game->player.x, game->player.y, game->player.angle);
                return;
            }
            j++;
        }
        i++;
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


int mlx_pixel_put_img(t_game *game, int x, int y, int color)
{
    char *dst;
    
    if (x < 0 || x >= game->win_img.width || y < 0 || y >= game->win_img.height)
    {
        return -1; // Out of bounds
    }
    
    dst = game->win_img.addr + (y * game->win_img.line_length + x * game->win_img.bpp);
    *(unsigned int*)dst = color;
    return 0;
}
void clear_image(t_game *game)
{
    int x, y;
    
    y = 0;
    while (y < game->win_img.height)
    {
        x = 0;
        while (x < game->win_img.width)
        {
            put_pixel(&game->win_img, x, y, COLOR_BLACK);
            x++;
        }
        y++;
    }
}

int update_window(t_game *game)
{
    clear_image(game);
    render_ground_and_ceiling(game);
    // draw_walls(game);
    cast_rays(game);
    mlx_put_image_to_window(game->mlx_ptr, game->win_ptr, game->win_img.img_ptr, 0, 0);
    return 0;
}


// void clear_image(t_game *game)
// {
//     int x;
//     int y;

//     // Clear the image by filling it with the background color
//     y = 0;
//     while (y < game->win_img.height)
//     {
//         x = 0;
//         while (x < game->win_img.width)
//         {
//             mlx_pixel_put_img(game, x, y, COLOR_BLACK); // Assuming COLOR_BLACK is defined as the background color
//             x++;
//         }
//         y++;
//     }
// }

int game_loop(t_game *game) {
    if (game->player.move_up)
        move_up(game);
    if (game->player.move_down)
        move_down(game);
    if (game->player.move_left)
        move_left(game);
    if (game->player.move_right)
        move_right(game);
    if (game->player.rotate_left)
        rotate_left(game);
    if (game->player.rotate_right)
        rotate_right(game);
    return 0;
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
    if (!game.mlx_ptr)
    {
        ft_printf(" Error initializing MLX\n");
        return EXIT_FAILURE;
    }
    game.win_ptr = mlx_new_window(game.mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT, "Cub3D");
    if (!game.win_ptr)
    {
        ft_printf(" Error creating MLX window\n");
        return EXIT_FAILURE;
    }

    if (init_images_and_textures(&game) != 0)
        return EXIT_FAILURE;
    
    init_player_from_map(&game);
    printf("Debug : Init player done \n");
    init_colors(&game);
    printf("Debug : Init colors done \n");
    cast_rays(&game);
    update_window(&game);
    // mlx_hook(game.win_ptr, X_EVENT_KEY_PRESS, 0, key_hook, &game);
    mlx_loop_hook(game.mlx_ptr, game_loop, &game);
    mlx_hook(game.win_ptr, X_EVENT_KEY_RELEASE, 0, key_release_hook, &game);
    printf("Debug : Key hook set \n");
    // mlx_loop_hook(game.mlx_ptr, update_window, &game);

    mlx_loop(game.mlx_ptr);
    
    return EXIT_SUCCESS;
}