/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 13:55:36 by fpedroso          #+#    #+#             */
/*   Updated: 2026/07/28 12:00:00 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	on_update(void *param);

void	render(t_game *game)
{
	game->show_minimap = true;
	game->show_rays = true;
	if (!config_mlx(game))
	{
		handle_error(ERR_MLX);
		return ;
	}
	if (!init_render(game))
	{
		mlx_terminate(game->mlx);
		handle_error(ERR_MLX);
		return ;
	}
	mlx_loop_hook(game->mlx, on_update, game);
	mlx_loop(game->mlx);
	mlx_terminate(game->mlx);
	free(game->map_pixels_buf);
	game->map_pixels_buf = NULL;
}

static void	on_update(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	compute(game);
	draw_frame(game);
}
