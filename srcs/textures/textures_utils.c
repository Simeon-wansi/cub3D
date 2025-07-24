/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:52:47 by sngantch          #+#    #+#             */
/*   Updated: 2025/07/24 22:30:25 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	extract_color_from_texture(t_texture **texture, t_render *render,
	int index)
{
	if ((*texture)->bpp == 32)
	{
		render->color = *(int *)((*texture)->addr + index);
		render->color &= 0x00FFFFFF;
	}
	else if ((*texture)->bpp == 24)
		render->color = ((unsigned char)(*texture)->addr[index
				+ 2] << 16) | ((unsigned char)(*texture)->addr[index
				+ 1] << 8) | (unsigned char)(*texture)->addr[index];
	else
		render->color = ((unsigned char)(*texture)->addr[index
				+ 2] << 16) | ((unsigned char)(*texture)->addr[index
				+ 1] << 8) | (unsigned char)(*texture)->addr[index];
}

void	initiate_texture(t_game *game)
{
	game->tex_paths = malloc(4 * sizeof(char *));
	if (!(game->tex_paths))
	{
		ft_printf("Error: Failed to allocate memory for texture paths\n");
		exit(EXIT_FAILURE);
	}
	game->tex_paths[0] = game->map.we_texture_path;
	game->tex_paths[1] = game->map.ea_texture_path;
	game->tex_paths[2] = game->map.no_texture_path;
	game->tex_paths[3] = game->map.so_texture_path;
	game->loading.total_textures = 4;
	game->loading.textures_loaded = 0;
	game->loading.loading_complete = false;
}

void	init_ui_textures(t_game *game)
{
	game->text_gun1.img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
			"./textures/gun.xpm", &game->text_gun1.width,
			&game->text_gun1.height);
	game->text_gun2.img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
			"./textures/gun2.xpm", &game->text_gun2.width,
			&game->text_gun2.height);
	if (!game->text_gun1.img_ptr || !game->text_gun2.img_ptr)
		exit_error("Failed to load gun textures");
	game->text_gun1.addr = mlx_get_data_addr(game->text_gun1.img_ptr,
			&game->text_gun1.bpp, &game->text_gun1.line_length,
			&game->text_gun1.endian);
	game->text_gun2.addr = mlx_get_data_addr(game->text_gun2.img_ptr,
			&game->text_gun2.bpp, &game->text_gun2.line_length,
			&game->text_gun2.endian);
	if (!game->text_gun1.addr || !game->text_gun2.addr)
		exit_error("Failed to get gun texture data address");
	game->show_gun2 = false;
}

void	draw_ui(t_game *game)
{
	if (game->show_gun2 == false)
		mlx_put_image_to_window(game->mlx_ptr, game->win_ptr,
			game->text_gun1.img_ptr, (WINDOW_WIDTH - game->text_gun1.width)
			/ 2, WINDOW_HEIGHT - game->text_gun1.height);
	else
		mlx_put_image_to_window(game->mlx_ptr, game->win_ptr,
			game->text_gun2.img_ptr, (WINDOW_WIDTH - game->text_gun2.width)
			/ 2, WINDOW_HEIGHT - game->text_gun2.height);
}
