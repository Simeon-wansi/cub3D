#include "../includes/cub3d.h"
#include <stdio.h>

void	init_player(t_player *player)
{
    player->x = WIDTH / 2;
    player->y = HEIGHT / 2;
    player->angle = PI / 2; // Start facing upwards (90 degrees)

    player->key_up = false;
    player->key_down = false;
    player->key_left = false;
    player->key_right = false;

    player->left_rotate = false;
    player->right_rotate = false;
}

int key_press(int keycode, t_player *player)
{
    printf("Key pressed: %d\n", keycode);
    if (keycode == W)
        player->key_up = true;
    if (keycode == S)
        player->key_down = true;
    if (keycode == A)
        player->key_left = true;
    if (keycode == D)
        player->key_right = true;
    if (keycode == LEFT)
        player->left_rotate = true;
    if (keycode == RIGHT)
        player->right_rotate = true;
    return (0);
}

int key_release(int keycode, t_player *player)
{
    if (keycode == W)
        player->key_up = false;
    if (keycode == S)
        player->key_down = false;
    if (keycode == A)
        player->key_left = false;
    if (keycode == D)
        player->key_right = false;
    if (keycode == LEFT)
        player->left_rotate = false;
    if (keycode == RIGHT)
        player->right_rotate = false;
   return (0);
}


void move_player(t_player *player)
{
    int speed;
    float angle_speed;
    float cos_angle;
    float sin_angle;

    speed = 3;
    angle_speed = 0.03;
    cos_angle = cos(player->angle);
    sin_angle = sin(player->angle);

    if (player->left_rotate)
        player->angle -= angle_speed; // Rotate left
    if (player->right_rotate)
        player->angle += angle_speed; // Rotate right
    if (player->angle > 2 * PI)
        player->angle  = 0;
    if (player->angle < 0)
        player->angle = 2 * PI;

    
    if (player->key_up)
    {
        player->x += speed * cos_angle; // Move forward in the direction of the angle
        player->y += speed * sin_angle; // Move forward in the direction of the angle
    }
    if (player->key_down)
    {
        player->x -= speed * cos_angle; // Move backward in the direction of the angle
        player->y -= speed * sin_angle; // Move backward in the direction of the angle
    }
    if (player->key_left)
    {
        player->x += speed * cos_angle; // Move left perpendicular to the angle
        player->y -= speed * sin_angle; // Move left perpendicular to the angle
    }
    if (player->key_right)
    {
        player->x -= speed * cos_angle; // Move right perpendicular to the angle
        player->y += speed * sin_angle; // Move right perpendicular to the angle
    }

}
