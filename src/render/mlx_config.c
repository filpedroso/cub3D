/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_config.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 22:11:25 by fpedroso          #+#    #+#             */
/*   Updated: 2026/07/25 18:56:57 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static bool	initiate_mlx(t_game *game);
static bool	add_map_img(t_game *game);
static bool	add_main_img(t_game *game);

bool	config_mlx(t_game *game)
{
	if (!initiate_mlx(game))
		return (false);
	if (!add_main_img(game))
	{
		mlx_terminate(game->mlx);
		return (false);
	}
	if (!add_map_img(game))
	{
		mlx_terminate(game->mlx);
		return (false);
	}
	return (true);
}

static bool	initiate_mlx(t_game *game)
{
	game->mlx = mlx_init(SCR_W, SCR_H, "cub3D", true);
	if (!game->mlx)
	{
		printf("%s\n", mlx_strerror(mlx_errno));
		return (false);
	}
	return (true);
}

/*
** Added to the window first, so the 3D view sits underneath the
** minimap overlay.
*/
static bool	add_main_img(t_game *game)
{
	game->main_img = mlx_new_image(game->mlx, SCR_W, SCR_H);
	if (!game->main_img)
	{
		printf("%s\n", mlx_strerror(mlx_errno));
		return (false);
	}
	if (mlx_image_to_window(game->mlx, game->main_img, 0, 0) == -1)
	{
		printf("%s\n", mlx_strerror(mlx_errno));
		return (false);
	}
	return (true);
}

/*
** Anchored bottom-left. The contents are laid out later by
** init_minimap, which owns the tile-to-pixel scale.
*/
static bool	add_map_img(t_game *game)
{
	uint32_t	width;
	uint32_t	height;

	width = roundf(SCR_W * MINIMAP_SCALE);
	height = roundf(SCR_H * MINIMAP_SCALE);
	game->map_img = mlx_new_image(game->mlx, width, height);
	if (!game->map_img)
	{
		printf("%s\n", mlx_strerror(mlx_errno));
		return (false);
	}
	if (mlx_image_to_window(game->mlx, game->map_img, 0,
			SCR_H - (int32_t)height) == -1)
	{
		printf("%s\n", mlx_strerror(mlx_errno));
		return (false);
	}
	return (true);
}
