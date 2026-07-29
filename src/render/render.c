/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 13:55:36 by fpedroso          #+#    #+#             */
/*   Updated: 2026/07/25 18:56:58 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	load_minimap(t_game *game);
void		on_update(void *param);
static bool	is_in_tile(t_game *game, int px_x, int px_y);

void	render(t_game *game)
{
	game->show_minimap = false;
	game->show_rays = false;
	if (!config_mlx(game))
	{
		handle_error(ERR_MLX);
		return;
	}
	load_minimap(game);
	mlx_loop_hook(game->mlx, on_update, game);
	mlx_loop(game->mlx);
	mlx_terminate(game->mlx);
}

void	on_update(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	compute(game);
	draw_frame(game);
}

static void	load_minimap(t_game *game)
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
		game->map_img->width * game->map_img->height * sizeof(uint32_t));
}

static bool	is_in_tile(t_game *game, int px_x, int px_y)
{
	uint32_t	col;
	uint32_t	row;

	col = px_x / TILE_SZ;
	row = px_y / TILE_SZ;
	if (col >= (uint32_t)game->map.cols || row >= (uint32_t)game->map.rows)
		return (false);
	if (col >= ft_strlen(game->map.grid[row]))
		return (false);
	return (game->map.grid[row][col] == '1');
}

