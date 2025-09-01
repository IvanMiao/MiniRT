/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_normal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 20:47:28 by jinhuang          #+#    #+#             */
/*   Updated: 2025/09/01 02:08:05 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"

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
