/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_frame.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 21:42:45 by fpedroso          #+#    #+#             */
/*   Updated: 2026/07/28 12:00:00 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** main_img is the 3D view and is never optional; only the minimap
** overlay answers to the M toggle.
*/
void	draw_frame(t_game *game)
{
	game->main_img->enabled = true;
	game->map_img->enabled = game->show_minimap;
	if (game->show_minimap)
		draw_minimap(game);
}
