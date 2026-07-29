/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_frame.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 21:42:45 by fpedroso          #+#    #+#             */
/*   Updated: 2026/07/28 20:28:44 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_frame(t_game *game)
{
	game->map_img->enabled = game->show_minimap;
	game->main_img->enabled = game->show_minimap;
	if (game->show_minimap)
		draw_minimap(game);
	// draw_3d(game);
}
