/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_minimap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 14:00:33 by fpedroso          #+#    #+#             */
/*   Updated: 2026/07/25 21:08:20 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static bool	belongs_in_circle(uint32_t y, uint32_t x);
static void	draw_player(t_game *game);

void	draw_minimap(t_game *game)
{
	ft_memcpy(game->map_img->pixels, game->map_pixels_buf,
		game->map_img->width * game->map_img->height * sizeof(uint32_t));
	draw_player(game);
	if (game->show_rays)
		draw_rays(game);
}

static void	draw_player(t_game *game)
{
	uint32_t	y;
	uint32_t	x;
	uint32_t	x_start;
	uint32_t	y_end;
	uint32_t	x_end;

	y_end = game->player.y + CIRCLE_R;
	x_start = game->player.x - CIRCLE_R;
	x_end = game->player.x + CIRCLE_R;
	y = game->player.y - CIRCLE_R;
	while (y <= y_end)
	{
		x = x_start;
		while (x <= x_end)
		{
			if (belongs_in_circle(x, y))
				mlx_put_pixel(game->map_img, x, y, RED);
			x++;
		}
		y++;
	}
}

static bool	belongs_in_circle(uint32_t y, uint32_t x)
{
	uint32_t	c_x;
	uint32_t	c_y;

	c_x = x - CIRCLE_R;
	c_y = y - CIRCLE_R;
	return ((c_x) * (c_x) + (c_y) * (c_y) <= CIRCLE_R * CIRCLE_R);
}
