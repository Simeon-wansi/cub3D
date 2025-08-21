/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:06:32 by sngantch          #+#    #+#             */
/*   Updated: 2025/08/20 20:07:09 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"
#include "../includes/libft/arena.h"
#include "parsing/parser_utils.h"

void	init_colors(t_game *game)
{
	int	red;
	int	green;
	int	blue;

	red = game->map.floor_color.red;
	green = game->map.floor_color.green;
	blue = game->map.floor_color.blue;
	game->floor_color = (red << 16) | (green << 8) | blue;
	red = game->map.ceiling_color.red;
	green = game->map.ceiling_color.green;
	blue = game->map.ceiling_color.blue;
	game->ceiling_color = (red << 16) | (green << 8) | blue;
}

static void	init_game_logic(t_game *game)
{
	load_texture(game);
	init_player_from_map(game, &game->player);
	game->player.game = game;
	init_smooth_movement(&game->player);
	init_mouse(game);
	init_ui_textures(game);
	game->game_running = true;
}

void	init_game(t_game *game)
{
	init_timing(game);
	init_performance(game);
	init_colors(game);
	game->mlx_ptr = mlx_init();
	if (!game->mlx_ptr)
		cleanup_and_exit(game, "Error initializing mlx");
	game->win_ptr = mlx_new_window(game->mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT,
			"Cub3D");
	if (!game->win_ptr)
		cleanup_and_exit(game, "Error creating window");
	game->win_img.img_ptr = mlx_new_image(game->mlx_ptr, WINDOW_WIDTH,
			WINDOW_HEIGHT);
	if (!game->win_img.img_ptr)
		cleanup_and_exit(game, "Error creating image");
	game->win_img.addr = mlx_get_data_addr(game->win_img.img_ptr,
			&game->win_img.bpp,
			&game->win_img.line_length,
			&game->win_img.endian);
	game->win_img.width = WINDOW_WIDTH;
	game->win_img.height = WINDOW_HEIGHT;
	fast_clear_image(game);
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr, game->win_img.img_ptr,
		0, 0);
	init_game_logic(game);
}

static void	show_message(void)
{
	printf("%s", B_CYN);
	printf("   ██████╗██╗   ██╗██████╗ ██████╗ ██████╗ \n");
	printf("  ██╔════╝██║   ██║██╔══██╗╚════██╗██╔══██╗\n");
	printf("  ██║     ██║   ██║██████╔╝ █████╔╝██║  ██║\n");
	printf("  ██║     ██║   ██║██╔══██╗ ╚═══██╗██║  ██║\n");
	printf("  ╚██████╗╚██████╔╝██████╔╝██████╔╝██████╔╝%s\n", R_CL);
	printf("%s   ╚═════╝ ╚═════╝ ╚═════╝ ╚═════╝ ╚═════╝%s\n\n", B_YL, R_CL);
	printf("%s════════════════════════════════════════════%s\n", B_GN, R_CL);
	printf("%s           🎮 3D RAYCASTING ENGINE 🎮%s\n", B_WH, R_CL);
	printf("%s════════════════════════════════════════════%s\n", B_GN, R_CL);
	printf("%sUsage: ./cub3d [map_file]%s\n\n", B_BL, R_CL);
	printf("%s### Movement Controls ###%s\n", B_YL, R_CL);
	printf("%sW/S%s: Forward/Backward  ", B_RD, R_CL);
	printf("%sA/D%s: Strafe Left/Right\n", B_RD, R_CL);
	printf("%s←/→%s: Rotate Left/Right ", B_RD, R_CL);
	printf("%sESC%s: Exit Game\n", B_RD, R_CL);
	printf("%sSPACE%s: Toggle Weapon   ", B_RD, R_CL);
	printf("%sPress ESC to exit%s\n", B_MG, R_CL);
}

int	main(int ac, char **av)
{
	t_game	game;

	if (ac != 2)
	{
		show_message();
		return (EXIT_FAILURE);
	}
	game.arena = arena_create(1024 * 1024);
	if (!game.arena)
		return (ft_printf("Error\nFailed to create memory\n"), 1);
	if (!init_map_using_parser(&game.map, av[1], game.arena))
		return (arena_destroy(game.arena), EXIT_FAILURE);
	init_game(&game);
	mlx_hook(game.win_ptr, X_EVENT_KEY_PRESS, 0, key_press, &game);
	mlx_hook(game.win_ptr, X_EVENT_KEY_RELEASE, 0, key_release, &game);
	mlx_hook(game.win_ptr, X_EVENT_KEY_EXIT, 0, close_game, &game);
	mlx_loop_hook(game.mlx_ptr, game_loop, &game);
	mlx_hook(game.win_ptr, X_EVENT_MOUSE_MOVE, 0, mouse_move, &game);
	mlx_loop(game.mlx_ptr);
	arena_destroy(game.arena);
	return (EXIT_SUCCESS);
}
