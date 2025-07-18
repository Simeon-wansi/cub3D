/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 17:51:46 by sngantch          #+#    #+#             */
/*   Updated: 2025/07/17 22:00:32 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	init_ray(t_ray *ray, t_player *player, float start_x)
{
	ray->cos_angle = cos(start_x);
	ray->sin_angle = sin(start_x);
	ray->ray_x = player->x;
	ray->ray_y = player->y;
	ray->prev_x = ray->ray_x;
	ray->prev_y = ray->ray_y;
}

void	get_wall_texture(t_ray *ray)
{
	if ((int)(ray->prev_x / BLOCK_SIZE) != (int)(ray->ray_x / BLOCK_SIZE))
	{
		if (ray->cos_angle < 0)
			ray->tex = TEX_WEST;
		else
			ray->tex = TEX_EAST;
		ray->tex_pos = fmodf(ray->prev_y, BLOCK_SIZE) / BLOCK_SIZE;
	}
	else
	{
		if (ray->sin_angle < 0)
			ray->tex = TEX_NORTH;
		else
			ray->tex = TEX_SOUTH;
		ray->tex_pos = fmodf(ray->prev_x, BLOCK_SIZE) / BLOCK_SIZE;
	}
}

void	draw_floor_and_ceiling(t_game *game, int start_y, int end_y, int column)
{
	int	y;
	int	color;

	color = game->ceiling_color;
	y = 0;
	while (y < start_y)
	{
		put_pixel(column, y, color, game);
		y++;
	}
	color = game->floor_color;
	y = end_y;
	while (y < WINDOW_HEIGHT)
	{
		put_pixel(column, y, color, game);
		y++;
	}
}

static void	calculate_render_properties(t_game *game, t_ray *ray, int column,
		t_render *render, t_texture **texture)
{
	render->height = (BLOCK_SIZE / ray->dist) * (WINDOW_HEIGHT / 2);
	render->start_y = (WINDOW_HEIGHT - render->height) / 2;
	render->end = render->start_y + render->height;
	draw_floor_and_ceiling(game, render->start_y, render->end, column);
	if (ray->tex < 0 || ray->tex >= 4)
		ray->tex = 0;
	(*texture) = &game->textures[ray->tex];
	render->step = (double)(*texture)->height / render->height;
	render->texture_y = 0;
	render->texture_x = (int)(ray->tex_pos * (*texture)->width);
	if (render->texture_x < 0)
		render->texture_x = 0;
	if (render->texture_x >= (*texture)->width)
		render->texture_x = (*texture)->width - 1;
}

static void	extract_color_from_texture(t_texture **texture, t_render *render,
		int index)
{
	if ((*texture)->bpp == 32)
	{
		render->color = *(int *)((*texture)->addr + index);
		render->color &= 0x00FFFFFF;
	}
	else if ((*texture)->bpp == 24)
		render->color = ((unsigned char)(*texture)->addr[index + 2] << 16) |
			((unsigned char)(*texture)->addr[index + 1] << 8) |
			(unsigned char)(*texture)->addr[index];
	else
		render->color = ((unsigned char)(*texture)->addr[index + 2] << 16) |
			((unsigned char)(*texture)->addr[index + 1] << 8) |
			(unsigned char)(*texture)->addr[index];
}

void	render_3d(t_game *game, t_ray *ray, int column)
{
	t_render	render;
	t_texture	*texture;
	int			ty;
	int			index;

	calculate_render_properties(game, ray, column, &render, &texture);
	while (render.start_y < render.end)
	{
		ty = (int)render.texture_y;
		if (ty < 0)
			ty = 0;
		if (ty >= texture->height)
			ty = texture->height - 1;
		index = ty * texture->line_length + render.texture_x * (texture->bpp
				/ 8);
		if (index >= 0 && index + (texture->bpp / 8) <= texture->height
			* texture->line_length)
			extract_color_from_texture(&texture, &render, index);
		else
			render.color = COLOR_WHITE;
		// Default color if index is out of bounds
		put_pixel(column, render.start_y, render.color, game);
		render.texture_y += render.step;
		render.start_y++;
	}
}

static void	caculate_step_dir_and_side_dist(t_dda *dda, t_player *player,
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
		if (dda->map_x < 0 || dda->map_x >= game->map.width ||
			dda->map_y < 0 || dda->map_y >= game->map.height)
			hit = 1;
		else if (game->map.grid[dda->map_y]
				&& game->map.grid[dda->map_y][dda->map_x] == '1')
			hit = 1;
		iterations++;
	}
	return (hit);
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

static void	calculate_texture_position(double wall_x, t_ray *ray)
{
	wall_x = wall_x / BLOCK_SIZE;
	ray->tex_pos = wall_x - floor(wall_x);
}

void	get_wall_texture_dda(t_ray *ray, t_dda *dda, t_player *player,
		t_dpoint ray_dir)
{
	double	wall_x;

	if (dda->side == 0)
	{
		if (dda->step_x < 0)
			ray->tex = TEX_WEST;
		else
			ray->tex = TEX_EAST;
		wall_x = player->y + ((dda->map_x - player->x / BLOCK_SIZE + (1
						- dda->step_x) / 2) / ray_dir.x) * ray_dir.y
			* BLOCK_SIZE;
		calculate_texture_position(wall_x, ray);
	}
	else
	{
		if (dda->step_y < 0)
			ray->tex = TEX_NORTH;
		else
			ray->tex = TEX_SOUTH;
		wall_x = player->x + ((dda->map_y - player->y / BLOCK_SIZE + (1
						- dda->step_y) / 2) / ray_dir.y) * ray_dir.x
			* BLOCK_SIZE;
		calculate_texture_position(wall_x, ray);
	}
	if (ray->tex_pos < 0.0)
		ray->tex_pos = 0.0;
	if (ray->tex_pos >= 1.0)
		ray->tex_pos = 0.999999;
}

void	draw_lines_dda(t_player *player, t_game *game, float start_x,
		int column)
{
	t_ray	ray;
	t_dda	dda;
	double	ray_dir_x;
	double	ray_dir_y;

	ray_dir_x = cos(start_x);
	ray_dir_y = sin(start_x);
	init_dda(&dda, player, ray_dir_x, ray_dir_y);
	if (perform_dda(&dda, game))
	{
		ray.dist = calculate_dda_distance(&dda, player, ray_dir_x, ray_dir_y);
		if (ray.dist <= 0 || isnan(ray.dist) || isinf(ray.dist))
			ray.dist = 1.0;
		get_wall_texture_dda(&ray, &dda, player, (t_dpoint){ray_dir_x,
				ray_dir_y});
		render_3d(game, &ray, column);
		game->perf.ray_cast++;
	}
}

void	draw_minimap(t_game *game)
{
	int		tile_size;
	t_point	p;
	t_point	player;
	t_point	dir;

	tile_size = 8;
	p.y = 0;
	while (game->map.grid[p.y])
	{
		p.x = 0;
		while (game->map.grid[p.y][p.x])
		{
			if (game->map.grid[p.y][p.x] == '1')
				draw_filled_square((t_point){p.x * tile_size, p.y * tile_size},
									tile_size,
									COLOR_RED,
									game);
			p.x++;
		}
		p.y++;
	}
	player.x = (int)((float)game->player.x / BLOCK_SIZE * tile_size);
	player.y = (int)((float)game->player.y / BLOCK_SIZE * tile_size);
	draw_filled_square((t_point){player.x - tile_size / 4, player.y - tile_size
			/ 4}, tile_size / 2, COLOR_BLUE, game);
	dir.x = player.x + (int)(game->player.dx * tile_size);
	dir.y = player.y + (int)(game->player.dy * tile_size);
	draw_line(player, dir, COLOR_GREEN, game);
}

int	game_loop(t_game *game)
{
	t_player	*player;
	int			i;
	double		start_x;
	double		fraction;

	if (!game->game_running)
		return (0);
	player = &game->player;
	update_timing(game);
	smooth_player_movement(player, game->timing.delta_time);
	game->perf.frame_start = get_time();
	game->perf.ray_cast = 0;
	fast_clear_image(game);
	fraction = M_PI / 3 / WINDOW_WIDTH;
	start_x = player->angle - (M_PI / 6);
	i = -1;
	while (++i < WINDOW_WIDTH)
	{
		draw_lines_dda(player, game, start_x, i);
		start_x += fraction;
	}
	draw_minimap(game);
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr, game->win_img.img_ptr,
			0, 0);
	track_performance(game);
	return (0);
}
