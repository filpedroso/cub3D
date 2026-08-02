/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_load.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 12:30:00 by fpedroso          #+#    #+#             */
/*   Updated: 2026/08/01 12:30:00 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static bool	load_one(t_game *game, t_face face, const char *path);

/*
** Decodes the four PNGs the parser found into game->tex, indexed by
** t_face so the frame loop can go from the face it hit straight to the
** texture without a four-way branch.
**
** Runs before config_mlx: mlx_load_png is pure lodepng and takes no mlx
** handle, so nothing here needs a window. That matters, because
** parse_texture only ever checked the .png extension, never that the
** file opens. This is the first real existence check, and doing it
** early means a bad path fails before a window flashes onto the screen
** and straight back off.
*/
bool	load_textures(t_game *game)
{
	if (!load_one(game, F_NORTH, game->config.tex_north))
		return (false);
	if (!load_one(game, F_SOUTH, game->config.tex_south))
		return (false);
	if (!load_one(game, F_WEST, game->config.tex_west))
		return (false);
	if (!load_one(game, F_EAST, game->config.tex_east))
		return (false);
	return (true);
}

/*
** On failure the textures already decoded are released here, so the
** caller only has to stop. MLX42 prints its own LodePNG diagnostic to
** stderr naming the file, which is worth keeping alongside our message.
*/
static bool	load_one(t_game *game, t_face face, const char *path)
{
	game->tex[face] = mlx_load_png(path);
	if (!game->tex[face])
	{
		free_textures(game);
		handle_error(ERR_MISSING_TEX);
		return (false);
	}
	return (true);
}

/*
** Idempotent, so the partial-failure path and the normal shutdown can
** both call it.
*/
void	free_textures(t_game *game)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (game->tex[i])
		{
			mlx_delete_texture(game->tex[i]);
			game->tex[i] = NULL;
		}
		i++;
	}
}
