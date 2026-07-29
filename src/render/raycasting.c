/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 18:31:05 by fpedroso          #+#    #+#             */
/*   Updated: 2026/07/28 12:00:00 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void				cast_one_ray(t_game *game, t_ray *ray);
static void				raycasting_calc(t_casting_ray *dda);
static t_casting_ray	calc_casting_info(t_player *pl, t_dbl_coord ray_dir);
static void				calc_step_and_side(t_casting_ray *dda, t_player *pl,
							t_dbl_coord ray_dir);

/*
** Rotates the precomputed FOV fan onto the current view direction, so
** the whole frame costs two trig calls instead of two per column.
**
** perp_dist is the fisheye correction. cast_one_ray works with a
** normalised direction vector, so what it returns is the true
** euclidean distance to the wall; feeding that straight into wall
** heights would bow the walls outwards. The correction factor is
** cos(ray angle - view angle), which is precisely cos_off.
*/
void	cast_rays(t_game *game)
{
	t_ray	*ray;
	double	view;
	double	cv;
	double	sv;
	int		i;

	view = (double)game->player.dir_ang * (M_PI / 180.0);
	cv = cos(view);
	sv = sin(view);
	i = 0;
	while (i < SCR_W)
	{
		ray = &game->rays[i];
		ray->dir.x = cv * ray->cos_off - sv * ray->sin_off;
		ray->dir.y = sv * ray->cos_off + cv * ray->sin_off;
		cast_one_ray(game, ray);
		ray->perp_dist = ray->hit_dist * ray->cos_off;
		i++;
	}
}

static void	cast_one_ray(t_game *game, t_ray *ray)
{
	t_casting_ray	dda;

	dda = calc_casting_info(&game->player, ray->dir);
	dda.side = 0;
	while (1)
	{
		raycasting_calc(&dda);
		if (dda.map_pos.y < 0 || dda.map_pos.y >= game->map.rows
			|| dda.map_pos.x < 0 || dda.map_pos.x >= game->map.cols
			|| is_solid(game->map.grid[dda.map_pos.y][dda.map_pos.x]))
			break ;
	}
	if (dda.side == 0)
		ray->hit_dist = dda.side_dist.x - dda.delta_dist.x;
	else
		ray->hit_dist = dda.side_dist.y - dda.delta_dist.y;
	ray->side = dda.side;
}

static void	raycasting_calc(t_casting_ray *dda)
{
	if (dda->side_dist.x < dda->side_dist.y)
	{
		dda->side_dist.x += dda->delta_dist.x;
		dda->map_pos.x += dda->step_dir.x;
		dda->side = 0;
	}
	else
	{
		dda->side_dist.y += dda->delta_dist.y;
		dda->map_pos.y += dda->step_dir.y;
		dda->side = 1;
	}
}

static t_casting_ray	calc_casting_info(t_player *pl, t_dbl_coord ray_dir)
{
	t_casting_ray	dda;

	dda.map_pos.x = (int)pl->x;
	dda.map_pos.y = (int)pl->y;
	if (ray_dir.x == 0)
		dda.delta_dist.x = 1e30;
	else
		dda.delta_dist.x = fabs(1.0 / ray_dir.x);
	if (ray_dir.y == 0)
		dda.delta_dist.y = 1e30;
	else
		dda.delta_dist.y = fabs(1.0 / ray_dir.y);
	calc_step_and_side(&dda, pl, ray_dir);
	return (dda);
}

static void	calc_step_and_side(t_casting_ray *dda, t_player *pl,
	t_dbl_coord ray_dir)
{
	if (ray_dir.x < 0)
	{
		dda->step_dir.x = -1;
		dda->side_dist.x = (pl->x - dda->map_pos.x) * dda->delta_dist.x;
	}
	else
	{
		dda->step_dir.x = 1;
		dda->side_dist.x = (dda->map_pos.x + 1.0 - pl->x) * dda->delta_dist.x;
	}
	if (ray_dir.y < 0)
	{
		dda->step_dir.y = -1;
		dda->side_dist.y = (pl->y - dda->map_pos.y) * dda->delta_dist.y;
	}
	else
	{
		dda->step_dir.y = 1;
		dda->side_dist.y = (dda->map_pos.y + 1.0 - pl->y) * dda->delta_dist.y;
	}
}
