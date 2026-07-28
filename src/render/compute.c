/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 21:41:13 by fpedroso          #+#    #+#             */
/*   Updated: 2026/07/27 21:41:13 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	minimap_toggles(t_game *game);

void	compute(t_game *game)
{
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
	minimap_toggles(game);
	update_pl_position(game);
	cast_rays(game);
}

static void	minimap_toggles(t_game *game)
{
	static bool	prev_minimap = false;
	static bool	prev_rays = false;
	bool		minimap_pressed;
	bool		rays_pressed;

	minimap_pressed = mlx_is_key_down(game->mlx, MLX_KEY_M);
	rays_pressed = mlx_is_key_down(game->mlx, MLX_KEY_R);
	if (minimap_pressed && !prev_minimap)
		game->show_minimap = !game->show_minimap;
	if (rays_pressed && !prev_rays)
		game->show_rays = !game->show_rays;
	prev_minimap = minimap_pressed;
	prev_rays = rays_pressed;
}
