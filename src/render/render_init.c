/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 12:00:00 by fpedroso          #+#    #+#             */
/*   Updated: 2026/07/28 12:00:00 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static uint32_t	fit_axis(int tiles, double scale);
static uint32_t	pack_rgb(const int rgb[3]);

/*
** Runs before config_mlx, because the minimap image is sized to the map
** rather than to a fixed square: fitting the map to the aspect ratio is
** what lets the drawn walls sit flush in the window corner instead of
** floating in a letterboxed square.
**
** Pure arithmetic on the parsed map and the screen macros, so it needs
** no MLX context. fit_w/fit_h are the box the minimap may not exceed
** (the window fraction MINIMAP_SCALE allows) minus the margin on both
** sides; computed here as locals rather than macros since Norminette
** does not allow arithmetic in a #define, and nothing else needs them.
*/
void	init_minimap_geometry(t_game *game)
{
	t_minimap	*mm;
	double		scale;
	double		fit_w;
	double		fit_h;

	fit_w = (double)SCR_W * MINIMAP_SCALE - 2 * MM_MARGIN;
	fit_h = (double)SCR_H * MINIMAP_SCALE - 2 * MM_MARGIN;
	scale = fmin(fit_w / game->map.cols, fit_h / game->map.rows);
	if (scale <= 0.0)
		scale = 1.0;
	mm = &game->minimap;
	mm->scale = scale;
	mm->off_x = MM_MARGIN;
	mm->off_y = MM_MARGIN;
	mm->radius = fmax(1.0, PL_RADIUS * scale);
	mm->width = fit_axis(game->map.cols, scale);
	mm->height = fit_axis(game->map.rows, scale);
}

/*
** Image extent along one axis: the map rounded to whole pixels, never
** collapsing to zero, plus the margin on both sides.
*/
static uint32_t	fit_axis(int tiles, double scale)
{
	return ((uint32_t)fmax(1.0, round(tiles * scale)) + 2 * MM_MARGIN);
}

/*
** Runs once, after config_mlx. Everything set up here depends only on
** data that is fixed for the whole run, so none of it belongs in the
** frame loop.
*/
bool	init_render(t_game *game)
{
	size_t	size;
	double	half_fov;

	half_fov = (FOV_DEG * M_PI / 180.0) / 2.0;
	game->horizon = SCR_H / 2;
	game->proj_plane = (SCR_W / 2.0) / tan(half_fov);
	init_fov_lut(game);
	game->ceil_rgba = pack_rgb(game->config.ceil);
	game->floor_rgba = pack_rgb(game->config.floor);
	size = (size_t)game->map_img->width * game->map_img->height
		* sizeof(uint32_t);
	game->map_pixels_buf = malloc(size);
	if (!game->map_pixels_buf)
	{
		perror("malloc");
		return (false);
	}
	bake_minimap_bg(game);
	return (true);
}

/*
** The parsed F and C lines into the 0xRRGGBBAA word the frame loop
** wants. Opaque: the 3D view is the bottom image, so anything less
** would blend it with the window background.
*/
static uint32_t	pack_rgb(const int rgb[3])
{
	return (((uint32_t)rgb[0] << 24) | ((uint32_t)rgb[1] << 16)
		| ((uint32_t)rgb[2] << 8) | 0xFF);
}

/*
** Precomputes the cosine and sine of every column's angular offset from
** the view direction. Those offsets never change, so cast_rays can
** rotate the whole fan from a single cos/sin per frame instead of
** calling trig twice per column.
**
** Columns are spaced by equal tangent, not by equal angle. The screen
** is a flat plane, so a linear sweep in angle is not a perspective
** projection: it bows straight walls outwards. Spreading the columns
** evenly across the projection plane and taking the arctangent back to
** an angle is what makes them straight. perp_dist = hit_dist * cos_off
** is unaffected, since perpendicular distance is euclidean distance
** times the cosine of the offset angle whatever the spacing.
*/
void	init_fov_lut(t_game *game)
{
	double	offset;
	double	half_fov;
	int		i;

	half_fov = (FOV_DEG * M_PI / 180.0) / 2.0;
	i = 0;
	while (i < RAY_COUNT)
	{
		offset = atan((2.0 * i / RAY_COUNT - 1.0) * tan(half_fov));
		game->rays[i].cos_off = cos(offset);
		game->rays[i].sin_off = sin(offset);
		i++;
	}
}
