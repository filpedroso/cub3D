/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test_with_render.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 22:07:43 by fpedroso          #+#    #+#             */
/*   Updated: 2026/07/25 16:52:21 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** ARQUIVO DE TESTE DO PARSER + RENDER — nao faz parte do projeto final
** FEITO A PARTIR DO ARQUIVO DE TESTE DO PARSER DA MONA
**
** Como usar:
**   1. No Makefile, trocar "main.c" por "main_test_with_render.c" em FILES
**   2. make re
**   3. ./cub3D maps/valid/map01.cub    <- deve rodar com raycasting
*/

#include "../include/cub3d.h"

static void	print_config(t_config *config)
{
	printf("\n--- CONFIG ---\n");
	printf("tex_north : %s\n", config->tex_north ? config->tex_north : "NULL");
	printf("tex_south : %s\n", config->tex_south ? config->tex_south : "NULL");
	printf("tex_west  : %s\n", config->tex_west ? config->tex_west : "NULL");
	printf("tex_east  : %s\n", config->tex_east ? config->tex_east : "NULL");
	printf("floor RGB : [%d, %d, %d]\n",
		config->floor[0], config->floor[1], config->floor[2]);
	printf("ceil  RGB : [%d, %d, %d]\n",
		config->ceil[0], config->ceil[1], config->ceil[2]);
}

static void	print_map(t_map *map)
{
	int	i;

	printf("\n--- MAP GRID (%d rows, %d cols max) ---\n", map->rows, map->cols);
	i = 0;
	while (i < map->rows)
	{
		printf("  row %02d: [%s]\n", i, map->grid[i]);
		i++;
	}
}

static void	print_player(t_player *player)
{
	printf("\n--- PLAYER ---\n");
	printf("x   : %f\n", player->x);
	printf("y   : %f\n", player->y);
	printf("dir : %c\n", player->dir);
}

int	main(int argc, char **argv)
{
	t_game	game;
	int		err;

	if (argc != 2)
	{
		handle_error(ERR_ARGS);
		return (ERROR);
	}
	memset(&game, 0, sizeof(game));
	err = parse_cub(argv[1], &game);
	if (err != ERR_NONE)
	{
		printf("\nParse falhou com codigo: %d\n", err);
		free_game(&game);
		return (ERROR);
	}
	printf("\nParse bem-sucedido!");
	print_config(&game.config);
	print_map(&game.map);
	print_player(&game.player);
	printf("\n--- VERIFICACAO EXTRA ---\n");
	printf("grid[player->y][player->x] = '%c' (esperado: '0')\n",
		game.map.grid[(int)game.player.y][(int)game.player.x]);
	printf("\nTudo certo! O parser esta funcionando.\n\n");
	render(&game);
	free_game(&game);
	return (SUCCESS);
}
