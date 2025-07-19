/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 17:51:56 by sngantch          #+#    #+#             */
/*   Updated: 2025/07/19 22:40:52 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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

int	key_press(int key_code, t_game *game)
{
	t_player	*player;

	player = &game->player;
	if (key_code == KEY_W)
		player->move_up = true;
	else if (key_code == KEY_S)
		player->move_down = true;
	else if (key_code == KEY_A)
		player->move_left = true;
	else if (key_code == KEY_D)
		player->move_right = true;
	else if (key_code == KEY_LEFT)
		player->rotate_left = true;
	else if (key_code == KEY_RIGHT)
		player->rotate_right = true;
	else if (key_code == KEY_SPACE)
	    game->show_gun2 = true;
	else if (key_code == KEY_ESC)
		close_game(game);
	return (0);
}


void update_player_direction(t_player *player)
{
	player->dx = cos(player->angle);
	player->dy = sin(player->angle);
}

int	key_release(int key_code, t_game *game)
{
	t_player	*player;

	
	player = &game->player;
	if (key_code == KEY_W)
		player->move_up = false;
	else if (key_code == KEY_S)
		player->move_down = false;
	else if (key_code == KEY_A)
		player->move_left = false;
	else if (key_code == KEY_D)
		player->move_right = false;
	else if (key_code == KEY_LEFT)
		player->rotate_left = false;
	else if (key_code == KEY_RIGHT)
		player->rotate_right = false;
	else if (key_code == KEY_SPACE)
	    game->show_gun2 = false;
	return (0);
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

bool	is_collision(double x, double y, t_game *game)
{
	if (colision_check(x - PLAYER_SIZE, y - PLAYER_SIZE, game))
		return (true);
	if (colision_check(x + PLAYER_SIZE, y - PLAYER_SIZE, game))
		return (true);
	if (colision_check(x - PLAYER_SIZE, y + PLAYER_SIZE, game))
		return (true);
	if (colision_check(x + PLAYER_SIZE, y + PLAYER_SIZE, game))
		return (true);
	return (false);
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