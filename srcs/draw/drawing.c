/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 17:52:00 by sngantch          #+#    #+#             */
/*   Updated: 2025/07/24 22:28:03 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	put_pixel(int x, int y, int color, t_game *game)
{
	int	pixel_index;

	if (x >= WINDOW_WIDTH || x < 0 || y >= WINDOW_HEIGHT || y < 0)
		return ;
	pixel_index = y * game->win_img.line_length + x * (game->win_img.bpp / 8);
	game->win_img.addr[pixel_index] = color & 0xFF;
	game->win_img.addr[pixel_index + 1] = (color >> 8) & 0xFF;
	game->win_img.addr[pixel_index + 2] = (color >> 16) & 0xFF;
}

void	draw_filled_square(t_point point, int size, int color, t_game *game)
{
	int	i;
	int	j;

	j = 0;
	while (j < size)
	{
		i = 0;
		while (i < size)
		{
			put_pixel(point.x + i, point.y + j, color, game);
			i++;
		}
		j++;
	}
}

static void	calculate_line_params(t_point p0, t_point p1, t_point *d,
		t_point *s)
{
	d->x = abs(p1.x - p0.x);
	d->y = abs(p1.y - p0.y);
	if (p0.x < p1.x)
		s->x = 1;
	else
		s->x = -1;
	if (p0.y < p1.y)
		s->y = 1;
	else
		s->y = -1;
}

void	draw_line(t_point p0, t_point p1, int color, t_game *game)
{
	t_point	d;
	t_point	s;
	int		err;
	int		e2;

	calculate_line_params(p0, p1, &d, &s);
	err = d.x - d.y;
	while (1)
	{
		put_pixel(p0.x, p0.y, color, game);
		if (p0.x == p1.x && p0.y == p1.y)
			break ;
		e2 = err * 2;
		if (e2 > -d.y)
		{
			err -= d.y;
			p0.x += s.x;
		}
		if (e2 < d.x)
		{
			err += d.x;
			p0.y += s.y;
		}
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
