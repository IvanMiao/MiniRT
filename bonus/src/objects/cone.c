/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhuang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 20:19:26 by jinhuang          #+#    #+#             */
/*   Updated: 2025/08/31 20:36:30 by jinhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"

static void	cone_setup(t_cone *co, t_ray *ray, double *a, double *b, double *c)
{
	t_vector	v;
	t_vector	oc;
	double		cos2;

	v = vector_normalize(co->normal);
	oc = vector_sub(ray->origin, co->center); // 以顶点为基准
	cos2 = cos(co->angle) * cos(co->angle);

	*a = pow(vector_dot(ray->direction, v), 2) - cos2 * vector_dot(ray->direction, ray->direction);
	*b = 2 * (vector_dot(ray->direction, v) * vector_dot(oc, v) - cos2 * vector_dot(ray->direction, oc));
	*c = pow(vector_dot(oc, v), 2) - cos2 * vector_dot(oc, oc);
}


static double	solve_cone_quadratic(t_cone *co, t_ray *ray)
{
	double	a;
	double	b;
	double	c;
	double	discr;
	double	t1;
	double	t2;

	cone_setup(co, ray, &a, &b, &c);
	discr = b * b - 4 * a * c;
	if (discr < 0)
		return (-1.0);
	t1 = (-b - sqrt(discr)) / (2 * a);
	t2 = (-b + sqrt(discr)) / (2 * a);
	if (t1 > 1e-6)
		return (t1);
	if (t2 > 1e-6)
		return (t2);
	return (-1.0);
}

// static double	hit_cone_body(t_cone *co, t_ray *ray)
// {
// 	double	t;
// 	double	m;
// 	t_vector	v;
// 	t_vector	hit_point;
// 	t_vector	hit_to_base;

// 	t = solve_cone_quadratic(co, ray);
// 	if (t < 0)
// 		return (-1.0);
// 	v = vector_normalize(co->normal);
// 	hit_point = vector_add(ray->origin, vector_mult(ray->direction, t));
// 	hit_to_base = vector_sub(hit_point, co->center);
// 	m = vector_dot(hit_to_base, v);
// 	if (m < 0 || m > co->height)
// 		return (-1.0);
// 	return (t);
// }

static double	hit_cone_body(t_cone *co, t_ray *ray)
{
	double		t;
	double		m;
	t_vector	v;
	t_vector	hit_point;

	t = solve_cone_quadratic(co, ray);
	if (t < 0)
		return (-1.0);

	v = vector_normalize(co->normal);
	hit_point = vector_add(ray->origin, vector_mult(ray->direction, t));

	// 判断是否在高度范围内
	m = vector_dot(vector_sub(hit_point, co->center), v);
	if (m < 0 || m > co->height)
		return (-1.0);

	return (t);
}


static double	hit_cone_cap(t_cone *co, t_ray *ray)
{
	t_plane	cap;
	t_vector	base;   // 底面圆心
	t_vector	p;
	double	t;
	double	r;

	base = vector_add(co->center, vector_mult(vector_normalize(co->normal), co->height));

	// 底面法线方向和 cone->normal 一致
	cap.point = base;
	cap.normal = co->normal;

	t = hit_plane(&cap, ray);
	if (t > 0)
	{
		p = vector_add(ray->origin, vector_mult(ray->direction, t));
		r = tan(co->angle) * co->height; // 半径
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
