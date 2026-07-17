/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 15:49:44 by fpedroso          #+#    #+#             */
/*   Updated: 2026/06/16 18:42:10 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void		redraw(t_game *game);
static bool		belongs_in_circle(uint32_t y, uint32_t x);

void	draw_player(void* param)
{
	t_game	*game;

	game = (t_game *)param;
	update_pl_position(game);
	redraw(game);
}

static void	redraw(t_game *game)
{
	uint32_t	y;
	uint32_t	x;

	y = 0;
	while (y < CIRCLE_R * 2)
	{
		x = 0;
		while (x < CIRCLE_R * 2)
		{
			if (belongs_in_circle(y, x))
			{
				mlx_put_pixel(game->player_img, x, y, RED);
			}
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
