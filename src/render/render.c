/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 13:55:36 by fpedroso          #+#    #+#             */
/*   Updated: 2026/05/30 13:55:36 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	on_update(void *param);

void	run_minimap_mode(t_game *game)
{
	if (!config_mlx(game))
	{
		handle_error(ERR_MLX);
		return;
	}
	game->player_img->instances[0].x = game->player.x * SQUARE_SZ;
	game->player_img->instances[0].y = game->player.y * SQUARE_SZ;
	mlx_loop_hook(game->mlx, on_update, game);
	mlx_loop(game->mlx);
	mlx_terminate(game->mlx);
}

void	on_update(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
	draw_minimap(game);
	draw_player(game);
	cast_rays(game);
}
