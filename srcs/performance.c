/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   performance.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 17:52:13 by sngantch          #+#    #+#             */
/*   Updated: 2025/07/17 19:07:55 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

double	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec + tv.tv_usec / 1000000.0);
}

void	init_timing(t_game *game)
{
	double	current_time;

	current_time = get_time();
	game->timing.last_frame_time = current_time;
	game->timing.frame_start = 0.016;
	game->timing.fps = 60.0;
	ft_printf("Timing system initialized\n");
}

/**
 * @brief Initializes performance monitoring counters and sample array
 * 
 * Performance counters are used to track game performance and average FPS.
 * And initializes the performance sample array to store FPS samples.
 * 
 * @param game The game data structure
 */
void	init_performance(t_game *game)
{
	int	i;

	game->perf.frame_time = 0.0;
	game->perf.render_time = 0.0;
	game->perf.logic_time = 0.0;
	game->perf.ray_cast = 0;
	game->perf.pixels_drawn = 0;
	game->perf.avg_fps = 0.0;
	game->perf.sample_index = 0;
	i = 0;
	while (i < 60)
	{
		game->perf.fps_samples[i] = 0.0;
		i++;
	}
	ft_printf("Performance monitoring initialized.\n");
}

/**
 * @brief Track performance metrics for current frame
 * 
 * This function tracks frame time, render time, logic time, ray cast count,
 * and pixels drawn. It also updates the FPS sample array and calculates the
 * average FPS.
 * In the loop it store the current frame time in the sample array, wraps
 * around after 60 samples, and calculates the average FPS from the samples.
 * @param game The game data structure
 */
void	track_performance(t_game *game)
{
	double	end_time;
	double	total;
	int		i;

	end_time = get_time();
	game->perf.frame_time = end_time - game->perf.frame_start;
	if (game->perf.frame_time > 0)
	{
		game->perf.fps_samples[game->perf.sample_index] = 1.0
			/ game->perf.frame_time;
		game->perf.sample_index = (game->perf.sample_index + 1) % 60;
	}
	total = 0.0;
	i = 0;
	while (i < 60)
	{
		total += game->perf.fps_samples[i];
		i++;
	}
	game->perf.avg_fps = total / 60.0;
}

void	draw_vertical_line(t_game *game, int x, int start_y, int end_y,
		int color)
{
	int		y;
	char	*pixel_ptr;

	y = start_y;
	if (y < 0)
		y = 0;
	if (end_y >= WINDOW_HEIGHT)
		end_y = WINDOW_HEIGHT - 1;
	pixel_ptr = game->win_img.addr + (y * game->win_img.line_length + x
			* (game->win_img.bpp / 8));
	//fats line drawing
	while (y <= end_y)
	{
		if (game->win_img.bpp == 32)
			*(pixel_ptr) = color; // Directly set pixel for 32 bpp
		else
		{
			// handling different bit depths
			pixel_ptr[0] = (color & 0xFF);       // Blue
			pixel_ptr[1] = (color >> 8) & 0xFF;  // Green
			pixel_ptr[2] = (color >> 16) & 0xFF; // Red
		}
		pixel_ptr += game->win_img.line_length;
		y++;
	}
	// We update the performance counter
	game->perf.pixels_drawn += (end_y - start_y + 1);
}

double	lerp(double a, double b, double t)
{
	return (a + (b - a) * t);
}

void	init_smooth_movement(t_player *player)
{
	player->smooth.velocity_x = 0.0;
	player->smooth.velocity_y = 0.0;
	player->smooth.acceleration = 0.8;  //  units per second squared
	player->smooth.deceleration = 12.0; // units per second squared
	player->smooth.max_speed = MOVE_SPEED * 20.0;
	// Maximum speed in units per second
}

void	set_player_velocity_null(t_player *player, double velocity_x,
		double velocity_y)
{
	player->smooth.velocity_x = velocity_x;
	player->smooth.velocity_y = velocity_y;
}

