/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 20:36:39 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/05 01:06:53 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"

static float	hit_main(t_cylinder *cy, t_ray *ray, t_cy_info *info)
{
	float	t1;
	float	t2;
	float	m1;
	float	m2;

	if (info->discri < 0)
		return (-1.0f);
	t1 = (-info->b - sqrtf(info->discri)) / (2 * info->a);
	t2 = (-info->b + sqrtf(info->discri)) / (2 * info->a);
	m1 = vector_dot(ray->direction, info->normal) * t1
		+ vector_dot(info->oc, info->normal);
	m2 = vector_dot(ray->direction, info->normal) * t2
		+ vector_dot(info->oc, info->normal);
	if (t1 > EPSILON && m1 >= 0 && m1 <= cy->height)
		return (t1);
	if (t2 > EPSILON && m2 >= 0 && m2 <= cy->height)
		return (t2);
	return (-1.0f);
}

static float	hit_caps2(t_plane *cap, t_ray *ray, t_cy_info *info)
{
	float		t;
	t_vector	p;
	float		radius_sq;

	radius_sq = info->radius * info->radius;
	t = hit_plane(cap, ray);
	if (t > 0)
	{
		p = vector_add(ray->origin, vector_mult(ray->direction, t));
		if (vector_length_sq(vector_sub(p, cap->point)) <= radius_sq)
			return (t);
	}
	return (-1.0f);
}

static float	hit_caps(t_cylinder *cy, t_ray *ray, t_cy_info *info)
{
	t_plane		cap;
	float		t1;
	float		t2;

	cap.point = vector_add(cy->center, vector_mult(info->normal, cy->height));
	cap.normal = info->normal;
	t1 = hit_caps2(&cap, ray, info);
	cap.point = cy->center;
	cap.normal = vector_mult(info->normal, -1);
	t2 = hit_caps2(&cap, ray, info);
	if (t1 > 0 && (t2 < 0 || t1 < t2))
		return (t1);
	return (t2);
}

float hit_cylinder(t_cylinder *cy, t_ray *ray)
{
	t_cy_info	info;
	float		side_t;
	float		caps_t;

	info.normal = vector_normalize(cy->normal);
	info.radius = cy->diameter / 2.0f;
	info.oc = vector_sub(ray->origin, cy->center);
	info.a = vector_dot(ray->direction, ray->direction)
		- powf(vector_dot(ray->direction, info.normal), 2);
	info.b = 2 * (vector_dot(ray->direction, info.oc)
			- (vector_dot(ray->direction, info.normal)
				* vector_dot(info.oc, info.normal)));
	info.c = vector_dot(info.oc, info.oc)
		- powf(vector_dot(info.oc, info.normal), 2)
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
	float		m;

	normal = vector_normalize(cy->normal);
	oc = vector_sub(point, cy->center);
	m = vector_dot(oc, normal);
	if (m > cy->height - EPSILON)
		return (normal);
	if (m < EPSILON)
		return (vector_mult(normal, -1));
	return (vector_normalize(vector_sub(oc, vector_mult(normal, m))));
}
