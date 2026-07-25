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

void	on_update(void *param);
static void	refresh_player_sprite(t_game *game);
static void	handle_view_toggles(t_game *game);
static void	sync_view_visibility(t_game *game);

void	render(t_game *game)
{
	if (!config_mlx(game))
	{
		handle_error(ERR_MLX);
		return;
	}
	mlx_loop_hook(game->mlx, on_update, game);
	mlx_loop(game->mlx);
	mlx_terminate(game->mlx);
}

void	on_update(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	handle_view_toggles(game);
	sync_view_visibility(game);
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
	update_pl_position(game);
	refresh_player_sprite(game);
	if (game->render_state.show_minimap)
	{
		draw_minimap(game);
		draw_player(game);
		if (game->render_state.show_rays)
			cast_rays(game);
	}
}

static void	refresh_player_sprite(t_game *game)
{
	double	scale;
	double	offset_y;

	scale = minimap_scale(game);
	offset_y = (double)SCR_H - game->map_img->height;
	game->player_img->instances[0].x = (game->player.x * scale) - CIRCLE_R;
	game->player_img->instances[0].y = offset_y + (game->player.y * scale) - CIRCLE_R;
}

static void	handle_view_toggles(t_game *game)
{
	static bool	prev_minimap;
	static bool	prev_rays;
	bool		minimap_pressed;
	bool		rays_pressed;

	minimap_pressed = mlx_is_key_down(game->mlx, MLX_KEY_M);
	rays_pressed = mlx_is_key_down(game->mlx, MLX_KEY_R);
	if (minimap_pressed && !prev_minimap)
		game->render_state.show_minimap = !game->render_state.show_minimap;
	if (rays_pressed && !prev_rays)
		game->render_state.show_rays = !game->render_state.show_rays;
	prev_minimap = minimap_pressed;
	prev_rays = rays_pressed;
}

static void	sync_view_visibility(t_game *game)
{
	game->map_img->enabled = game->render_state.show_minimap;
	game->player_img->enabled = game->render_state.show_minimap;
}
