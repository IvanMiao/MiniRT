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

#define SAMPLES_PER_PIXEL 16

static void	put_pixel(t_img *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

// static t_color	sample_maas(t_minirt *rt, int x, int y)
// {
// 	int				s;
// 	t_ray			ray;
// 	t_hit_record	hit;
// 	t_color			pixel_color;

// 	s = 0;
// 	pixel_color = color_init_d(0, 0, 0);
// 	while (s < SAMPELS_PER_PIXEL)
// 	{
// 		ray = gen_cam_ray(&rt->camera, x, y);
// 		hit = trace_ray(rt->object, &ray);
// 		if (hit.hit)
// 			pixel_color = color_add(pixel_color, combine_light
// 					(rt, hit.point, hit.normal, hit.obj_ptr->color));
// 		s++;
// 	}
// 	pixel_color = color_div(pixel_color, SAMPELS_PER_PIXEL);
// 	return (pixel_color);
// }


// static void	main_loop(t_minirt *rt)
// {
// 	int		x;
// 	int		y;
// 	t_color	pixel_color;

// 	y = 0;
// 	while (y < HEIGHT)
// 	{
// 		x = 0;
// 		while (x < WIDTH)
// 		{
// 			pixel_color = sample_maas(rt, x, y);
// 			put_pixel(&rt->img, x, y, color_to_int(pixel_color));
// 			x++;
// 		}
// 		y++;
// 	}
// }

#define MAX_DEPTH 20

// 反射向量计算函数
static t_vector reflect_vector(t_vector I, t_vector N) {
    double dot = vector_dot(I, N);
    return vector_sub(I, vector_mult(N, 2.0 * dot));
}


static void	check_final_color(t_color *final_color)
{
	if (final_color->r > 1.0)
		final_color->r = 1.0;
	if (final_color->g > 1.0)
		final_color->g = 1.0;
	if (final_color->b > 1.0)
		final_color->b = 1.0;
}

/**
 * @brief 递归光线追踪计算颜色
 * 
 * @param rt      miniRT 主结构体，包含场景信息
 * @param ray     当前射线
 * @param depth   递归深度
 * @return t_color 最终计算的颜色
 */
static t_color ray_trace(t_minirt *rt, t_ray ray, int depth)
{
    t_hit_record hit;
    t_color direct_color;
    t_color reflect_color;
    t_color final_color;
    t_ray reflect_ray;

    if (depth > MAX_DEPTH)
        return (color_init_d(0, 0, 0)); // 超过最大递归深度返回黑色

    // 寻找射线与场景最近交点
    hit = trace_ray(rt->object, &ray);
    if (!hit.hit)
        return (color_init_d(0, 0, 0)); // 没有击中返回背景色

    // 计算当前交点的光照颜色（环境+漫反射+镜面反射）
    direct_color = combine_light(rt, hit.point, hit.normal, hit.obj_color);

    // 判断材质是否有反射属性，这里假设物体结构有reflectivity字段，或者你可以自定义
    // 如果没有，先写成0
    double reflectivity = 0.2;
    // if (hit.obj_ptr->material)
    //     reflectivity = hit.obj_ptr->material->reflectivity;

    if (reflectivity > 0.0)
    {
        // 计算反射光线起点（交点沿法线稍微偏移，防止自相交）
        reflect_ray.origin = vector_add(hit.point, vector_mult(hit.normal, EPSILON));

        // 计算反射光线方向
        reflect_ray.direction = reflect_vector(ray.direction, hit.normal);
        reflect_ray.direction = vector_normalize(reflect_ray.direction);

        // 递归计算反射颜色
        reflect_color = ray_trace(rt, reflect_ray, depth + 1);

        // 按反射率加权混合颜色
        final_color.r = direct_color.r * (1.0 - reflectivity) + reflect_color.r * reflectivity;
        final_color.g = direct_color.g * (1.0 - reflectivity) + reflect_color.g * reflectivity;
        final_color.b = direct_color.b * (1.0 - reflectivity) + reflect_color.b * reflectivity;

        // 限制颜色范围
        check_final_color(&final_color);
        return final_color;
    }
    else
    {
        // 无反射直接返回当前光照颜色
        return direct_color;
    }
}

static t_color sample_maas(t_minirt *rt, int x, int y)
{
	int s;
	t_ray ray;
	t_color pixel_color = color_init_d(0, 0, 0);

	for (s = 0; s < SAMPLES_PER_PIXEL; s++)
	{
		// 这里可以加入随机抖动实现抗锯齿
		double jitter_x = (double)rand() / RAND_MAX;
		double jitter_y = (double)rand() / RAND_MAX;

		ray = gen_cam_ray(&rt->camera, x + jitter_x, y + jitter_y);

		pixel_color = color_add(pixel_color, ray_trace(rt, ray, 0));
	}
	pixel_color = color_div(pixel_color, SAMPLES_PER_PIXEL);
	check_final_color(&pixel_color);
	return pixel_color;
}

// static void main_loop(t_minirt *rt)
// {
// 	int		x;
// 	int		y;
// 	t_color pixel_color;
// 	t_ray	ray;

// 	y = 0;
// 	while (y < HEIGHT)
// 	{
// 		x = 0;
// 		while (x < WIDTH)
// 		{
// 			// 根据像素(x,y)生成主射线
// 			ray = gen_cam_ray(&rt->camera, x, y);

// 			// 调用递归光线追踪，深度从0开始
// 			pixel_color = ray_trace(rt, ray, 0);

// 			// 写入像素
// 			put_pixel(&rt->img, x, y, color_to_int(pixel_color));

// 			x++;
// 		}
// 		y++;
// 	}
// }

static void main_loop(t_minirt *rt)
{
	int x, y;
	t_color pixel_color;

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