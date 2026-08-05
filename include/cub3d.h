/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/08/04 21:16:57 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <math.h>
# include <fcntl.h>
# include "../MLX42/include/MLX42/MLX42.h"
# include "../libft/libft.h"

/* ========================================================================== */
/*                                  DEFINES                                   */
/* ========================================================================== */

# define SUCCESS 0
# define ERROR -1
# define TRUE 1
# define FALSE 0

# define BLUE	0x0000FFFF
# define RED	0xFF0000FF
# define PURPLE	0xFF00FFFF
# define BLACK	0xFFFAFAFA
# define GRAY	0xAAAAAAFF

// PALLETE 1
# define CYAN		0x77FFFFFF
# define VIOLET		0x9B30FFFF
# define GOLD		0xFFC020FF
# define OFFWHITE	0xF0F0E6FF

// PALLETTE 2
/* Every colour here is 0xRRGGBBAA, so the trailing FF is the opaque
 * alpha and is not optional: 0xe0ecf3 would be read as R=00 G=e0 B=ec
 * with alpha f3, which is a different colour and slightly transparent. */
# define BLUE1	0xe0ecf3FF
# define BLUE2	0xbdd2dcFF
# define BLUE3	0x98b2c4FF
# define BLUE4	0x4e6d86FF

/* 3D view, debug palette. One colour per wall face so a corner shows at a
 * glance which face the ray convention picked, plus a black floor ruled
 * with neon pink on the world tile boundaries: a wrong horizon, a bad
 * fisheye correction or a wrong distance scale all show up as bent or
 * unevenly spaced lines, which a flat floor would hide. */
# define SKY			0x0A1433FF
# define FLOOR			GRAY
# define FLOOR_LINES	OFFWHITE
# define FACE_N		BLUE1
# define FACE_S		BLUE2
# define FACE_W		BLUE3
# define FACE_E		BLUE4

/* -------------------------------------------------------------------- */
/*  TUNABLE. These are the knobs. Values that would need arithmetic on   */
/*  them (Norminette forbids computed #defines) are derived once, at    */
/*  init time, into t_game fields or plain locals instead — see          */
/*  init_render, init_fov_lut and init_minimap_geometry in               */
/*  render_init.c.                                                       */
/* -------------------------------------------------------------------- */

/* Window, in pixels. */
# define SCR_W 900
# define SCR_H 600

/* Camera aperture, in degrees. */
# define FOV_DEG 60.0

/* Player. World space is measured in tiles, so PL_RADIUS is a half-width
 * in tiles and MOVE_SPEED is tiles per frame; PAN_INCR is degrees per
 * frame. PL_RADIUS drives both collision and the minimap dot, so what is
 * drawn is exactly the footprint that collides. */
# define PL_RADIUS 0.25
# define MOVE_SPEED 0.04
# define PAN_INCR 2.0

/* Minimap. MINIMAP_SCALE is the largest fraction of the window it may
 * occupy: the map is fitted inside that box preserving aspect ratio, and
 * the resulting image is anchored flush into the bottom-left corner.
 * MM_MARGIN insets the map from the image edge, so 0 puts the map hard
 * against the window corner. MM_RAY_STEP thins the ray fan, which is far
 * denser than a minimap can resolve. */
# define MINIMAP_SCALE 0.25
# define MM_MARGIN 0
# define MM_RAY_STEP 10

/* Target on-screen width, in pixels, of a floor grid line. The world-space
 * threshold is derived from this per row, so a line keeps roughly this
 * width all the way to the horizon instead of fattening up close and
 * thinning out to nothing far away. */
# define GRID_PX 1.5

/* One ray per screen column. */
# define RAY_COUNT SCR_W

/* ========================================================================== */
/*                                   ENUMS                                    */
/* ========================================================================== */

typedef enum e_error
{
	ERR_NONE = 0,
	ERR_ARGS,
	ERR_NO_FILE,
	ERR_NO_CUB,
	ERR_MALLOC,
	ERR_MAP_CHARS,
	ERR_MAP_OPEN,
	ERR_MAP_PLAYER,
	ERR_MISSING_TEX,
	ERR_INVALID_COLOR,
	ERR_INVALID_ID,
	ERR_MLX
}	t_error;

/* Which of the four wall faces a ray struck, named after the direction
 * the surface faces. Doubles as the index into t_game.tex, so the whole
 * pipeline stays indexed instead of repeating a four-way branch. The
 * order is fixed by that: it must match how load_textures fills tex[]. */
typedef enum e_face
{
	F_NORTH = 0,
	F_SOUTH,
	F_WEST,
	F_EAST
}	t_face;

/* ========================================================================== */
/*                                 STRUCTURES                                 */
/* ========================================================================== */

typedef struct s_config
{
	char	*tex_north;
	char	*tex_south;
	char	*tex_west;
	char	*tex_east;
	int		floor[3];
	int		ceil[3];
}	t_config;

typedef struct s_map
{
	char	**grid;
	int		rows;
	int		cols;
}	t_map;

typedef struct s_player
{
	double	x;
	double	y;
	char	dir;
	float	dir_ang;
}	t_player;

typedef struct s_dbl_coord
{
	double	x;
	double	y;
}	t_dbl_coord;

typedef struct s_int_coord
{
	int32_t	x;
	int32_t	y;
}	t_int_coord;

typedef struct s_int_dir
{
	int32_t	up;
	int32_t	down;
	int32_t	left;
	int32_t	right;
}	t_int_dir;

