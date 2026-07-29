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

static void	set_minimap_geometry(t_game *game);

/*
** Runs once, right after config_mlx. Everything set up here depends
** only on the parsed map and the image dimensions, both of which are
** fixed for the whole run, so none of it belongs in the frame loop.
*/
bool	init_render(t_game *game)
{
	init_fov_lut(game);
	if (!init_minimap(game))
		return (false);
	return (true);
}

/*
** Precomputes the cosine and sine of every column's angular offset
** from the view direction. Those offsets never change, so cast_rays
** can rotate the whole fan from a single cos/sin per frame instead of
** calling trig twice per column.
*/
void	init_fov_lut(t_game *game)
{
	double	offset;
	int		i;

	i = 0;
	while (i < SCR_W)
	{
		offset = -HALF_FOV + (i * FOV / SCR_W);
		game->rays[i].cos_off = cos(offset);
		game->rays[i].sin_off = sin(offset);
		i++;
	}
}

bool	init_minimap(t_game *game)
{
	size_t	size;

	set_minimap_geometry(game);
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
** Fits the map inside the minimap image and centres it. The scale is
** the only tile-to-pixel conversion factor in the program; the player
** dot radius is derived from it so that what is drawn matches the
** footprint will_collide tests.
*/
static void	set_minimap_geometry(t_game *game)
{
	double	usable_w;
	double	usable_h;
	double	scale;

	usable_w = (double)game->map_img->width - 2 * MM_MARGIN;
	usable_h = (double)game->map_img->height - 2 * MM_MARGIN;
	scale = fmin(usable_w / game->map.cols, usable_h / game->map.rows);
	game->minimap.scale = scale;
	game->minimap.off_x = MM_MARGIN + (usable_w - game->map.cols * scale) / 2;
	game->minimap.off_y = MM_MARGIN + (usable_h - game->map.rows * scale) / 2;
	game->minimap.radius = fmax(1.0, PL_RADIUS * scale);
}
