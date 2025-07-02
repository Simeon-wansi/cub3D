#ifndef CUB3D_H
# define CUB3D_H

/* ========== INCLUDES ========== */
# include "minilibx_opengl_20191021/mlx.h"
# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <stdbool.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>

/* ========== WINDOW CONSTANTS ========== */
# define WINDOW_WIDTH 750
# define WINDOW_HEIGHT 700
# define MINIMAP_SCALE 0.2

/* ========== GAME CONSTANTS ========== */
# define MOVE_SPEED 5.0
# define ROTATE_SPEED 0.05
# define BLOCK_SIZE 40
# define PLAYER_SIZE 0.3


/* ========== KEY CODES (macOS) ========== */
# define KEY_ESC 53
# define KEY_W 13
# define KEY_A 0
# define KEY_S 1
# define KEY_D 2
# define KEY_LEFT 123
# define KEY_RIGHT 124

# define X_EVENT_KEY_PRESS 2
# define X_EVENT_KEY_RELEASE 3
# define X_EVENT_KEY_EXIT 17

/* ========== COLORS ========== */
# define COLOR_RED 0xFF0000
# define COLOR_GREEN 0x00FF00
# define COLOR_BLUE 0x0000FF
# define COLOR_WHITE 0xFFFFFF
# define COLOR_BLACK 0x000000
# define COLOR_CYAN 0x00FFFF
# define COLOR_YELLOW 0xFFFF00

/* ========== MAP CHARACTERS ========== */
# define EMPTY '0'
# define WALL '1'
# define PLAYER_NORTH 'N'
# define PLAYER_SOUTH 'S'
# define PLAYER_EAST 'E'
# define PLAYER_WEST 'W'
# define SPACE ' '


# define TEX_NORTH 2
# define TEX_SOUTH 3
# define TEX_WEST 0
# define TEX_EAST 1
# define TEX_NONE -1
# define TEX_WALL 4
/* ========== PLAYER DIRECTIONS ========== */
# define NORTH 0
# define SOUTH 1
# define EAST 2
# define WEST 3

/* ========== DEBUG & ERROR ========== */
# define DEBUG 0
# define ERROR_MSG "Error\n"

/* ========== STRUCTURES ========== */

typedef struct s_game t_game; // Forward declaration

typedef struct s_color
{
	int	red;
	int	green;
	int	blue;
}	t_color;

typedef struct s_img
{
	void	*img_ptr;
	char	*addr;
	int		bpp;
	int		line_length;
	int		endian;
	int		width;
	int		height;
}	t_img;

typedef struct s_map
{
	int		width;
	int		height;
	char	**grid;
	char	*no_texture_path;
	char	*so_texture_path;
	char	*we_texture_path;
	char	*ea_texture_path;
	char    player_dir; // 'N', 'S', 'E', or 'W'
	int 		player_x; // Player X position in grid
	int 		player_y; // Player Y position in grid

	t_color		floor_color;
	t_color		ceiling_color;
}	t_map;

typedef struct s_player
{
	double	x;
	double	y;
	double	angle;

	float	dx;	/* Direction vector X */
	float	dy;	/* Direction vector Y */


	/* Movement flags */
	bool	move_up;
	bool	move_down;
	bool	move_left;
	bool	move_right;
	bool	rotate_left;
	bool	rotate_right;
	t_game	*game;	/* Pointer to the game structure for access to game state */
}	t_player;

typedef struct s_ray
{
	double	cos_angle;
	double	sin_angle;
	double	ray_x;
	double	ray_y;
	double  prev_x;
	double 	prev_y;
	double  dist;
	int     tex;
	double  tex_pos;
}	t_ray;


typedef struct s_texture
{
	int width;
	int height;
	void *img_ptr;
	char *addr;
	int bpp;
	int line_length;
	int endian;
}	t_texture;


typedef struct s_wall
{
	double		wall_height;	
	char 		*wall_texture;	
	t_img		texture_img;
	int		texture_x;	
	int		texture_y;
	int 	wall_y;
} 			t_wall;

typedef struct s_game
{
	void		*mlx_ptr;
	void		*win_ptr;
	t_img		win_img;
	 
	t_map		map;
	t_player	player;
	t_texture	textures[4];
	int         floor_color;
	int         ceiling_color;
	bool		game_running;

	t_wall		wall;
}	t_game;


typedef struct s_render
{
	int height;	
	int start_y;
	int end;
	double step;
	int texture_x;	
	int texture_y;
	int color;		
}   t_render;
/* ========== FUNCTION PROTOTYPES ========== */



// int mlx_pixel_put_img(t_game *game, int x, int y, int color);
void init_colors(t_game *game);
int find_max_width(char *map_str);
char *trim_whitespace(char *str);
int count_map_rows(char *map_str);
void clear_image(t_game *game);
void init_map(t_map *map, char *map_path);
void init_player_from_map(t_game *game, t_player *player);
int close_game(t_game *game);
t_color parse_color(char *color_str);
int key_press(int key_code, t_game *game);
int key_release(int key_code, t_game *game);
void put_pixel(int x, int y, int color, t_game *game);
bool colision_check(double px, double py, t_game *game);
bool is_collision(double x, double y, t_game *game);
void move_player(t_player *player);
void rotate_player(t_player *player);
void move_right(t_player *player);
void move_left(t_player *player);
void move_down(t_player *player);
void move_up(t_player *player);
int game_loop(t_game *game);
void load_texture(t_game *game);
void draw_square(int x, int y, int size, int color, t_game *game);
void draw_minimap(t_game *game);
int close_game(t_game *game);
// void free_map(char **map);
#endif