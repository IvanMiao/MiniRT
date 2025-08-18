/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 03:53:08 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/18 20:19:15 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	test_sphere(t_minirt *rt)
{
	int			x;
	int			y;
	t_ray		ray;
	double		t;
	t_sphere	sphere;
	t_vector	hit_point;
	t_vector	normal;
	t_color		final_color;
	int			pixel_color;

	setup_scene(rt, &sphere);
	setup_cam_coords(&rt->camera);

	// int	background_color = color_to_int(color_scale(rt->ambient.color, rt->ambient.ratio));
	
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			ray = gen_cam_ray(&rt->camera, x, y);
			
			t = hit_sphere(&sphere, &ray);
			if (t > 0.0)
			{
				hit_point = vector_add(ray.origin, vector_mult(ray.direction, t));
				normal = sphere_normal_at(&sphere, hit_point);
				
				final_color = calculate_combined_light(rt, hit_point, normal, sphere.color);
				
				pixel_color = color_to_int(final_color);
				put_pixel(&rt->img, x, y, pixel_color);
			}
			else
				put_pixel(&rt->img, x, y, 0x000000); // black or this?
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(rt->mlx, rt->mlx_win, rt->img.img, 0, 0);
	mlx_loop(rt->mlx);
}
