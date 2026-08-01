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

static void	view_toggles(t_game *game);
static bool	pressed_once(t_game *game, keys_t key, int slot);

void	compute(t_game *game)
{
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
	view_toggles(game);
	update_pl_position(game);
	cast_rays(game);
}

static void	view_toggles(t_game *game)
{
	if (pressed_once(game, MLX_KEY_M, 0))
		game->show_minimap = !game->show_minimap;
	if (pressed_once(game, MLX_KEY_R, 1))
		game->show_rays = !game->show_rays;
	if (pressed_once(game, MLX_KEY_T, 2))
		game->show_tex = !game->show_tex;
}

/*
** True on the frame a key goes down, not for as long as it is held,
** which is what a toggle needs. Each caller owns a slot in the
** edge-detection state; keeping them in one array rather than a static
** bool per key is what lets a fourth toggle cost one line instead of
** pushing the function past the five-variable limit.
*/
static bool	pressed_once(t_game *game, keys_t key, int slot)
{
	static bool	prev[4] = {false, false, false, false};
	bool		now;
	bool		fired;

	now = mlx_is_key_down(game->mlx, key);
	fired = (now && !prev[slot]);
	prev[slot] = now;
	return (fired);
}
