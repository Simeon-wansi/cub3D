/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:26:45 by sngantch          #+#    #+#             */
/*   Updated: 2025/07/24 22:29:44 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	caculate_step_dir_and_side_dist(t_dda *dda, t_player *player,
		double ray_dir_x, double ray_dir_y)
{
	if (ray_dir_x < 0)
	{
		dda->step_x = -1;
		dda->side_dist_x = (player->x / BLOCK_SIZE - dda->map_x) * dda->delta_x;
	}
	else
	{
		dda->step_x = 1;
		dda->side_dist_x = (dda->map_x + 1.0 - player->x / BLOCK_SIZE)
			* dda->delta_x;
	}
	if (ray_dir_y < 0)
	{
		dda->step_y = -1;
		dda->side_dist_y = (player->y / BLOCK_SIZE - dda->map_y) * dda->delta_y;
	}
	else
	{
		dda->step_y = 1;
		dda->side_dist_y = (dda->map_y + 1.0 - player->y / BLOCK_SIZE)
			* dda->delta_y;
	}
}

void	calculate_texture_position(double wall_x, t_ray *ray)
{
	wall_x = wall_x / BLOCK_SIZE;
	ray->tex_pos = wall_x - floor(wall_x);
}

static void	calculate_wall_x_xaxis(t_player *player, t_dda *dda,
		t_dpoint ray_dir, t_ray *ray)
{
	double	wall_x;

	wall_x = player->y + ((dda->map_x - player->x / BLOCK_SIZE + (1
					- dda->step_x) / 2) / ray_dir.x) * ray_dir.y * BLOCK_SIZE;
	calculate_texture_position(wall_x, ray);
}

static void	calculate_wall_x_yaxis(t_player *player, t_dda *dda,
		t_dpoint ray_dir, t_ray *ray)
{
	double	wall_x;

	wall_x = player->x + ((dda->map_y - player->y / BLOCK_SIZE + (1
					- dda->step_y) / 2) / ray_dir.y) * ray_dir.x * BLOCK_SIZE;
	calculate_texture_position(wall_x, ray);
}

void	get_wall_texture_dda(t_ray *ray, t_dda *dda, t_player *player,
		t_dpoint ray_dir)
{
	if (dda->side == 0)
	{
		if (dda->step_x < 0)
			ray->tex = TEX_WEST;
		else
			ray->tex = TEX_EAST;
		calculate_wall_x_xaxis(player, dda, ray_dir, ray);
	}
	else
	{
		if (dda->step_y < 0)
			ray->tex = TEX_NORTH;
		else
			ray->tex = TEX_SOUTH;
		calculate_wall_x_yaxis(player, dda, ray_dir, ray);
	}
	if (ray->tex_pos < 0.0)
		ray->tex_pos = 0.0;
	if (ray->tex_pos >= 1.0)
		ray->tex_pos = 0.999999;
}
