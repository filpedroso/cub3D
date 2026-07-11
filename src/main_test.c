/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 21:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/06/13 17:35:03 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** ARQUIVO DE TESTE DO PARSER — nao faz parte do projeto final
**
** Como usar:
**   1. No Makefile, trocar "main.c" por "main_test.c" em FILES
**   2. make re
**   3. ./cub3D maps/valid/map01.cub    <- deve passar
**   4. ./cub3D maps/invalid/invalid01.cub  <- deve dar ERR_MAP_CHARS
**   5. ./cub3D maps/valid/map01.ber    <- deve dar ERR_NO_CUB
**   6. ./cub3D                         <- deve dar ERR_ARGS
**
** O que cada teste valida:
**   - parse_cub retorna ERR_NONE no mapa valido
**   - config foi populada (paths das texturas)
**   - map.grid foi montado (rows correto)
**   - player foi encontrado (x, y, dir)
**   - spawn foi substituido por '0' no grid
**   - erros sao detectados e impressos corretamente
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
	printf("dir : %c\n", player->dir_c);
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
	free_game(&game);
	return (SUCCESS);
}
