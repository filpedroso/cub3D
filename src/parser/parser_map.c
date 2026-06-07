/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 21:23:33 by mona              #+#    #+#             */
/*   Updated: 2026/06/07 20:14:36 by mona             ###   ########.fr       */
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
 * @brief Reads the map grid from the file descriptor and populates t_map.
 *
 * Starts with first_map_line (already read by parse_meta) and continues
 * reading with get_next_line until EOF. Builds the grid dynamically
 * using ft_append_line, then runs validation steps.
 *
 * @param fd             Open file descriptor positioned after metadata.
 * @param map            Pointer to t_map to be populated.
 * @param first_map_line First map line already consumed by parse_meta.
 * @param player         Pointer to t_player to be populated via find_player.
 *
 * @return ERR_NONE on success, or an error code on failure:
 * @retval ERR_MALLOC     If ft_append_line fails to allocate memory.
 * @retval ERR_MAP_CHARS  If the grid contains invalid characters.
 * @retval ERR_MAP_PLAYER If no valid player spawn is found.
 *
 * TODO: add map->cols calculation (longest row).
 * TODO: add has_closed_walls validation -> ERR_MAP_OPEN.
 */
int	parse_map_grid(int fd, t_map *map, char *first_map_line, t_player *player)
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
	if (!has_only_valid_chars(map->grid))
		return (handle_error(ERR_MAP_CHARS));
	if (find_player(map->grid, player) != ERR_NONE)
		return (handle_error(ERR_MAP_PLAYER));
	return (ERR_NONE);
}
