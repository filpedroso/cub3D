/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_pl_pos.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 19:05:40 by fpedroso          #+#    #+#             */
/*   Updated: 2026/07/25 18:57:02 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void		walk(t_game *game, char dir);
void		pan_side(t_game *game, char side);
static bool	will_collide(t_game *game, int32_t x_incr, int32_t y_incr);


void	update_pl_position(t_game *game)
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
	int32_t	dx_px;
	int32_t	dy_px;
	double	dx;
	double	dy;

	rad = game->player.dir_ang * (M_PI / 180.0);
	dx_px = (int32_t)(cos(rad) * MVMT_INCR);
	dy_px = (int32_t)(sin(rad) * MVMT_INCR);
	if (dir == 'B')
	{
		dx_px = -dx_px;
		dy_px = -dy_px;
	}
	dx = (double)dx_px / SQUARE_SZ;
	dy = (double)dy_px / SQUARE_SZ;
	if (!will_collide(game, dx_px, 0))
		game->player.x += dx;
	if (!will_collide(game, 0, dy_px))
		game->player.y += dy;
}

void	pan_side(t_game *game, char side)
{
	if (side == 'L')
		game->player.dir_ang -= PAN_INCR;
	else
		game->player.dir_ang += PAN_INCR;
}

static bool	will_collide(t_game *game, int32_t x_incr, int32_t y_incr)
{
	t_int_coord	pl;
	t_int_dir	dir;

	pl.x = (int32_t)(game->player.x * SQUARE_SZ) + x_incr;
	pl.y = (int32_t)(game->player.y * SQUARE_SZ) + y_incr;
	dir.left = pl.x / (int32_t)SQUARE_SZ;
	dir.right = (pl.x + (int32_t)CIRCLE_DIAM - 1) / (int32_t)SQUARE_SZ;
	dir.up = pl.y / (int32_t)SQUARE_SZ;
	dir.down = (pl.y + (int32_t)CIRCLE_DIAM - 1) / (int32_t)SQUARE_SZ;
	if (dir.left < 0 || dir.right >= game->map.cols
		|| dir.up < 0 || dir.down >= game->map.rows)
		return (true);
	if (game->map.grid[dir.up][dir.left] == '1')
		return (true);
	if (game->map.grid[dir.up][dir.right] == '1')
		return (true);
	if (game->map.grid[dir.down][dir.left] == '1')
		return (true);
	if (game->map.grid[dir.down][dir.right] == '1')
		return (true);
	return (false);
}
