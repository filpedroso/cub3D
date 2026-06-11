/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 13:55:36 by fpedroso          #+#    #+#             */
/*   Updated: 2026/05/30 13:55:36 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d.h"

void	config_mlx(t_game *game);
void	ft_hook(void* param);
void	draw_square(void* param);
void	draw_minimap(t_game *game, int gridmap[MAP_W][MAP_H]);
int32_t	ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a);
bool	is_in_cube(int gridmap[MAP_W][MAP_H], int i, int j);
bool	is_in_grid(int i, int j);
void	draw_player(void* param);
bool	belongs_in_circle(uint32_t y, uint32_t x);






void	run_minimap_mode(t_game *game)
{
	if (!config_mlx(game))
	{
		ft_error();
		return;
	}
	draw_minimap(game);
	mlx_loop_hook(game->mlx, draw_player, game);
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
	game->map_img = image;
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

void draw_player(void* param)
{
	t_game	*game;
	t_point	starting_offset;

	game = (t_game *)param;
	update_position(game);
	starting_offset = get_starting_pos();
	redraw(game, starting_offset);
}

void	redraw(t_game *game, t_point offset)
{
	uint32_t	color;
	uint32_t	y;
	uint32_t	x;

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

void update_position(t_game *game)
{
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
	if (mlx_is_key_down(game->mlx, MLX_KEY_UP))
		game->player_img->instances[0].y -= 5;
	if (mlx_is_key_down(game->mlx, MLX_KEY_DOWN))
		game->player_img->instances[0].y += 5;
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		game->player_img->instances[0].x -= 5;
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		game->player_img->instances[0].x += 5;
	if (is_collision(game))
	{
		game->player_img->instances[0].x -= 5;
		game->player_img->instances[0].y -= 5;
	}
}

bool is_collision(mlx_image_t* img1, int inst1_id, mlx_image_t* img2, int inst2_id)
{
    // Image 1 bounds
    int x1 = img1->instances[inst1_id].x;
    int y1 = img1->instances[inst1_id].y;
    int w1 = img1->width;
    int h1 = img1->height;

    // Image 2 bounds
    int x2 = img2->instances[inst2_id].x;
    int y2 = img2->instances[inst2_id].y;
    int w2 = img2->width;
    int h2 = img2->height;

    // AABB Collision logic
    if (x1 < x2 + w2 &&   // img1 left edge is left of img2 right edge
        x1 + w1 > x2 &&   // img1 right edge is right of img2 left edge
        y1 < y2 + h2 &&   // img1 top edge is above img2 bottom edge
        y1 + h1 > y2)     // img1 bottom edge is below img2 top edge
    {
        return true; // Images are colliding
    }
    
    return false; // No collision
}
int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}
