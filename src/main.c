/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 14:25:43 by fpedroso          #+#    #+#             */
/*   Updated: 2026/06/01 14:36:44 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	run_minimap_mode(t_game *game, int gridmap[MAP_W][MAP_H]);
void	config_mlx(t_game *game);
void	ft_hook(void* param);
void	draw_square(void* param);
void	draw_minimap(t_game *game, int gridmap[MAP_W][MAP_H]);
int32_t	ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a);
bool	is_in_cube(int gridmap[MAP_W][MAP_H], int i, int j);
bool	is_in_grid(int i, int j);
void	draw_circle(void* param);
bool	belongs_in_circle(uint32_t y, uint32_t x);

static int	gridmap[MAP_H][MAP_W] =
{
	{1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,2,0,1,0,0,1,1,0,1},
	{1,0,0,0,0,1,1,1,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,1,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,1,0,1},
	{1,0,0,0,0,0,1,1,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1}
};

int	main(void)
{
	t_game	game;

	ft_memset(&game, 0, sizeof(game));
	run_minimap_mode(&game, gridmap);
}

void	run_minimap_mode(t_game *game, int gridmap[MAP_W][MAP_H])
{
	config_mlx(game);
	draw_minimap(game, gridmap);
	mlx_loop_hook(game->mlx, draw_circle, game);
	mlx_loop_hook(game->mlx, ft_hook, game);
	mlx_loop(game->mlx);
	mlx_terminate(game->mlx);
}

mlx_image_t	*create_minimap_image(t_game *game)
{
	mlx_image_t	*image;

	image = mlx_new_image(game->mlx, MAP_PX_W, MAP_PX_H);
	if (!image)
	{
		mlx_close_window(game->mlx);
		puts(mlx_strerror(mlx_errno));
		return (NULL);
	}
	if (mlx_image_to_window(game->mlx, image, 0, 0) == -1)
	{
		mlx_close_window(game->mlx);
		puts(mlx_strerror(mlx_errno));
		return (NULL);
	}
	return (image);
}

void	draw_minimap(t_game *game, int gridmap[MAP_W][MAP_H])
{
	mlx_image_t	*image;
	uint32_t	i;
	uint32_t	j;

	image = create_minimap_image(game);
	if (!image)
		return ;
	i = 0;
	while(i < image->height)
	{
		j = 0;
		while (j < image->width)
		{
			if (is_in_cube(gridmap, i, j))
				mlx_put_pixel(image, i, j, BLUE);
			if (is_in_grid(i, j))
				mlx_put_pixel(image, i, j, BLACK);
			j++;
		}
		i++;
	}
}

bool	is_in_grid(int i, int j)
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

bool	is_in_cube(int gridmap[MAP_W][MAP_H], int i, int j)
{
	uint32_t	inset_x;
	uint32_t	inset_y;

	inset_x = i / SQUARE_SZ;
	inset_y = j / SQUARE_SZ;

	if (inset_x >= MAP_W || inset_y >= MAP_H)
		return (false);
	return (gridmap[inset_y][inset_x] == 1);
}

t_point	get_starting_pos()
{
	t_point		ret;
	uint32_t	i;
	uint32_t	j;

	i = 0;
	while (i < MAP_H)
	{
		j = 0;
		while (j < MAP_W)
		{
			if (gridmap[i][j] == 2)
			{
				ret.y = i * SQUARE_SZ;
				ret.x = j * SQUARE_SZ;
				return (ret);
			}
			j++;
		}
		i++;
	}
	return ((t_point){0, 0});
}

void draw_circle(void* param)
{
	t_game		*game;
	uint32_t	color;
	t_point		offset;
	uint32_t	y;
	uint32_t	x;
	
	game = (t_game *)param;
	offset = get_starting_pos();
	y = 0;
	while (y < CIRCLE_R * 2)
	{
		x = 0;
		while (x < CIRCLE_R * 2)
		{
			if (belongs_in_circle(y, x))
			{
				color = ft_pixel(rand() % 0xFF, 0xFF, 0xFF, 0xA0);
				mlx_put_pixel(game->image, x + offset.x , y + offset.y, color);
			}
			x++;
		}
		y++;
	}
}



bool	belongs_in_circle(uint32_t y, uint32_t x)
{
	uint32_t	c_x;
	uint32_t	c_y;

	c_x = x - CIRCLE_R;
	c_y = y - CIRCLE_R;
	return ((c_x) * (c_x) + (c_y) * (c_y) <= CIRCLE_R * CIRCLE_R);
}

void draw_square(void* param)
{
	t_game		*game = (t_game *)param;
	uint32_t	color;

	for (uint32_t i = 0; i < SQUARE_SZ; ++i)
	{
		for (uint32_t y = 0; y < SQUARE_SZ; ++y)
		{
			color = ft_pixel(
				rand() % 0xFF,			// R
				0xFF,					// G
				0xFF,					// B
				0xA0 			// A
			);
			mlx_put_pixel(game->image, i, y, color);
		}
	}
}

void	config_mlx(t_game *game)
{
	game->mlx = mlx_init(SCR_W, SCR_H, "minimap", true);
	if (!game->mlx)
	{
		puts(mlx_strerror(mlx_errno));
		return;
	}
	game->image = mlx_new_image(game->mlx, SCR_W, SCR_H);
	if (!game->image)
	{
		mlx_close_window(game->mlx);
		puts(mlx_strerror(mlx_errno));
		return;
	}
	if (mlx_image_to_window(game->mlx, game->image, 0, 0) == -1)
	{
		mlx_close_window(game->mlx);
		puts(mlx_strerror(mlx_errno));
		return;
	}
}

void ft_hook(void* param)
{
	t_game	*game = (t_game *)param;

	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
	if (mlx_is_key_down(game->mlx, MLX_KEY_UP))
		game->image->instances[0].y -= 5;
	if (mlx_is_key_down(game->mlx, MLX_KEY_DOWN))
		game->image->instances[0].y += 5;
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		game->image->instances[0].x -= 5;
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		game->image->instances[0].x += 5;
}

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}
