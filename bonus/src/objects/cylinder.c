/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 20:36:39 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/26 20:26:30 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"

static double	hit_main(t_cylinder *cy, t_ray *ray, t_cy_info *info)
{
	double	t1;
	double	t2;
	double	m1;
	double	m2;

	if (info->discri < 0)
		return (-1.0);
	t1 = (-info->b - sqrt(info->discri)) / (2 * info->a);
	t2 = (-info->b + sqrt(info->discri)) / (2 * info->a);
	m1 = vector_dot(ray->direction, info->normal) * t1
		+ vector_dot(info->oc, info->normal);
	m2 = vector_dot(ray->direction, info->normal) * t2
		+ vector_dot(info->oc, info->normal);
	if (t1 > 1e-6 && m1 >= 0 && m1 <= cy->height)
		return (t1);
	if (t2 > 1e-6 && m2 >= 0 && m2 <= cy->height)
		return (t2);
	return (-1.0);
}

static double	hit_caps(t_cylinder *cy, t_ray *ray, t_cy_info *info)
{
	t_plane		cap;
	double		t;
	double		closest_t;
	t_vector	p;

	closest_t = -1.0;
	cap.point = vector_add(cy->center, vector_mult(info->normal, cy->height));
	cap.normal = info->normal;
	t = hit_plane(&cap, ray);
	if (t > 0)
	{
		p = vector_add(ray->origin, vector_mult(ray->direction, t));
		if (vector_length_sq(vector_sub(p, cap.point)) <= pow(info->radius, 2))
			closest_t = t;
	}
	cap.point = cy->center;
	cap.normal = vector_mult(info->normal, -1);
	t = hit_plane(&cap, ray);
	if (t > 0)
	{
		p = vector_add(ray->origin, vector_mult(ray->direction, t));
		if (vector_length_sq(vector_sub(p, cap.point)) <= pow(info->radius, 2)
			&& (closest_t < 0 || t < closest_t))
			closest_t = t;
	}
	return (closest_t);
}

double	hit_cylinder(t_cylinder *cy, t_ray *ray)
{
	t_cy_info	info;
	double		side_t;
	double		caps_t;

	info.normal = vector_normalize(cy->normal);
	info.radius = cy->diameter / 2.0;
	info.oc = vector_sub(ray->origin, cy->center);
	info.a = vector_dot(ray->direction, ray->direction)
		- pow(vector_dot(ray->direction, info.normal), 2);
	info.b = 2 * (vector_dot(ray->direction, info.oc)
			- (vector_dot(ray->direction, info.normal)
				* vector_dot(info.oc, info.normal)));
	info.c = vector_dot(info.oc, info.oc)
		- pow(vector_dot(info.oc, info.normal), 2)
		- info.radius * info.radius;
	info.discri = info.b * info.b - 4 * info.a * info.c;
	side_t = hit_main(cy, ray, &info);
	caps_t = hit_caps(cy, ray, &info);
	if (side_t > 0 && (caps_t < 0 || side_t < caps_t))
		return (side_t);
	return (caps_t);
}

t_vector	cylinder_normal_at(t_cylinder *cy, t_vector point)
{
	t_vector	normal;
	t_vector	oc;
	double		m;
	double		epsilon;

	epsilon = 1e-4;
	normal = vector_normalize(cy->normal);
	oc = vector_sub(point, cy->center);
	m = vector_dot(oc, normal);
	if (m > cy->height - epsilon)
		return (normal);
	if (m < epsilon)
		return (vector_mult(normal, -1));
	return (vector_normalize(vector_sub(oc, vector_mult(normal, m))));
}
