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
static bool	init_or_fail(t_game *game);

bool	render(t_game *game)
{
	game->show_minimap = true;
	game->show_rays = true;
	game->show_tex = true;
	init_minimap_geometry(game);
	if (!init_or_fail(game))
		return (false);
	mlx_loop_hook(game->mlx, on_update, game);
	mlx_loop(game->mlx);
	mlx_terminate(game->mlx);
	free_textures(game);
	free(game->map_pixels_buf);
	game->map_pixels_buf = NULL;
	return (true);
}

/*
** Textures, then the MLX window, then the derived render state: each
** stage can only run once the one before it exists, so a failure here
** unwinds exactly what that stage had already allocated before handing
** false back to render.
*/
static bool	init_or_fail(t_game *game)
{
	if (!load_textures(game))
		return (false);
	if (!config_mlx(game))
	{
		free_textures(game);
		handle_error(ERR_MLX);
		return (false);
	}
	if (!init_render(game))
	{
		mlx_terminate(game->mlx);
		free_textures(game);
		handle_error(ERR_MLX);
		return (false);
	}
	return (true);
}

static void	on_update(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	compute(game);
	draw_frame(game);
}
