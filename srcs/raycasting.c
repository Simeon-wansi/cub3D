/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 17:51:46 by sngantch          #+#    #+#             */
/*   Updated: 2025/07/17 20:07:52 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

// double distance(double x1, double y1, double x2, double y2, t_game *game)
// {
//     double dx;
//     double dy;
//     double angle;
//     double fix_dist;

//     dx = x2 - x1;
//     dy = y2 - y1;
//     angle = atan2(dy, dx) - game->player.angle;
//     fix_dist = sqrt(dx * dx + dy * dy) * cos(angle);
//     return (fix_dist);
// }

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

static void calculate_render_properties(t_game *game, t_ray *ray, int column, t_render *render, t_texture **texture)
{
    render->height = (BLOCK_SIZE / ray->dist) * (WINDOW_HEIGHT / 2);
    render->start_y = (WINDOW_HEIGHT - render->height ) / 2;
    render->end = render->start_y + render->height;
    draw_floor_and_ceiling(game, render->start_y, render->end, column);
    if (ray->tex < 0 || ray->tex >= 4)
        ray->tex = 0; // Default to first texture if invalid
    (*texture) = &game->textures[ray->tex];
    render->step = (double)(*texture)->height / render->height;
    render->texture_y = 0;
    render->texture_x = (int)(ray->tex_pos * (*texture)->width);
    if (render->texture_x < 0)
        render->texture_x = 0;
    if (render->texture_x >= (*texture)->width)
        render->texture_x = (*texture)->width - 1;
}


static void extract_color_from_texture(t_texture **texture, t_render *render, int index)
{
    if ((*texture)->bpp == 32)
    { // 32 bit color (ARGB or similar
       render->color = *(int *)((*texture)->addr + index);
        render->color &= 0x00FFFFFF; // Mask to remove alpha     // Remove alpha channel if present            
    }
    else if ((*texture)->bpp == 24) // 24 bit color (RGB)
        render->color =  ((unsigned char)(*texture)->addr[index + 2] << 16)|
                       ((unsigned char)(*texture)->addr[index + 1] << 8) |
                       (unsigned char)(*texture)->addr[index] ;
    else // Default fallback forother formats
        render->color = ((unsigned char)(*texture)->addr[index + 2] << 16)  |
                       ((unsigned char)(*texture)->addr[index + 1] << 8) |
                       (unsigned char)(*texture)->addr[index];
}

void render_3d(t_game *game, t_ray *ray, int column)
{
   t_render render;
   t_texture *texture;
   int ty;
   int index;

    calculate_render_properties(game, ray, column, &render, &texture);
    while(render.start_y < render.end)
    {
        ty = (int)render.texture_y;
        if (ty < 0)
            ty = 0;
        if (ty >= texture->height)
            ty = texture->height - 1;
        index = ty * texture->line_length + render.texture_x * (texture->bpp / 8); // Safe color extraction with bounds checking  // if (index >= 0 && index + 2 < texture->height * texture->line_length)
        if (index >= 0 && index + (texture->bpp / 8) <= texture->height * texture->line_length)
        {// Extract color based on bits per pixel
            // if (texture->bpp == 32)
            // { // 32 bit color (ARGB or similar
            //    render.color = *(int *)(texture->addr + index);
            //     render.color &= 0x00FFFFFF; // Mask to remove alpha     // Remove alpha channel if present            
            // }
            // else if (texture->bpp == 24) // 24 bit color (RGB)
            //     render.color =  ((unsigned char)texture->addr[index + 2] << 16)|
            //                    ((unsigned char)texture->addr[index + 1] << 8) |
            //                    (unsigned char)texture->addr[index] ;
            // else // Default fallback forother formats
            //     render.color = ((unsigned char)texture->addr[index + 2] << 16)  |
            //                    ((unsigned char)texture->addr[index + 1] << 8) |
            //                    (unsigned char)texture->addr[index];
            extract_color_from_texture(&texture, &render, index);
         }
         else 
            render.color = COLOR_WHITE; // Default color if index is out of bounds
         put_pixel(column, render.start_y, render.color, game);
        render.texture_y += render.step;
        render.start_y++;
    }
}


static void caculate_step_dir_and_side_dist(t_dda *dda, t_player *player, double ray_dir_x, double ray_dir_y)
{
    if (ray_dir_x < 0)
    {
        dda->step_x = -1;
        dda->side_dist_x = (player->x / BLOCK_SIZE - dda->map_x ) * dda->delta_x;
    }
    else
    {
        dda->step_x = 1;
        dda->side_dist_x = (dda->map_x + 1.0 - player->x / BLOCK_SIZE) * dda->delta_x;
    }
    if (ray_dir_y < 0)
    {
        dda->step_y = -1;
        dda->side_dist_y = (player->y / BLOCK_SIZE - dda->map_y) * dda->delta_y;
    }
    else
    {
        dda->step_y = 1;
        dda->side_dist_y = (dda->map_y + 1.0 - player->y / BLOCK_SIZE) * dda->delta_y;
    }
}

void init_dda(t_dda *dda, t_player *player, double ray_dir_x, double ray_dir_y)
{
    //  calculate which grid celle we're in
    dda->map_x = (int)(player->x / BLOCK_SIZE);
    dda->map_y = (int)(player->y / BLOCK_SIZE);
    // calculae delta distance (avoid division by zero)
    if (ray_dir_x == 0)
        dda->delta_x = 1e30; // Avoid division by zero
    else
        dda->delta_x = fabs(1.0 / ray_dir_x);
    if (ray_dir_y == 0)
        dda->delta_y = 1e30; // Avoid division by zero
    else
        dda->delta_y = fabs(1.0 / ray_dir_y);

    caculate_step_dir_and_side_dist(dda, player, ray_dir_x, ray_dir_y);
    // calculate step direction and inital side_dist
    // if (ray_dir_x < 0)
    // {
    //     dda->step_x = -1;
    //     dda->side_dist_x = (player->x / BLOCK_SIZE - dda->map_x ) * dda->delta_x;
    // }
    // else
    // {
    //     dda->step_x = 1;
    //     dda->side_dist_x = (dda->map_x + 1.0 - player->x / BLOCK_SIZE) * dda->delta_x;
    // }
    // if (ray_dir_y < 0)
    // {
    //     dda->step_y = -1;
    //     dda->side_dist_y = (player->y / BLOCK_SIZE - dda->map_y) * dda->delta_y;
    // }
    // else
    // {
    //     dda->step_y = 1;
    //     dda->side_dist_y = (dda->map_y + 1.0 - player->y / BLOCK_SIZE) * dda->delta_y;
    // }
}


static void jump_dda(t_dda **dda, int side)
{
    if (side == 0) // x-side
    {
        (*dda)->side_dist_x += (*dda)->delta_x;
        (*dda)->map_x += (*dda)->step_x;
        (*dda)->side = 0; // hit x- side
    }
    else // y-side
    {
        (*dda)->side_dist_y += (*dda)->delta_y;
        (*dda)->map_y += (*dda)->step_y;
        (*dda)->side = 1; // hit y- side
    }
}

int perform_dda(t_dda *dda, t_game *game)
{
    int hit;
    int max_iterations;
    int iterations;

    hit = 0;
    max_iterations = game->map.width * game->map.height; // Limit iterations to avoid infinite loop
    iterations = 0;
    while(!hit && iterations < max_iterations)
    {
        // Jump to the next grid line
        if (dda->side_dist_x < dda->side_dist_y)
            jump_dda(&dda, 0);
        else
          jump_dda(&dda, 1);
        // check if we hiy wall or went out of bounds
        if (dda->map_x < 0 || dda->map_x >= game->map.width ||
            dda->map_y < 0 || dda->map_y >= game->map.height)
            hit = 1; // Out of bounds
        else if (game->map.grid[dda->map_y] && game->map.grid[dda->map_y][dda->map_x] == '1')
            hit = 1; // Hit a wall
        iterations++;
    }
    return (hit);
}

static double calculate_dda_distance(t_dda *dda, t_player *player, double ray_dir_x, double ray_dir_y)
{
    double perp_wall_dist;
    
    if (dda->side == 0) // x-side
    {
        perp_wall_dist = (dda->map_x - player->x / BLOCK_SIZE + (1 - dda->step_x) / 2) / ray_dir_x;
    }
    else // y-side
    {
        perp_wall_dist = (dda->map_y - player->y / BLOCK_SIZE + (1 - dda->step_y) / 2) / ray_dir_y;
    }

    return perp_wall_dist * BLOCK_SIZE; // Convert to world units
}


static void calculate_texture_position(double wall_x, t_ray *ray)
{
    wall_x = wall_x / BLOCK_SIZE; // Normalize to grid size
    ray->tex_pos = wall_x - floor(wall_x); // Fractional part for texture mapping
}

void get_wall_texture_dda(t_ray *ray, t_dda *dda, t_player *player, double ray_dir_x, double ray_dir_y)
{
    double wall_x;

    if (dda->side == 0) // Hit x -side (vertical wall)
    {
        if (dda->step_x < 0)
            ray->tex = TEX_WEST; // West wall
        else
            ray->tex = TEX_EAST; // East wall  // Calculate where exactly the wall was hit (X cordinates)
        wall_x = player->y +  ((dda->map_x - player->x / BLOCK_SIZE + (1 - dda->step_x) / 2) / ray_dir_x) * ray_dir_y * BLOCK_SIZE;
        calculate_texture_position(wall_x, ray); // Normalize to grid size and calculate texture position
    }
    else // Hit y-side (horizontal wall)
    {
        if (dda->step_y < 0)
            ray->tex = TEX_NORTH; // North wall
        else
            ray->tex = TEX_SOUTH; // South wall // Calculate where exactly the wall was hit (X cordinates)
        wall_x = player->x + ((dda->map_y - player->y / BLOCK_SIZE + (1 - dda->step_y) / 2) / ray_dir_y) * ray_dir_x * BLOCK_SIZE;
        calculate_texture_position(wall_x, ray);
    } // Ensure texture position is in valdie range (0 to 1)
    if (ray->tex_pos < 0.0)
        ray->tex_pos = 0.0;
    if (ray->tex_pos >= 1.0)
        ray->tex_pos = 0.999999; // Prevent overflow in texture mapping
}


void draw_lines_dda(t_player *player, t_game *game, float start_x, int column)
{
    t_ray ray;
    t_dda dda;
    double ray_dir_x;
    double ray_dir_y;
    
    // initialize direction of the ray
    ray_dir_x = cos(start_x);
    ray_dir_y = sin(start_x);

    // Initisalize dda
    init_dda(&dda, player, ray_dir_x, ray_dir_y);
    if(perform_dda(&dda, game))
    {
        // calculate distance 
        ray.dist = calculate_dda_distance(&dda, player, ray_dir_x, ray_dir_y);
        // Validate distance 
        if (ray.dist <= 0 || isnan(ray.dist) || isinf(ray.dist))
             ray.dist = 1.0;
        // Get wall texture
        get_wall_texture_dda(&ray, &dda, player, ray_dir_x, ray_dir_y);
        // Render 3d 
        render_3d(game, &ray, column);
        //update performance counter
        game->perf.ray_cast++;
    }
}

void draw_minimap(t_game *game)
{
    int tile_size;
    t_point p;
    t_point player;
    t_point dir;
    
    tile_size = 8;
    p.y = 0;
    while(game->map.grid[p.y])
    {
        p.x = 0;
        while( game->map.grid[p.y][p.x])
        {
            if (game->map.grid[p.y][p.x] == '1')
                draw_filled_square((t_point){p.x * tile_size, p.y * tile_size }, tile_size, COLOR_RED, game);
            // else if (game->map.grid[p.y][p.x] == '0')
            //     draw_filled_square((t_point){p.x * tile_size, p.y * tile_size }, tile_size, COLOR_WHITE, game);
            p.x++;
        }
        p.y++;
    }
    //  draw player position
    player.x = (int)((float)game->player.x / BLOCK_SIZE * tile_size);
    player.y = (int)((float)game->player.y / BLOCK_SIZE * tile_size);
    draw_filled_square( (t_point){player.x - tile_size/4, player.y - tile_size/4}, tile_size / 2, COLOR_BLUE, game);
    // Draw player direction indicator
    dir.x = player.x + (int)(game->player.dx * tile_size);
    dir.y = player.y + (int)(game->player.dy * tile_size);
    draw_line(player, dir, COLOR_GREEN, game);
}

int game_loop(t_game *game)
{
    t_player *player;
    int i;
    double start_x;
    double fraction;
    
    if (!game->game_running)
        return (0); // Exit if game is not running
    player = &game->player;
    update_timing(game);
    smooth_player_movement(player, game->timing.delta_time);
    game->perf.frame_start = get_time();
    game->perf.ray_cast = 0; // Reset ray cast count for this frame
    fast_clear_image(game);
    fraction = M_PI / 3 / WINDOW_WIDTH; // Angle increment per pixel
    start_x = player->angle - (M_PI / 6); // Start angle for the first ray
    i = -1;
    while( ++i < WINDOW_WIDTH)
    {
        draw_lines_dda(player, game, start_x, i); 
        start_x += fraction; // Increment angle for next ray
    }
    draw_minimap(game);
    // if (DEBUG)
    //     draw_debug_overlay(game);
    mlx_put_image_to_window(game->mlx_ptr, game->win_ptr, game->win_img.img_ptr, 0, 0);
    track_performance(game);
    return (0);
}

