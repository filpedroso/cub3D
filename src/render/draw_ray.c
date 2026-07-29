/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ray.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 19:41:28 by fpedroso          #+#    #+#             */
/*   Updated: 2026/07/25 21:08:20 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static t_canvas_ray	prep_drawing_ray(t_player pl, t_dbl_coord ray_dir,
	double perp_dist);

void	draw_ray(t_game *game, t_player pl, t_dbl_coord ray_dir,
	double perp_dist)
{
	mlx_image_t		*map_img;
	t_dbl_coord		current_ray;
	t_canvas_ray	d_ray;
	double			steps;
	int				i;

	d_ray = prep_drawing_ray(pl, ray_dir, perp_dist);
	steps = fmax(fabs(d_ray.diff.x), fabs(d_ray.diff.y));
	map_img = game->map_img;
	i = 0;
	current_ray = d_ray.current;
	while (i <= (int)steps)
	{
		if (current_ray.x >= 0 && current_ray.x < map_img->width &&
			current_ray.y >= 0 && current_ray.y < map_img->height)
			mlx_put_pixel(map_img, (uint32_t)current_ray.x, (uint32_t)current_ray.y, PURPLE);
		current_ray.x += d_ray.diff.x / steps;
		current_ray.y += d_ray.diff.y / steps;
		i++;
	}
}

static t_canvas_ray	prep_drawing_ray(t_player pl, t_dbl_coord ray_dir, double perp_dist)
{
	t_canvas_ray	drawing_ray;
	
	drawing_ray.start.x = pl.x * TILE_SZ;
	drawing_ray.start.y = pl.y * TILE_SZ;
	drawing_ray.end.x = drawing_ray.start.x + ray_dir.x * perp_dist * TILE_SZ;
	drawing_ray.end.y = drawing_ray.start.y + ray_dir.y * perp_dist * TILE_SZ;
	drawing_ray.diff.x = drawing_ray.end.x - drawing_ray.start.x;
	drawing_ray.diff.y = drawing_ray.end.y - drawing_ray.start.y;
	drawing_ray.current = drawing_ray.start;
	return (drawing_ray);
}
