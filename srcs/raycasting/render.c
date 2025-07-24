/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:37:33 by sngantch          #+#    #+#             */
/*   Updated: 2025/07/24 22:36:47 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	render_3d(t_game *game, t_ray *ray, int column)
{
	t_render	render;
	t_texture	*texture;
	int			ty;
	int			index;

	game->ray = *ray;
	calculate_render_properties(game, column, &render, &texture);
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
		put_pixel(column, render.start_y, render.color, game);
		render.texture_y += render.step;
		render.start_y++;
	}
}

void	draw_minimap(t_game *game)
{
	t_point	p;
	t_point	player;
	t_point	dir;

	p.y = 0;
	while (game->map.grid[p.y])
	{
		p.x = 0;
		while (game->map.grid[p.y][p.x])
		{
			if (game->map.grid[p.y][p.x] == '1')
				draw_filled_square((t_point){p.x * TILE_SIZE, p.y * TILE_SIZE},
					TILE_SIZE, COLOR_RED, game);
			p.x++;
		}
		p.y++;
	}
	player.x = (int)((float)game->player.x / BLOCK_SIZE * TILE_SIZE);
	player.y = (int)((float)game->player.y / BLOCK_SIZE * TILE_SIZE);
	draw_filled_square((t_point){player.x - TILE_SIZE / 4, player.y - TILE_SIZE
		/ 4}, TILE_SIZE / 2, COLOR_BLUE, game);
	dir.x = player.x + (int)(game->player.dx * TILE_SIZE);
	dir.y = player.y + (int)(game->player.dy * TILE_SIZE);
	draw_line(player, dir, COLOR_GREEN, game);
}

static void	update_game_state(t_game *game, t_player *player)
{
	update_timing(game);
	smooth_player_movement(player, game->timing.delta_time);
	game->perf.frame_start = get_time();
	game->perf.ray_cast = 0;
	fast_clear_image(game);
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
	update_game_state(game, player);
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
	draw_ui(game);
	return (0);
}

void	calculate_render_properties(t_game *game, int column, t_render *render,
		t_texture **texture)
{
	t_ray	*ray;

	ray = &game->ray;
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
