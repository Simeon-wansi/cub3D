/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 17:51:56 by sngantch          #+#    #+#             */
/*   Updated: 2025/07/24 21:13:01 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	init_player_from_map(t_game *game, t_player *player)
{
	player->x = (game->map.player_x + 0.5) * BLOCK_SIZE;
	player->y = (game->map.player_y + 0.5) * BLOCK_SIZE;
	if (game->map.player_dir == 'N')
		player->angle = M_PI_2;
	else if (game->map.player_dir == 'S')
		player->angle = 3 * M_PI_2;
	else if (game->map.player_dir == 'E')
		player->angle = 0;
	else if (game->map.player_dir == 'W')
		player->angle = M_PI;
	player->dx = cos(player->angle);
	player->dy = sin(player->angle);
	player->move_up = false;
	player->move_down = false;
	player->move_left = false;
	player->move_right = false;
	player->rotate_left = false;
	player->rotate_right = false;
}

void	update_player_direction(t_player *player)
{
	player->dx = cos(player->angle);
	player->dy = sin(player->angle);
}

void	rotate_player(t_player *player)
{
	if (player->rotate_left)
		player->angle -= ROTATE_SPEED;
	if (player->rotate_right)
		player->angle += ROTATE_SPEED;
	if (player->angle < 0)
		player->angle += 2 * M_PI;
	else if (player->angle >= 2 * M_PI)
		player->angle -= 2 * M_PI;
}

void	move_player(t_player *player)
{
	player->dx = cos(player->angle);
	player->dy = sin(player->angle);
	rotate_player(player);
	move_up(player);
	move_down(player);
	move_left(player);
	move_right(player);
}
