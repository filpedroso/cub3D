/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_column.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 20:10:00 by fpedroso          #+#    #+#             */
/*   Updated: 2026/07/30 20:10:00 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Everything above the wall. Runs to col->start, which prep_column
** already clamped, so a wall taller than the window simply draws no
** ceiling rather than looping backwards.
*/
void	draw_ceiling(t_game *game, t_column *col)
{
	int32_t	y;

	y = 0;
	while (y < col->start)
	{
		mlx_put_pixel(game->main_img, col->x, y, SKY);
		y++;
	}
}

void	draw_wall(t_game *game, t_column *col)
{
	int32_t	y;

	y = col->start;
	while (y <= col->end)
	{
		mlx_put_pixel(game->main_img, col->x, y, col->color);
		y++;
	}
}

/*
** Which of the four faces the ray landed on, named after the direction
** the wall surface faces rather than the direction of travel. side 1 is
** a y-step, so a ray heading south (dir.y > 0, since the angle
** convention is screen-space: E = 0, S = 90) can only have struck the
** north face of the tile it entered. Same reasoning on x.
**
** Placeholder colours for now; the texture pass swaps this for a
** lookup into the four loaded PNGs, keeping the same branch structure.
*/
uint32_t	face_color(t_ray *ray)
{
	if (ray->side == 1)
	{
		if (ray->dir.y > 0)
			return (FACE_N);
		return (FACE_S);
	}
	if (ray->dir.x > 0)
		return (FACE_W);
	return (FACE_E);
}
