/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 20:14:02 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/06 19:23:00 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

#define SAMPELS_PER_PIXEL 4

static void	put_pixel(t_img *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

static void	main_loop(t_minirt *rt)
{
	int				x;
	int				y;
	int				sample;
	t_color			pixel_color;
	t_color			sample_color;
	t_ray			ray;
	t_hit_record	hit;
	double			offset_x;
	double			offset_y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			pixel_color = color_init_d(0.0, 0.0, 0.0);
			
			// Multi-sampling for anti-aliasing
			sample = 0;
			while (sample < g_msaa_samples)
			{
				offset_x = random_double_range(-0.5, 0.5);
				offset_y = random_double_range(-0.5, 0.5);
				ray = gen_cam_ray_msaa(&rt->camera, x, y, offset_x, offset_y);
				hit = trace_ray(rt->object, &ray);
				
				if (hit.hit)
					sample_color = combine_light(rt, hit.point, hit.normal, hit.obj_ptr->color);
				else
					sample_color = color_init_d(0.0, 0.0, 0.0);
				
				pixel_color = color_add(pixel_color, sample_color);
				sample++;
			}
			
			// Average the samples
			pixel_color = color_div(pixel_color, g_msaa_samples);
			put_pixel(&rt->img, x, y, color_to_int(pixel_color));
			x++;
		}
		y++;
	}
}

void	render(t_minirt *rt)
{
	setup_cam_coords(&rt->camera);
	
	// Try OpenCL rendering first
	if (rt->opencl_ctx.initialized) {
		printf("Using OpenCL rendering with MSAA (%d samples per pixel)\n", g_msaa_samples);
		if (render_opencl(rt, &rt->opencl_ctx)) {
			mlx_put_image_to_window(rt->mlx, rt->mlx_win, rt->img.img, 0, 0);
			mlx_loop(rt->mlx);
			return;
		}
		printf("OpenCL rendering failed, falling back to CPU rendering\n");
	}
	
	// Fallback to CPU rendering
	printf("Using CPU rendering with MSAA (%d samples per pixel)\n", g_msaa_samples);
	main_loop(rt);
	mlx_put_image_to_window(rt->mlx, rt->mlx_win, rt->img.img, 0, 0);
	mlx_loop(rt->mlx);
}
