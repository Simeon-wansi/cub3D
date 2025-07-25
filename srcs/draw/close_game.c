/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_game.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:15:14 by sngantch          #+#    #+#             */
/*   Updated: 2025/07/25 20:01:28 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	cleanup_and_exit(t_game *game, char *message)
{
	ft_printf("Error: %s\n", message);
	if (game->tex_paths)
		free(game->tex_paths);
	if (game->win_img.img_ptr)
		mlx_destroy_image(game->mlx_ptr, game->win_img.img_ptr);
	game->win_img.img_ptr = NULL;
	if (game->win_ptr)
		mlx_destroy_window(game->mlx_ptr, game->win_ptr);
	if (game->text_gun1.img_ptr)
		mlx_destroy_image(game->mlx_ptr, game->text_gun1.img_ptr);
	if (game->text_gun2.img_ptr)
		mlx_destroy_image(game->mlx_ptr, game->text_gun2.img_ptr);
	exit(EXIT_FAILURE);
}

void	fast_clear_image(t_game *game)
{
	ft_memset(game->win_img.addr, 0, WINDOW_HEIGHT * game->win_img.line_length);
}

int	close_game(t_game *game)
{
	int	i;

	game->game_running = false;
	i = 0;
	while (i < 4)
	{
		if (game->textures[i].img_ptr)
		{
			mlx_destroy_image(game->mlx_ptr, game->textures[i].img_ptr);
			game->textures[i].img_ptr = NULL;
			printf("Texture %d destroyed\n", i);
		}
		i++;
	}
	if (game->text_gun1.img_ptr)
		mlx_destroy_image(game->mlx_ptr, game->text_gun1.img_ptr);
	if (game->text_gun2.img_ptr)
		mlx_destroy_image(game->mlx_ptr, game->text_gun2.img_ptr);
	free(game->tex_paths);
	if (game->win_img.img_ptr)
		mlx_destroy_image(game->mlx_ptr, game->win_img.img_ptr);
	if (game->win_ptr)
		mlx_destroy_window(game->mlx_ptr, game->win_ptr);
	printf("Game closed successfully\n");
	exit(EXIT_SUCCESS);
}
