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

static void	set_player_position(mlx_instance_t *player_instance);

void	run_minimap_mode(t_game *game)
{
	if (!config_mlx(game))
	{
		handle_error(ERR_MLX);
		return;
	}
	draw_minimap(game);
	set_player_position(game->player_img->instances);
	mlx_loop_hook(game->mlx, draw_player, game);
	mlx_loop(game->mlx);
	mlx_terminate(game->mlx);
}

static void	set_player_position(mlx_instance_t *player_instance)
{
	uint32_t	i;
	uint32_t	j;

	i = 0;
	while (i < MAP_H)
	{
		j = 0;
		while (j < MAP_W)
		{
			if (gridmap[i][j] == 2)
			{
				player_instance[0].y = i * SQUARE_SZ;
				player_instance[0].x = j * SQUARE_SZ;
				return ;
			}
			j++;
		}
		i++;
	}
	return ;
}
