/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 20:48:46 by sngantch          #+#    #+#             */
/*   Updated: 2025/08/16 15:21:46 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	get_texture_color(t_texture *texture, t_point *t, int *index)
{
	if (t->y < 0)
		t->y = 0;
	if (t->y >= texture->height)
		t->y = texture->height - 1;
	if (t->x < 0)
		t->x = 0;
	if (t->x >= texture->width)
		t->x = texture->width - 1;
	*index = t->y * texture->line_length + t->x * (texture->bpp / 8);
}

void	render_3d(t_game *game, t_ray *ray, int column)
{
	t_render	render;
	t_texture	*texture;
	double		tex_y_exact;
	t_point		t;
	int			index;

	game->ray = *ray;
	calculate_render_properties(game, column, &render, &texture);
	tex_y_exact = 0.0;
	while (render.start_y < render.end)
	{
		t.y = (int)tex_y_exact;
		t.x = render.texture_x;
		get_texture_color(texture, &t, &index);
		if (index >= 0 && index < (texture->height * texture->line_length))
			extract_color_from_texture(&texture, &render, index);
		else
			render.color = COLOR_GREY;
		put_pixel(column, render.start_y, render.color, game);
		tex_y_exact += render.step;
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
