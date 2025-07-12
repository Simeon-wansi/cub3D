#include "../includes/cub3d.h"

double get_time(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec + tv.tv_usec / 1000000.0);
}

void init_timing(t_game *game)
{
    double current_time;

    current_time = get_time();
    game->timing.delta_time = current_time - game->timing.last_frame_time;
    game->timing.last_frame_time = current_time;

    if (game->timing.delta_time < 0.01)
        game->timing.delta_time = 0.01; // Prevent too small delta time
    else if (game->timing.delta_time > 0.1)
        game->timing.delta_time = 0.1; // Prevent too large delta time

    game->timing.fps = 1.0 / game->timing.delta_time;
}

void init_performance(t_game *game)
{
    int i;

    game->perf.frame_time = 0.0;
    game->perf.render_time = 0.0;
    game->perf.logic_time = 0.0;
    game->perf.ray_cast = 0;
    game->perf.pixels_drawn = 0;
    game->perf.avg_fps = 0.0;
    game->perf.sample_index = 0;

    //Initialisae FPS sample array
    i = 0;
    while (i < 60)
    {
        game->perf.fps_samples[i] = 0.0;
        i++;
    }
    printf("Performance monitoring initialized.\n");
}

void track_performance(t_game *game)
{
    double end_time;
    double total;
    int i;

    end_time = get_time();
    game->perf.frame_time =  end_time -  game->perf.frame_start;
    
    // calculate average FPS
    total = 0.0;
    i  = 0;
    while (i < 60)
    {
        total += game->perf.fps_samples[i];
        i++;
    }
    game->perf.avg_fps = total / 60.0;
}

void draw_debug_overlay(t_game *game)
{
    static int framecounter = 0;

    framecounter++;

    if (framecounter % 60 == 0)
    {
        ft_printf("=== PERFORMANCE DEBUG ===\n");
        ft_printf("FPS: %.1f\n", game->perf.avg_fps);
        ft_printf("Frame Time: %.2f ms\n", game->perf.frame_time * 1000.0);
        ft_printf("Rays Cast : %d\n", game->perf.ray_cast);
        ft_printf("Player Position: (%.1f, %.1f)\n", game->player.x, game->player.y);
        ft_printf("Player Angle: %.2f\n", game->player.angle);
        ft_printf("=========================\n");
    }
}


/*Optimized rendering functions */

void fast_clear_image(t_game *game)
{
    // Use memset to quickly clear the image buffer
    memset(game->win_img.addr, 0, WINDOW_HEIGHT * game->win_img.line_length);
}


void draw_vertical_line(t_game *game, int x, int start_y, int end_y, int color)
{
    int y;
    char *pixel_ptr;

    y =  start_y;
    if (y < 0)
        y = 0;
    if (end_y >= WINDOW_HEIGHT)
        end_y = WINDOW_HEIGHT - 1;
    pixel_ptr = game->win_img.addr + (y * game->win_img.line_length + x * (game->win_img.bpp / 8));
    
    //fats line drawing 
    while (y <= end_y)
    {
        if (game->win_img.bpp == 32)
            *(pixel_ptr) = color; // Directly set pixel for 32 bpp
        else 
        {
            // handling different bit depths
            pixel_ptr[0] = (color & 0xFF); // Blue
            pixel_ptr[1] = (color >> 8) & 0xFF; // Green
            pixel_ptr[2] = (color >> 16) & 0xFF; // Red
        }
        pixel_ptr += game->win_img.line_length;
        y++;
    }
    // We update the performance counter
    game->perf.pixels_drawn += (end_y - start_y + 1);
}


double lerp (double a, double b, double t)
{
    return a + (b - a) * t;
}

void init_smooth_movement(t_player *player)
{
    player->smooth.velocity_x = 0.0;
    player->smooth.velocity_y = 0.0;
    player->smooth.acceleration = 0.8;//  units per second squared
    player->smooth.deceleration = 12.0; // units per second squared
}


void smooth_player_movement(t_player *player, double delta_time)
{
    double target_velocity_x;
    double target_velocity_y;
    double acceleration;
    double new_x;
    double new_y;

    target_velocity_x = 0.0;
    target_velocity_y = 0.0;

    // We calculate the target velocity base on player input
    if (player->move_up)
    {
        target_velocity_x += player->dx * player->smooth.max_speed;
        target_velocity_y += player->dy * player->smooth.max_speed;
    }
    if (player->move_down)
    {
        target_velocity_x -= player->dx * player->smooth.max_speed;
        target_velocity_y -= player->dy * player->smooth.max_speed;
    }
    if (player->move_left)
    {
        target_velocity_x -= player->dy * player->smooth.max_speed;
        target_velocity_y += player->dx * player->smooth.max_speed;
    }
    if (player->move_right)
    {
        target_velocity_x += player->dy * player->smooth.max_speed;
        target_velocity_y -= player->dx * player->smooth.max_speed;
    }

    // smooth acceleration / Deceleration
 
    if (target_velocity_x != 0 || target_velocity_y != 0)
        acceleration = player->smooth.acceleration;
    else
        acceleration = player->smooth.deceleration;
    
    player->smooth.velocity_x = lerp(player->smooth.velocity_x, target_velocity_x, acceleration * delta_time);
    player->smooth.velocity_y = lerp(player->smooth.velocity_y, target_velocity_y, acceleration * delta_time);
    
    //  apply smooth movement with collision detection

    if (fabs (player->smooth.velocity_x) > 0.01 || fabs(player->smooth.velocity_y) > 0.01)
    {
        new_x = player->x + player->smooth.velocity_x * delta_time;
        new_y = player->y + player->smooth.velocity_y * delta_time;

        if (!swept_collision_check(player, new_x, new_y, player->game))
        {
            player->x = new_x;
            player->y = new_y;
        }
        else
        {
            // If collision detected, reset velocity to zero , we stop the smooth movement
            player->smooth.velocity_x = 0.0;
            player->smooth.velocity_y = 0.0;
        }
    }
}

bool swept_collision_check(t_player *player, double new_x, double new_y, t_game *game)
{
    double steps;
    double step_x;
    double step_y;
    int i;
    double check_x;
    double check_y;

    steps = 5.0; // Number of steps for swept collision check
    step_x = (new_x - player->x) / steps;
    step_y = (new_y - player->y) / steps;

    i = 0;
    while (i < steps)
    {
        check_x = player->x + step_x * i;
        check_y = player->y + step_y * i;

        if (colision_check(check_x, check_y, game))
            return true; // Collision detected

        i++;
    }
    return false; // No collision detected
}