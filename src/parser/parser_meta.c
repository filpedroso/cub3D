/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_meta.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 14:55:45 by mona              #+#    #+#             */
/*   Updated: 2026/06/07 20:17:30 by mona             ###   ########.fr       */
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
	if (line[0] == '1' || line[0] == '0' || line[0] == 'N'  || line[0] == 'S'
		|| line[0] == 'E' || line[0] == 'W' || line[0] == ' ')
		return (TRUE);
	return (FALSE);
}

/**
 * @brief Extracts and validates a texture path from a metadata line.
 *
 * Advances past the two-character identifier (e.g. "NO") and any
 * trailing spaces, then validates that the remaining path exists and
 * ends with the ".png" extension. On success, duplicates the path
 * string and stores it in the destination pointer.
 *
 * @param line  The full metadata line (e.g. "NO ./assets/north.png").
 * @param dest  Pointer to the char* where the path will be stored.
 *
 * @return ERR_NONE on success, or an error code on failure:
 * @retval ERR_MISSING_TEX If the path is empty or not a .png file.
 * @retval ERR_MALLOC      If ft_strdup fails to allocate memory.
 */
static int	parse_texture(const char *line, char **dest)
{
	const char	*path;
	char	*tmp;

	path = line + 2;
	while (*path == ' ')
		path++;
	if (*path == '\0')
		return (handle_error(ERR_MISSING_TEX));
	if (!has_png_extension(path))
		return (handle_error(ERR_MISSING_TEX));
	tmp = ft_strdup(path);
	if (!tmp)
		return (handle_error(ERR_MALLOC));
	*dest = tmp;
	return (ERR_NONE);
}

/*
** parse_color - extrai e valida uma cor RGB
**
** Recebe a linha completa ex: "F 220,100,0"
** e o array int[3] onde guardar os valores (ex: config->floor)
**
** Passos:
** 1. Avancar alem do identificador ("F " ou "C ")
**    - pular espacos extras
** 2. Usar ft_split com ',' para separar os tres valores
**    - se nao tiver exatamente 3 partes, retornar ERR_INVALID_COLOR
** 3. Para cada valor:
**    - verificar que e so digitos (ft_isdigit em cada char)
**    - converter com ft_atoi
**    - verificar que esta no range [0, 255]
**    - se qualquer verificacao falhar, retornar ERR_INVALID_COLOR
** 4. Guardar os tres valores no array dest[0], dest[1], dest[2]
** 5. Liberar o resultado do ft_split
** 6. Retornar ERR_NONE
*/
static int	parse_color(const char *line, int dest[3])
{
	(void)line;
	(void)dest;
	return (ERR_NONE);
}

/**
 * @brief Identifies the metadata identifier and dispatches to the correct parser.
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
		return (parse_texture(line, &config->tex_north));
	else if (ft_strncmp(line, "SO ", 3) == 0)
		return (parse_texture(line, &config->tex_south));
	else if (ft_strncmp(line, "WE ", 3) == 0)
		return (parse_texture(line, &config->tex_west));
	else if (ft_strncmp(line, "EA ", 3) == 0)
		return (parse_texture(line, &config->tex_east));
	else if (ft_strncmp(line, "F ", 2) == 0)
		return (parse_color(line, config->floor));
	else if (ft_strncmp(line, "C ", 2) == 0)
		return (parse_color(line, config->ceil));
	return (handle_error(ERR_INVALID_ID));
}

/**
 * @brief Reads and parses all metadata lines from the .cub file descriptor.
 *
 * Reads line by line using get_next_line, skipping empty lines, and
 * dispatching each metadata line to dispatch_meta_line. Stops when a
 * map line is detected via is_map_line, preserving that line in
 * first_map_line for parse_map_grid to consume.
 *
 * @param fd              Open file descriptor positioned at the start of the file.
 * @param config          Pointer to t_config to be populated with metadata.
 * @param first_map_line  Address of a char* to store the first map line.
 *
 * @return ERR_NONE on success, or an error code on failure:
 * @retval ERR_MISSING_TEX   If any of the four textures (NO/SO/WE/EA) is absent.
 * @retval ERR_INVALID_COLOR If a color line (F/C) is malformed.
 * @retval ERR_INVALID_ID    If an unrecognized identifier is encountered.
 * @retval ERR_MALLOC        If memory allocation fails during parsing.
 */
int	parse_meta(int fd, t_config *config, char **first_map_line)
{
	char	*line;
	int		err;

	line = get_next_line(fd);
	while (line && !is_map_line(line))
	{
		trim_newline(line);
		err = dispatch_meta_line(line, config);
		if (err != ERR_NONE)
		{
			free(line);
			return (err);
		}
		free(line);
		line = get_next_line(fd);
	}
	if (line)
		*first_map_line = line;
	if (!config->tex_north || !config->tex_south
		|| !config->tex_west || !config->tex_east)
		return (handle_error(ERR_MISSING_TEX));
	// falta lidar com a color quando parse_color estiver pronto
	return (ERR_NONE);
}
