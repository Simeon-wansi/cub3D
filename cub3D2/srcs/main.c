#include "../includes/cub3d.h"
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


void init_game(t_game *game)
{
    printf("Debug: Initializing game...\n");
    
    init_colors(game);
    game->mlx_ptr = mlx_init();
    game->win_ptr = mlx_new_window(game->mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT, "Cub3D");
    game->win_img.img_ptr = mlx_new_image(game->mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT);
    game->win_img.addr = mlx_get_data_addr(game->win_img.img_ptr, &game->win_img.bpp,
                                           &game->win_img.line_length, &game->win_img.endian);
    game->win_img.width = WINDOW_WIDTH;
    game->win_img.height = WINDOW_HEIGHT;
    printf("Debug: Image created successfully - addr: %p, bpp: %d, line_length: %d\n", 
        game->win_img.addr, game->win_img.bpp, game->win_img.line_length);
    
    clear_image(game);
    mlx_put_image_to_window(game->mlx_ptr, game->win_ptr, game->win_img.img_ptr, 0, 0);
    load_texture(game);
    init_player_from_map(game, &game->player);
    game->player.game = game;
    printf("Debug: Game initialized successfully\n");
   
}

// void free_map(t_map *map)
// {
//     int i;

//     if (map->grid)
//     {
//         i = 0;
//         while (i < map->height)
//         {
//             free(map->grid[i]);
//             i++;
//         }
//         free(map->grid);
//     }
//     free(map->no_texture_path);
//     free(map->so_texture_path);
//     free(map->we_texture_path);
//     free(map->ea_texture_path);
// }

int main(int ac, char **av)
{
    t_game game;

    if (ac != 2)
    {
        ft_printf(" Error Usage: %s <map_file>\n", av[0]);
        return EXIT_FAILURE;
    }

    init_map(&game.map, av[1]);
    init_game(&game);

    mlx_hook(game.win_ptr, X_EVENT_KEY_PRESS, 0, key_press, &game);
    mlx_hook(game.win_ptr, X_EVENT_KEY_RELEASE, 0, key_release, &game);
    mlx_hook(game.win_ptr, X_EVENT_KEY_EXIT, 0, close_game, &game);
    mlx_loop_hook(game.mlx_ptr, game_loop, &game);
    printf("Debug: Entering mlx loop\n"); 
    mlx_loop(game.mlx_ptr);
    return EXIT_SUCCESS;
}