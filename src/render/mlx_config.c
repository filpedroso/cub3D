/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_config.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 22:11:25 by fpedroso          #+#    #+#             */
/*   Updated: 2026/06/10 22:11:25 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d.h"

static bool	initiate_mlx(t_game *game);
static bool	add_map_img(t_game *game);
static bool	add_player_img(t_game *game);

bool	config_mlx(t_game *game)
{
	if (!initiate_mlx(game))
		return (false);
	if (!add_map_img(game))
	{
		mlx_close_window(game->mlx);
		return (false);
	}
	if (!add_player_img(game))
	{
		mlx_close_window(game->mlx);
		return (false);
	}
	return (true);
}

static bool	initiate_mlx(t_game *game)
{
	game->mlx = mlx_init(SCR_W, SCR_H, "minimap", true);
	if (!game->mlx)
	{
		puts(mlx_strerror(mlx_errno));
		return (false);
	}
	return (true);
}

static bool	add_map_img(t_game *game)
{
	game->map_img = mlx_new_image(game->mlx, MAP_PX_W, MAP_PX_H);
	if (!game->map_img)
	{
		puts(mlx_strerror(mlx_errno));
		return (false);
	}
	if (mlx_image_to_window(game->mlx, game->map_img, 0, 0) == -1)
	{
		puts(mlx_strerror(mlx_errno));
		return (false);
	}
	return (true);
}

static bool	add_player_img(t_game *game)
{
	game->player_img = mlx_new_image(game->mlx, CIRCLE_DIAM, CIRCLE_DIAM);
	if (!game->player_img)
	{
		puts(mlx_strerror(mlx_errno));
		return (false);
	}
	if (mlx_image_to_window(game->mlx, game->player_img, 0, 0) == -1)
	{
		puts(mlx_strerror(mlx_errno));
		return (false);
	}
	return (true);
}
