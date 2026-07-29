/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_minimap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 14:00:33 by fpedroso          #+#    #+#             */
/*   Updated: 2026/07/28 12:00:00 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	draw_rays(t_game *game);
static void	draw_player(t_game *game);
static void	plot_disc(t_game *game, t_dbl_coord centre, double radius);

/*
** The player is drawn last so the ray fan, which emanates from it,
** cannot bury it.
*/
void	draw_minimap(t_game *game)
{
	ft_memcpy(game->map_img->pixels, game->map_pixels_buf,
		(size_t)game->map_img->width * game->map_img->height
		* sizeof(uint32_t));
	if (game->show_rays)
		draw_rays(game);
	draw_player(game);
}

/*
** One line per ray would be several hundred into an image barely a
** hundred pixels wide. MM_RAY_STEP thins the fan to what the minimap
** can actually resolve; the 3D view still consumes all SCR_W rays.
*/
static void	draw_rays(t_game *game)
{
	int	i;

	i = 0;
	while (i < SCR_W)
	{
		draw_ray(game, i);
		i += MM_RAY_STEP;
	}
}

static void	draw_player(t_game *game)
{
	t_dbl_coord	pos;

	pos.x = game->player.x;
	pos.y = game->player.y;
	plot_disc(game, tile_to_px(&game->minimap, pos), game->minimap.radius);
}

/*
** Filled disc, clamped to the image. MLX42's mlx_put_pixel is built on
** assert(), so an out-of-range coordinate aborts the process rather
** than being skipped: the clamp is required, not defensive.
*/
static void	plot_disc(t_game *game, t_dbl_coord centre, double radius)
{
	t_int_coord	px;
	t_int_coord	end;
	double		dx;
	double		dy;

	px.y = (int32_t)fmax(0.0, centre.y - radius);
	end.y = (int32_t)fmin(centre.y + radius,
			(double)game->map_img->height - 1);
	end.x = (int32_t)fmin(centre.x + radius,
			(double)game->map_img->width - 1);
	while (px.y <= end.y)
	{
		px.x = (int32_t)fmax(0.0, centre.x - radius);
		while (px.x <= end.x)
		{
			dx = px.x - centre.x;
			dy = px.y - centre.y;
			if (dx * dx + dy * dy <= radius * radius)
				mlx_put_pixel(game->map_img, px.x, px.y, RED);
			px.x++;
		}
		px.y++;
	}
}
