/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   performance.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 17:52:13 by sngantch          #+#    #+#             */
/*   Updated: 2025/08/07 00:58:23 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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
}

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

void	init_smooth_movement(t_player *player)
{
	player->smooth.velocity_x = 0.0;
	player->smooth.velocity_y = 0.0;
	player->smooth.acceleration = 0.8;
	player->smooth.deceleration = 12.0;
	player->smooth.max_speed = MOVE_SPEED * 20.0;
}

void	set_player_velocity_null(t_player *player, double velocity_x,
		double velocity_y)
{
	player->smooth.velocity_x = velocity_x;
	player->smooth.velocity_y = velocity_y;
}

// Calculate new position based on current position
// and velocity to apply smooth movement
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
	{
		if (!swept_collision_check(player, new_pos.x, player->y, player->game))
		{
			player->x = new_pos.x;
			player->smooth.velocity_y = 0.0;
		}
		else if (!swept_collision_check(player, player->x, new_pos.y,
				player->game))
		{
			player->y = new_pos.y;
			player->smooth.velocity_x = 0.0;
		}
		else
			set_player_velocity_null(player, 0.0, 0.0);
	}
}
