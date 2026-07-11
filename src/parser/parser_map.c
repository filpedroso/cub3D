/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 21:23:33 by mona              #+#    #+#             */
/*   Updated: 2026/06/07 22:55:14 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/**
 * @brief Validates that the map contains only allowed characters.
 *
 * Iterates through every character in the map and verifies it matches
 * one of the valid game elements. Valid characters are:
 * - '0': Empty space
 * - '1': Wall
 * - 'N': North position on player
 * - 'S': South position on player
 * - 'E': East position on player
 * - 'W': West position on player
 * - 'D': doors(bonus)
 * - ' ': space
 * 
 * @param map A NULL-terminated 2D array of strings representing the map.
 *
 * @return 1 if all characters are valid, 0 if any invalid character is found.
 */
int	has_only_valid_chars(char **map)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			c = map[i][j];
			if (c == '\n')
				break ;
			if (c != '0' && c != '1' && c != 'N' && c != 'S' && c != 'E'
				&& c != 'W' && c != 'D' && c != ' ')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

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
 * Walks every walkable cell in the grid ('0', 'N', 'S', 'E', 'W', 'D')
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
					|| c == 'W' || c == 'D')
				&& !check_cell_neighbors(map, rows, i, j))
				return (FALSE);
			j++;
		}
		i++;
	}
	return (TRUE);
}

/**
 * @brief Finds the player spawn in the map grid and populates t_player.
 *
 * Searches the grid for a spawn character (N, S, E, W), stores the
 * position (x, y) and direction in the player struct, then replaces
 * the spawn character with '0' so the raycaster treats it as floor.
 *
 * @param map     NULL-terminated 2D array representing the map grid.
 * @param player  Pointer to t_player to be populated.
 *
 * @return ERR_NONE if the player was found and populated successfully.
 * @retval ERR_MAP_PLAYER If no spawn character is found in the grid.
 */
int	find_player(char **map, t_player *player)
{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == 'N' || map[i][j] == 'S'
				|| map[i][j] == 'E' || map[i][j] == 'W')
			{
				player->x = j;
				player->y = i;
				player->dir = map[i][j];
				map[i][j] = '0';
				return (ERR_NONE);
			}
			j++;
		}
		i++;
	}
	return (handle_error(ERR_MAP_PLAYER));
}

/**
 * @brief Reads all map lines from the fd and builds the char** grid.
 *
 * Starts with first_map_line (already read by parse_meta) and
 * continues reading with get_next_line until EOF, appending each
 * trimmed line to map->grid via ft_append_line.
 *
 * @param fd             Open file descriptor positioned after metadata.
 * @param map            Pointer to t_map to be populated.
 * @param first_map_line First map line already consumed by parse_meta.
 *
 * @return ERR_NONE on success, or ERR_MALLOC if ft_append_line fails.
 */
static int	build_map_grid(int fd, t_map *map, char *first_map_line)
{
	char	*line;
	int		count;

	line = first_map_line;
	count = 0;
	while (line)
	{
		trim_newline(line);
		map->grid = ft_append_line(map->grid, line, count);
		if (!map->grid)
		{
			free(line);
			return (handle_error(ERR_MALLOC));
		}
		count++;
		free(line);
		line = get_next_line(fd);
	}
	map->rows = count;
	return (ERR_NONE);
}

/**
 * @brief Reads the map grid from the file descriptor and populates t_map.
 *
 * Delegates line reading to build_map_grid, then runs the validation
 * pipeline in order: character validity, wall closure, player spawn.
 *
 * @param fd             Open file descriptor positioned after metadata.
 * @param map            Pointer to t_map to be populated.
 * @param first_map_line First map line already consumed by parse_meta.
 * @param player         Pointer to t_player to be populated via find_player.
 *
 * @return ERR_NONE on success, or an error code on failure:
 * @retval ERR_MALLOC     If build_map_grid fails to allocate memory.
 * @retval ERR_MAP_CHARS  If the grid contains invalid characters.
 * @retval ERR_MAP_OPEN   If the map is not fully closed by walls.
 * @retval ERR_MAP_PLAYER If no valid player spawn is found.
 *
 * TODO: add map->cols calculation (longest row).
 */
int	parse_map_grid(int fd, t_map *map, char *first_map_line, t_player *player)
{
	int	err;

	err = build_map_grid(fd, map, first_map_line);
	if (err != ERR_NONE)
		return (err);
	if (!has_only_valid_chars(map->grid))
		return (handle_error(ERR_MAP_CHARS));
	if (!has_closed_walls(map->grid, map->rows))
		return (handle_error(ERR_MAP_OPEN));
	if (find_player(map->grid, player) != ERR_NONE)
		return (handle_error(ERR_MAP_PLAYER));
	return (ERR_NONE);
}
