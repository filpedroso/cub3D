/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 21:23:39 by mona              #+#    #+#             */
/*   Updated: 2026/08/09 19:27:28 by maria-ol         ###   ########.fr       */
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
				ERR_MLX           ← problema de inicialização da MLX42
				ERR_DUPLICATE_ID  ← NO/SO/WE/EA/F/C repetido no .cub
				ERR_MAP_EMPTY     ← header valido, nenhuma linha de mapa
 *
 * Messages go to STDERR_FILENO, not stdout: they are diagnostics, and
 * keeping them off stdout is what lets a caller redirect the two apart.
 *
 * @return Returns ERROR (-1), the internal "not ERR_NONE" sentinel every
 * parsing path propagates. It is not the process exit code — main maps
 * any failure onto 1.
 * @note ERR_NONE (index 0) is NULL and should never be passed to this function.
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
		"Error\nmap must have exactly one player spawn (N/S/E/W)\n",
		"Error\nfailed to load/missing texture: (NO/SO/WE/EA)\n",
		"Error\ninvalid RGB\n",
		"Error\ninvalid identifier in .cub\n",
		"Error\nMLX42 initialization failed\n",
		"Error\nduplicate identifier in .cub\n",
		"Error\nmissing map in .cub file\n"
	};

	if (error > 0 && error < (int)(sizeof(messages) / sizeof(messages[0])))
		ft_putstr_fd((char *)messages[error], STDERR_FILENO);
	return (ERROR);
}

/**
 * @brief Reports whether a map cell blocks movement and rays.
 *
 * Anything that is not open floor blocks. Spaces are legal map
 * characters and appear inside the map body (see subject_map.cub),
 * plus pad_grid introduces them on short rows, so testing for '1'
 * alone would let rays leak out through the void.
 *
 * Player spawn characters never reach here: find_player rewrites
 * them to '0' during parsing.
 *
 * @param c The grid character to test.
 *
 * @return true if the cell is solid, false if it is walkable floor.
 */
bool	is_solid(char c)
{
	return (c != '0');
}

/**
 * @brief Reports whether a map cell is a player spawn marker.
 *
 * The four cardinals are the only spawn characters the subject
 * defines. Kept as an explicit comparison rather than a strchr over
 * "NSEW" because strchr also matches the terminating '\0', which would
 * report every end-of-row as a spawn.
 *
 * @param c The grid character to test.
 *
 * @return true if c is one of 'N', 'S', 'E' or 'W'.
 */
bool	is_spawn(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}
