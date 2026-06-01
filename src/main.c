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

void	run_minimap_mode(t_game *game);
void	config_mlx(t_game *game);
void	ft_hook(void* param);
void	ft_randomize(void* param);
int32_t	ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a);

int	main(void)
{
	t_game	game;
	memset(&game, 0, sizeof(game));
/* 	int	gridmap[WIDTH][HEIGHT] =
	{
		{1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1}
	}; */

	run_minimap_mode(&game);
}

void	run_minimap_mode(t_game *game)
{
	config_mlx(game);
	mlx_loop_hook(game->mlx, ft_randomize, game);
	mlx_loop_hook(game->mlx, ft_hook, game);
	mlx_loop(game->mlx);
	mlx_terminate(game->mlx);
}

void	config_mlx(t_game *game)
{
	game->mlx = mlx_init(SCR_W, SCR_H, "minimap", true);
	if (!game->mlx)
	{
		puts(mlx_strerror(mlx_errno));
		return;
	}
	game->image = mlx_new_image(game->mlx, 128, 128);
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

void ft_randomize(void* param)
{
	t_game	*game = (t_game *)param;

	for (uint32_t i = 0; i < game->image->width; ++i)
	{
		for (uint32_t y = 0; y < game->image->height; ++y)
		{
			uint32_t color = ft_pixel(
				rand() % 0xFF, // R
				rand() % 0xFF, // G
				rand() % 0xFF, // B
				rand() % 0xFF  // A
			);
			mlx_put_pixel(game->image, i, y, color);
		}
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
