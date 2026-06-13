/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 15:49:44 by fpedroso          #+#    #+#             */
/*   Updated: 2026/06/13 15:49:44 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void		update_position(t_game *game);
static void		redraw(t_game *game);
static bool		belongs_in_circle(uint32_t y, uint32_t x);
static bool		will_collide(mlx_image_t *map_img, mlx_image_t *player_img, int32_t x_incr, int32_t y_incr);

void	draw_player(void* param)
{
	t_game	*game;

	game = (t_game *)param;
	update_position(game);
	redraw(game);
}

static void	update_position(t_game *game)
{
	if (mlx_is_key_down(game->mlx, MLX_KEY_UP))
	{
		if (!will_collide(game->map_img, game->player_img, 0, -MVMT_INCR))
			game->player_img->instances[0].y -= MVMT_INCR;
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_DOWN))
	{
		if (!will_collide(game->map_img, game->player_img, 0, MVMT_INCR))
			game->player_img->instances[0].y += MVMT_INCR;
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
	{
		if (!will_collide(game->map_img, game->player_img, -MVMT_INCR, 0))
			game->player_img->instances[0].x -= MVMT_INCR;
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
	{
		if (!will_collide(game->map_img, game->player_img, MVMT_INCR, 0))
			game->player_img->instances[0].x += MVMT_INCR;
	}
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



static bool	will_collide(mlx_image_t *map_img, mlx_image_t *player_img, int32_t x_incr, int32_t y_incr)
{
	uint32_t	map_pixel;
	uint32_t	px;
	uint32_t	py;
	uint32_t	i;
	uint32_t	j;

	px = player_img->instances[0].x + x_incr;
	py = player_img->instances[0].y + y_incr;
	i = 0;
	while (i < player_img->height)
	{
		j = 0;
		while (j < player_img->width)
		{
			if ((px + j) < map_img->width && (py + i) < map_img->height)
			{
				map_pixel = ((uint32_t *)map_img->pixels)[(py + i) * map_img->width + (px + j)];
				if (map_pixel == 4294901760)
					return (true);
			}
			j++;
		}
		i++;
	}
	return (false);
}
