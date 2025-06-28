#ifndef CUB3D_H
# define CUB3D_H

# include "minilibx_opengl_20191021/mlx.h"
#include "./libft/libft.h"
#include "./libft/arena.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>

# define WIDTH 1280 
# define HEIGHT 720

# define BLOCK 64
# define MAP_WIDTH 24
# define LEFT 123
# define RIGHT 124
# define W 13
# define A 0
# define S 1
# define D 2

# define PI 3.1415926

# define DEBUG 1

typedef struct s_textures
{
    char    *north;
    char    *south;
    char    *west;
    char    *east;
}   t_textures;

typedef struct s_colors
{
    int floor_r;
    int floor_g;
    int floor_b;
    int ceiling_r;
    int ceiling_g;
    int ceiling_b;
}   t_colors;

typedef struct s_map
{
    char    **board;
    int     cols;
    int     rows;
    int     player_pos_x;
    int     player_pos_y;
    char    player_dir;
}   t_map;

typedef struct s_config
{
    t_textures  textures;
    t_colors    colors;
    t_map       map;
    bool        parsed_no;
    bool        parsed_so;
    bool        parsed_we;
    bool        parsed_ea;
    bool        parsed_f;
    bool        parsed_c;
}   t_config;


typedef struct s_player
{
    float x;
    float y;
    float angle;
    bool key_up;
    bool key_down;
    bool key_left;
    bool key_right;
    bool left_rotate;
    bool right_rotate;
}       t_player;


typedef struct s_game
{
    void *mlx_ptr;
    void *win_ptr;
    void *img_ptr;

    char *data;
    int bpp;
    int size_line;
    int endian;

    char **map;
    t_player player;
}       t_game;


void	init_player(t_player *player);
int key_press(int keycode, t_player *player);
int key_release(int keycode, t_player *player);
void move_player(t_player *player);

// Parser functions
t_config *parse_cub_file(char *filename, t_arena *arena);
void print_error(char *message);
void print_config(t_config *config);

# endif