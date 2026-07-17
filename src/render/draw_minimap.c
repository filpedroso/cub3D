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

static void	draw_minimap_pixel(t_game *game, uint32_t px_x, uint32_t px_y);
static bool	is_in_cube(t_game *game, int px_x, int px_y);
static bool	is_in_grid(uint32_t width, uint32_t height, int px_x, int px_y);

void	draw_minimap(t_game *game)
{
	mlx_image_t	*image;
	uint32_t	px_y;
	uint32_t	px_x;

	image = game->map_img;
	ft_memset(image->pixels, 0, image->width * image->height * sizeof(uint32_t));
	px_y = 0;
	while (px_y < image->height)
	{
		px_x = 0;
		while (px_x < image->width)
		{
			draw_minimap_pixel(game, px_x, px_y);
			px_x++;
		}
		px_y++;
	}
	game->map_img = image;
}

static void	draw_minimap_pixel(t_game *game, uint32_t px_x, uint32_t px_y)
{
	if (is_in_cube(game, px_x, px_y))
		mlx_put_pixel(game->map_img, px_x, px_y, BLUE);
	if (is_in_grid(game->map_img->width, game->map_img->height, px_x, px_y))
		mlx_put_pixel(game->map_img, px_x, px_y, BLACK);
}

static bool	is_in_cube(t_game *game, int px_x, int px_y)
{
	uint32_t	col;
	uint32_t	row;

	col = px_x / SQUARE_SZ;
	row = px_y / SQUARE_SZ;
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
