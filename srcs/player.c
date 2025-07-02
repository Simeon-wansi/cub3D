#include "../includes/cub3d.h"

void init_player_from_map(t_game *game, t_player *player)
{
    player->x = (game->map.player_x + 0.5) * BLOCK_SIZE; // Center player on block
    player->y = (game->map.player_y + 0.5) * BLOCK_SIZE; // Center player on block
    if (game->map.player_dir == 'N')
        player->angle = M_PI_2; // Facing North
    else if (game->map.player_dir == 'S')
        player->angle = 3 * M_PI_2; // Facing South
    else if (game->map.player_dir == 'E')
        player->angle = 0; // Facing East
    else if (game->map.player_dir == 'W')
        player->angle = M_PI; // Facing West
    player->dx = cos(player->angle);
    player->dy = sin(player->angle);
    // Initialize movement flags
    player->move_up = false;
    player->move_down = false;
    player->move_left = false;
    player->move_right = false;
    player->rotate_left = false;
    player->rotate_right = false;
}

int key_press(int key_code, t_game *game)
{
    t_player *player;

    player = &game->player;
    if (key_code == KEY_W)
        player->move_up = true;
    else if (key_code == KEY_S)
        player->move_down = true;
    else if (key_code == KEY_A)
        player->move_left = true;
    else if (key_code == KEY_D)
        player->move_right = true;
    else if (key_code == KEY_LEFT)
        player->rotate_left = true;
    else if (key_code == KEY_RIGHT)
        player->rotate_right = true;
    else if (key_code == KEY_ESC)
        close_game(game);
    return 0;
}

int key_release(int key_code, t_game *game)
{
    t_player *player;

    player = &game->player;
    if (key_code == KEY_W)
        player->move_up = false;
    else if (key_code == KEY_S)
        player->move_down = false;
    else if (key_code == KEY_A)
        player->move_left = false;
    else if (key_code == KEY_D)
        player->move_right = false;
    else if (key_code == KEY_LEFT)
        player->rotate_left = false;
    else if (key_code == KEY_RIGHT)
        player->rotate_right = false;
    return 0;
}

void rotate_player(t_player *player)
{
    if (player->rotate_left)
        player->angle -= ROTATE_SPEED;
    if (player->rotate_right)
        player->angle += ROTATE_SPEED;
    
    // Normalize angle to [0, 2*PI]
    if (player->angle < 0)
        player->angle += 2 * M_PI;
    else if (player->angle >= 2 * M_PI)
        player->angle -= 2 * M_PI;
}
bool colision_check(double px, double py, t_game *game)
{
    int x;
    int y;

    x = (int)(px / BLOCK_SIZE);
    y = (int)(py / BLOCK_SIZE);
    // Check if the coordinates are within the bounds of the map
    if (x < 0 || y < 0 || y >= game->map.height || x >= game->map.width)
        return true; // Out of bounds, treat as collision
    
    // Additional safety check
    if (!game->map.grid[y])
        return true;
    
    return (game->map.grid[y][x] == '1'); // Check if the cell is not empty
}

bool is_collision(double x, double y, t_game *game)
{
    if (colision_check(x - PLAYER_SIZE, y - PLAYER_SIZE, game)) // TOP left corner
        return true;
    if (colision_check(x + PLAYER_SIZE, y - PLAYER_SIZE, game)) // TOP right corner
        return true;
    if (colision_check(x - PLAYER_SIZE, y + PLAYER_SIZE, game)) // BOTTOM left corner
        return true;
    if (colision_check(x + PLAYER_SIZE, y + PLAYER_SIZE, game)) // BOTTOM right corner
        return true;
    return false; // No collision
}






void move_player(t_player *player)
{
    player->dx = cos(player->angle);
    player->dy = sin(player->angle);
    rotate_player(player);
    move_up(player);
    move_down(player);
    move_left(player);
    move_right(player);
}