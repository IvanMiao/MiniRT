/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 23:12:07 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/06 19:18:04 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

/**
 * @brief Generate a ray for the pixel(x, y)
 * 
 * @param camera
 * @param x 0 < x < WIDTH-1
 * @param y 0 < y < HEIGHT-1
 * @return The ray
 * 1) screen_y * -1, because axe Y is reversed.
 * 2) x + 0.5f, y + 0.5f, because we want the ray to
 * 		cross the center of the pixel
 */
t_ray	gen_cam_ray(t_camera *camera, int x, int y)
{
	t_ray		ray;
	float		rand_nb;
	float		screen_x;
	float		screen_y;
	t_vector	screen_point;

	rand_nb = (float)rand() / ((float)RAND_MAX + 1.0f);
	screen_x = (2.0f * (x + rand_nb) / WIDTH - 1.0f) * camera->half_width;
	rand_nb = (float)rand() / ((float)RAND_MAX + 1.0f);
	screen_y = -(2.0f * (y + rand_nb) / HEIGHT - 1.0f) * camera->half_height;
	screen_point = camera->viewpoint;
	screen_point = vector_add(screen_point, vector_mult(camera->u, screen_x));
	screen_point = vector_add(screen_point, vector_mult(camera->v, screen_y));
	screen_point = vector_add(screen_point, vector_mult(camera->w, -1.0f));
	ray.origin = camera->viewpoint;
	ray.direction = vector_sub(screen_point, camera->viewpoint);
	ray.direction = vector_normalize(ray.direction);
	return (ray);
}

/**
 * @brief Generate a ray for the pixel(x, y) with random offset for MSAA
 * 
 * @param camera
 * @param x 0 < x < WIDTH-1
 * @param y 0 < y < HEIGHT-1
 * @param offset_x Random offset in x direction [-0.5, 0.5]
 * @param offset_y Random offset in y direction [-0.5, 0.5]
 * @return The ray
 */
t_ray	gen_cam_ray_msaa(t_camera *camera, int x, int y, double offset_x, double offset_y)
{
	t_ray		ray;
	double		screen_x;
	double		screen_y;
	t_vector	screen_point;

	screen_x = (2.0 * (x + 0.5 + offset_x) / WIDTH - 1.0) * camera->half_width;
	screen_y = -(2.0 * (y + 0.5 + offset_y) / HEIGHT - 1.0) * camera->half_height;
	screen_point = camera->viewpoint;
	screen_point = vector_add(screen_point, vector_mult(camera->u, screen_x));
	screen_point = vector_add(screen_point, vector_mult(camera->v, screen_y));
	screen_point = vector_add(screen_point, vector_mult(camera->w, -1.0));
	ray.origin = camera->viewpoint;
	ray.direction = vector_sub(screen_point, camera->viewpoint);
	ray.direction = vector_normalize(ray.direction);
	return (ray);
}
