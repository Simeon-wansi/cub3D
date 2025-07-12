// #include "../includes/cub3d.h"

// t_color parse_color(char *color_str)
// {
//     t_color color;
//     char **components;
//     int i;

//     components = ft_split(color_str, ',');
//     if (!components || !components[0] || !components[1] || !components[2])
//     {
//         ft_printf(" Error parsing color: %s\n", color_str);
//         exit(EXIT_FAILURE);
//     }

//     color.red = atoi(components[0]);
//     color.green = atoi(components[1]);
//     color.blue = atoi(components[2]);

//     i = 0;
//     while (components[i])
//     {
//         free(components[i]);
//         i++;
//     }
//     free(components);
//     printf("Debug: Parsed color - R: %d, G: %d, B: %d\n", color.red, color.green, color.blue);
//     return color;
// }
// int count_map_rows(char *map_str)
// {
//     int fd;
//     char *line;
//     int count;
//     char *trimmed_line;
//     bool in_map_section;

//     in_map_section = false;
//     count = 0;
//     fd = open(map_str, O_RDONLY);
//     if (fd < 0)
//     {
//         ft_printf(" Error opening map file: %s\n", map_str);
//         return -1;
//     }

//     line = get_next_line(fd);
//     while (line)
//     {
//        trimmed_line = trim_whitespace(line);
//        if (!in_map_section && (trimmed_line[0] == '1' || trimmed_line[0] == '0'))
//            in_map_section = true;

//        if (in_map_section && strlen(trimmed_line) > 0)
//             count++;
    
//         free(line);
//         line = get_next_line(fd);
//     }
//     close(fd);
//     return count;
// }

// char *trim_whitespace(char *str)
// {
//     char *end;

//     while (isspace((unsigned char)*str))
//         str++;

//     if (*str == '\0') 
//         return str;
    
//     end = str + strlen(str) - 1;
//     while (end > str && isspace((unsigned char)*end))
//         end--;

//     end[1] = '\0';
//     return str;
// }

// int find_max_width(char *map_str)
// {
//     int fd;
//     char *line;
//     int max_width;
//     char *trimmed_line;

//     max_width = 0;
//     fd = open(map_str, O_RDONLY);
//     if (fd < 0)
//     {
//         ft_printf("Error opening map file: %s\n", map_str);
//         return -1;
//     }
//     line = get_next_line(fd);
//     while (line)
//     {
//         trimmed_line = trim_whitespace(line);
//         if ((int)strlen(trimmed_line) > max_width)
//             max_width = strlen(trimmed_line);
        
//         free(line);
//         line = get_next_line(fd);
//     }
//     close(fd);
//     return max_width;
// }


// bool find_player_position(t_map *map)
// {
//     int i;
//     int j;
//     int player_count;

//     player_count = 0;
//     i = 0;

//     while(i < map->height)
//     {
//         j = 0;
//         while(j < map->width)
//         {
//             if (map->grid[i][j] == 'N' || map->grid[i][j] == 'S' || 
//                 map->grid[i][j] == 'E' || map->grid[i][j] == 'W')
//             {
//                 if (player_count > 0)
//                 {
//                     ft_printf("Error: Multiple player positions found in map\n");
//                     return false;
//                 }
//                 map->player_dir = map->grid[i][j];
//                 map->player_x = j;
//                 map->player_y = i;
//                 player_count++;
//             }
//             j++;
//         }
//         i++;
//     }

//     return (player_count == 1);
    
// }

// void init_map(t_map *map, char *map_path)
// {
//     int fd;
//     char *line;
//     char *trimmed_line;
//     int map_row = 0;
//     int in_map_section = false;
//     int i;

//     // Initialize map dimensions and allocate grid
//     map->height = count_map_rows(map_path);
//     map->width = find_max_width(map_path);
//     map->grid = malloc(sizeof(char *) * (map->height + 1)); // +1 for NULL termination
//     if (!map->grid)
//     {
//         ft_printf("Error allocating memory for map grid\n");
//         exit(EXIT_FAILURE);
//     }

//     // Open map file
//     fd = open(map_path, O_RDONLY);
//     if (fd < 0)
//     {
//         ft_printf("Error opening map file: %s\n", map_path);
//         exit(EXIT_FAILURE);
//     }

//     line = get_next_line(fd);
//     while (line)
//     {
//         trimmed_line = trim_whitespace(line);

//         // Texture paths and colors
//         if (strncmp(trimmed_line, "NO", 2) == 0 && isspace(trimmed_line[2]))
//             map->no_texture_path = strdup(trim_whitespace(trimmed_line + 2));
//         else if (strncmp(trimmed_line, "SO", 2) == 0 && isspace(trimmed_line[2]))
//             map->so_texture_path = strdup(trim_whitespace(trimmed_line + 2));
//         else if (strncmp(trimmed_line, "WE", 2) == 0 && isspace(trimmed_line[2]))
//             map->we_texture_path = strdup(trim_whitespace(trimmed_line + 2));
//         else if (strncmp(trimmed_line, "EA", 2) == 0 && isspace(trimmed_line[2]))
//             map->ea_texture_path = strdup(trim_whitespace(trimmed_line + 2));
//         else if (strncmp(trimmed_line, "F", 1) == 0 && isspace(trimmed_line[1]))
//             map->floor_color = parse_color(trim_whitespace(trimmed_line + 1));
//         else if (strncmp(trimmed_line, "C", 1) == 0 && isspace(trimmed_line[1]))
//             map->ceiling_color = parse_color(trim_whitespace(trimmed_line + 1));

//         // Map grid lines
//         else if (trimmed_line[0] == '1' || trimmed_line[0] == '0')
//         {
//             in_map_section = true;
//             map->grid[map_row] = malloc(sizeof(char) * (map->width + 1));
//             if (!map->grid[map_row])
//             {
//                 ft_printf("Error allocating memory for map row\n");
//                 exit(EXIT_FAILURE);
//             }

//             i = 0;
//             while (i < (int)strlen(trimmed_line) && i < map->width)
//             {
//                 map->grid[map_row][i] = trimmed_line[i];
//                 i++;
//             }
//             while (i < map->width)
//             {
//                 map->grid[map_row][i] = ' ';
//                 i++;
//             }
//             map->grid[map_row][i] = '\0';
//             map_row++;
//         }

//         // free(trimmed_line);
//         free(line);
//         line = get_next_line(fd);
//     }

//     map->grid[map_row] = NULL; // Null-terminate the grid
//     // debug verification of map value
//     if (!find_player_position(map))
//     {
//         ft_printf("Error: Player position not found in map\n");
//         exit(EXIT_FAILURE);
//     }
//     printf("Debug: Map loaded with %d rows and %d columns\n", map->height, map->width);
//     printf("debug: texture paths:\n");
//     printf("NO: %s\n", map->no_texture_path ? map->no_texture_path : "None");
//     printf("SO: %s\n", map->so_texture_path ? map->so_texture_path : "None");
//     printf("WE: %s\n", map->we_texture_path ? map->we_texture_path : "None");   
//     close(fd);
// }

