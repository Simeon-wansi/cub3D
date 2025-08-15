/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:26:45 by sngantch          #+#    #+#             */
/*   Updated: 2025/08/15 20:16:18 by sngantch         ###   ########.fr       */
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
	double	normalized_pos;

	normalized_pos = fmod(wall_x, BLOCK_SIZE);
	if (normalized_pos < 0)
		normalized_pos += BLOCK_SIZE;
	ray->tex_pos = normalized_pos / BLOCK_SIZE;
	if (ray->tex_pos < 0.0)
		ray->tex_pos = 0.0;
	if (ray->tex_pos >= 1.0)
		ray->tex_pos = 1.0 - 0.000001;
}

double	calculate_wall_hit_position_x(t_dda *dda, t_player *player,
		t_dpoint ray_dir)
{
	double	intersection_dis;
	double	wall_hit_pos;

	intersection_dis = (dda->map_x - player->x / BLOCK_SIZE + (1 - dda->step_x)
			/ 2) / ray_dir.x;
	wall_hit_pos = (player->y / BLOCK_SIZE) + intersection_dis * ray_dir.y;
	wall_hit_pos = fmod(wall_hit_pos, 1.0);
	if (wall_hit_pos < 0.0)
		wall_hit_pos += 1.0;
	return (wall_hit_pos);
}

double	calculate_wall_hit_position_y(t_dda *dda, t_player *player,
		t_dpoint ray_dir)
{
	double	intersection_dis;
	double	wall_hit_pos;

	intersection_dis = (dda->map_y - player->y / BLOCK_SIZE + (1 - dda->step_y)
			/ 2) / ray_dir.y;
	wall_hit_pos = (player->x / BLOCK_SIZE) + intersection_dis * ray_dir.x;
	wall_hit_pos = fmod(wall_hit_pos, 1.0);
	if (wall_hit_pos < 0.0)
		wall_hit_pos += 1.0;
	return (wall_hit_pos);
}

void	get_wall_texture_dda(t_ray *ray, t_dda *dda, t_player *player,
		t_dpoint ray_dir)
{
	double	wall_hit_pos;

	if (dda->side == 0)
	{
		if (dda->step_x < 0)
			ray->tex = TEX_WEST;
		else
			ray->tex = TEX_EAST;
		wall_hit_pos = calculate_wall_hit_position_x(dda, player, ray_dir);
	}
	else
	{
		if (dda->step_y < 0)
			ray->tex = TEX_NORTH;
		else
			ray->tex = TEX_SOUTH;
		wall_hit_pos = calculate_wall_hit_position_y(dda, player, ray_dir);
	}
	ray->tex_pos = wall_hit_pos;
	if (ray->tex_pos < 0.0)
		ray->tex_pos = 0.0;
	if (ray->tex_pos >= 1.0)
		ray->tex_pos = 0.999999;
}
