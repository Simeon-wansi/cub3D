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
# define WINDOW_WIDTH 1024
# define WINDOW_HEIGHT 1024

/* ========== GAME CONSTANTS ========== */
# define MOVE_SPEED 0.1
# define ROTATE_SPEED 0.05
# define BLOCK_SIZE 64



/* ========== KEY CODES (macOS) ========== */
# define KEY_ESC 53
# define KEY_W 13
# define KEY_A 0
# define KEY_S 1
# define KEY_D 2
# define KEY_LEFT 123
# define KEY_RIGHT 124

/* ========== COLORS ========== */
# define COLOR_RED 0xFF0000
# define COLOR_GREEN 0x00FF00
# define COLOR_BLUE 0x0000FF
# define COLOR_WHITE 0xFFFFFF
# define COLOR_BLACK 0x000000

/* ========== MAP CHARACTERS ========== */
# define EMPTY '0'
# define WALL '1'
# define PLAYER_NORTH 'N'
# define PLAYER_SOUTH 'S'
# define PLAYER_EAST 'E'
# define PLAYER_WEST 'W'
# define SPACE ' '

/* ========== PLAYER DIRECTIONS ========== */
# define NORTH 0
# define SOUTH 1
# define EAST 2
# define WEST 3

/* ========== DEBUG & ERROR ========== */
# define DEBUG 0
# define ERROR_MSG "Error\n"

/* ========== STRUCTURES ========== */

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

	t_color		floor_color;
	t_color		ceiling_color;
}	t_map;

typedef struct s_player
{
	double	x;
	double	y;
	double	angle;
	int		direction;
	
	//direction vector 
	double dx;
	double dy;

	//field of view
	double	fov;		/* Field of view in radians */

	/* Movement flags */
	bool	move_up;
	bool	move_down;
	bool	move_left;
	bool	move_right;
	bool	rotate_left;
	bool	rotate_right;
}	t_player;

typedef struct s_ray
{
	double	start_x;
	double	start_y;
	double	dir_x;
	double	dir_y;
	double	angle;
	double	distance;
	int		hit_wall;
	int		wall_side;		/* 0 = NS wall, 1 = EW wall */
	double	wall_x;			/* exact hit point on wall for texturing */
}	t_ray;


typedef struct s_texture
{
	t_img	north;
	t_img	south;
	t_img	west;
	t_img	east;
}	t_texture;


typedef struct s_wall
{
	double		wall_height;		/* Height of the wall in pixels */
	char 		*wall_texture;	/* Pointer to the wall texture */
	t_img		texture_img;	/* Image structure for the wall texture */
	int		bottom;		/* Bottom Y coordinate of the wall */
	int		color;		/* Color of the wall */
	int		texture_x;	/* X coordinate in the texture */
	int		texture_y;	/* Y coordinate in the texture */

} 			t_wall;

typedef struct s_game
{
	void		*mlx_ptr;
	void		*win_ptr;
	t_img		win_img;
	
	t_map		map;
	t_player	player;
	t_texture	textures;

	t_ray		rays[WINDOW_WIDTH];	/* Array of rays for raycasting */

	int         floor_color;
	int         ceiling_color;
	bool		game_running;
}	t_game;

/* ========== FUNCTION PROTOTYPES ========== */

/* Error handling */
void	error_exit(char *msg);

/* Map parsing */
// int		parse_map_file(char *filename, t_game *game);
// int		validate_map(t_map *map);
// void	init_player_position(t_game *game);

/* Game initialization */
int init_images_and_textures(t_game *game);
// int		init_images(t_game *game);
void	init_player_from_map(t_game *game);
// int		init_game(t_game *game);
// void	init_mlx(t_game *game);
// int		load_textures(t_game *game);

/* Input handling */
// int		key_press(int keycode, t_game *game);
// int		key_release(int keycode, t_game *game);
// int		close_game(t_game *game);

/* Player movement */
// void	update_player(t_game *game);
// void	move_player(t_game *game);
// void	rotate_player(t_game *game);

/* Raycasting */
void	cast_rays(t_game *game);
void 	cast_single_ray(t_game *game, t_ray *ray);
// void	cast_single_ray(t_game *game, t_ray *ray, double angle);
// double	calculate_distance(t_player *player, t_ray *ray);

/* Rendering */
void render_ground_and_ceiling(t_game *game);
// int		render_frame(t_game *game);
void	draw_walls(t_game *game);
// void	draw_floor_ceiling(t_game *game);
void	put_pixel(t_img *img, int x, int y, int color);


/* Utilities */
// double	normalize_angle(double angle);
// int		is_wall(t_map *map, int x, int y);
// void	free_game(t_game *game);


#endif