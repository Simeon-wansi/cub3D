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
    printf("Debug: Floor color initialized to R: %d, G: %d, B: %d\n", game->map.floor_color.red, game->map.floor_color.green, game->map.floor_color.blue);
}

void debug_texture_info(t_game *game)
{
    int i;
    printf("\n=== TEXTURE DEBUG INFO ===\n");
    
    i = 0;
    while (i < 4)
    {
        printf("Texture %d:\n", i);
        printf("  Width: %d, Height: %d\n", game->textures[i].width, game->textures[i].height);
        printf("  BPP: %d, Line Length: %d\n", game->textures[i].bpp, game->textures[i].line_length);
        printf("  Endian: %d\n", game->textures[i].endian);
        printf("  Address: %p\n", game->textures[i].addr);
        
        // Test a few pixels
        if (game->textures[i].addr && game->textures[i].width > 0 && game->textures[i].height > 0)
        {
            int test_index = 0;
            if (test_index < game->textures[i].height * game->textures[i].line_length)
            {
                unsigned char r = (unsigned char)game->textures[i].addr[test_index + 2];
                unsigned char g = (unsigned char)game->textures[i].addr[test_index + 1];
                unsigned char b = (unsigned char)game->textures[i].addr[test_index];
                printf("  Sample pixel (0,0): R=%d, G=%d, B=%d\n", r, g, b);
            }
        }
        else
        {
            printf("  ERROR: Invalid texture data!\n");
        }
        
        i++;
    }
    printf("========================\n\n");
}