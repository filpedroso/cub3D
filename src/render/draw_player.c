/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 15:49:44 by fpedroso          #+#    #+#             */
/*   Updated: 2026/06/16 18:42:10 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void		update_position(t_game *game);
static void		redraw(t_game *game);
static bool		belongs_in_circle(uint32_t y, uint32_t x);
static bool		will_collide(t_game *game, int32_t x_incr, int32_t y_incr);
void			pan_side(t_game *game, char side);
void			walk(t_game *game, char dir);

void	draw_player(void* param)
{
	t_game	*game;

	game = (t_game *)param;
	update_position(game);
	redraw(game);
}

static void	update_position(t_game *game)
{
	if (mlx_is_key_down(game->mlx, MLX_KEY_UP))
		walk(game, 'F');
	if (mlx_is_key_down(game->mlx, MLX_KEY_DOWN))
		walk(game, 'B');
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		pan_side(game, 'L');
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		pan_side(game, 'R');
}

void	walk(t_game *game, char dir)
{
	double	rad;
	int32_t	dx;
	int32_t	dy;

	rad = game->player.dir_ang * (M_PI / 180.0);
	dx = (int32_t)(cos(rad) * MVMT_INCR);
	dy = (int32_t)(sin(rad) * MVMT_INCR);
	if (dir == 'B')
	{
		dx = -dx;
		dy = -dy;
	}
	if (!will_collide(game, dx, 0))
		game->player_img->instances[0].x += dx;
	if (!will_collide(game, 0, dy))
		game->player_img->instances[0].y += dy;
}

void	pan_side(t_game *game, char side)
{
	if (side == 'L')
	{
		game->player.dir_ang -= 3.0;
	}
	else
	{
		game->player.dir_ang += 3.0;
	}
}

static void	redraw(t_game *game)
{
	uint32_t	y;
	uint32_t	x;

	y = 0;
	while (y < CIRCLE_R * 2)
	{
		x = 0;
		while (x < CIRCLE_R * 2)
		{
			if (belongs_in_circle(y, x))
			{
				mlx_put_pixel(game->player_img, x, y, RED);
			}
			x++;
		}
		y++;
	}
}

static bool	belongs_in_circle(uint32_t y, uint32_t x)
{
	uint32_t	c_x;
	uint32_t	c_y;

	c_x = x - CIRCLE_R;
	c_y = y - CIRCLE_R;
	return ((c_x) * (c_x) + (c_y) * (c_y) <= CIRCLE_R * CIRCLE_R);
}

static bool	will_collide(t_game *game, int32_t x_incr, int32_t y_incr)
{
	int32_t	px;
	int32_t	py;
	int		left;
	int		right;
	int		top;
	int		bottom;

	px = game->player_img->instances[0].x + x_incr;
	py = game->player_img->instances[0].y + y_incr;
	left = px / (int)SQUARE_SZ;
	right = (px + (int)CIRCLE_DIAM - 1) / (int)SQUARE_SZ;
	top = py / (int)SQUARE_SZ;
	bottom = (py + (int)CIRCLE_DIAM - 1) / (int)SQUARE_SZ;
	if (left < 0 || right >= MAP_W || top < 0 || bottom >= MAP_H)
		return (true);
	if (gridmap[top][left] == 1)
		return (true);
	if (gridmap[top][right] == 1)
		return (true);
	if (gridmap[bottom][left] == 1)
		return (true);
	if (gridmap[bottom][right] == 1)
		return (true);
	return (false);
}
