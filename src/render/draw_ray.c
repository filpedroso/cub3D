/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ray.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 19:41:28 by fpedroso          #+#    #+#             */
/*   Updated: 2026/06/16 19:41:28 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static t_canvas_ray	prep_drawing_ray(t_player pl, t_dbl_coord ray_dir,
	double perp_dist);

void	draw_ray(t_game *game, t_player pl, t_dbl_coord ray_dir,
	double perp_dist)
{
	t_canvas_ray	d_ray;
	double			steps;
	int				i;

	d_ray = prep_drawing_ray(pl, ray_dir, perp_dist);

	steps = fmax(fabs(d_ray.diff.x), fabs(d_ray.diff.y));
	i = 0;
	while (i <= (int)steps)
	{
		if (d_ray.current.x >= 0 && d_ray.current.x < game->map_img->width &&
			d_ray.current.y >= 0 && d_ray.current.y < game->map_img->height)
			mlx_put_pixel(game->map_img, (uint32_t)d_ray.current.x, (uint32_t)d_ray.current.y, PURPLE);
		d_ray.current.x += d_ray.diff.x / steps;
		d_ray.current.y += d_ray.diff.y / steps;
		i++;
	}
}

static t_canvas_ray	prep_drawing_ray(t_player pl, t_dbl_coord ray_dir, double perp_dist)
{
	t_canvas_ray	drawing_ray;
	
	drawing_ray.start.x = pl.x * SQUARE_SZ;
	drawing_ray.start.y = pl.y * SQUARE_SZ;
	drawing_ray.end.x = drawing_ray.start.x + ray_dir.x * perp_dist * SQUARE_SZ;
	drawing_ray.end.y = drawing_ray.start.y + ray_dir.y * perp_dist * SQUARE_SZ;
	drawing_ray.diff.x = drawing_ray.end.x - drawing_ray.start.x;
	drawing_ray.diff.y = drawing_ray.end.y - drawing_ray.start.y;
	drawing_ray.current = drawing_ray.start;
	return (drawing_ray);
}
