/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_color.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 18:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/07/11 18:00:00 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/**
 * @brief Counts the number of parts produced by ft_split.
 *
 * @param parts A NULL-terminated array of strings.
 *
 * @return The number of non-NULL elements in parts.
 */
static int	count_parts(char **parts)
{
	int	i;

	i = 0;
	while (parts[i])
		i++;
	return (i);
}

/**
 * @brief Validates and converts a single RGB component.
 *
 * Ensures parts[i] holds one run of digits within [0, 255], then stores
 * the converted value in dest[i].
 *
 * Surrounding spaces are tolerated, so "C 255,214, 178" parses like
 * "C 255,214,178". Demanding the terminator after the trailing run is
 * what stops that from going too far: "2 20" stays rejected, since the
 * inner space leaves characters unconsumed.
 *
 * @param parts The split RGB parts (e.g. {"220", "100", "0"}).
 * @param i     Index of the part to validate (0, 1, or 2).
 * @param dest  The int[3] destination array (e.g. config->floor).
 *
 * @return ERR_NONE on success, or ERR_INVALID_COLOR on failure.
 */
static int	check_color_part(char **parts, int i, int *dest)
{
	char	*s;
	int		digits;
	int		value;

	s = parts[i];
	digits = 0;
	while (*s == ' ')
		s++;
	while (ft_isdigit(*s))
	{
		digits++;
		s++;
	}
	while (*s == ' ')
		s++;
	if (digits == 0 || *s != '\0')
		return (ERR_INVALID_COLOR);
	value = ft_atoi(parts[i]);
	if (value < 0 || value > 255)
		return (ERR_INVALID_COLOR);
	dest[i] = value;
	return (ERR_NONE);
}

/**
 * @brief Extracts and validates an RGB color from a metadata line.
 *
 * Advances past the one-character identifier ("F " or "C ") and any
 * trailing spaces, splits the remainder on ',', and validates that
 * it yields exactly three in-range digit values.
 *
 * @param line The full metadata line (e.g. "F 220,100,0").
 * @param dest The int[3] where the R, G, B values will be stored.
 *
 * @return ERR_NONE on success, or an error code on failure:
 * @retval ERR_INVALID_COLOR If parts count, digits, or range are wrong.
 * @retval ERR_MALLOC If ft_split fails to allocate memory.
 */
int	parse_color(const char *line, int dest[3])
{
	char	**parts;
	int		i;

	line += 2;
	while (*line == ' ')
		line++;
	parts = ft_split(line, ',');
	if (!parts)
		return (handle_error(ERR_MALLOC));
	if (count_parts(parts) != 3)
		return (color_error(parts));
	i = 0;
	while (i < 3)
	{
		if (check_color_part(parts, i, dest) != ERR_NONE)
			return (color_error(parts));
		i++;
	}
	free_map(parts);
	return (ERR_NONE);
}

/**
 * @brief Rejects a color identifier (F/C) seen a second time.
 *
 * dest[0] holds the -1 sentinel until the first matching line fills it
 * in, so a value other than -1 here means this identifier already
 * appeared once in the file.
 *
 * @param line The full metadata line, forwarded to parse_color.
 * @param dest The int[3] destination (config->floor or config->ceil).
 *
 * @return ERR_NONE on success, ERR_DUPLICATE_ID if already set, or
 * whatever parse_color returns on a malformed line.
 */
int	dispatch_color(const char *line, int dest[3])
{
	if (dest[0] != -1)
		return (handle_error(ERR_DUPLICATE_ID));
	return (parse_color(line, dest));
}
