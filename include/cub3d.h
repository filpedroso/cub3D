/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/06/01 14:07:44 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <math.h>
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
	ERR_NONE,
	ERR_SYNTAX,
	ERR_CMD_NOT_FOUND,
	ERR_NO_FILE,
	ERR_PERMISSION,
	ERR_MALLOC,
	ERR_TOO_MANY_ARGS,
	ERR_NUM_REQUIRED,
	ERR_NOT_VALID_ID,
	ERR_HOME_NOT_SET,
	ERR_OLDPWD_NOT_SET
}	t_error;

/* ========================================================================== */
/*                                 STRUCTURES                                 */
/* ========================================================================== */


typedef struct s_map
{
	int	grid[WIDTH][HEIGHT];
} t_map;


typedef struct	s_game
{
	mlx_t		*mlx;
	mlx_image_t	*image;
	t_map	*map;
} t_game;


/* ========================================================================== */
/*                                  PARSING                                   */
/* ========================================================================== */

/* ========================================================================== */
/*                                RAYCASTING                                  */
/* ========================================================================== */


/* ========================================================================== */
/*                                  UTILS                                     */
/* ========================================================================== */

// Error handling

#endif