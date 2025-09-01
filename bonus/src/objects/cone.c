/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 20:19:26 by jinhuang          #+#    #+#             */
/*   Updated: 2025/09/01 20:26:31 by jinhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"

// static void	fill_co_info(t_cone *co, t_ray *ray, t_co_info *info)
// {
// 	info->a = info->dv * info->dv - co->cos2_a;
// 	info->b = 2 * (info->dv * info->ocv 
// 		- co->cos2_a * vector_dot(ray->direction, info->oc));
// 	info->c = info->ocv * info->ocv 
// 	- co->cos2_a * vector_dot(info->oc, info->oc);
// 	info->discr = info->b * info->b - 4 * info->a * info->c;
// }

static void	fill_co_info(t_cone *co, t_ray *ray, t_co_info *info)
{
	info->a = info->dv * info->dv - co->cos2_a;
	info->b = 2 * (info->dv * info->ocv
			- co->cos2_a * vector_dot(ray->direction, info->oc));
	info->c = info->ocv * info->ocv;
	info->c -= co->cos2_a * vector_dot(info->oc, info->oc);
	info->discr = info->b * info->b - 4 * info->a * info->c;
}

static double	hit_cone_body(t_cone *co, t_ray *ray)
{
	double		t1;
	double		t2;
	double		m1;
	double		m2;
	t_co_info	info;

	info.oc = vector_sub(ray->origin, co->center);
	info.dv = vector_dot(ray->direction, co->normal);
	info.ocv = vector_dot(info.oc, co->normal);
	fill_co_info(co, ray, &info);
	if (info.discr < 0)
		return (-1.0);
	t1 = (-info.b - sqrt(info.discr)) / (2 * info.a);
	m1 = info.ocv + t1 * info.dv;
	if (t1 <= EPSILON || m1 < 0 || m1 > co->height)
		t1 = -1.0;
	t2 = (-info.b + sqrt(info.discr)) / (2 * info.a);
	m2 = info.ocv + t2 * info.dv;
	if (t2 <= EPSILON || m2 < 0 || m2 > co->height)
		t2 = -1.0;
	if (t1 > 0 && t2 > 0)
		return (fmin(t1, t2));
	if (t1 > 0)
		return (t1);
	return (t2);
}

static double	hit_cone_cap(t_cone *co, t_ray *ray)
{
	t_plane		cap;
	t_vector	base;
	t_vector	p;
	double		t;
	double		r;

	base = vector_mult(vector_normalize(co->normal), co->height);
	cap.point = vector_add(co->center, base);
	cap.normal = co->normal;
	t = hit_plane(&cap, ray);
	if (t > 0)
	{
		p = vector_add(ray->origin, vector_mult(ray->direction, t));
		r = tan(co->angle) * co->height;
		if (vector_length_sq(vector_sub(p, base)) <= r * r)
			return (t);
	}
	return (-1.0);
}

double	hit_cone(t_cone *co, t_ray *ray)
{
	double	t_body;
	double	t_cap;

	t_body = hit_cone_body(co, ray);
	t_cap = hit_cone_cap(co, ray);
	if (t_body > 0 && (t_cap < 0 || t_body < t_cap))
		return (t_body);
	return (t_cap);
}

t_vector	cone_normal_at(t_cone *co, t_vector point)
{
	t_vector	v;
	t_vector	oc;
	double		m;
	double		k;
	t_vector	normal;

	v = vector_normalize(co->normal);
	oc = vector_sub(point, co->center);
	m = vector_dot(oc, v);
	if (fabs(m - co->height) < EPSILON)
		return (v);
	k = tan(co->angle);
	normal = vector_sub(oc, vector_mult(v, m * (1 + k * k)));
	if (vector_length_sq(normal) < EPSILON)
		return (vector_mult(v, -1));
	return (vector_normalize(normal));
}
