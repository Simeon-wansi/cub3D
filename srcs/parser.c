/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:07:27 by hmensah-          #+#    #+#             */
/*   Updated: 2025/06/18 20:05:10 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

t_map   *load_map(char *filename, t_arena *arena)
{
    t_map   *map;

    map = arena_alloc(arena, sizeof(t_map));
    if (!map)
        return (NULL);
    map->board = arena_alloc(arena, sizeof(char **) * (5 + 1));
    return (map);
}