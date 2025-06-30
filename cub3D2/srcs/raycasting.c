#include "../includes/cub3d.h"


void normalize_direction(t_ray *ray)
{
    double magnitude;

    magnitude = sqrt(ray->dir_x * ray->dir_x + ray->dir_y * ray->dir_y);
    if (magnitude != 0)
    {
        ray->dir_x /= magnitude;
        ray->dir_y /= magnitude;
    }
}



void put_pixel(t_img *img, int x, int y, int color)
{
    char *dst;

    if (x < 0 || x >= img->width || y < 0 || y >= img->height)
        return; // Out of bounds check

    dst = img->addr + (y * img->line_length + x * (img->bpp / 8));
    *(unsigned int *)dst = color;
}


void ray_intersection(t_game *game, t_ray *ray)
{
    int map_x;
    int map_y;
    double delta_dist_x;
    double delta_dist_y;
    int step_x;
    int step_y;
    double side_dist_x;
    double side_dist_y;
    int side;

    map_x = (int)(ray->start_x / BLOCK_SIZE);
    map_y = (int)(ray->start_y / BLOCK_SIZE);
    // delta_dist_x = fabs(1 / ray->dir_x);
    // delta_dist_y = fabs(1 / ray->dir_y);
    delta_dist_x = (ray->dir_x == 0) ? 1e-10 : fabs(1 / ray->dir_x);
    delta_dist_y = (ray->dir_y == 0) ? 1e-10 : fabs(1 / ray->dir_y);

    //Setupt side distances and steps
    if (ray->dir_x < 0)
    {
        step_x = -1;
        side_dist_x = (ray->start_x - map_x * BLOCK_SIZE) * delta_dist_x;
    }
    else
    {
        step_x = 1;
        side_dist_x = (map_x * BLOCK_SIZE + BLOCK_SIZE - ray->start_x) * delta_dist_x;
    }

    if (ray->dir_y < 0)
    {
        step_y = -1;
        side_dist_y = (ray->start_y - map_y * BLOCK_SIZE) * delta_dist_y;
    }
    else
    {
        step_y = 1;
        side_dist_y = (map_y * BLOCK_SIZE + BLOCK_SIZE - ray->start_y) * delta_dist_y;
    }

    // DDA loop
    while (1)
    {
        if (side_dist_x < side_dist_y)
        {
            side_dist_x += delta_dist_x;
            map_x += step_x;
            side = 0; // NS wall
        }
        else
        {
            side_dist_y += delta_dist_y;
            map_y += step_y;
            side = 1; // EW wall
        }

        if (map_x < 0 || map_x >= game->map.width || map_y < 0 || map_y >= game->map.height)
        {
            ray->hit_wall = 0; // Out of bounds
            return;
        }

        if (game->map.grid[map_y][map_x] == '1') // Wall hit
        {
            ray->hit_wall = 1;
            ray->wall_side = side;
            printf("Ray hit wall at map[%d][%d], distance: %f\n", map_y, map_x, ray->distance);
            break;
        }

    }
    

    //store side of wall
    ray->wall_side = side;;

    // Store the exact hit point on the wall for texturing
    // In ray_intersection(), at the end:
    // if (side == 0)
    //     ray->distance = fabs(((double)map_x - ray->start_x/BLOCK_SIZE + (1 - step_x) / 2) / ray->dir_x) * BLOCK_SIZE;
    // else
    //     ray->distance = fabs(((double)map_y - ray->start_y/BLOCK_SIZE + (1 - step_y) / 2) / ray->dir_y) * BLOCK_SIZE;

    if (side == 0)
        ray->distance = side_dist_x - delta_dist_x;
    else
        ray->distance = side_dist_y - delta_dist_y;

} 

// void determine_wall_texture(t_game *game, t_ray *ray)
void determine_wall_texture(t_ray *ray)
{
    double hit_x;

    if (ray->wall_side == 0) // NS wall
        hit_x = ray->start_y + ray->distance * ray->dir_y;
    else
        hit_x = ray->start_x + ray->distance * ray->dir_x;
    
    // Wall_x is used to get texture X offset
    ray->wall_x = fmod(hit_x, BLOCK_SIZE) / BLOCK_SIZE;
}



void render_ground_and_ceiling(t_game *game)
{
    int x, y;
    int ceiling_color = game->ceiling_color;
    int floor_color = game->floor_color;
    
    // Draw ceiling (top half)
    y = 0;
    while (y < WINDOW_HEIGHT / 2)
    {
        x = 0;
        while (x < WINDOW_WIDTH)
        {
            put_pixel( &game->win_img, x, y, ceiling_color);
            x++;
        }
        y++;
    }
    
    // Draw floor (bottom half)
    while (y < WINDOW_HEIGHT)
    {
        x = 0;
        while (x < WINDOW_WIDTH)
        {
            put_pixel(&game->win_img, x, y, floor_color);
            x++;
        }
        y++;
    }
    printf("Rendering floor: %x, ceiling: %x\n", game->floor_color, game->ceiling_color);
}



