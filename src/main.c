/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 14:25:43 by fpedroso          #+#    #+#             */
/*   Updated: 2026/06/01 13:50:30 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	main(void)
{
	t_game	*game;
	int	gridmap[WIDTH][HEIGHT] =
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
	};

	run_minimap_mode(game);
}

void	run_minimap_mode(t_game *game)
{
	config_mlx(game);
	mlx_loop_hook(mlx, ft_randomize, mlx);
	mlx_loop_hook(mlx, ft_hook, mlx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
}

void	config_mlx(t_game *game)
{
	game->mlx = mlx_init(SCR_W, SCR_H, "minimap", true);
	if (!game->mlx)
	{
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	game->image = mlx_new_image(game->mlx, 128, 128);
	if (!game->image)
	{
		mlx_close_window(game->mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (mlx_image_to_window(game->mlx, image, 0, 0) == -1)
	{
		mlx_close_window(game->mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
}
