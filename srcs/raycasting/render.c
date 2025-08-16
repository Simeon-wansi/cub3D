/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:37:33 by sngantch          #+#    #+#             */
/*   Updated: 2025/08/16 15:21:38 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"



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

static void	calculate_texture_properties(t_ray *ray, t_render *render,
		t_texture *texture)
{
	double	clamped_tex_pos;

	if (ray->tex < 0 || ray->tex >= 4)
		ray->tex = 0;
	render->step = (double)texture->height / (double)render->height;
	if (render->height <= 0)
		render->step = 1.0;
	clamped_tex_pos = ray->tex_pos;
	if (clamped_tex_pos < 0.0)
		clamped_tex_pos = 0.0;
	if (clamped_tex_pos >= 1.0)
		clamped_tex_pos = 0.999999;
	render->texture_x = (int)(clamped_tex_pos * (double)texture->width);
	if (render->texture_x < 0)
		render->texture_x = 0;
	if (render->texture_x >= texture->width)
		render->texture_x = texture->width - 1;
}

void	calculate_render_properties(t_game *game, int column, t_render *render,
		t_texture **texture)
{
	t_ray	*ray;

	ray = &game->ray;
	render->height = (int)(double)BLOCK_SIZE / ray->dist * (WINDOW_HEIGHT / 2);
	render->start_y = (WINDOW_HEIGHT - render->height) / 2;
	render->end = render->start_y + render->height;
	if (render->start_y < 0)
		render->start_y = 0;
	if (render->end > WINDOW_HEIGHT)
		render->end = WINDOW_HEIGHT;
	draw_floor_and_ceiling(game, render->start_y, render->end, column);
	(*texture) = &game->textures[ray->tex];
	calculate_texture_properties(ray, render, *texture);
}
