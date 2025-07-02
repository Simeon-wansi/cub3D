#include "../includes/cub3d.h"

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
       if (!in_map_section && (trimmed_line[0] == '1' || trimmed_line[0] == '0'))
           in_map_section = true;

       if (in_map_section && strlen(trimmed_line) > 0)
            count++;
    
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
    return count;
}

char *trim_whitespace(char *str)
{
    char *end;

    while (isspace((unsigned char)*str))
        str++;

    if (*str == '\0') 
        return str;
    
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    end[1] = '\0';
    return str;
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