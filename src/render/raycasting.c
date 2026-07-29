/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 18:31:05 by fpedroso          #+#    #+#             */
/*   Updated: 2026/07/25 18:57:03 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static double			cast_one_ray(t_map *map, double pos_x, double pos_y, t_dbl_coord ray_dir);
static void				raycasting_calc(t_casting_ray *casting_ray);
static t_casting_ray	calc_casting_info(double pos_x, double pos_y, t_dbl_coord ray_dir);
static void				calc_step_and_side(t_casting_ray *casting_ray, double pos_x, double pos_y, t_dbl_coord ray_dir);

void	cast_rays(t_game *game)
{
	t_ray		ray;
	t_player	pl;
	double		pl_view_ang; // em radianos
	int			i;

	pl_view_ang = (double)game->player.dir_ang * (M_PI / 180.0);
	pl.x = game->player.x;
	pl.y = game->player.y;
	i = 0;
	while (i < SCR_W)
	{
		ray.angle = pl_view_ang - HALF_FOV + (i * FOV / SCR_W);
		ray.dir_coord.x = cos(ray.angle);
		ray.dir_coord.y = sin(ray.angle);
		ray.perp_dist = cast_one_ray(&game->map, pl.x, pl.y, ray.dir_coord);
		game->rays[i] = ray;
		i++;
	}
}

static double	cast_one_ray(t_map *map, double pos_x, double pos_y, t_dbl_coord ray_dir)
{
	t_casting_ray	casting_ray;
	double			perp_dist;

	casting_ray = calc_casting_info(pos_x, pos_y, ray_dir);
	casting_ray.side = 0;
	while (1)
	{
		raycasting_calc(&casting_ray);
		if (casting_ray.map_pos.y < 0 || casting_ray.map_pos.y >= map->rows
			|| casting_ray.map_pos.x < 0 || casting_ray.map_pos.x >= map->cols
			|| map->grid[casting_ray.map_pos.y][casting_ray.map_pos.x] == '1')
			break;
	}
	if (casting_ray.side == 0)
		perp_dist = casting_ray.side_dist.x - casting_ray.delta_dist.x;
	else
		perp_dist = casting_ray.side_dist.y - casting_ray.delta_dist.y;
	return (perp_dist);
}

static void	raycasting_calc(t_casting_ray *casting_ray)
{
	if (casting_ray->side_dist.x < casting_ray->side_dist.y)
	{
		casting_ray->side_dist.x += casting_ray->delta_dist.x;
		casting_ray->map_pos.x += casting_ray->step_dir.x;
		casting_ray->side = 0;
	}
	else
	{
		casting_ray->side_dist.y += casting_ray->delta_dist.y;
		casting_ray->map_pos.y += casting_ray->step_dir.y;
		casting_ray->side = 1;
	}
}

static t_casting_ray	calc_casting_info(double pos_x, double pos_y, t_dbl_coord ray_dir)
{
	t_casting_ray	casting_ray;

	casting_ray.map_pos.x = (int)pos_x;
	casting_ray.map_pos.y = (int)pos_y;
	if (ray_dir.x == 0)
		casting_ray.delta_dist.x = 1e30;
	else
		casting_ray.delta_dist.x = fabs(1.0 / ray_dir.x);
	if (ray_dir.y == 0)
		casting_ray.delta_dist.y = 1e30;
	else
		casting_ray.delta_dist.y = fabs(1.0 / ray_dir.y);
	calc_step_and_side(&casting_ray, pos_x, pos_y, ray_dir);
	return (casting_ray);
}

static void	calc_step_and_side(t_casting_ray *casting_ray, double pos_x, double pos_y, t_dbl_coord ray_dir)
{
	if (ray_dir.x < 0)
	{
		casting_ray->step_dir.x = -1;
		casting_ray->side_dist.x = (pos_x - casting_ray->map_pos.x) * casting_ray->delta_dist.x;
	}
	else
	{
		casting_ray->step_dir.x = 1;
		casting_ray->side_dist.x = (casting_ray->map_pos.x + 1.0 - pos_x) * casting_ray->delta_dist.x;
	}
	if (ray_dir.y < 0)
	{
		casting_ray->step_dir.y = -1;
		casting_ray->side_dist.y = (pos_y - casting_ray->map_pos.y) * casting_ray->delta_dist.y;
	}
	else
	{
		casting_ray->step_dir.y = 1;
		casting_ray->side_dist.y = (casting_ray->map_pos.y + 1.0 - pos_y) * casting_ray->delta_dist.y;
	}
}