void render_wall_texture(t_game *game, t_wall *wall, int column)
{
    int y;
    int pixel_index;
    int hex_color;
    double texture_pos;
    double texture_step;

    // Add bounds checking
    if (column < 0 || column >= WINDOW_WIDTH)
        return;
        
    y = 0;
    texture_pos = 0;
    texture_step = BLOCK_SIZE / wall->wall_height;
    
    while (y < wall->wall_height && wall->wall_y + y < WINDOW_HEIGHT)
    {
        if (wall->wall_y + y >= 0) // Only draw visible pixels
        {
            wall->texture_y = (int)texture_pos & (BLOCK_SIZE - 1);
            
            // Fix the texture pixel index calculation 
            // pixel_index = wall->texture_y * wall->texture_img.line_length + (int)(wall->texture_x * (wall->texture_img.bpp / 8));
            pixel_index = wall->texture_y * wall->texture_img.line_length + wall->texture_x * (wall->texture_img.bpp / 8);
                         
            // Add bounds check for texture
            if (pixel_index >= 0 && pixel_index < wall->texture_img.height * wall->texture_img.line_length)
            {
                hex_color = *(unsigned int *)(wall->texture_img.addr + pixel_index);
                // mlx_pixel_put_img(game, column, wall->wall_y + y, hex_color);
                put_pixel(&game->win_img, column, wall->wall_y + y, hex_color);
            }
        }
        texture_pos += texture_step;
        y++;
    }
}


void cast_single_ray(t_game *game, t_ray *ray, int column)
{
    normalize_direction(ray);
    ray_intersection(game, ray);
    
    // Set texture based on wall side
    if (ray->wall_side == 0) // NS wall
    {
        if (ray->dir_x > 0)
            game->wall.texture_img = game->textures.north;
        else
            game->wall.texture_img = game->textures.south;
    } 
    else // EW wall
    {
        if (ray->dir_y > 0)
            game->wall.texture_img = game->textures.east;
        else
            game->wall.texture_img = game->textures.west;
    }
    
    // Calculate wall height and position
    double corrected_dist = ray->distance * cos(ray->angle - game->player.angle);
    game->wall.wall_height = (int)((BLOCK_SIZE * WINDOW_HEIGHT) / corrected_dist);
    game->wall.wall_y = (WINDOW_HEIGHT - game->wall.wall_height) / 2;
    if (game->wall.wall_y < 0)
        game->wall.wall_y = 0;
    
    // Set texture x coordinate
    determine_wall_texture(ray);
    game->wall.texture_x = (int)(ray->wall_x * BLOCK_SIZE);
    // Prevent division by zero or very small values
    if (corrected_dist < 0.1) 
        corrected_dist = 0.1;

    game->wall.wall_height = (int)((BLOCK_SIZE * WINDOW_HEIGHT) / corrected_dist);

    // Cap maximum wall height to prevent buffer overflows
    if (game->wall.wall_height > WINDOW_HEIGHT * 3)
        game->wall.wall_height = WINDOW_HEIGHT * 3;
    // // Now render with all parameters set
    render_wall_texture(game, &game->wall, column);
}

void cast_rays(t_game *game)
{
    int column;
    double camera_x;
    double ray_dir_x;
    double ray_dir_y;


    column = 0;
    while(column < WINDOW_WIDTH)
    {
        // Calculate camera x coordinate
        camera_x = (2 * (double)column / (double)WINDOW_WIDTH) - 1;
        double plane_x = tan(game->player.fov / 2); // Camera plane length 

        // Calculate ray direction
        ray_dir_x = game->player.dx + game->player.dy * camera_x * plane_x;
        ray_dir_y = game->player.dy - game->player.dx * camera_x * plane_x;
        // Normalize the ray direction    
        

        // fill ray struct
        game->rays[column].start_x = game->player.x;
        game->rays[column].start_y = game->player.y;
        game->rays[column].dir_x = ray_dir_x;
        game->rays[column].dir_y = ray_dir_y;
        game->rays[column].angle = atan2(ray_dir_y, ray_dir_x);

        // Debug 
        // printf("Ray %d: Start(%.2f, %.2f) Dir(%.2f, %.2f) Angle(%.2f)\n", 
        //        column, game->rays[column].start_x, game->rays[column].start_y, 
        //        game->rays[column].dir_x, game->rays[column].dir_y, 
        //        game->rays[column].angle);

        //run DDA
        cast_single_ray(game, &game->rays[column], column);
        column++;
    }
    printf("Debug: Rays casted for %d columns\n", column);
    
    
}