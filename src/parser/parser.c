/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 14:48:50 by mona              #+#    #+#             */
/*   Updated: 2026/06/07 20:13:55 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/**
 * @brief Entry point of the .cub file parser.
 *
 * Validates the file extension, opens the file descriptor, and
 * orchestrates the full parsing pipeline: metadata (textures and
 * colors) followed by the map grid. Closes the fd in all paths,
 * including on error.
 *
 * @param path  Path to the .cub scene file.
 * @param game  Pointer to t_game to be fully populated on success.
 *
 * @return ERR_NONE on success, or an error code on failure:
 * @retval ERR_NO_CUB   If the file does not have a .cub extension.
 * @retval ERR_NO_FILE  If the file cannot be opened.
 * @retval ERR_MISSING_TEX   If any texture identifier is missing.
 * @retval ERR_INVALID_COLOR If a color line is malformed.
 * @retval ERR_INVALID_ID    If an unknown identifier is found.
 * @retval ERR_MAP_CHARS     If the map contains invalid characters.
 * @retval ERR_MAP_PLAYER    If the player spawn is missing or duplicated.
 * @retval ERR_MALLOC        If any memory allocation fails.
 */
int	parse_cub(const char *path, t_game *game)
{
	int			fd;
	int			err;
	char		*first_line;
	
	if (!has_cub_extension(path))
		return (handle_error(ERR_NO_CUB));
	fd = open(path, O_RDONLY);
	if (fd == ERROR)
		return (handle_error(ERR_NO_FILE));
	err = parse_meta(fd, &game->config, &first_line);
	if (err != ERR_NONE)
	{
		close(fd);
		return (err);
	}
	err = parse_map_grid(fd, &game->map, first_line, &game->player);
	if (err != ERR_NONE)
	{
		close(fd);
		return (err);
	}
	close(fd);
	return (ERR_NONE);
}
