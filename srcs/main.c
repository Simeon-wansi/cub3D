#include "../includes/cub3d.h"


void put_pixel(int x, int y, int color, t_game *game)
{
    if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0)
        return;
    
    int index = y * game->size_line + x * (game->bpp / 8);
    game->data[index] = color & 0xFF; // Blue
    game->data[index + 1] = (color >> 8) & 0xFF; // Green
    game->data[index + 2] = (color >> 16) & 0xFF; // Red
}


void draw_square(int x , int y, int size, int color, t_game *game)
{
    int i;

    i = -1;
    while (++i < size)
        put_pixel(x + i, y, color, game);
    i = -1;
    while (++i < size)
        put_pixel(x, y + i, color, game);
    i = -1;
    while (++i < size)
        put_pixel(x + i, y + size, color, game);
    i = -1;
    while (++i < size)
        put_pixel(x + size, y + i, color, game);
}

void draw_map(t_game *game)
{
    char **map;
    int color;
    int y;
    int x;

    map = game->map;
    color = 0x0000FF; // Blue for walls
    y = 0;
    while (map[y] != NULL)
    {
        x = 0;
        while(x < map[y][x])
        {
            if (map[y][x] == '1')
                draw_square(x * BLOCK, y * BLOCK, BLOCK, color, game);
            x++;
        }
        y++;
    }
}
void clear_image(t_game *game)
{
    int y;
    int x;

    y = -1;
    while(++y < HEIGHT)
    {
        x = -1;
        while(++x < WIDTH)
            put_pixel(x, y, 0x000000, game); // Clear with black color
    }
}
char **get_map(void)
{
    char **map;

    map = malloc(sizeof(char *) * 11);
    if (!map)
        return (NULL);
    
    map[0] = "11111111111111111111";
    map[1] = "10000000000000000001";
    map[2] = "10000000001000000001";    
    map[3] = "10000000000000000001";
    map[4] = "10000001000000000001";
    map[5] = "10000000000000001001";
    map[6] = "10000000000000100001";
    map[7] = "10000000000000000001";
    map[8] = "10000000000000000001";
    map[9] = "11111111111111111111";
    map[10] = NULL; // Null-terminate the array


    return (map);
}

void init_game(t_game *game)
{
    init_player(&game->player);
    game->map = get_map();
    game->mlx_ptr = mlx_init();
    game->win_ptr = mlx_new_window(game->mlx_ptr, WIDTH, HEIGHT, "Cub3D");
    game->img_ptr = mlx_new_image(game->mlx_ptr, WIDTH, HEIGHT);
    game->data = mlx_get_data_addr(game->img_ptr, &game->bpp, &game->size_line, &game->endian);
    mlx_put_image_to_window(game->mlx_ptr, game->win_ptr, game->img_ptr, 0, 0);
}

bool touch(float px, float py, t_game *game)
{
    int x;
    int y;

    x = px / BLOCK;
    y = py / BLOCK;
    if (game->map[y][x] == '1')
        return (true);
    return (false);
}

float distance(float x, float y)
{
    return sqrt(x * x + y * y);
}


float fixed_dist(float x1, float y1, float x2, float y2, t_game *game)
{
    float delta_x;
    float delta_y;
    float angle;
    float fix_dist;

     
    delta_x = x2 - x1;
    delta_y = y2 - y1;
    angle = atan2(delta_y, delta_x) - game->player.angle;
    fix_dist = distance(delta_x, delta_y) * cos(angle);
    return (fix_dist);
}
void draw_line(t_player *player, t_game *game, float start_x, int i)
{
    float cos_angle;
    float sin_angle;
    float ray_x;
    float ray_y;
    float dist;
    float height;
    int start_y;
    int end;

    cos_angle = cos(start_x);
    sin_angle = sin(start_x);
    ray_x = player->x;
    ray_y = player->y;
    while(!touch(ray_x, ray_y, game))
    {
        if (DEBUG)
            put_pixel(ray_x, ray_y, 0xFF0000, game); // Draw ray in red
        ray_x += cos_angle;
        ray_y += sin_angle;
    }
    if (!DEBUG)
    {
        dist = fixed_dist(player->x, player->y, ray_x, ray_y, game);
        printf("Distance: %f\n", dist);
        height  = (BLOCK / dist) * (WIDTH / 2);
        start_y = (HEIGHT / 2) - (height / 2);
        end  = start_y + height;
        while(start_y < end)
            put_pixel(i, start_y++, 0x0000FF, game); // Draw vertical line in blue
    }
}
int draw_loop(t_game *game)
{
    t_player *player;
    float fraction;
    float start_x;
    int i;

    player = &game->player;
    move_player(player);
    clear_image(game);
    if (DEBUG)
    {
        draw_square(player->x, player->y, 10, 0x00FF00, game);
        draw_map(game);
    }
   

    fraction = PI / 3 / WIDTH;
    start_x = player->angle - PI / 6; // Start angle
    i = 0;
    while (i < WIDTH)
    {
        draw_line(player, game, start_x, i);
        start_x += fraction;
        i++;
    }
   
    mlx_put_image_to_window(game->mlx_ptr, game->win_ptr, game->img_ptr, 0, 0);
    return (0);
}

int main(void)
{
    t_game game;

    init_game(&game);

    
    mlx_hook(game.win_ptr, 2, 0, key_press, &game.player);
    mlx_hook(game.win_ptr, 3, 0, key_release, &game.player);

    mlx_loop_hook(game.mlx_ptr, draw_loop, &game);

    mlx_loop(game.mlx_ptr);
    return (0);
}