/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_meta.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 14:55:45 by mona              #+#    #+#             */
/*   Updated: 2026/08/10 17:48:52 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/**
 * @brief Detects if a line marks the beginning of the map grid.
 *
 * Checks the first character of the line to determine if it belongs
 * to the map section of the .cub file. Map lines start with a valid
 * map character: '0', '1', 'N', 'S', 'E', 'W', or ' ' (space).
 *
 * @param line The line string to check. Must not be NULL.
 *
 * @return TRUE if the line is a map line, FALSE otherwise.
 * @retval FALSE If line is NULL or empty.
 * @retval TRUE If the first character is a valid map character.
 */
static int	is_map_line(const char *line)
{
	if (!line || line[0] == '\0')
		return (FALSE);
	if (line[0] == '1' || line[0] == '0' || line[0] == ' ')
		return (TRUE);
	if (line[0] == 'N' || line[0] == 'S'
		|| line[0] == 'E' || line[0] == 'W')
		return (line[1] == ' ' || line[1] == '\0' || line[1] == '\n');
	return (FALSE);
}

/**
 * @brief Identifies the metadata id and dispatches to the correct parser.
 *
 * Compares the beginning of the line against all valid identifiers
 * (NO, SO, WE, EA, F, C) and calls the appropriate parsing function.
 * Returns ERR_INVALID_ID if no known identifier matches.
 *
 * @param line    The metadata line to identify and dispatch.
 * @param config  Pointer to t_config to be populated.
 *
 * @return ERR_NONE on success, or an error code on failure:
 * @retval ERR_MISSING_TEX   If a texture path is invalid or missing.
 * @retval ERR_INVALID_COLOR If a color value is out of range or malformed.
 * @retval ERR_INVALID_ID    If the line starts with an unknown identifier.
 * @retval ERR_MALLOC        If memory allocation fails during parsing.
 */
static int	dispatch_meta_line(const char *line, t_config *config)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		return (dispatch_tex(line, &config->tex_north));
	else if (ft_strncmp(line, "SO ", 3) == 0)
		return (dispatch_tex(line, &config->tex_south));
	else if (ft_strncmp(line, "WE ", 3) == 0)
		return (dispatch_tex(line, &config->tex_west));
	else if (ft_strncmp(line, "EA ", 3) == 0)
		return (dispatch_tex(line, &config->tex_east));
	else if (ft_strncmp(line, "F ", 2) == 0)
		return (dispatch_color(line, config->floor));
	else if (ft_strncmp(line, "C ", 2) == 0)
		return (dispatch_color(line, config->ceil));
	return (handle_error(ERR_INVALID_ID));
}

/**
 * @brief Validates that all required metadata was successfully parsed.
 *
 * Checks that all four textures were set and that both floor and
 * ceiling colors were parsed (via the -1 sentinel set at the start
 * of parse_meta).
 *
 * @param config Pointer to the populated t_config.
 *
 * @return ERR_NONE on success, or an error code on failure:
 * @retval ERR_MISSING_TEX   If any of the four textures is absent.
 * @retval ERR_INVALID_COLOR If floor or ceiling color was never set.
 */
static int	validate_config(t_config *config)
{
	if (!config->tex_north || !config->tex_south
		|| !config->tex_west || !config->tex_east)
		return (handle_error(ERR_MISSING_TEX));
	if (config->floor[0] == -1 || config->ceil[0] == -1)
		return (handle_error(ERR_INVALID_COLOR));
	return (ERR_NONE);
}

/**
 * @brief Reads metadata lines until the map section is reached.
 *
 * Loops via get_next_line, trimming each line and dispatching
 * non-empty ones through dispatch_meta_line. Stops as soon as a
 * map line is detected, storing it (or NULL, at EOF) in
 * first_map_line for parse_map_grid to consume.
 *
 * @param fd             Open file descriptor at the start of the file.
 * @param config         Pointer to t_config to be populated.
 * @param first_map_line Address of a char* to store the first map line.
 *
 * @return ERR_NONE on success, or an error code from dispatch_meta_line.
 */
static int	read_meta_lines(int fd, t_config *config, char **first_map_line)
{
	char	*line;
	int		err;

	line = get_next_line(fd);
	while (line)
	{
		trim_newline(line);
		if (is_map_line(line))
			break ;
		if (line[0] != '\0')
		{
			err = dispatch_meta_line(line, config);
			if (err != ERR_NONE)
			{
				free(line);
				return (err);
			}
		}
		free(line);
		line = get_next_line(fd);
	}
	*first_map_line = line;
	return (ERR_NONE);
}

/**
 * @brief Reads and parses all metadata lines from the .cub file descriptor.
 *
 * Initializes the color sentinels, delegates line reading and
 * dispatching to read_meta_lines, then validates that every required
 * field (four textures, floor and ceiling colors) was populated.
 *
 * @param fd Open file descriptor positioned at the start of the file.
 * @param config Pointer to t_config to be populated with metadata.
 * @param first_map_line Address of a char* to store the first map line.
 *
 * @return ERR_NONE on success, or an error code on failure:
 * @retval ERR_MISSING_TEX   If any of the 4 textures (NO/SO/WE/EA) is absent.
 * @retval ERR_INVALID_COLOR If a color line (F/C) is malformed.
 * @retval ERR_INVALID_ID    If an unrecognized identifier is encountered.
 * @retval ERR_MALLOC        If memory allocation fails during parsing.
 */
int	parse_meta(int fd, t_config *config, char **first_map_line)
{
	int	err;

	config->floor[0] = -1;
	config->ceil[0] = -1;
	err = read_meta_lines(fd, config, first_map_line);
	if (err != ERR_NONE)
		return (err);
	return (validate_config(config));
}
