/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_bake.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 12:00:00 by fpedroso          #+#    #+#             */
/*   Updated: 2026/07/28 12:00:00 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Rasterises the static walls into map_img once and keeps a pristine
** copy in map_pixels_buf. draw_minimap restores from that copy every
** frame rather than walking the grid again.
*/
void	bake_minimap_bg(t_game *game)
{
	uint32_t	px_y;
	uint32_t	px_x;

	px_y = 0;
	while (px_y < game->map_img->height)
	{
		px_x = 0;
		while (px_x < game->map_img->width)
		{
			if (is_in_tile(game, px_x, px_y))
				mlx_put_pixel(game->map_img, px_x, px_y, BLUE);
			px_x++;
		}
		px_y++;
	}
	ft_memcpy(game->map_pixels_buf, game->map_img->pixels,
		(size_t)game->map_img->width * game->map_img->height
		* sizeof(uint32_t));
}

/*
** Inverse of tile_to_px, for a single pixel. Uses is_solid rather than
** a bare '1' test so the minimap paints exactly what blocks movement
** and stops rays.
*/
bool	is_in_tile(t_game *game, uint32_t px_x, uint32_t px_y)
{
	double	tile_x;
	double	tile_y;

	tile_x = ((double)px_x - game->minimap.off_x) / game->minimap.scale;
	tile_y = ((double)px_y - game->minimap.off_y) / game->minimap.scale;
	if (tile_x < 0 || tile_y < 0)
		return (false);
	if ((int)tile_x >= game->map.cols || (int)tile_y >= game->map.rows)
		return (false);
	return (is_solid(game->map.grid[(int)tile_y][(int)tile_x]));
}

/*
** The single world-to-minimap conversion. Every overlay drawn on the
** minimap goes through here, so the scale lives in exactly one place.
*/
t_dbl_coord	tile_to_px(t_minimap *mm, t_dbl_coord tile)
{
	t_dbl_coord	px;

	px.x = mm->off_x + tile.x * mm->scale;
	px.y = mm->off_y + tile.y * mm->scale;
	return (px);
}
