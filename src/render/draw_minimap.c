/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_minimap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 14:00:33 by fpedroso          #+#    #+#             */
/*   Updated: 2026/07/25 18:56:59 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	draw_minimap_pixel(t_game *game, uint32_t px_x, uint32_t px_y,
	double scale);
static bool	is_in_cube(t_game *game, int px_x, int px_y, double scale);
static bool	is_in_grid(uint32_t width, uint32_t height, int px_x, int px_y);

double	minimap_scale(t_game *game)
{
	double	scale_x;
	double	scale_y;

	scale_x = (double)game->map_img->width / (double)game->map.cols;
	scale_y = (double)game->map_img->height / (double)game->map.rows;
	if (scale_x < scale_y)
		return (scale_x);
	return (scale_y);
}

void	draw_minimap(t_game *game)
{
	mlx_image_t	*image;
	uint32_t	px_y;
	uint32_t	px_x;
	double		scale;

	image = game->map_img;
	scale = minimap_scale(game);
	ft_memset(image->pixels, 0, image->width * image->height * sizeof(uint32_t));
	px_y = 0;
	while (px_y < image->height)
	{
		px_x = 0;
		while (px_x < image->width)
		{
			draw_minimap_pixel(game, px_x, px_y, scale);
			px_x++;
		}
		px_y++;
	}
	game->map_img = image;
}

static void	draw_minimap_pixel(t_game *game, uint32_t px_x, uint32_t px_y,
	double scale)
{
	if (is_in_cube(game, px_x, px_y, scale))
		mlx_put_pixel(game->map_img, px_x, px_y, BLUE);
	if (is_in_grid(game->map_img->width, game->map_img->height, px_x, px_y))
		mlx_put_pixel(game->map_img, px_x, px_y, BLACK);
}

static bool	is_in_cube(t_game *game, int px_x, int px_y, double scale)
{
	uint32_t	col;
	uint32_t	row;

	col = (uint32_t)(px_x / scale);
	row = (uint32_t)(px_y / scale);
	if (col >= (uint32_t)game->map.cols || row >= (uint32_t)game->map.rows)
		return (false);
	if (col >= ft_strlen(game->map.grid[row]))
		return (false);
	return (game->map.grid[row][col] == '1');
}

static bool	is_in_grid(uint32_t width, uint32_t height, int px_x, int px_y)
{
	uint32_t	col_off;
	uint32_t	row_off;

	if (px_y == (int)height - 1 || px_x == (int)width - 1)
		return (true);
	if (px_y > (int)height || px_x > (int)width)
		return (false);
	col_off = px_x % (int)SQUARE_SZ;
	row_off = px_y % (int)SQUARE_SZ;
	return (col_off == 0 || row_off == 0);
}
