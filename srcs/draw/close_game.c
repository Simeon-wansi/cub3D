/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_game.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:15:14 by sngantch          #+#    #+#             */
/*   Updated: 2025/07/24 21:46:15 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	exit_error(char *message)
{
	ft_printf("Error : %s\n", message);
	exit(EXIT_FAILURE);
}

void	fast_clear_image(t_game *game)
{
	ft_memset(game->win_img.addr, 0, WINDOW_HEIGHT * game->win_img.line_length);
}

int	close_game(t_game *game)
{
	int	i;

	printf("Debug: Closing game...\n");
	game->game_running = false;
	i = 0;
	while (i < 4)
	{
		if (game->textures[i].img_ptr)
		{
			mlx_destroy_image(game->mlx_ptr, game->textures[i].img_ptr);
			game->textures[i].img_ptr = NULL;
			printf("Debug: Texture %d destroyed\n", i);
		}
		i++;
	}
	free(game->tex_paths);
	if (game->win_img.img_ptr)
		mlx_destroy_image(game->mlx_ptr, game->win_img.img_ptr);
	if (game->win_ptr)
		mlx_destroy_window(game->mlx_ptr, game->win_ptr);
	printf("Debug: Game closed successfully\n");
	exit(EXIT_SUCCESS);
}
