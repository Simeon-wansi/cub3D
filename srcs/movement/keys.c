/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 21:12:22 by sngantch          #+#    #+#             */
/*   Updated: 2025/07/24 21:46:46 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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
