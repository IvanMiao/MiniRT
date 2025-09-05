/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 20:19:26 by jinhuang          #+#    #+#             */
/*   Updated: 2025/09/05 01:51:38 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects_bonus.h"

static void	fill_co_info(t_cone *co, t_ray *ray, t_co_info *info)
{
	float	cos2;

	cos2 = cosf(co->angle) * cosf(co->angle);
	info->a = info->dv * info->dv - cos2;
	info->b = 2 * (info->dv * info->ocv
			- cos2 * vector_dot(ray->direction, info->oc));
	info->c = info->ocv * info->ocv
		- cos2 * vector_dot(info->oc, info->oc);
	info->discr = info->b * info->b - 4 * info->a * info->c;
}

static float	hit_cone_body(t_cone *co, t_ray *ray)
{
	float		t1;
	float		t2;
	float		m1;
	float		m2;
	t_co_info	info;

	info.oc = vector_sub(ray->origin, co->center);
	info.dv = vector_dot(ray->direction, co->normal);
	info.ocv = vector_dot(info.oc, co->normal);
	fill_co_info(co, ray, &info);
	if (info.discr < 0)
		return (-1.0f);
	t1 = (-info.b - sqrtf(info.discr)) / (2 * info.a);
	m1 = info.ocv + t1 * info.dv;
	if (t1 <= EPSILON || m1 < 0 || m1 > co->height)
		t1 = -1.0f;
	t2 = (-info.b + sqrtf(info.discr)) / (2 * info.a);
	m2 = info.ocv + t2 * info.dv;
	if (t2 <= EPSILON || m2 < 0 || m2 > co->height)
		t2 = -1.0f;
	if (t1 > 0 && t2 > 0)
		return (fminf(t1, t2));
	if (t1 > 0)
		return (t1);
	return (t2);
}

static float	hit_cone_cap(t_cone *co, t_ray *ray)
{
	t_plane		cap;
	t_vector	base;
	t_vector	p;
	float		t;
	float		r;

	base = vector_add(co->center,
			vector_mult(vector_normalize(co->normal), co->height));
	cap.point = base;
	cap.normal = co->normal;
	t = hit_plane(&cap, ray);
	if (t > 0)
	{
		p = vector_add(ray->origin, vector_mult(ray->direction, t));
		r = tanf(co->angle) * co->height;
		if (vector_length_sq(vector_sub(p, base)) <= r * r)
			return (t);
	}
	return (-1.0f);
}

float	hit_cone(t_cone *co, t_ray *ray)
{
	float	t_body;
	float	t_cap;

	t_body = hit_cone_body(co, ray);
	t_cap = hit_cone_cap(co, ray);
	if (t_body > 0 && (t_cap < 0 || t_body < t_cap))
		return (t_body);
	return (t_cap);
}

static t_vector	cone_normal_2(t_cone *co, t_vector oc, t_vector v, float m)
{
	t_vector	radial;
	t_vector	unit_radial;
	float		cos_angle;
	float		sin_angle;
	t_vector	normal;

	radial = vector_sub(oc, vector_mult(v, m));
	if (vector_length_sq(radial) < EPSILON * EPSILON)
		return (vector_init(1, 0, 0));
	unit_radial = vector_normalize(radial);
	cos_angle = cosf(co->angle);
	sin_angle = sinf(co->angle);
	normal = vector_add(
		vector_mult(unit_radial, sin_angle),
		vector_mult(v, -cos_angle)
	);
	return (vector_normalize(normal));
}

t_vector	cone_normal_at(t_cone *co, t_vector point)
{
	t_vector	v;
	t_vector	oc;
	float		m;
	float		k;
	t_vector	normal;

	v = vector_normalize(co->normal);
	oc = vector_sub(point, co->center);
	m = vector_dot(oc, v);
	if (fabsf(m - co->height) < EPSILON)
		return (v);
	k = tanf(co->angle);
	if (m < EPSILON)
		return (cone_normal_2(co, oc, v, m));
	normal = vector_sub(oc, vector_mult(v, m * (1 + k * k)));
	// if (vector_length_sq(normal) < EPSILON)
	// 	return (vector_mult(v, -1));
	return (vector_normalize(normal));
}
