/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 14:48:50 by mona              #+#    #+#             */
/*   Updated: 2026/06/07 22:54:28 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/**
 * @brief Exhausts the fd via get_next_line before closing it.
 *
 * get_next_line keeps its own static leftover buffer per fd, freed only
 * once it reads true EOF. Any parse path that stops early (an error, or
 * simply not needing the rest of the file) leaves that buffer stranded
 * forever if fd is just close()'d, since close() knows nothing about
 * get_next_line's internal heap state. Draining it here first — no
 * matter which implementation of get_next_line this is linked against —
 * guarantees its own free-at-EOF path always runs.
 *
 * @param fd Open file descriptor to drain and close.
 */
static void	close_and_drain(int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (line)
	{
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}

/**
 * @brief Entry point of the .cub file parser.
 *
 * Validates the file extension, opens the file descriptor, and
 * orchestrates the full parsing pipeline: metadata (textures and
 * colors) followed by the map grid. Closes the fd in all paths,
 * including on error.
 *
 * first_line is only ever populated once read_meta_lines finds the
 * map's first row — which can happen even when parse_meta goes on to
 * fail (validate_config runs after it, e.g. a missing texture). NULL
 * it up front so the failure branch can always free it: a no-op via
 * free(NULL) when read_meta_lines never got that far, and the fix for
 * a real leak when it did.
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

	first_line = NULL;
	if (!has_cub_extension(path))
		return (handle_error(ERR_NO_CUB));
	fd = open(path, O_RDONLY);
	if (fd == ERROR)
		return (handle_error(ERR_NO_FILE));
	err = parse_meta(fd, &game->config, &first_line);
	if (err != ERR_NONE)
	{
		free(first_line);
		close_and_drain(fd);
		return (err);
	}
	err = parse_map_grid(fd, &game->map, first_line, &game->player);
	if (err != ERR_NONE)
	{
		close_and_drain(fd);
		return (err);
	}
	close_and_drain(fd);
	return (ERR_NONE);
}