// Calculate new position based on current position and velocity to apply smooth movement
void	new_pos_with_collision_smooth_mvmt(t_player *player, double delta_time)
{
	t_dpoint	new_pos;

	new_pos.x = player->x + player->smooth.velocity_x * delta_time;
	new_pos.y = player->y + player->smooth.velocity_y * delta_time;
	if (!swept_collision_check(player, new_pos.x, new_pos.y, player->game))
	{
		player->x = new_pos.x;
		player->y = new_pos.y;
	}
	else
	{ // // If collision detected, reset velocity to zero ,
		// we stop the smooth movement // player->smooth.velocity_x = 0.0;
		// player->smooth.velocity_y = 0.0;
		// if full movement block we try sliding along the walls
		//try x movement
		if (!swept_collision_check(player, new_pos.x, player->y, player->game))
		{
			player->x = new_pos.x;
			player->smooth.velocity_y = 0.0; // Stop vertical movement
		}                                    //try y movement
		else if (!swept_collision_check(player, player->x, new_pos.y,
					player->game))
		{
			player->y = new_pos.y;
			player->smooth.velocity_x = 0.0; // Stop horizontal movement
		}
		else
			set_player_velocity_null(player, 0.0, 0.0);
		// If both movements are blocked, we stop the player
	}
}

void	set_targed_speed(t_dpoint *velocity, double velocity_x,
		double velocity_y)
{
	velocity->x = velocity_x;
	velocity->y = velocity_y;
}

static void update_player_rotation_and_direction(t_player *player, t_dpoint *target_velocity)
{
	if (player->rotate_left) // Handle rotation
		player->angle -= ROTATE_SPEED;
	if (player->rotate_right)
		player->angle += ROTATE_SPEED;
	if (player->angle < 0) // Normalize angle to [0, 2*PI]
		player->angle += 2 * M_PI;
	else if (player->angle >= 2 * M_PI)
		player->angle -= 2 * M_PI;
	player->dx = cos(player->angle);
	player->dy = sin(player->angle);
	target_velocity->x = 0.0;
	target_velocity->y = 0.0;
}

static void update_velocity(t_player *plyr, t_dpoint *target_velocity, double acceleration, double delta_time)
{
	plyr->smooth.velocity_x = lerp(plyr->smooth.velocity_x, target_velocity->x,
		acceleration * delta_time);
	plyr->smooth.velocity_y = lerp(plyr->smooth.velocity_y, target_velocity->y,
		acceleration * delta_time);
}

/**
 * @brief Smoothly moves the player based on input and elapsed time.
 *
 * This function calculates the target velocity of the player based on
 * directional input (up, down, left, right) and applies smooth acceleration
 * or deceleration using linear interpolation. The player's angle is updated
 * for rotation input, and the direction vectors are recalculated. The function
 * also handles collision detection, allowing the player to slide along walls
 * if a full movement is blocked. The player's velocity is adjusted to zero if
 * both horizontal and vertical movements are impeded.
 *
 * @param player The player structure containing movement and game state.
 * @param delta_time The time elapsed since the last frame,
	used for smooth movement.
 */

void	smooth_player_movement(t_player *plyr, double delta_time)
{
	t_dpoint	target_velocity;
	double		acceleration;

	update_player_rotation_and_direction(plyr, &target_velocity);
	if (plyr->move_up)
		set_targed_speed(&target_velocity, plyr->dx * plyr->smooth.max_speed,
			plyr->dy * plyr->smooth.max_speed);
	if (plyr->move_down)
        set_targed_speed(&target_velocity, -plyr->dx * plyr->smooth.max_speed,
            -plyr->dy * plyr->smooth.max_speed);
	if (plyr->move_left)
        set_targed_speed(&target_velocity, -plyr->dy * plyr->smooth.max_speed,
            plyr->dx * plyr->smooth.max_speed);
	if (plyr->move_right)
        set_targed_speed(&target_velocity, plyr->dy * plyr->smooth.max_speed,
            -plyr->dx * plyr->smooth.max_speed);
	if (target_velocity.x != 0 || target_velocity.y != 0)
		acceleration = plyr->smooth.acceleration;
	else
		acceleration = plyr->smooth.deceleration;
	update_velocity(plyr, &target_velocity, acceleration, delta_time);
	if (fabs(plyr->smooth.velocity_x) > 0.01
        || fabs(plyr->smooth.velocity_y) > 0.01)
		new_pos_with_collision_smooth_mvmt(plyr, delta_time);
}


bool	swept_collision_check(t_player *player, double new_x, double new_y,
		t_game *game)
{
	double steps;
	int i;
	t_dpoint check;
	t_dpoint step;

	steps = 10.0; // Number of steps for swept collision check
	step.x = (new_x - player->x) / steps;
	step.y = (new_y - player->y) / steps;

	i = 0;
	while (i < steps)
	{
		check.x = player->x + step.x * i;
		check.y = player->y + step.y * i;

		if (is_collision(check.x, check.y, game))
			return (true); // Collision detected
		i++;
	}
	if (is_collision(new_x, new_y, game))
		return (true);
	return (false); // No collision detected
}
