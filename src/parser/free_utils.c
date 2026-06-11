/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 21:00:00 by maria-ol          #+#    #+#             */
/*   Updated: 2026/06/07 14:27:25 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/**
 * @brief Frees a dynamically allocated 2D map array.
 *
 * Iterates through each row of the map and frees it, then frees the
 * array of pointers itself. Safely handles NULL maps by returning early.
 * This is the standard cleanup function for map grids loaded from files.
 * 
 * @param map A NULL-terminated 2D array of strings to free.
 */
void	free_map(char **map)
{
	int	i;

	if (!map)
		return ;
	i = 0;
	while (map[i])
	{
		free(map[i]);
		i++;
	}
	free(map);
}

/**
 * @brief Frees a partially allocated visited array during pathfinding.
 *
 * Used for error handling during pathfinding when memory allocation fails
 * partway through creating the visited map. Frees only the rows that were
 * successfully allocated (from 0 to 'until'), then frees the array itself.
 * 
 * @param visited The partially allocated 2D array to free.
 * @param until The number of rows that were successfully allocated.
 */
void	free_visited_partial(char **visited, int until)
{
	while (until > 0)
		free(visited[--until]);
	free(visited);
}

/**
 * @brief Frees a complete visited array used in pathfinding.
 *
 * Deallocates a fully allocated visited map that was used during the
 * flood-fill pathfinding algorithm. Iterates through all rows and frees
 * them, then frees the array of pointers. Safely handles NULL arrays.
 * 
 * @param visited A NULL-terminated 2D array of characters to free.
 */
void	free_visited(char **visited)
{
	int	i;

	if (!visited)
		return ;
	i = 0;
	while (visited[i])
	{
		free(visited[i]);
		i++;
	}
	free(visited);
}
