/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 17:51:46 by sngantch          #+#    #+#             */
/*   Updated: 2025/08/19 12:56:39 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	init_dda(t_dda *dda, t_player *player, double ray_dir_x,
		double ray_dir_y)
{
	dda->map_x = (int)(player->x / BLOCK_SIZE);
	dda->map_y = (int)(player->y / BLOCK_SIZE);
	if (ray_dir_x == 0)
		dda->delta_x = 1e30;
	else
		dda->delta_x = fabs(1.0 / ray_dir_x);
	if (ray_dir_y == 0)
		dda->delta_y = 1e30;
	else
		dda->delta_y = fabs(1.0 / ray_dir_y);
	caculate_step_dir_and_side_dist(dda, player, ray_dir_x, ray_dir_y);
}

static void	jump_dda(t_dda **dda, int side)
{
	if (side == 0)
	{
		(*dda)->side_dist_x += (*dda)->delta_x;
		(*dda)->map_x += (*dda)->step_x;
		(*dda)->side = 0;
	}
	else
	{
		(*dda)->side_dist_y += (*dda)->delta_y;
		(*dda)->map_y += (*dda)->step_y;
		(*dda)->side = 1;
	}
}

static double	calculate_dda_distance(t_dda *dda, t_player *player,
		double ray_dir_x, double ray_dir_y)
{
	double	perp_wall_dist;

	if (dda->side == 0)
	{
		perp_wall_dist = (dda->map_x - player->x / BLOCK_SIZE + (1
					- dda->step_x) / 2) / ray_dir_x;
	}
	else
	{
		perp_wall_dist = (dda->map_y - player->y / BLOCK_SIZE + (1
					- dda->step_y) / 2) / ray_dir_y;
	}
	return (perp_wall_dist * BLOCK_SIZE);
}

int	perform_dda(t_dda *dda, t_game *game)
{
	int	hit;
	int	max_iterations;
	int	iterations;

	hit = 0;
	max_iterations = game->map.width * game->map.height;
	iterations = 0;
	while (!hit && iterations < max_iterations)
	{
		if (dda->side_dist_x < dda->side_dist_y)
			jump_dda(&dda, 0);
		else
			jump_dda(&dda, 1);
		if (dda->map_x < 0 || dda->map_x >= game->map.width || dda->map_y < 0
			|| dda->map_y >= game->map.height)
			hit = 1;
		else if (game->map.grid[dda->map_y]
			&& game->map.grid[dda->map_y][dda->map_x] == '1')
			hit = 1;
		iterations++;
	}
	return (hit);
}

/**
 * Draws a line of the 3D scene using the DDA algorithm.
 * player: The player's position and angle.
 * game: The game's data.
 * start_x: The starting angle of the line to draw.
 * column: The column index of the line to draw.
 * raw_dist: The distance from the player to the wall.
 * Now we calcule the ray.dist with to avoide the fisheye
 */
void	draw_lines_dda(t_player *player, t_game *game, float start_x,
		int column)
{
	t_ray	ray;
	t_dda	dda;
	double	ray_dir_x;
	double	ray_dir_y;
	double	raw_dist;

	ray_dir_x = cos(start_x);
	ray_dir_y = sin(start_x);
	init_dda(&dda, player, ray_dir_x, ray_dir_y);
	if (perform_dda(&dda, game))
	{
		raw_dist = calculate_dda_distance(&dda, player, ray_dir_x, ray_dir_y);
		ray.dist = raw_dist * cos(start_x - player->angle);
		if (ray.dist <= 0 || isnan(ray.dist) || isinf(ray.dist))
			ray.dist = 1.0;
		get_wall_texture_dda(&ray, &dda, player, (t_dpoint){ray_dir_x,
			ray_dir_y});
		render_3d(game, &ray, column);
		game->perf.ray_cast++;
	}
}
