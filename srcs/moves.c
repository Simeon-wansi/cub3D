/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:09:39 by sngantch          #+#    #+#             */
/*   Updated: 2025/07/17 18:53:10 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void move_up(t_player *player)
{
    double new_x;
    double new_y;

    new_x = player->x + player->dx * MOVE_SPEED;
    new_y = player->y + player->dy * MOVE_SPEED;

    if (player->move_up)
    {
        if (!is_collision(new_x, new_y, player->game))
        {
            player->x = new_x;
            player->y = new_y;
        }
        else
        {
            if (!is_collision(new_x, player->y, player->game))
                player->x = new_x;
            if (!is_collision(player->x, new_y, player->game))
                player->y = new_y;
        }
    }
}

void move_down(t_player *player)
{
    double new_x;
    double new_y;

    new_x = player->x - player->dx * MOVE_SPEED;
    new_y = player->y - player->dy * MOVE_SPEED;

    if (player->move_down)
    {
        if (!is_collision(new_x, new_y, player->game))
        {
            player->x = new_x;
            player->y = new_y;
        }
        else
        {
            // Try sliding along the walls
            if (!is_collision(new_x, player->y, player->game))
                player->x = new_x;
            if (!is_collision(player->x, new_y, player->game))
                player->y = new_y;
        }
    }
}
void move_left(t_player *player)
{
    double new_x;
    double new_y;

    new_x = player->x - player->dy * MOVE_SPEED; 
    new_y = player->y + player->dx * MOVE_SPEED;

    if (player->move_left)
    {
        if (!is_collision(new_x, new_y, player->game))
        {
            player->x = new_x;
            player->y = new_y;
        }
        else
        {
            // Try sliding along the walls
            if (!is_collision(new_x, player->y, player->game))
                player->x = new_x;
            if (!is_collision(player->x, new_y, player->game))
                player->y = new_y;
        }
    }
}


void move_right(t_player *player)
{
    double new_x;
    double new_y;

    new_x = player->x + player->dy * MOVE_SPEED;
    new_y = player->y - player->dx * MOVE_SPEED;

    if (player->move_right)
    {
        if (!is_collision(new_x, new_y, player->game))
        {
            player->x = new_x;
            player->y = new_y;
        }
        else
        {
            // Try sliding along the walls
            if (!is_collision(new_x, player->y, player->game))
                player->x = new_x;
            if (!is_collision(player->x, new_y, player->game))
                player->y = new_y;
        }
    }
}
