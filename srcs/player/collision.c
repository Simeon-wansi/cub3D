/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 21:09:01 by sngantch          #+#    #+#             */
/*   Updated: 2025/08/16 15:39:10 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

// I want to make a calision check that checks with a minimum distance

bool	colision_check(double px, double py, t_game *game)
{
	int	x;
	int	y;

	x = (int)(px / BLOCK_SIZE);
	y = (int)(py / BLOCK_SIZE);
	if (x < 0 || y < 0 || y >= game->map.height || x >= game->map.width)
		return (true);
	if (!game->map.grid[y])
		return (true);
	return (game->map.grid[y][x] == '1');
}

// bool	is_collision(double x, double y, t_game *game)
// {
// 	if (colision_check(x - PLAYER_SIZE, y - PLAYER_SIZE, game))
// 		return (true);
// 	if (colision_check(x + PLAYER_SIZE, y - PLAYER_SIZE, game))
// 		return (true);
// 	if (colision_check(x - PLAYER_SIZE, y + PLAYER_SIZE, game))
// 		return (true);
// 	if (colision_check(x + PLAYER_SIZE, y + PLAYER_SIZE, game))
// 		return (true);
// 	return (false);
// }

bool	is_collision(double x, double y, t_game *game)
{
	if (colision_check(x - PLAYER_SIZE - MIN_DISTANCE, y - PLAYER_SIZE
			- MIN_DISTANCE, game))
		return (true);
	if (colision_check(x + PLAYER_SIZE + MIN_DISTANCE, y - PLAYER_SIZE
			- MIN_DISTANCE, game))
		return (true);
	if (colision_check(x - PLAYER_SIZE - MIN_DISTANCE, y + PLAYER_SIZE
			+ MIN_DISTANCE, game))
		return (true);
	if (colision_check(x + PLAYER_SIZE + MIN_DISTANCE, y + PLAYER_SIZE
			+ MIN_DISTANCE, game))
		return (true);
	return (false);
}
