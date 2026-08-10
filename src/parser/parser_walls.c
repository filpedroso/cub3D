/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_walls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 18:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/07/11 18:00:00 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/**
 * @brief Checks whether a single grid cell is a valid neighbor.
 *
 * A neighbor is invalid if it falls outside the grid's row bounds,
 * outside the specific row's column bounds (rows may have different
 * lengths), or if it is a space character (an opening to the outside).
 *
 * @param map  NULL-terminated 2D array representing the map grid.
 * @param rows Total number of rows in the grid.
 * @param row  Row index of the neighbor to check.
 * @param col  Column index of the neighbor to check.
 *
 * @return TRUE if the neighbor is a valid, closed cell, FALSE otherwise.
 */
static int	is_valid_neighbor(char **map, int rows, int row, int col)
{
	if (row < 0 || row >= rows)
		return (FALSE);
	if (col < 0 || (size_t)col >= ft_strlen(map[row]))
		return (FALSE);
	if (map[row][col] == ' ')
		return (FALSE);
	return (TRUE);
}

/**
 * @brief Checks the four cardinal neighbors of a single grid cell.
 *
 * @param map  NULL-terminated 2D array representing the map grid.
 * @param rows Total number of rows in the grid.
 * @param i    Row index of the cell being checked.
 * @param j    Column index of the cell being checked.
 *
 * @return TRUE if all four neighbors are valid, FALSE otherwise.
 */
static int	check_cell_neighbors(char **map, int rows, int i, int j)
{
	if (!is_valid_neighbor(map, rows, i - 1, j))
		return (FALSE);
	if (!is_valid_neighbor(map, rows, i + 1, j))
		return (FALSE);
	if (!is_valid_neighbor(map, rows, i, j - 1))
		return (FALSE);
	if (!is_valid_neighbor(map, rows, i, j + 1))
		return (FALSE);
	return (TRUE);
}

/**
 * @brief Validates that the map is fully enclosed by walls.
 *
 * Walks every walkable cell in the grid ('0', 'N', 'S', 'E', 'W')
 * and checks that its four neighbors are valid closed cells. If any
 * walkable cell borders a space or the edge of the grid, the map is
 * considered open.
 *
 * @param map  NULL-terminated 2D array representing the map grid.
 * @param rows Total number of rows in the grid.
 *
 * @return TRUE if the map is closed, FALSE if any leak is found.
 */
int	has_closed_walls(char **map, int rows)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	while (i < rows)
	{
		j = 0;
		while (map[i][j])
		{
			c = map[i][j];
			if ((c == '0' || c == 'N' || c == 'S' || c == 'E'
					|| c == 'W')
				&& !check_cell_neighbors(map, rows, i, j))
				return (FALSE);
			j++;
		}
		i++;
	}
	return (TRUE);
}
