/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_pl_pos.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 19:05:40 by fpedroso          #+#    #+#             */
/*   Updated: 2026/07/28 12:00:00 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	walk(t_game *game, char dir);
static void	pan_side(t_game *game, char side);
static bool	will_collide(t_game *game, double x, double y);

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

/*
** Movement stays in world tiles from end to end. The previous integer
** pixel step made real speed vary with heading and truncate to zero at
** shallow angles. Axes are tested one at a time so a blocked wall
** still allows sliding along it.
*/
static void	walk(t_game *game, char dir)
{
	double	rad;
	double	dx;
	double	dy;

	rad = game->player.dir_ang * (M_PI / 180.0);
	dx = cos(rad) * MOVE_SPEED;
	dy = sin(rad) * MOVE_SPEED;
	if (dir == 'B')
	{
		dx = -dx;
		dy = -dy;
	}
	if (!will_collide(game, game->player.x + dx, game->player.y))
		game->player.x += dx;
	if (!will_collide(game, game->player.x, game->player.y + dy))
		game->player.y += dy;
}

static void	pan_side(t_game *game, char side)
{
	if (side == 'L')
		game->player.dir_ang -= PAN_INCR;
	else
		game->player.dir_ang += PAN_INCR;
}

/*
** Tests the player's axis-aligned footprint centred on (x, y). The
** position is the centre of the box, matching what the minimap draws,
** rather than its top-left corner as before.
*/
static bool	will_collide(t_game *game, double x, double y)
{
	t_int_dir	box;

	box.left = (int32_t)floor(x - PL_RADIUS);
	box.right = (int32_t)floor(x + PL_RADIUS);
	box.up = (int32_t)floor(y - PL_RADIUS);
	box.down = (int32_t)floor(y + PL_RADIUS);
	if (box.left < 0 || box.right >= game->map.cols
		|| box.up < 0 || box.down >= game->map.rows)
		return (true);
	if (is_solid(game->map.grid[box.up][box.left]))
		return (true);
	if (is_solid(game->map.grid[box.up][box.right]))
		return (true);
	if (is_solid(game->map.grid[box.down][box.left]))
		return (true);
	if (is_solid(game->map.grid[box.down][box.right]))
		return (true);
	return (false);
}
