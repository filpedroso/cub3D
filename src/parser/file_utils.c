/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 14:02:13 by mona              #+#    #+#             */
/*   Updated: 2026/06/07 15:52:46 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/**
 * @brief Checks if the filename has the required .cub extension.
 *
 * Validates that the provided filename ends with the ".cub" extension,
 * which is required for map files in the cub3d game. The function
 * performs a string comparison on the last 4 characters of the filename.
 * 
 * @param filename The filename string to validate. Must not be NULL.
 *
 * @return TRUE if the filename has the .cub extension, FALSE otherwise.
 * @retval FALSE If filename is NULL or shorter than 4 characters.
 * @retval TRUE If filename ends with ".cub".
 */
int	has_cub_extension(const char *filename)
{
	size_t	len;

	if (!filename)
		return (FALSE);
	len = ft_strlen(filename);
	if (len < 4)
		return (FALSE);
	if (ft_strncmp(filename + len - 4, ".cub", 4) == 0)
		return (TRUE);
	return (FALSE);
}

/**
 * @brief Checks if the filename has the required .png extension.
 *
 * Validates that the provided filename ends with the ".png" extension,
 * which is required for map files in the cub3d game. The function
 * performs a string comparison on the last 4 characters of the filename.
 * 
 * @param filename The filename string to validate. Must not be NULL.
 *
 * @return TRUE if the filename has the .png extension, FALSE otherwise.
 * @retval FALSE If filename is NULL or shorter than 4 characters.
 * @retval TRUE If filename ends with ".png".
 */
int	has_png_extension(const char *filename)
{
	size_t	len;

	if (!filename)
		return (FALSE);
	len = ft_strlen(filename);
	if (len < 4)
		return (FALSE);
	if (ft_strncmp(filename + len - 4, ".png", 4) == 0)
		return (TRUE);
	return (FALSE);
}

/**
 * @brief Removes the trailing newline character from a string.
 *
 * This helper function checks if the last character of the string is a
 * newline ('\n') and replaces it with a null terminator to clean up
 * lines read from files.
 * 
 * @param line The string to trim. Must not be NULL.
 *
 * @return The trimmed string (same pointer as input).
 */
char	*trim_newline(char *line)
{
	size_t	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	return (line);
}
