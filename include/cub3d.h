/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/07/25 21:08:20 by fpedroso         ###   ########.fr       */
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

# define SCR_W 500
# define SCR_H 500

# define BLUE	0x0000FFFF
# define RED	0xFF0000FF
# define PURPLE	0xFF00FFFF
# define BLACK	0xFFFAFAFA

# define FOV       (60.0 * M_PI / 180.0)
# define HALF_FOV  (30.0 * M_PI / 180.0)

/* World space is measured in tiles. PL_RADIUS is the player's half-width
 * and drives both collision and the minimap dot, so what is drawn is
 * exactly the footprint that collides. */
# define PL_RADIUS 0.25
# define MOVE_SPEED 0.06
# define PAN_INCR 3.0

/* Minimap image size, as a fraction of the window. MM_MARGIN is the
 * border kept clear inside it; MM_RAY_STEP thins the ray fan, which is
 * far denser than a 125px image can show. */
# define MINIMAP_SCALE 0.25
# define MM_MARGIN 2
# define MM_RAY_STEP 8

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

/* Everything here is derived from the map dimensions at init and is
 * constant for the run: scale converts world tiles to minimap pixels,
 * off_x/off_y centre the map inside the image. */
typedef struct s_minimap
{
	double	scale;
	double	off_x;
	double	off_y;
	double	radius;
}	t_minimap;

typedef struct s_game
{
	mlx_t		*mlx;
	mlx_image_t	*map_img;
	mlx_image_t	*main_img;
	uint8_t		*map_pixels_buf;
	t_ray		rays[SCR_W];
	bool		show_minimap;
	bool		show_rays;
	t_map		map;
	t_minimap	minimap;
	t_config	config;
	t_player	player;
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

/* ========================================================================== */
/*                                   MLX                                      */
/* ========================================================================== */
bool	config_mlx(t_game *game);

/* ========================================================================== */
/*                                  PARSING                                   */
/* ========================================================================== */
int		has_cub_extension(const char *filename);
int		has_png_extension(const char *filename);
char	*trim_newline(char *line);
int		parse_texture(const char *line, char **dest);
int		parse_color(const char *line, int dest[3]);
int		has_closed_walls(char **map, int rows);
int		parse_map_grid(int fd, t_map *map, char *first_map_line, t_player *player);
int		parse_meta(int fd, t_config *config, char **first_map_line);
int		parse_cub(const char *path, t_game *game);
int		find_player(char **map, t_player *player);
int		pad_grid(t_map *map);

/* ========================================================================== */
/*                                RENDERING                                   */
/* ========================================================================== */
void	render(t_game *game);
void	compute(t_game *game);
void	draw_frame(t_game *game);
void	draw_minimap(t_game *game);
void	update_pl_position(t_game *game);

/* ========================================================================== */
/*                              RENDER INIT                                   */
/* ========================================================================== */
bool		init_render(t_game *game);
void		init_fov_lut(t_game *game);
bool		init_minimap(t_game *game);
void		bake_minimap_bg(t_game *game);
bool		is_in_tile(t_game *game, uint32_t px_x, uint32_t px_y);
t_dbl_coord	tile_to_px(t_minimap *mm, t_dbl_coord tile);

/* ========================================================================== */
/*                                RAYCASTING                                  */
/* ========================================================================== */
void	cast_rays(t_game *game);
void	draw_ray(t_game *game, int index);

/* ========================================================================== */
/*                                  UTILS                                     */
/* ========================================================================== */

int		handle_error(t_error error);
bool	is_solid(char c);
void	free_map(char **map);
void	free_visited(char **visited);
void	free_visited_partial(char **visited, int until);
int		color_error(char **parts);
void	free_game(t_game *game);

#endif
