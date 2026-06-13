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
void	esc_kills(void *param);

void	run_minimap_mode(t_game *game)
{
	if (!config_mlx(game))
	{
		handle_error(ERR_MLX);
		return;
	}
	draw_minimap(game);
	set_player_position(game->player_img->instances);
	mlx_loop_hook(game->mlx, esc_kills, game);
	mlx_loop_hook(game->mlx, draw_player, game);
	mlx_loop(game->mlx);
	mlx_terminate(game->mlx);
}

void	esc_kills(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
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
