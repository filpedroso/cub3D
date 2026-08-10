/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 21:00:00 by maria-ol          #+#    #+#             */
/*   Updated: 2026/06/07 14:27:25 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/**
 * @brief Frees a dynamically allocated 2D map array.
 *
 * Iterates through each row of the map and frees it, then frees the
 * array of pointers itself. Safely handles NULL maps by returning early.
 * This is the standard cleanup function for map grids loaded from files.
 * 
 * @param map A NULL-terminated 2D array of strings to free.
 */
void	free_map(char **map)
{
	int	i;

	if (!map)
		return ;
	i = 0;
	while (map[i])
	{
		free(map[i]);
		i++;
	}
	free(map);
}

/**
 * @brief Frees a split result and reports an invalid color error.
 *
 * Centralizes the cleanup + error pattern shared by every failure
 * path in parse_color, avoiding repetition across call sites.
 *
 * @param parts The split result to free (NULL-safe via free_map).
 *
 * @return ERR_INVALID_COLOR, wrapped through handle_error.
 */
int	color_error(char **parts)
{
	free_map(parts);
	return (handle_error(ERR_INVALID_COLOR));
}

/**
 * @brief Frees all heap-allocated memory owned by the parser in t_game.
 *
 * Frees the four texture path strings in config and the map grid.
 * Scoped to parser-owned data only — MLX resources (mlx, image) are
 * owned by the render side and must be released separately.
 *
 * @param game Pointer to the t_game whose parser data will be freed.
 */
void	free_game(t_game *game)
{
	free(game->config.tex_north);
	free(game->config.tex_south);
	free(game->config.tex_west);
	free(game->config.tex_east);
	free_map(game->map.grid);
}
