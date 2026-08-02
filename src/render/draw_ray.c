/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ray.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 19:41:28 by fpedroso          #+#    #+#             */
/*   Updated: 2026/07/28 12:00:00 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static t_canvas_ray	prep_drawing_ray(t_game *game, int index);

/*
** Traces one ray onto the minimap, from the player to the wall it hit.
** Reads hit_dist, the true euclidean distance, rather than perp_dist,
** which is the fisheye-corrected value the 3D view wants.
*/
void	draw_ray(t_game *game, int index)
{
	t_canvas_ray	d_ray;
	t_dbl_coord		cur;
	double			steps;
	int				i;

	d_ray = prep_drawing_ray(game, index);
	steps = fmax(fabs(d_ray.diff.x), fabs(d_ray.diff.y));
	if (steps < 1.0)
		return ;
	cur = d_ray.start;
	i = 0;
	while (i <= (int)steps)
	{
		if (cur.x >= 0 && cur.x < game->map_img->width
			&& cur.y >= 0 && cur.y < game->map_img->height)
			mlx_put_pixel(game->map_img, (uint32_t)cur.x,
				(uint32_t)cur.y, PURPLE);
		cur.x += d_ray.diff.x / steps;
		cur.y += d_ray.diff.y / steps;
		i++;
	}
}

/*
** Both endpoints are computed in world tiles and converted once, so
** the minimap scale is applied in a single place.
*/
static t_canvas_ray	prep_drawing_ray(t_game *game, int index)
{
	t_canvas_ray	d_ray;
	t_dbl_coord		world;
	t_ray			*ray;

	ray = &game->rays[index];
	world.x = game->player.x;
	world.y = game->player.y;
	d_ray.start = tile_to_px(&game->minimap, world);
	world.x = game->player.x + ray->dir.x * ray->hit_dist;
	world.y = game->player.y + ray->dir.y * ray->hit_dist;
	d_ray.end = tile_to_px(&game->minimap, world);
	d_ray.diff.x = d_ray.end.x - d_ray.start.x;
	d_ray.diff.y = d_ray.end.y - d_ray.start.y;
	return (d_ray);
}
