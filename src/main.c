/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 14:25:43 by fpedroso          #+#    #+#             */
/*   Updated: 2026/06/01 14:36:44 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* scratch/integration only: shim feeding parse_cub's dynamic grid into the
** fixed-size gridmap[MAP_H][MAP_W] the render code reads. Real fix (render
** using game.map directly) is a joint decision with Fil, not made here. */
int	gridmap[MAP_H][MAP_W];

static void	copy_grid(t_map *map, t_player *player)
{
	int	i;
	int	j;

	i = 0;
	while (i < MAP_H)
	{
		j = 0;
		while (j < MAP_W)
		{
			if (i < map->rows && (size_t)j < ft_strlen(map->grid[i])
				&& map->grid[i][j] == '1')
				gridmap[i][j] = 1;
			else
				gridmap[i][j] = 0;
			j++;
		}
		i++;
	}
	gridmap[(int)player->y][(int)player->x] = 2;
}

static float	dir_c_to_angle(char dir_c)
{
	if (dir_c == 'E')
		return (0.0f);
	if (dir_c == 'S')
		return (90.0f);
	if (dir_c == 'W')
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
	copy_grid(&game.map, &game.player);
	game.player.dir_ang = dir_c_to_angle(game.player.dir_c);
	run_minimap_mode(&game);
	free_game(&game);
	return (SUCCESS);
}
