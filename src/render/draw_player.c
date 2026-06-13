/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 15:49:44 by fpedroso          #+#    #+#             */
/*   Updated: 2026/06/13 15:49:44 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void		update_position(t_game *game);
static void		redraw(t_game *game);
static bool		belongs_in_circle(uint32_t y, uint32_t x);
// static bool		is_collision(mlx_image_t* img1, int inst1_id, mlx_image_t* img2, int inst2_id);

void	draw_player(void* param)
{
	t_game	*game;

	game = (t_game *)param;
	update_position(game);
	redraw(game);
}

static void	update_position(t_game *game)
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
	/* if (is_collision(game))
	{
		game->player_img->instances[0].x -= 5;
		game->player_img->instances[0].y -= 5;
	} */
}

static void	redraw(t_game *game)
{
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
				mlx_put_pixel(game->player_img, x, y, RED);
			}
			x++;
		}
		y++;
	}
}

static bool	belongs_in_circle(uint32_t y, uint32_t x)
{
	uint32_t	c_x;
	uint32_t	c_y;

	c_x = x - CIRCLE_R;
	c_y = y - CIRCLE_R;
	return ((c_x) * (c_x) + (c_y) * (c_y) <= CIRCLE_R * CIRCLE_R);
}





/* static bool is_collision(mlx_image_t* img1, int inst1_id, mlx_image_t* img2, int inst2_id)
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
} */