typedef struct s_point
{
	uint32_t	x;
	uint32_t	y;
}	t_point;

/* cos_off/sin_off are the cosine and sine of this column's fixed angular
 * offset from the view direction. They are filled once by init_fov_lut
 * and never touched again, which lets cast_rays rotate the whole fan
 * from a single cos/sin per frame. cos_off doubles as the fisheye
 * correction factor, since perp_dist = hit_dist * cos(angle - view). */
typedef struct s_ray
{
	double		cos_off;
	double		sin_off;
	t_dbl_coord	dir;
	double		hit_dist;
	double		perp_dist;
	int			side;
}	t_ray;

/* Fixed for the run, computed from the map dimensions before the images
 * exist: scale converts world tiles to minimap pixels, width/height are
 * the image size that fits the map at that scale, and off_x/off_y are
 * where the map starts inside it. */
typedef struct s_minimap
{
	double		scale;
	double		off_x;
	double		off_y;
	double		radius;
	uint32_t	width;
	uint32_t	height;
}	t_minimap;

/* tex is indexed by t_face, so a column goes straight from the face it
 * hit to the texture to sample. ceil_rgba/floor_rgba are the parsed F/C
 * colours packed once at init, since the frame loop wants a single
 * uint32_t and config keeps them as three ints. horizon and proj_plane
 * are likewise computed once at init (see init_render): horizon is
 * where floor meets ceiling and where a wall of infinite distance
 * collapses to; proj_plane converts a world length at a given
 * perpendicular distance into pixels, px = len * proj_plane / d. */
typedef struct s_game
{
	mlx_t			*mlx;
	mlx_image_t		*map_img;
	mlx_image_t		*main_img;
	uint8_t			*map_pixels_buf;
	mlx_texture_t	*tex[4];
	uint32_t		ceil_rgba;
	uint32_t		floor_rgba;
	int32_t			horizon;
	double			proj_plane;
	t_ray			rays[RAY_COUNT];
	bool			show_minimap;
	bool			show_rays;
	bool			show_tex;
	t_map			map;
	t_minimap		minimap;
	t_config		config;
	t_player		player;
}	t_game;

typedef struct s_canvas_ray
{
	t_dbl_coord	start;
	t_dbl_coord	end;
	t_dbl_coord	diff;
}	t_canvas_ray;

typedef struct s_casting_ray
{
	t_dbl_coord	delta_dist;
	t_int_coord	map_pos;
	t_int_coord	step_dir;
	t_dbl_coord	side_dist;
	int			side;
}	t_casting_ray;

/* One vertical strip of the 3D view, resolved from a t_ray before any
 * pixel is written. start and end are already clamped into the window,
 * because mlx_put_pixel asserts rather than clips; line_h is the same
 * height left unclamped, which is what lets a wall taller than the
 * window start partway down its texture instead of squashing a whole
 * copy into view. plane is the ray direction divided by cos_off: the
 * floor cast needs euclidean distance along the ray, and dividing out
 * the fisheye factor once per column gives it without a per-pixel trig
 * call. */
typedef struct s_column
{
	int32_t		x;
	int32_t		start;
	int32_t		end;
	double		line_h;
	t_face		face;
	t_dbl_coord	plane;
}	t_column;

/* ========================================================================== */
/*                                   MLX                                      */
/* ========================================================================== */
bool		config_mlx(t_game *game);

/* ========================================================================== */
/*                                  PARSING                                   */
/* ========================================================================== */
int			has_cub_extension(const char *filename);
int			has_png_extension(const char *filename);
char		*trim_newline(char *line);
int			parse_texture(const char *line, char **dest);
int			parse_color(const char *line, int dest[3]);
int			has_closed_walls(char **map, int rows);
int			parse_map_grid(int fd, t_map *map, char *first_map_line,
				t_player *player);
int			parse_meta(int fd, t_config *config, char **first_map_line);
int			parse_cub(const char *path, t_game *game);
int			find_player(char **map, t_player *player);
int			pad_grid(t_map *map);

/* ========================================================================== */
/*                                RENDERING                                   */
/* ========================================================================== */
void		render(t_game *game);
void		compute(t_game *game);
void		draw_frame(t_game *game);
void		draw_minimap(t_game *game);
void		draw_3d(t_game *game);
void		draw_view_tex(t_game *game);
void		draw_view_dbg(t_game *game);
void		draw_floor(t_game *game, t_column *col);
void		update_pl_position(t_game *game);
t_column	prep_column(t_game *game, int index);

/* ========================================================================== */
/*                              RENDER INIT                                   */
/* ========================================================================== */
void		init_minimap_geometry(t_game *game);
bool		load_textures(t_game *game);
void		free_textures(t_game *game);
bool		init_render(t_game *game);
void		init_fov_lut(t_game *game);
void		bake_minimap_bg(t_game *game);
bool		is_in_tile(t_game *game, uint32_t px_x, uint32_t px_y);
t_dbl_coord	tile_to_px(t_minimap *mm, t_dbl_coord tile);

/* ========================================================================== */
/*                                RAYCASTING                                  */
/* ========================================================================== */
void		cast_rays(t_game *game);
void		draw_ray(t_game *game, int index);

/* ========================================================================== */
/*                                  UTILS                                     */
/* ========================================================================== */

int			handle_error(t_error error);
bool		is_solid(char c);
void		free_map(char **map);
void		free_visited(char **visited);
void		free_visited_partial(char **visited, int until);
int			color_error(char **parts);
void		free_game(t_game *game);

#endif
