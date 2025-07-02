#include "../includes/cub3d.h"

void free_map(char **map)
{
    int i;

    if (!map)
        return;
    i = 0;
    while (map[i])
    {
        free(map[i]);
        i++;
    }
    free(map);
}

int close_game(t_game *game)
{
    printf("Debug: Closing game...\n");

    //Clean up in reverse orderrs
    if (game->win_img.img_ptr)
        mlx_destroy_image(game->mlx_ptr, game->win_img.img_ptr);
    
    if (game->win_ptr)
        mlx_destroy_window(game->mlx_ptr, game->win_ptr);
    
    // Free map data
    if (game->map.grid)
        free_map(game->map.grid);

    // Free texttures paths
    if (game->map.no_texture_path)
        free(game->map.no_texture_path);
    if (game->map.so_texture_path)
        free(game->map.so_texture_path);
    if (game->map.we_texture_path)
        free(game->map.we_texture_path);
    if (game->map.ea_texture_path)
        free(game->map.ea_texture_path);

    printf("Debug: Game closed successfully\n");
    exit(EXIT_SUCCESS);
}