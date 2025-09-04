/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 02:42:19 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/05 01:21:03 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects_bonus.h"

float	hit_plane(t_plane *plane, t_ray *ray)
{
	float		t;
	float		dn;
	t_vector	op;

	op = vector_sub(plane->point, ray->origin);
	dn = vector_dot(ray->direction, plane->normal);
	if (fabsf(dn) < EPSILON)
		return (-1.0f);
	t = vector_dot(op, plane->normal) / dn;
	if (t > EPSILON)
		return (t);
	return (-1.0f);
}
