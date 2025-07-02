#include "../includes/cub3d.h"


void put_pixel(int x, int y, int color, t_game *game)
{
   int pixel_index;

   if (x >= WINDOW_WIDTH || x < 0 || y >= WINDOW_HEIGHT || y < 0)
       return;
    pixel_index = y * game->win_img.line_length + x * (game->win_img.bpp / 8);
    game->win_img.addr[pixel_index] = color & 0xFF; // Blue
    game->win_img.addr[pixel_index + 1] = (color >> 8) & 0xFF; // Green
    game->win_img.addr[pixel_index + 2] = (color >> 16) & 0xFF; // Red
}

void draw_square(int x, int y, int size, int color, t_game *game)
{
    int i;

    i = -1;
    while(++i < size)
        put_pixel(x + i, y, color, game); // Top side
    i = -1;
    while(++i < size)
        put_pixel(x + i, y + size - 1, color, game); // Bottom side
    i = -1;
    while(++i < size)
        put_pixel(x, y + i, color, game); // Left side
    i = -1;
    while(++i < size)
        put_pixel(x + size - 1, y + i, color, game); // Right side
    // Draw the center of the square
    put_pixel(x + size / 2, y + size / 2, color, game); // Center pixel
}