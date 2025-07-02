#include "../includes/cub3d.h"



void init_ray(t_ray *ray, t_player *player, float start_x)
{
    ray->cos_angle = cos(start_x);
    ray->sin_angle = sin(start_x);
    ray->ray_x = player->x;
    ray->ray_y = player->y;
    ray->prev_x = ray->ray_x;
    ray->prev_y = ray->ray_y;
}

double distance(double x1, double y1, double x2, double y2, t_game *game)
{
    double dx;
    double dy;
    double angle;
    double fix_dist;

    dx = x2 - x1;
    dy = y2 - y1;
    angle = atan2(dy, dx) - game->player.angle;
    fix_dist = sqrt(dx * dx + dy * dy) * cos(angle);
    return (fix_dist);
}

void get_wall_texture(t_ray *ray)
{
    if ((int)(ray->prev_x / BLOCK_SIZE) != (int)(ray->ray_x / BLOCK_SIZE))
    {
       if (ray->cos_angle < 0)
            ray->tex = TEX_WEST; // West wall
        else
            ray->tex = TEX_EAST; // East wall
        ray->tex_pos = fmodf(ray->prev_y, BLOCK_SIZE) / BLOCK_SIZE; 
    }
    else
    {
        if (ray->sin_angle < 0)
            ray->tex = TEX_NORTH; // North wall
        else
            ray->tex = TEX_SOUTH; // South wall
        ray->tex_pos = fmodf(ray->prev_x, BLOCK_SIZE) / BLOCK_SIZE; 
    }
}

void draw_floor_and_ceiling(t_game *game, int start_y, int end_y, int column)
{
    int y;
    int color;

    // Draw ceiling
    color = game->ceiling_color;
    y = 0;
    while (y < start_y)
    {
        put_pixel(column, y, color, game);
        y++;
    }
    // Draw floor
    color = game->floor_color;
    y = end_y;
    while (y < WINDOW_HEIGHT)
    {
        put_pixel(column, y, color, game);
        y++;
    }
}


void render_3d(t_game *game, t_ray *ray, int column)
{
   t_render render;
   t_texture *texture;
   int ty;
   int index;

    render.height = (BLOCK_SIZE / ray->dist) * (WINDOW_HEIGHT / 2);
    render.start_y = (WINDOW_HEIGHT - render.height ) / 2;
    render.end = render.start_y + render.height;

    draw_floor_and_ceiling(game, render.start_y, render.end, column);
    if (ray->tex < 0 || ray->tex >= 4)
        ray->tex = 0; // Default to first texture if invalid
    texture = &game->textures[ray->tex];
    render.step = (double)texture->height / render.height;
    render.texture_y = 0;
    render.texture_x = (int)(ray->tex_pos * texture->width);
    if (render.texture_x < 0)
        render.texture_x = 0;
    if (render.texture_x >= texture->width)
        render.texture_x = texture->width - 1;


    while(render.start_y < render.end)
    {
        ty = (int)render.texture_y;

        if (ty < 0)
            ty = 0;
        if (ty >= texture->height)
            ty = texture->height - 1;
        
        index = ty * texture->line_length + render.texture_x * (texture->bpp / 8);


        render.color = (unsigned char)texture->addr[index] |
                    ((unsigned char)texture->addr[index + 1] << 8) |
                    ((unsigned char)texture->addr[index + 2] << 16);
        put_pixel(column, render.start_y, render.color, game);
        
        render.texture_y += render.step;
        render.start_y++;
    }

}

void draw_lines(t_player *player, t_game *game, float start_x, int column)
{
    t_ray ray;
    // double ray_angle;

    init_ray(&ray, player, start_x);
    while(!colision_check(ray.ray_x, ray.ray_y, game) )
    {
        put_pixel(ray.ray_x * MINIMAP_SCALE, ray.ray_y * MINIMAP_SCALE, COLOR_CYAN, game);
        ray.prev_x = ray.ray_x;
        ray.prev_y = ray.ray_y;

        ray.ray_x += ray.cos_angle;
        ray.ray_y += ray.sin_angle;
        // if (ray.ray_x < -BLOCK_SIZE || ray.ray_x > (game->map.width + 1) * BLOCK_SIZE ||
        // ray.ray_y < -BLOCK_SIZE || ray.ray_y > (game->map.height + 1) * BLOCK_SIZE)
        // {
        //     printf("Warning: Ray went outside map bounds\n");
        //     break;
        // }
    }

    ray.dist = distance(player->x, player->y, ray.ray_x, ray.ray_y, game);
    if (ray.dist <= 0 || isnan(ray.dist) || isinf(ray.dist)) {
        printf("Warning: Invalid ray distance: %f\n", ray.dist);
        ray.dist = 1.0; // Set minimum distance to avoid division by zero
    }
    get_wall_texture(&ray);
    render_3d(game, &ray, column);
}

void draw_minimap(t_game *game)
{
    int title_size;
    int x;
    int y;

    title_size = 8; // Size of each tile in the minimap
    y = 0;
    while(game->map.grid[y])
    {
        x = 0;
        while(game->map.grid[y][x])
        {
            if (game->map.grid[y][x] == '1') // Wall
                // draw_square(x * title_size, y * title_size, title_size, COLOR_RED, game);
                put_pixel(x * title_size, y * title_size, COLOR_RED, game);
            else if (game->map.grid[y][x] == '0') // Empty space
                draw_square(x * title_size, y * title_size, title_size, COLOR_GREEN, game);

            x++;
        }
        y++;
    }
    draw_square(game->player.x / BLOCK_SIZE * title_size, 
                game->player.y / BLOCK_SIZE * title_size, 
                title_size, COLOR_YELLOW, game);
}

int game_loop(t_game *game)
{
    t_player *player;
    int i;
    double start_x;
    double fraction;
    


    player = &game->player;
    move_player(player);
    clear_image(game);

    fraction = M_PI / 3 / WINDOW_WIDTH; 
    start_x = player->angle - (M_PI / 6); // Start angle for the first ray
    i = 0;
    while( i < WINDOW_WIDTH)
    {
        draw_lines(player, game, start_x, i);
        // Increment angle for next ray
        start_x += fraction;
        i++;
    }
    draw_minimap(game);
    mlx_put_image_to_window(game->mlx_ptr, game->win_ptr, game->win_img.img_ptr, 0, 0);
    printf("Debug: Game loop executed, player position: (%.2f, %.2f)\n", player->x, player->y);
    return (0);
}