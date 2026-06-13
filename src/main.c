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

int	gridmap[MAP_H][MAP_W] =
    {
        {1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,0,0,1,1,0,1},
        {1,0,0,0,0,1,1,1,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,1,0,0,0,0,0,0,1},
        {1,0,0,0,2,0,1,1,0,1},
        {1,0,0,0,0,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1}
    };

int	main(void)
{
	t_game	game;

	ft_memset(&game, 0, sizeof(game));
	run_minimap_mode(&game);
}




/* t_game	mock_game_data(void)
{
	t_game	game;
	int		i;
	int		j;

	ft_memset(&game, 0, sizeof(game));
	game.map.rows = MAP_H;
	game.map.cols = MAP_W;
	game.map.grid = malloc(sizeof(char *) * (MAP_H + 1));
	if (!game.map.grid)
		return (game);
	for (i = 0; i < MAP_H; i++)
	{
		game.map.grid[i] = malloc(sizeof(char) * (MAP_W + 1));
		if (!game.map.grid[i])
			return (game);
		for (j = 0; j < MAP_W; j++)
		{
			if (gridmap[i][j] == 1)
				game.map.grid[i][j] = '1';
			else
				game.map.grid[i][j] = '0';
		}
		game.map.grid[i][MAP_W] = '\0';
	}
	game.map.grid[MAP_H] = NULL;

	game.player.x = 1.5;
	game.player.y = 1.5;
	
	game.player.dir = 'N';
	game.map.grid[(int)game.player.y][(int)game.player.x] = game.player.dir;
	return (game);
} */
