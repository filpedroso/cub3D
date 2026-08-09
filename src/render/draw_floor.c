/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_floor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 20:10:00 by fpedroso          #+#    #+#             */
/*   Updated: 2026/08/01 12:26:23 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static t_dbl_coord	grid_eps(t_game *game, t_column *col, double row_dist);
static uint32_t		grid_color(t_dbl_coord eps, t_dbl_coord world);

/*
** Everything below the wall, projected back onto the ground plane.
** row_dist inverts the wall projection, so at the wall's bottom pixel
** it comes out exactly equal to that wall's perp_dist and the two meet
** with no seam. col->plane is the ray direction with the fisheye factor
** divided back out, which is what turns a perpendicular row distance
** into the true world point under the pixel.
**
** The eye sits at mid-wall height, hence the 0.5. The loop never starts
** on the horizon itself, where the division would blow up: a very
** distant wall can round its bottom edge down onto it.
*/
void	draw_floor(t_game *game, t_column *col)
{
	t_dbl_coord	world;
	double		row_dist;
	int32_t		y;

	y = (int32_t)fmax(col->end + 1, game->horizon + 1);
	while (y < SCR_H)
	{
		row_dist = (0.5 * SCR_H) / (y - (double)game->horizon);
		world.x = game->player.x + row_dist * col->plane.x;
		world.y = game->player.y + row_dist * col->plane.y;
		mlx_put_pixel(game->main_img, col->x, y,
			grid_color(grid_eps(game, col, row_dist), world));
		y++;
	}
}

/*
** Half-width of a grid line, in world units, per axis.
**
** One threshold for both axes does not work. The floor is compressed
** far harder vertically than horizontally: near the horizon a single
** screen row spans a large jump in depth, so a line sized only for the
** horizontal scale falls between two rows and vanishes. depth is the
** world distance one screen row covers, lat the distance one screen
** column covers; each axis takes whichever of the two it actually
** varies along, so every line stays about GRID_PX wide on screen.
**
** The 0.45 cap stops the floor right under the player flooding pink.
*/
static t_dbl_coord	grid_eps(t_game *game, t_column *col, double row_dist)
{
	t_dbl_coord	eps;
	double		depth;
	double		lat;

	depth = GRID_PX * row_dist * row_dist / SCR_H;
	lat = 0.5 * GRID_PX * row_dist / game->proj_plane;
	eps.x = fmin(0.45, fmax(fabs(col->plane.x) * depth, lat));
	eps.y = fmin(0.45, fmax(fabs(col->plane.y) * depth, lat));
	return (eps);
}

/*
** Pink within eps of a tile boundary on either axis, black elsewhere.
*/
static uint32_t	grid_color(t_dbl_coord eps, t_dbl_coord world)
{
	double	fx;
	double	fy;

	fx = world.x - floor(world.x);
	fy = world.y - floor(world.y);
	if (fmin(fx, 1.0 - fx) < eps.x || fmin(fy, 1.0 - fy) < eps.y)
		return (FLOOR_LINES);
	return (FLOOR);
}
