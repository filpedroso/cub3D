/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/06/07 19:57:50 by mona             ###   ########.fr       */
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
# define WIDTH 600
# define HEIGHT 400
# define SCR_W 500
# define SCR_H 500
# define SQUARE_SZ 40
# define MAP_W 10
# define MAP_H 10
# define MAP_PX_H (SQUARE_SZ * MAP_H)
# define MAP_PX_W (SQUARE_SZ * MAP_W)
# define BLUE 0x0000FFFF
# define BLACK 0xFFFAFAFA
# define CIRCLE_R 10

/* ========================================================================== */
/*                                   ENUMS                                    */
/* ========================================================================== */

// Error types
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
	ERR_INVALID_ID
}	t_error;

/* ========================================================================== */
/*                                 STRUCTURES                                 */
/* ========================================================================== */


// typedef struct s_map
// {
// 	int	grid[WIDTH][HEIGHT];
// } t_map;

typedef struct	s_config
{
	char	*tex_north;
	char	*tex_south;
	char	*tex_west;
	char	*tex_east;
	int		floor[3];
	int		ceil[3];
}	t_config;

typedef struct	s_map
{
	char	**grid;
	int		rows;
	int		cols;
}	t_map;

typedef struct	s_player
{
	double	x;
	double	y;
	char	dir;
}	t_player;


typedef struct	s_game
{
	mlx_t		*mlx;
	mlx_image_t	*image;
	t_map		map;
	t_config	config;
	t_player	player;
}	t_game;


/* ========================================================================== */
/*                                  PARSING                                   */
/* ========================================================================== */
int		has_cub_extension(const char *filename);
int		has_png_extension(const char *filename);
char	*trim_newline(char *line);
int		parse_map_grid(int fd, t_map *map, char *first_map_line, t_player *player);
int		parse_meta(int fd, t_config *config, char **first_map_line);
int		parse_cub(const char *path, t_game *game);
int		find_player(char **map, t_player *player);

/* ========================================================================== */
/*                                RAYCASTING                                  */
/* ========================================================================== */



/* ========================================================================== */
/*                                  UTILS                                     */
/* ========================================================================== */

// Error handling
int	handle_error(t_error error);

//free
void	free_map(char **map);
void	free_visited(char **visited);
void	free_visited_partial(char **visited, int until);

#endif