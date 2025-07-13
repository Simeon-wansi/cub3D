#include "../includes/cub3d.h"
#include "../includes/libft/arena.h"
#include "parsing/parser_utils.h"
#include <fcntl.h>
#include <string.h>


void clear_image(t_game *game)
{
	int x, y;
	
	y = 0;
	while (y < WINDOW_HEIGHT)
	{
		x = 0;
		while (x < WINDOW_WIDTH)
		{
			put_pixel(x, y, COLOR_BLACK, game);
			x++;
		}
		y++;
	}
}

void exit_error(char *message)
{
	ft_printf("Error : %s\n", message);
	exit(EXIT_FAILURE);
}


void init_game(t_game *game)
{
	init_timing(game);
	init_performance(game);
	init_colors(game);
	game->game_running = true;
	game->mlx_ptr = mlx_init();
	if (!game->mlx_ptr)
		exit_error("Error initializing mlx");
	game->win_ptr = mlx_new_window(game->mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT, "Cub3D");
	if (!game->win_ptr)
		exit_error("Error creating window");
	game->win_img.img_ptr = mlx_new_image(game->mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!game->win_img.img_ptr)
		exit_error("Error creating image");
	game->win_img.addr = mlx_get_data_addr(game->win_img.img_ptr, &game->win_img.bpp,
										   &game->win_img.line_length, &game->win_img.endian);
	game->win_img.width = WINDOW_WIDTH;
	game->win_img.height = WINDOW_HEIGHT;
	// we clear the image to black
	fast_clear_image(game);
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr, game->win_img.img_ptr, 0, 0);
	load_texture(game);
	if (DEBUG)
		debug_texture_info(game);
	init_player_from_map(game, &game->player);
	game->player.game = game;
	init_smooth_movement(&game->player);
	printf("Debug: Game initialized successfully\n");
}

int main(int ac, char **av)
{
	t_game game;
	t_arena *arena;

	if (ac != 2)
	{
		ft_printf("Error\nUsage: %s <map_file>\n", av[0]);
		return EXIT_FAILURE;
	}
	arena = arena_create(1024 * 1024);
	if (!arena)
		return (ft_printf("Error\nFailed to create memory\n"), 1);
	if (!init_map_using_parser(&game.map, av[1], arena))
		return (arena_destroy(arena), EXIT_FAILURE);

	// Debug to check if the map parsing was successful
	if (DEBUG)
	{
		ft_printf("Map parsed successfully:\n");
		ft_printf("Width: %d, Height: %d\n", game.map.width, game.map.height);
		for (int i = 0; i < game.map.height; i++)
			ft_printf("%s\n", game.map.grid[i]);
	}
	init_game(&game);
	mlx_hook(game.win_ptr, X_EVENT_KEY_PRESS, 0, key_press, &game);
	mlx_hook(game.win_ptr, X_EVENT_KEY_RELEASE, 0, key_release, &game);
	mlx_hook(game.win_ptr, X_EVENT_KEY_EXIT, 0, close_game, &game);
	mlx_loop_hook(game.mlx_ptr, game_loop, &game);
	printf("Debug: Entering mlx loop\n"); 
	mlx_loop(game.mlx_ptr);
	arena_destroy(arena);
	return (EXIT_SUCCESS);
}