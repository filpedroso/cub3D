/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 14:02:13 by mona              #+#    #+#             */
/*   Updated: 2026/06/07 14:02:37 by mona             ###   ########.fr       */
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
 * @return 1 if the filename has the .cub extension, 0 otherwise.
 * @retval 0 If filename is NULL or shorter than 4 characters.
 * @retval 1 If filename ends with ".cub".
 */
int	has_cub_extension(const char *filename)
{
	size_t	len;

	if (!filename)
		return (0);
	len = ft_strlen(filename);
	if (len < 4)
		return (0);
	if (ft_strncmp(filename + len - 4, ".cub", 4) == 0)
		return (1);
	return (0);
}
