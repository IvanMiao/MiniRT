/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 20:14:02 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/01 17:35:49 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

static void	put_pixel(t_img *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

static t_color	sample_maas(t_minirt *rt, int x, int y)
{
	int				s;
	t_ray			ray;
	t_hit_record	hit;
	t_color			pixel_color;

	s = 0;
	pixel_color = color_init_d(0, 0, 0);
	while (s < SAMPELS_PER_PIXEL)
	{
		ray = gen_cam_ray(&rt->camera, x, y);
		hit = trace_ray(rt->object, &ray);
		if (hit.hit)
			pixel_color = color_add(pixel_color, combine_light
					(rt, hit.point, hit.normal, hit.obj_ptr->color));
		s++;
	}
	pixel_color = color_div(pixel_color, SAMPELS_PER_PIXEL);
	return (pixel_color);
}

static void	main_loop(t_minirt *rt)
{
	int		x;
	int		y;
	t_color	pixel_color;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			pixel_color = sample_maas(rt, x, y);
			put_pixel(&rt->img, x, y, color_to_int(pixel_color));
			x++;
		}
		y++;
	}
}

void	render(t_minirt *rt)
{
	setup_cam_coords(&rt->camera);
	main_loop(rt);
	mlx_put_image_to_window(rt->mlx, rt->mlx_win, rt->img.img, 0, 0);
	mlx_loop(rt->mlx);
}
