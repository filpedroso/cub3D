/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_minimap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 14:00:33 by fpedroso          #+#    #+#             */
/*   Updated: 2026/06/13 14:00:33 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	draw_minimap_pixel(mlx_image_t *image, uint32_t i, uint32_t j);
static bool	is_in_cube(int i, int j);
static bool	is_in_grid(int i, int j);

void	draw_minimap(t_game *game)
{
	mlx_image_t	*image;
	uint32_t	i;
	uint32_t	j;

	image = game->map_img;
	i = 0;
	while(i < image->height)
	{
		j = 0;
		while (j < image->width)
		{
			draw_minimap_pixel(image, i, j);
			j++;
		}
		i++;
	}
	game->map_img = image;
}

static void	draw_minimap_pixel(mlx_image_t *image, uint32_t i, uint32_t j)
{
	if (is_in_cube(i, j))
		mlx_put_pixel(image, i, j, BLUE);
	if (is_in_grid(i, j))
		mlx_put_pixel(image, i, j, BLACK);
}

static bool	is_in_cube(int i, int j)
{
	uint32_t	inset_x;
	uint32_t	inset_y;

	inset_x = i / SQUARE_SZ;
	inset_y = j / SQUARE_SZ;

	if (inset_x >= MAP_W || inset_y >= MAP_H)
		return (false);
	return (gridmap[inset_y][inset_x] == 1);
}

static bool	is_in_grid(int i, int j)
{
	uint32_t	inset_x;
	uint32_t	inset_y;

	if (i == MAP_PX_H - 1 || j == MAP_PX_W - 1)
		return (true);
	if (i > MAP_PX_H || j > MAP_PX_W)
		return (false);
	inset_x = i % SQUARE_SZ;
	inset_y = j % SQUARE_SZ;
	return (inset_x == 0 || inset_y == 0);
}
