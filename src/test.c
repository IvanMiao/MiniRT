/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 03:53:08 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/20 01:56:41 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	test_sphere(t_minirt *rt)
{
	int				x;
	int				y;
	t_ray			ray;
	t_hit_record	hit;
	t_color			final_color;
	int				pixel_color;

	setup_scene(rt);
	setup_cam_coords(&rt->camera);

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			ray = gen_cam_ray(&rt->camera, x, y);
			hit = trace_ray(rt->object, &ray);
			if (hit.hit)
			{
				final_color = calculate_combined_light(rt, hit.point, hit.normal, hit.obj_ptr->color);
				pixel_color = color_to_int(final_color);
				put_pixel(&rt->img, x, y, pixel_color);
			}
			else
				put_pixel(&rt->img, x, y, 0x000000);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(rt->mlx, rt->mlx_win, rt->img.img, 0, 0);
	mlx_loop(rt->mlx);
}
