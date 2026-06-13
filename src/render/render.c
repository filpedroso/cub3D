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

void	ft_hook(void* param);
void	draw_player(void* param);
bool	belongs_in_circle(uint32_t y, uint32_t x);


void	run_minimap_mode(t_game *game)
{
	if (!config_mlx(game))
	{
		handle_error(ERR_MLX);
		return;
	}
	draw_minimap(game);
	mlx_loop_hook(game->mlx, draw_player, game);
	mlx_loop(game->mlx);
	mlx_terminate(game->mlx);
}
