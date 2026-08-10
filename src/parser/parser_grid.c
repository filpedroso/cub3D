/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_grid.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 12:00:00 by fpedroso          #+#    #+#             */
/*   Updated: 2026/08/09 19:50:47 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/**
 * @brief Reallocates a row to cols characters, space-filling the tail.
 *
 * @param row  The row to grow. Freed on success.
 * @param len  Current length of the row.
 * @param cols Target width.
 *
 * @return The new row, or NULL if allocation failed (row untouched).
 */
static char	*grow_row(char *row, int len, int cols)
{
	char	*padded;
	int		i;

	padded = malloc((size_t)cols + 1);
	if (!padded)
		return (NULL);
	i = 0;
	while (i < len)
	{
		padded[i] = row[i];
		i++;
	}
	while (i < cols)
	{
		padded[i] = ' ';
		i++;
	}
	padded[cols] = '\0';
	free(row);
	return (padded);
}

/**
 * @brief Pads every row with spaces so the grid becomes rectangular.
 *
 * build_map_grid sets map->cols to the longest row but leaves shorter
 * rows short, so an indexed read of grid[y][x] for x < cols could run
 * past a row's terminator. subject_map.cub has rows of 33, 31 and 29
 * characters, which was enough to overrun the raycaster.
 *
 * Padding here makes "the grid is rectangular" a postcondition of
 * parsing, so every consumer downstream can index with nothing but a
 * rows/cols bounds check. The filler is a space, which is neither
 * wall nor floor: no ray can reach it (see is_solid) and the minimap
 * leaves it unpainted, so the drawing matches the shape written in
 * the .cub.
 *
 * Runs after validation, so has_only_valid_chars, has_closed_walls
 * and find_player all still see the file exactly as it was written.
 *
 * @param map Pointer to t_map whose grid is normalised in place.
 *
 * @return ERR_NONE on success, or ERR_MALLOC if a row cannot grow.
 */
int	pad_grid(t_map *map)
{
	char	*padded;
	int		len;
	int		i;

	i = 0;
	while (i < map->rows)
	{
		len = (int)ft_strlen(map->grid[i]);
		if (len < map->cols)
		{
			padded = grow_row(map->grid[i], len, map->cols);
			if (!padded)
				return (handle_error(ERR_MALLOC));
			map->grid[i] = padded;
		}
		i++;
	}
	return (ERR_NONE);
}

/**
 * @brief Records every spawn marker on one row and returns how many.
 *
 * Scans a single row, and for each spawn found stores it in player and
 * rewrites the cell to '0' so the raycaster sees plain floor. The
 * position is offset to the centre of the tile: spawning exactly on a
 * corner leaves the DDA with a side_dist of zero on one axis.
 *
 * It keeps scanning after the first hit instead of returning early,
 * because the caller needs the true total to reject a map carrying two
 * spawns. When a row does hold more than one, the last one wins the
 * player fields, any count other than 1 makes find_player fail and the
 * whole t_game is freed.
 *
 * @param row    A single grid row, modified in place.
 * @param player Pointer to t_player to be populated.
 * @param y      Index of this row in the grid, used as the y coordinate.
 *
 * @return The number of spawn markers found on this row.
 */
int	take_row_spawns(char *row, t_player *player, int y)
{
	int	x;
	int	count;

	x = 0;
	count = 0;
	while (row[x])
	{
		if (is_spawn(row[x]))
		{
			player->x = x + 0.5;
			player->y = y + 0.5;
			player->dir = row[x];
			row[x] = '0';
			count++;
		}
		x++;
	}
	return (count);
}
