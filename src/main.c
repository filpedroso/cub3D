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

int	main(int argc, char **argv)
{
	t_game	game;

	ft_memset(&game, 0, sizeof(game));
	if (argc != 2)
	{
		handle_error(ERR_ARGS);
		return (ERROR);
	}
	if (parse_cub(argv[1], &game) != ERR_NONE)
		return (ERROR);
	game.player.dir_ang = dir_to_angle(game.player.dir);
	render(&game);
	free_game(&game);
	return (SUCCESS);
}
