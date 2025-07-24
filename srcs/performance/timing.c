/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:06:16 by sngantch          #+#    #+#             */
/*   Updated: 2025/07/24 21:29:55 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

double	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec + tv.tv_usec / 1000000.0);
}

void	init_timing(t_game *game)
{
	double	current_time;

	current_time = get_time();
	game->timing.last_frame_time = current_time;
	game->timing.frame_start = 0.016;
	game->timing.fps = 60.0;
	ft_printf("Timing system initialized\n");
}

double	lerp(double a, double b, double t)
{
	return (a + (b - a) * t);
}

void	update_timing(t_game *game)
{
	double	current_time;

	current_time = get_time();
	game->timing.delta_time = current_time - game->timing.last_frame_time;
	game->timing.last_frame_time = current_time;
	if (game->timing.delta_time > 0.1)
		game->timing.delta_time = 0.1;
	game->timing.fps = 1.0 / game->timing.delta_time;
}
