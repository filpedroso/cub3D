/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_frame.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 21:42:45 by fpedroso          #+#    #+#             */
/*   Updated: 2026/07/30 19:25:45 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static t_face	face_of(t_ray *ray);

/*
** main_img is the 3D view and is never optional; only the minimap
** overlay answers to the M toggle.
*/
void	draw_frame(t_game *game)
{
	game->main_img->enabled = true;
	game->map_img->enabled = game->show_minimap;
	if (game->show_minimap)
		draw_minimap(game);
	draw_3d(game);
}

/*
** The whole textured-versus-debug decision, taken once per frame. Both
** paths run their own column loop, so nothing downstream tests the flag
** again: no branch per column, and none per pixel.
*/
void	draw_3d(t_game *game)
{
	if (game->show_tex)
		draw_view_tex(game);
	else
		draw_view_dbg(game);
}

/*
** Turns a cast ray into a drawable strip. perp_dist is already the
** fisheye-corrected distance, so the projection is the bare
** SCR_H / distance; start and end are clamped here, once, because
** mlx_put_pixel asserts on out-of-range coordinates instead of
** clipping them, while line_h keeps the unclamped height for the
** texture walk.
*/
t_column	prep_column(t_game *game, int index)
{
	t_column	col;
	t_ray		*ray;

	ray = &game->rays[index];
	col.line_h = (double)SCR_H / fmax(ray->perp_dist, 1e-6);
	col.x = index;
	col.start = (int32_t)fmax(0.0, HORIZON - col.line_h / 2.0);
	col.end = (int32_t)fmin(HORIZON + col.line_h / 2.0, (double)SCR_H - 1);
	col.face = face_of(ray);
	col.plane.x = ray->dir.x / ray->cos_off;
	col.plane.y = ray->dir.y / ray->cos_off;
	return (col);
}

/*
** Which of the four faces the ray landed on, named after the direction
** the wall surface faces rather than the direction of travel. side 1 is
** a y-step, so a ray heading south (dir.y > 0, since the angle
** convention is screen-space: E = 0, S = 90) can only have struck the
** north face of the tile it entered. Same reasoning on x.
*/
static t_face	face_of(t_ray *ray)
{
	if (ray->side == 1)
	{
		if (ray->dir.y > 0)
			return (F_NORTH);
		return (F_SOUTH);
	}
	if (ray->dir.x > 0)
		return (F_WEST);
	return (F_EAST);
}
