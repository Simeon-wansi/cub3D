/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 20:02:46 by sngantch          #+#    #+#             */
/*   Updated: 2025/07/18 20:32:38 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int init_mouse(t_game *game)
{
    game->mouse.last_x = WINDOW_WIDTH / 2;
    game->mouse.is_active = true;
    mlx_mouse_hide();
    mlx_mouse_move(game->win_ptr, game->mouse.last_x, WINDOW_HEIGHT / 2);
    return (0);
}

int mouse_move(int x, int y, t_game *game)
{
    int delta_x;
    
    (void)y; // Ignore y for now, I care only about horizontal movement
    if(!game->mouse.is_active)
        return (0);
    delta_x = x - game->mouse.last_x;
    if (delta_x != 0)
    {
        game->player.angle += delta_x * MOUSE_SENSITIVITY; // Adjust sensitivity as needed
        if (game->player.angle < 0)
            game->player.angle += 2 * M_PI;
        else if (game->player.angle >= 2 * M_PI)
            game->player.angle -= 2 * M_PI;
        update_player_direction(&game->player);
    }
    mlx_mouse_move(game->win_ptr, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    game->mouse.last_x = WINDOW_WIDTH / 2; // Reset last_x to center
    return (0);
}