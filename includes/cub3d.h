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
# include <sys/time.h>
# include <time.h>


/* ========== WINDOW CONSTANTS ========== */
# define WINDOW_WIDTH 750
# define WINDOW_HEIGHT 600
# define MINIMAP_SCALE 0.2

/* ========== GAME CONSTANTS ========== */
# define MOVE_SPEED 5.0
# define ROTATE_SPEED 0.05
# define BLOCK_SIZE 40
# define PLAYER_SIZE 12


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
# define X_EVENT_MOUSE_MOVE 6
# define MOUSE_SENSITIVITY 0.01

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

/* ========== OPTIMISATION STRUCTURE ========== */

typedef struct s_dda
{
	double delta_x;
	double delta_y;
	int step_x;
	int step_y;
	double side_dist_x;
	double side_dist_y;
	int map_x;
	int map_y;
	int side;
} t_dda;


typedef struct s_timing
{
	double last_frame_time;
	double delta_time;
	double fps;
	double frame_start;
} t_timing;


typedef struct s_perf
{
	double frame_time;
	double render_time;
	double logic_time;
	int ray_cast;
	int pixels_drawn;
	double avg_fps;
	double fps_samples[60];
	int sample_index;
	double frame_start;
} t_perf;

typedef struct s_smooth_movement
{
	double velocity_x;
	double velocity_y;
	double acceleration;
	double deceleration;
	double max_speed;
} t_smooth_movement;

typedef struct s_loading
{
	int textures_loaded;
	int total_textures;
	bool loading_complete;
} t_loading;


typedef struct s_mouse
{
	int last_x;
	bool is_active;
} t_mouse;

/* ========== STRUCTURES ========== */

typedef struct s_game t_game; // Forward declaration

typedef struct s_point
{
	int x;
	int y;
} t_point;

typedef struct s_dpoint
{
    double x;
    double y;
} t_dpoint;

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

	t_smooth_movement smooth;
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
	char		**tex_paths;

	t_wall		wall;
	t_timing timing;
	t_perf perf;
	t_loading loading;
	t_mouse mouse;
	t_texture text_gun1;
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

//Performance functions
double get_time(void);
void init_timing(t_game *game);
void init_performance(t_game *game);
void update_timing(t_game *game);
void track_performance(t_game *game);
// void draw_debug_overlay(t_game *game);
void fast_clear_image(t_game *game);
void draw_vertical_line(t_game *game, int x, int start_y, int end_y, int color);
double lerp (double a, double b, double t);
void init_smooth_movement(t_player *player);
bool swept_collision_check(t_player *player, double new_x, double new_y, t_game *game);
void smooth_player_movement(t_player *player, double delta_time);

// TExttures
void load_texture(t_game *game);
void create_fallback_texture(t_game *game, int tex_index);
int validate_texture_file(char *texture_path);
// void show_loading_progress( t_game *game , int progress);

//ray
void get_wall_texture(t_ray *ray);
int perform_dda(t_dda *dda, t_game *game);


//Debug 
// void debug_texture_info(t_game *game);

void	init_game(t_game *game);

void exit_error(char *message);
// int mlx_pixel_put_img(t_game *game, int x, int y, int color);
void init_colors(t_game *game);
// int find_max_width(char *map_str);
// char *trim_whitespace(char *str);
// int count_map_rows(char *map_str);
// void clear_image(t_game *game);
// void init_map(t_map *map, char *map_path);
void init_player_from_map(t_game *game, t_player *player);
int close_game(t_game *game);
// t_color parse_color(char *color_str);
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

// void draw_square(int x, int y, int size, int color, t_game *game);
void draw_minimap(t_game *game);
void draw_line(t_point p0, t_point p1, int color, t_game *game);
void draw_filled_square(t_point point, int size, int color, t_game *game);
int close_game(t_game *game);
// void free_map(char **map);

void update_player_direction(t_player *player);
int mouse_move(int x, int y, t_game *game);
int init_mouse(t_game *game);


#endif