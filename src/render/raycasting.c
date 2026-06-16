/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 18:31:05 by fpedroso          #+#    #+#             */
/*   Updated: 2026/06/13 18:31:05 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void		draw_ray(t_game *game, t_player pl, t_dbl_coord ray_dir, double perp_dist);
static double	cast_one_ray(double pos_x, double pos_y, t_dbl_coord ray_dir);

void	cast_rays(t_game *game)
{
	double		ray_angle; // em radianos
	double		perp_dist;
	t_dbl_coord	ray_dir;
	t_player	pl;
	int			i;

	ray_angle = 90 * (M_PI / 180.0);

	// faz virar float no plano matematico (e nao em pixels)
	pl.x = (double)(game->player_img->instances[0].x + CIRCLE_R) / SQUARE_SZ;
	pl.y = (double)(game->player_img->instances[0].y + CIRCLE_R) / SQUARE_SZ;

	i = 0;
	while (i < SCR_W)
	{
		ray_angle = (90 * (M_PI / 180.0)) - HALF_FOV + (i * FOV / SCR_W);

		ray_dir.x = cos(ray_angle);
		ray_dir.y = sin(ray_angle);

		perp_dist = cast_one_ray(pl.x, pl.y, ray_dir);
		draw_ray(game, pl, ray_dir, perp_dist);
		i++;
	}
}

static void	draw_ray(t_game *game, t_player pl, t_dbl_coord ray_dir, double perp_dist)
{
	t_dbl_coord	start;
	t_dbl_coord	end;
	t_dbl_coord	current;
	t_dbl_coord	diff;
	double		steps;
	int			i;

	start.x = pl.x * SQUARE_SZ;
	start.y = pl.y * SQUARE_SZ;
	end.x = start.x + ray_dir.x * perp_dist * SQUARE_SZ;
	end.y = start.y + ray_dir.y * perp_dist * SQUARE_SZ;
	diff.x = end.x - start.x;
	diff.y = end.y - start.y;
	steps = fmax(fabs(diff.x), fabs(diff.y));
	current = start;
	i = 0;
	while (i <= (int)steps)
	{
		if (current.x >= 0 && current.x < game->map_img->width &&
			current.y >= 0 && current.y < game->map_img->height)
			mlx_put_pixel(game->map_img, (uint32_t)current.x, (uint32_t)current.y, PURPLE);
		current.x += diff.x / steps;
		current.y += diff.y / steps;
		i++;
	}
}

static double	cast_one_ray(double pos_x, double pos_y, t_dbl_coord ray_dir)
{
	t_dbl_coord	delta_dist;
	t_int_coord	map_pos;
	t_int_coord	step_dir;
	t_dbl_coord	side_dist;
	double		perp_dist;
	int			side;

	map_pos.x = (int)pos_x;
	map_pos.y = (int)pos_y;

	delta_dist.x = (ray_dir.x == 0) ? 1e30 : fabs(1.0 / ray_dir.x);
	delta_dist.y = (ray_dir.y == 0) ? 1e30 : fabs(1.0 / ray_dir.y);

	if (ray_dir.x < 0)
	{
		step_dir.x = -1;
		side_dist.x = (pos_x - map_pos.x) * delta_dist.x;
	}
	else
	{
		step_dir.x = 1;
		side_dist.x = (map_pos.x + 1.0 - pos_x) * delta_dist.x;
	}
	if (ray_dir.y < 0)
	{
		step_dir.y = -1;
		side_dist.y = (pos_y - map_pos.y) * delta_dist.y;
	}
	else
	{
		step_dir.y = 1;
		side_dist.y = (map_pos.y + 1.0 - pos_y) * delta_dist.y;
	}
	while (1)
	{
		if (side_dist.x < side_dist.y)
		{
			side_dist.x += delta_dist.x;
			map_pos.x += step_dir.x;
			side = 0;
		}
		else
		{
			side_dist.y += delta_dist.y;
			map_pos.y += step_dir.y;
			side = 1;
		}
		if (gridmap[map_pos.y][map_pos.x] == 1)
			break;
	}
	if (side == 0)
		perp_dist = side_dist.x - delta_dist.x;
	else
		perp_dist = side_dist.y - delta_dist.y;
	return (perp_dist);
}
