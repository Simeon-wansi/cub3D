/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   performance_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:04:55 by sngantch          #+#    #+#             */
/*   Updated: 2025/07/24 22:29:07 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	set_targed_speed(t_dpoint *velocity, double velocity_x,
		double velocity_y)
{
	velocity->x = velocity_x;
	velocity->y = velocity_y;
}

static void	update_player_rotation_and_direction(t_player *player,
													t_dpoint *target_velocity)
{
	if (player->rotate_left)
		player->angle -= ROTATE_SPEED;
	if (player->rotate_right)
		player->angle += ROTATE_SPEED;
	if (player->angle < 0)
		player->angle += 2 * M_PI;
	else if (player->angle >= 2 * M_PI)
		player->angle -= 2 * M_PI;
	player->dx = cos(player->angle);
	player->dy = sin(player->angle);
	target_velocity->x = 0.0;
	target_velocity->y = 0.0;
}

static void	update_velocity(t_player *plyr, t_dpoint *target_velocity,
		double acceleration, double delta_time)
{
	plyr->smooth.velocity_x = lerp(plyr->smooth.velocity_x, target_velocity->x,
			acceleration * delta_time);
	plyr->smooth.velocity_y = lerp(plyr->smooth.velocity_y, target_velocity->y,
			acceleration * delta_time);
}

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
	double		steps;
	int			i;
	t_dpoint	check;
	t_dpoint	step;

	steps = 10.0;
	step.x = (new_x - player->x) / steps;
	step.y = (new_y - player->y) / steps;
	i = 0;
	while (i < steps)
	{
		check.x = player->x + step.x * i;
		check.y = player->y + step.y * i;
		if (is_collision(check.x, check.y, game))
			return (true);
		i++;
	}
	if (is_collision(new_x, new_y, game))
		return (true);
	return (false);
}

// void	draw_vertical_line(t_game *game, int x, int start_y, int end_y,
// 		int color)
// {
// 	int		y;
// 	char	*pixel_ptr;

// 	y = start_y;
// 	if (y < 0)
// 		y = 0;
// 	if (end_y >= WINDOW_HEIGHT)
// 		end_y = WINDOW_HEIGHT - 1;
// 	pixel_ptr = game->win_img.addr + (y * game->win_img.line_length + x
// 			* (game->win_img.bpp / 8));
// 	while (y <= end_y)
// 	{
// 		if (game->win_img.bpp == 32)
// 			*(pixel_ptr) = color;
// 		else
// 		{
// 			pixel_ptr[0] = (color & 0xFF);       // Blue
// 			pixel_ptr[1] = (color >> 8) & 0xFF;  // Green
// 			pixel_ptr[2] = (color >> 16) & 0xFF; // Red
// 		}
// 		pixel_ptr += game->win_img.line_length;
// 		y++;
// 	}
// 	game->perf.pixels_drawn += (end_y - start_y + 1);
// }