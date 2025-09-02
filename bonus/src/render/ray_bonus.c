/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 23:12:07 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/02 19:22:48 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt_bonus.h"

/**
 * @brief Generate a ray for the pixel(x, y), using maas
 * 
 * @param camera
 * @param x 0 < x < WIDTH-1
 * @param y 0 < y < HEIGHT-1
 * @return The ray
 * 1) screen_y * -1, because axe Y is reversed.
 * 2) x + 0.5, y + 0.5, because we want the ray to
 * 		cross the center of the pixel
 */
t_ray	gen_cam_ray(t_camera *camera, int x, int y)
{
	t_ray		ray;
	double		rand_nb;
	double		screen_x;
	double		screen_y;
	t_vector	screen_point;

	rand_nb = (double)rand() / (RAND_MAX + 1.0);
	screen_x = (2.0 * (x + rand_nb) / WIDTH - 1.0) * camera->half_width;
	rand_nb = (double)rand() / (RAND_MAX + 1.0);
	screen_y = -(2.0 * (y + rand_nb) / HEIGHT - 1.0) * camera->half_height;
	screen_point = camera->viewpoint;
	screen_point = vector_add(screen_point, vector_mult(camera->u, screen_x));
	screen_point = vector_add(screen_point, vector_mult(camera->v, screen_y));
	screen_point = vector_add(screen_point, vector_mult(camera->w, -1.0));
	ray.origin = camera->viewpoint;
	ray.direction = vector_sub(screen_point, camera->viewpoint);
	ray.direction = vector_normalize(ray.direction);
	return (ray);
}
