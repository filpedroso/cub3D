/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_tex_view.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 12:30:00 by fpedroso          #+#    #+#             */
/*   Updated: 2026/08/01 12:30:00 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void		fill_background(t_game *game);
static void		draw_wall_tex(t_game *game, t_column *col);
static int32_t	tex_column(t_game *game, t_column *col);
static uint32_t	texel(mlx_texture_t *tex, int32_t x, int32_t y);

/*
** The default view: the four NO/SO/WE/EA textures on the walls, the
** parsed F and C colours above and below.
**
** Ceiling and floor are flat here, so unlike the debug view they do not
** depend on the column and are painted in one row-major sweep before
** the walls go on top. Writing a column into a row-major image steps
** SCR_W * 4 bytes between neighbouring pixels, a cache miss each; doing
** the background in rows costs one extra write only where a wall later
** covers it, and buys back every other pixel.
*/
void	draw_view_tex(t_game *game)
{
	t_column	col;
	int			i;

	fill_background(game);
	i = 0;
	while (i < RAY_COUNT)
	{
		col = prep_column(game, i);
		draw_wall_tex(game, &col);
		i++;
	}
}

static void	fill_background(t_game *game)
{
	uint32_t	x;
	uint32_t	y;
	uint32_t	color;

	y = 0;
	while (y < SCR_H)
	{
		color = game->floor_rgba;
		if (y < (uint32_t)HORIZON)
			color = game->ceil_rgba;
		x = 0;
		while (x < SCR_W)
		{
			mlx_put_pixel(game->main_img, x, y, color);
			x++;
		}
		y++;
	}
}

/*
** Walks one texture column down the wall slice. step is how much
** texture height one screen row consumes; tex_pos starts from the
** unclamped top of the wall, so a wall taller than the window opens
** partway into the texture rather than squeezing a whole copy into
** view. tex_y is clamped rather than masked: both texture sets happen
** to be powers of two, but nothing enforces that.
*/
static void	draw_wall_tex(t_game *game, t_column *col)
{
	mlx_texture_t	*tex;
	double			tex_pos;
	double			step;
	int32_t			tex_x;
	int32_t			y;

	tex = game->tex[col->face];
	tex_x = tex_column(game, col);
	step = (double)tex->height / col->line_h;
	tex_pos = (col->start - HORIZON + col->line_h / 2.0) * step;
	y = col->start;
	while (y <= col->end)
	{
		mlx_put_pixel(game->main_img, col->x, y, texel(tex, tex_x,
				(int32_t)fmin(fmax(tex_pos, 0.0), (double)tex->height - 1)));
		tex_pos += step;
		y++;
	}
}

/*
** One texel as the 0xRRGGBBAA word mlx_put_pixel expects. lodepng
** normalises every PNG to RGBA8 on load, so this needs no knowledge of
** the file's original colour type.
*/
static uint32_t	texel(mlx_texture_t *tex, int32_t x, int32_t y)
{
	const uint8_t	*p;

	p = &tex->pixels[((size_t)y * tex->width + x) * 4];
	return (((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16)
		| ((uint32_t)p[2] << 8) | p[3]);
}

/*
** Where along the wall's width the ray landed, as a texture column.
**
** The hit point needs the true euclidean hit_dist, not the
** fisheye-corrected perp_dist, because dir is a unit vector; this is
** the same reconstruction prep_drawing_ray does for the minimap.
**
** Two of the four faces have to be mirrored, or their texture comes out
** back to front. Sweeping the screen left to right always increases the
** ray's angular offset, but whether that makes wall_x grow or shrink
** depends on where the camera is pointing: d(dir.x)/d(offset) is -sin,
** d(dir.y)/d(offset) is cos of the view angle. Working that through the
** one heading each face can be seen from gives +1 for south and west
** but -1 for north and east, so those last two are the ones that need
** reversing to keep the texture reading left to right.
**
** Note this is the opposite pair to the condition in the usual lodev
** tutorial; that formulation assumes its own camera-plane convention,
** and ours builds the fan by rotating angular offsets instead.
*/
static int32_t	tex_column(t_game *game, t_column *col)
{
	t_ray	*ray;
	double	wall_x;
	int32_t	tex_x;

	ray = &game->rays[col->x];
	if (ray->side == 0)
		wall_x = game->player.y + ray->hit_dist * ray->dir.y;
	else
		wall_x = game->player.x + ray->hit_dist * ray->dir.x;
	wall_x -= floor(wall_x);
	tex_x = (int32_t)(wall_x * game->tex[col->face]->width);
	tex_x = (int32_t)fmin(fmax(tex_x, 0),
			(double)game->tex[col->face]->width - 1);
	if (col->face == F_NORTH || col->face == F_EAST)
		tex_x = (int32_t)game->tex[col->face]->width - 1 - tex_x;
	return (tex_x);
}
