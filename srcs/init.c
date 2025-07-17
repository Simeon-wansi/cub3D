/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 17:52:03 by sngantch          #+#    #+#             */
/*   Updated: 2025/07/17 20:10:11 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void init_colors(t_game *game)
{
    int red;
    int green;
    int blue;

    red = game->map.floor_color.red;
    green = game->map.floor_color.green;
    blue = game->map.floor_color.blue;
    game->floor_color = (red << 16) | (green << 8) | blue;
    red = game->map.ceiling_color.red;
    green = game->map.ceiling_color.green;
    blue = game->map.ceiling_color.blue;
    game->ceiling_color = (red << 16) | (green << 8) | blue;
    ft_printf("Floor color initialized to R: %d, G: %d, B: %d\n", game->map.floor_color.red, game->map.floor_color.green, game->map.floor_color.blue);
}

void	init_game(t_game *game)
{
	init_timing(game);
	init_performance(game);
	init_colors(game);
	game->game_running = true;
	game->mlx_ptr = mlx_init();
	if (!game->mlx_ptr)
		exit_error("Error initializing mlx");
	game->win_ptr = mlx_new_window(game->mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT,
			"Cub3D");
	if (!game->win_ptr)
		exit_error("Error creating window");
	game->win_img.img_ptr = mlx_new_image(game->mlx_ptr, WINDOW_WIDTH,
			WINDOW_HEIGHT);
	if (!game->win_img.img_ptr)
		exit_error("Error creating image");
	game->win_img.addr = mlx_get_data_addr(game->win_img.img_ptr,
						&game->win_img.bpp, &game->win_img.line_length,
											&game->win_img.endian);
	game->win_img.width = WINDOW_WIDTH;
	game->win_img.height = WINDOW_HEIGHT;
	fast_clear_image(game);
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr, game->win_img.img_ptr,
			0, 0);
	load_texture(game);
	// if (DEBUG)
	// 	debug_texture_info(game);
	init_player_from_map(game, &game->player);
	game->player.game = game;
	init_smooth_movement(&game->player);
}

// void debug_texture_info(t_game *game)
// {
//     int i;
//     printf("\n=== TEXTURE DEBUG INFO ===\n");
    
//     i = 0;
//     while (i < 4)
//     {
//         ft_printf("Texture %d:\n", i);
//         ft_printf("  Width: %d, Height: %d\n", game->textures[i].width, game->textures[i].height);
//         ft_printf("  BPP: %d, Line Length: %d\n", game->textures[i].bpp, game->textures[i].line_length);
//         ft_printf("  Endian: %d\n", game->textures[i].endian);
//         ft_printf("  Address: %p\n", game->textures[i].addr);
//         // Test a few pixels
//         if (game->textures[i].addr && game->textures[i].width > 0 && game->textures[i].height > 0)
//         {
//             int test_index = 0;
//             if (test_index < game->textures[i].height * game->textures[i].line_length)
//             {
//                 unsigned char r = (unsigned char)game->textures[i].addr[test_index + 2];
//                 unsigned char g = (unsigned char)game->textures[i].addr[test_index + 1];
//                 unsigned char b = (unsigned char)game->textures[i].addr[test_index];
//                 ft_printf("  Sample pixel (0,0): R=%d, G=%d, B=%d\n", r, g, b);
//             }
//         }
//         else
//         {
//             ft_printf("  ERROR: Invalid texture data!\n");
//         }
//         i++;
//     }
//     ft_printf("========================\n\n");
// }
