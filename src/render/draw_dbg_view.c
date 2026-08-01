/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_dbg_view.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 20:10:00 by fpedroso          #+#    #+#             */
/*   Updated: 2026/08/01 12:30:00 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void		draw_ceiling(t_game *game, t_column *col);
static void		draw_wall_solid(t_game *game, t_column *col);
static uint32_t	dbg_face_color(t_face face);

/*
** The T view: one flat colour per wall face, and a ruled floor.
**
** Kept around because it is the cheapest way to see a perspective bug.
** The floor lines run along the world tile boundaries, so a wrong
** horizon, a bad fisheye correction or a wrong distance scale all show
** up as bent or unevenly spaced lines. Textures hide every one of those.
**
** Unlike the textured view this cannot fill its background in rows: the
** floor genuinely varies per column, so ceiling, wall and floor are
** drawn as three spans of a single strip.
*/
void	draw_view_dbg(t_game *game)
{
	t_column	col;
	int			i;

	i = 0;
	while (i < RAY_COUNT)
	{
		col = prep_column(game, i);
		draw_ceiling(game, &col);
		draw_wall_solid(game, &col);
		draw_floor(game, &col);
		i++;
	}
}

/*
** Everything above the wall. Runs to col->start, which prep_column
** already clamped, so a wall taller than the window simply draws no
** ceiling rather than looping backwards.
*/
static void	draw_ceiling(t_game *game, t_column *col)
{
	int32_t	y;

	y = 0;
	while (y < col->start)
	{
		mlx_put_pixel(game->main_img, col->x, y, SKY);
		y++;
	}
}

static void	draw_wall_solid(t_game *game, t_column *col)
{
	uint32_t	color;
	int32_t		y;

	color = dbg_face_color(col->face);
	y = col->start;
	while (y <= col->end)
	{
		mlx_put_pixel(game->main_img, col->x, y, color);
		y++;
	}
}

/*
** Indexed by t_face, in the order the enum declares them. A corner then
** shows four different colours at once, which is what makes a wrong
** face rule obvious at a glance.
*/
static uint32_t	dbg_face_color(t_face face)
{
	static const uint32_t	colors[4] = {FACE_N, FACE_S, FACE_W, FACE_E};

	return (colors[face]);
}
