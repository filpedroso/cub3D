/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 14:25:43 by fpedroso          #+#    #+#             */
/*   Updated: 2026/07/25 16:52:21 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Converts the player's spawn cardinal into a facing angle.
 *
 * Angles are in degrees, growing clockwise from East at 0. That is not
 * the usual math convention because screen space is not: the raycaster
 * works in image coordinates, where Y grows downward. South therefore
 * lands on +90 and North on 270, keeping the angle consistent with the
 * direction vector the renderer derives from it via cos/sin.
 *
 * 'N' is the fallback rather than a fourth explicit branch: the parser
 * only ever stores one of NSEW in player.dir, so anything reaching the
 * final return is North by elimination.
 *
 * @param dir Spawn cardinal from the map ('N', 'S', 'E' or 'W').
 *
 * @return The facing angle in degrees, in the range [0, 360).
 */
static float	dir_to_angle(char dir)
{
	if (dir == 'E')
		return (0.0f);
	if (dir == 'S')
		return (90.0f);
	if (dir == 'W')
		return (180.0f);
	return (270.0f);
}

/**
 * @brief Entry point: parse the scene, run the render loop, clean up.
 *
 * Every failure exits 1, not ERROR. ERROR is -1, the sentinel the
 * parser propagates internally; returning it from main would reach the
 * shell as 255, since an exit status is the low byte of the return
 * value. 1 is what a caller testing for failure expects.
 *
 * @param argc Argument count; exactly 2 is required.
 * @param argv Argument vector; argv[1] is the .cub scene file.
 *
 * @return SUCCESS (0) on a clean run, 1 on any failure.
 */
int	main(int argc, char **argv)
{
	t_game	game;

	ft_memset(&game, 0, sizeof(game));
	if (argc != 2)
	{
		handle_error(ERR_ARGS);
		return (1);
	}
	if (parse_cub(argv[1], &game) != ERR_NONE)
	{
		free_game(&game);
		return (1);
	}
	game.player.dir_ang = dir_to_angle(game.player.dir);
	if (!render(&game))
	{
		free_game(&game);
		return (1);
	}
	free_game(&game);
	return (SUCCESS);
}
