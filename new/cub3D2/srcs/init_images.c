#include "../includes/cub3d.h"

void error_exit(char *msg)
{
    ft_printf(" Error: %s\n", msg);
    exit(EXIT_FAILURE);
}

//The mlx_xpm_file_to_image function converts an XPM image file 
//into an image object that can be used with the MiniLibX graphics library. 
//It takes a pointer to the MiniLibX instance (mlx_ptr), the file path (filename), 
//and pointers to integers (width and height) to store the dimensions of the loaded image, 
//returning a pointer to the created image or nullptr on failure.


static int init_images(t_game *game, char *img_path, t_img *image)
{
    image->img_ptr = mlx_xpm_file_to_image(game->mlx_ptr, img_path, &image->width, &image->height);
    if (!image->img_ptr)
    {
        ft_printf(" Error loading image: %s\n", img_path);
        //free_and_clean(game); // to do later
        return -1; // This line will never be reached, but added for clarity
    }
    image->addr = mlx_get_data_addr(image->img_ptr, &image->bpp, &image->line_length, &image->endian);
    if (!image->addr)
    {
        ft_printf(" Error getting image data address\n");
        //free_and_clean(game); // to do later
        return -1; // This line will never be reached, but added for clarity
    }
    image->bpp /= 8; // Convert bits per pixel to bytes per pixel
    return 0;
}
int init_textures(t_game *game)
{
    if (init_images(game, game->map.no_texture_path, &game->textures.north) != 0 ||
        init_images(game, game->map.so_texture_path, &game->textures.south) != 0 ||
        init_images(game, game->map.we_texture_path, &game->textures.west) != 0 ||
        init_images(game, game->map.ea_texture_path, &game->textures.east) != 0)
    {
        ft_printf(" Error loading textures\n"); 
        return -1; // This line will never be reached, but added for clarity
    }
    return 0; // Success
}

int init_window_images(t_game *game)  
{
    game->win_img.height = WINDOW_HEIGHT;
    game->win_img.width = WINDOW_WIDTH;
    game->win_img.img_ptr = mlx_new_image(game->mlx_ptr, game->win_img.width, game->win_img.height);
    if (!game->win_img.img_ptr)
    {
        //free_and_clean(game); // to do later
        return -1; // This line will never be reached, but added for clarity
    }
    game->win_img.addr = mlx_get_data_addr(game->win_img.img_ptr, &game->win_img.bpp,
                                           &game->win_img.line_length, &game->win_img.endian);
    if (!game->win_img.addr)
    {
        //free_and_clean(game); // to do later
        return -1; // This line will never be reached, but added for clarity
    }
   game->win_img.bpp /= 8; // Convert bits per pixel to bytes per pixel
    return 0; // Success
}


int init_images_and_textures(t_game *game)
{
    if (init_window_images(game) != 0 || init_textures(game) != 0)
    {
        error_exit("Failed to initialize images or textures");
        return -1; // This line will never be reached, but added for clarity
    }
    return 0;
}