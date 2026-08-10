/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 21:23:33 by mona              #+#    #+#             */
/*   Updated: 2026/08/09 19:43:13 by maria-ol         ###   ########.fr       */
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
				&& c != 'W' && c != ' ')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

/**
 * @brief Requires exactly one player spawn and populates t_player.
 *
 * Delegates each row to take_row_spawns, which fills player and clears
 * the marker to '0', and sums the spawns found across the whole grid.
 *
 * The count is what makes the map valid, not the first hit: the subject
 * describes N/S/E/W as "the player's start position", singular, and
 * defines no behaviour for a second one. Scanning to the end and
 * demanding a total of exactly 1 rejects the empty map and the
 * two-spawn map through the same test.
 *
 * Without this, a duplicate spawn used to survive parsing: the leftover
 * marker stayed in the grid, and since is_solid treats everything but
 * '0' as blocking, it silently became a phantom wall mid-room.
 *
 * @param map     NULL-terminated 2D array representing the map grid.
 * @param player  Pointer to t_player to be populated.
 *
 * @return ERR_NONE if exactly one spawn was found and populated.
 * @retval ERR_MAP_PLAYER If the grid holds no spawn, or more than one.
 */
int	find_player(char **map, t_player *player)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (map[i])
	{
		count += take_row_spawns(map[i], player, i);
		i++;
	}
	if (count != 1)
		return (ERR_MAP_PLAYER);
	return (ERR_NONE);
}

/**
 * @brief Updates map->cols if line is longer than the current max.
 *
 * Tracks the length of the longest row seen so far, since map rows
 * may have different lengths (ragged grid).
 *
 * @param map  Pointer to t_map whose cols field will be updated.
 * @param line The row to measure.
 */
static void	update_max_cols(t_map *map, const char *line)
{
	int	len;

	len = (int)ft_strlen(line);
	if (len > map->cols)
		map->cols = len;
}

/**
 * @brief Reads all map lines from the fd and builds the char** grid.
 *
 * Starts with first_map_line (already read by parse_meta) and
 * continues reading with get_next_line until EOF, appending each
 * trimmed line to map->grid via ft_append_line. Tracks map->cols
 * as the length of the longest row along the way.
 *
 * The new array goes through tmp rather than straight into map->grid,
 * because ft_append_line leaves the old array untouched when it fails.
 * Assigning its NULL directly would drop the only pointer to every row
 * read so far, and free_game would have nothing left to release.
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
	char	**tmp;
	int		count;

	line = first_map_line;
	count = 0;
	map->cols = 0;
	while (line)
	{
		trim_newline(line);
		update_max_cols(map, line);
		tmp = ft_append_line(map->grid, line, count);
		if (!tmp)
		{
			free(line);
			return (handle_error(ERR_MALLOC));
		}
		map->grid = tmp;
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
 * pipeline in order: grid exists, character validity, wall closure,
 * player spawn.
 *
 * The emptiness check comes first and is not optional. A file whose
 * header parses cleanly but carries no map line at all leaves
 * first_map_line NULL, so build_map_grid never enters its loop and
 * reports success with grid still NULL and rows still 0. Every
 * validator below walks grid[i] unguarded, so without this the next
 * line dereferences NULL.
 *
 * @param fd             Open file descriptor positioned after metadata.
 * @param map            Pointer to t_map to be populated.
 * @param first_map_line First map line already consumed by parse_meta.
 * @param player         Pointer to t_player to be populated via find_player.
 *
 * @return ERR_NONE on success, or an error code on failure:
 * @retval ERR_MALLOC     If build_map_grid fails to allocate memory.
 * @retval ERR_MAP_EMPTY  If the file holds no map line at all.
 * @retval ERR_MAP_CHARS  If the grid contains invalid characters.
 * @retval ERR_MAP_OPEN   If the map is not fully closed by walls.
 * @retval ERR_MAP_PLAYER If no valid player spawn is found.
 */
int	parse_map_grid(int fd, t_map *map, char *first_map_line, t_player *player)
{
	int	err;

	err = build_map_grid(fd, map, first_map_line);
	if (err != ERR_NONE)
		return (err);
	if (!map->grid || map->rows == 0)
		return (handle_error(ERR_MAP_EMPTY));
	if (!has_only_valid_chars(map->grid))
		return (handle_error(ERR_MAP_CHARS));
	if (!has_closed_walls(map->grid, map->rows))
		return (handle_error(ERR_MAP_OPEN));
	if (find_player(map->grid, player) != ERR_NONE)
		return (handle_error(ERR_MAP_PLAYER));
	return (pad_grid(map));
}
