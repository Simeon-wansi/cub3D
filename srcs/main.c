/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:06:32 by sngantch          #+#    #+#             */
/*   Updated: 2025/07/18 20:29:57 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"
#include "../includes/libft/arena.h"
#include "parsing/parser_utils.h"

void	exit_error(char *message)
{
	ft_printf("Error : %s\n", message);
	exit(EXIT_FAILURE);
}

void	fast_clear_image(t_game *game)
{
	ft_memset(game->win_img.addr, 0, WINDOW_HEIGHT * game->win_img.line_length);
}

int	main(int ac, char **av)
{
	t_game	game;
	t_arena	*arena;

	if (ac != 2)
	{
		ft_printf("Error\nUsage: %s <map_file>\n", av[0]);
		return (EXIT_FAILURE);
	}
	arena = arena_create(1024 * 1024);
	if (!arena)
		return (ft_printf("Error\nFailed to create memory\n"), 1);
	if (!init_map_using_parser(&game.map, av[1], arena))
		return (arena_destroy(arena), EXIT_FAILURE);
	init_game(&game);
	mlx_hook(game.win_ptr, X_EVENT_KEY_PRESS, 0, key_press, &game);
	mlx_hook(game.win_ptr, X_EVENT_KEY_RELEASE, 0, key_release, &game);
	mlx_hook(game.win_ptr, X_EVENT_KEY_EXIT, 0, close_game, &game);
	mlx_loop_hook(game.mlx_ptr, game_loop, &game);
	mlx_hook(game.win_ptr, X_EVENT_MOUSE_MOVE, 0, mouse_move, &game);
	mlx_loop(game.mlx_ptr);
	arena_destroy(arena);
	return (EXIT_SUCCESS);
}
