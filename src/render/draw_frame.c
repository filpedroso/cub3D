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

static t_column	prep_column(t_game *game, int index);

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
** One ray per column, so the fan cast_rays produced maps straight onto
** the window with no resampling. Ceiling, wall and floor between them
** cover every row of the strip, which is why main_img is never cleared:
** each frame overwrites the whole image.
*/
void	draw_3d(t_game *game)
{
	t_column	col;
	int			i;

	i = 0;
	while (i < RAY_COUNT)
	{
		col = prep_column(game, i);
		draw_ceiling(game, &col);
		draw_wall(game, &col);
		draw_floor(game, &col);
		i++;
	}
}

/*
** Turns a cast ray into a drawable strip. perp_dist is already the
** fisheye-corrected distance, so the projection is the bare
** SCR_H / distance; start and end are clamped here, once, because
** mlx_put_pixel asserts on out-of-range coordinates instead of
** clipping them.
*/
static t_column	prep_column(t_game *game, int index)
{
	t_column	col;
	t_ray		*ray;
	double		line_h;

	ray = &game->rays[index];
	line_h = (double)SCR_H / fmax(ray->perp_dist, 1e-6);
	col.x = index;
	col.start = (int32_t)fmax(0.0, HORIZON - line_h / 2.0);
	col.end = (int32_t)fmin(HORIZON + line_h / 2.0, (double)SCR_H - 1);
	col.color = face_color(ray);
	col.plane.x = ray->dir.x / ray->cos_off;
	col.plane.y = ray->dir.y / ray->cos_off;
	return (col);
}
