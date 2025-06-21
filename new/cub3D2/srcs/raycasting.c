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
    delta_dist_x = fabs(1 / ray->dir_x);
    delta_dist_y = fabs(1 / ray->dir_y);


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
            break;
        }
    }

    //store side of wall
    ray->wall_side = side;;

    // Store the exact hit point on the wall for texturing
    if (side == 0)
    ray->distance = (map_x - ray->start_x/BLOCK_SIZE + (1 - step_x) / 2) / ray->dir_x;
    else
        ray->distance = (map_y - ray->start_y/BLOCK_SIZE + (1 - step_y) / 2) / ray->dir_y;
} 

// void determine_wall_texture(t_game *game, t_ray *ray)
void determine_wall_texture(t_game *game, t_ray *ray, t_)
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
    int i;
    int color;
    int x;

    i = 0;
    while (i < WINDOW_WIDTH)
    {
        if (i < WINDOW_WIDTH / 2)
        {
            // Render ceiling
            color = game->ceiling_color;
            mlx_pixel_put(game->mlx_ptr, game->win_ptr, i, 0, color);
        }
        else
        {
            // Render ground
            color = game->floor_color;
            mlx_pixel_put(game->mlx_ptr, game->win_ptr, i, WINDOW_HEIGHT - 1, color);
        }
        // Render vertical line for each ray
        x  = 0;
        while(x < WINDOW_WIDTH)
        {
            put_pixel(&game->win_img, i, x, color);
            x++;
        }
        i++;
    }
    
}




void cast_single_ray(t_game *game, t_ray *ray)
{
    normalize_direction(ray);
    ray_intersection(game, ray);
    determine_wall_texture(ray);
    render_ground_and_ceiling(game);
}

void cast_rays(t_game *game)
{
    int i;
    double camera_x;
    double ray_dir_x;
    double ray_dir_y;


    i = 0;
    while(i < WINDOW_WIDTH)
    {
        // Calculate camera x coordinate
        camera_x = (2 * (double)i / (double)WINDOW_WIDTH) - 1; // Normalized to [-1, 1]

        // Calculate ray direction from camera plane
        ray_dir_x = game->player.dx - game->player.dy * camera_x;
        ray_dir_y = game->player.dy + game->player.dx * camera_x;

        
        

        // fill ray struct
        game->rays[i].start_x = game->player.x;
        game->rays[i].start_y = game->player.y;
        game->rays[i].dir_x = ray_dir_x;
        game->rays[i].dir_y = ray_dir_y;
        game->rays[i].angle = atan2(ray_dir_y, ray_dir_x);

        //run DDA
        cast_single_ray(game, &game->rays[i]);
        i++;
    }
}