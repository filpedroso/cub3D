/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 21:23:39 by mona              #+#    #+#             */
/*   Updated: 2026/06/07 14:10:57 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/**
 * @brief Centralized error handler for the cub3d game.
 *
 * This function manages all error messages through a static lookup table that
 * maps error codes from the t_error enum to their corresponding error messages.
 * All messages follow the format "Error\n<description>\n" for consistency.
 *
 * The function uses array indexing for O(1) lookup time and avoids repetitive
 * if-else chains. Messages are stored as const char* for memory efficiency.
 *
 * @param error Error code from the t_error enum. Valid values:
 *             	ERR_NONE
				ERR_ARGS          ← argc != 2
				ERR_NO_FILE       ← não conseguiu abrir
				ERR_NO_CUB        ← extensão errada
				ERR_MALLOC        ← falha de alocação
				ERR_MAP_CHARS     ← caracteres inválidos no mapa
				ERR_MAP_OPEN      ← mapa não fechado por paredes
				ERR_MAP_PLAYER    ← nenhum ou mais de um spawn
				ERR_MISSING_TEX   ← faltou alguma textura NO/SO/WE/EA
				ERR_INVALID_COLOR ← cor RGB inválida
				ERR_INVALID_ID    ← identificador desconhecido no .cub
 *
 * @return Always returns 1 to indicate error status for main() exit code.
 * @note The first element (index 0) is NULL as error codes start at 1.
 * @see t_error enum definition in cub3d.h
 * @see main() in cub3d.c for usage examples
 */
int	handle_error(t_error error)
{
	static const char	*messages[] = {
		NULL,
		"Error\ninvalid arguments\n",
		"Error\ninvalid file\n",
		"Error\nmissing .cub extension\n",
		"Error\nallocation with malloc failed\n",
		"Error\nmap has invalid characters\n",
		"Error\nmap is not closed by walls\n",
		"Error\ninvalid number of player\n",
		"Error\nfailed to load/missing texture: (NO/SO/WE/EA)\n",
		"Error\ninvalid RGB\n",
		"Error\ninvalid identifier in .cub\n"
	};

	if (error > 0 && error < (int)(sizeof(messages) / sizeof(messages[0])))
	{
		ft_printf("%s", messages[error]);
	}
	return (1);
}
