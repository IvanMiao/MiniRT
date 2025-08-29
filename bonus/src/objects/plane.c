/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 02:42:19 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/19 02:50:30 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"

double	hit_plane(t_plane *plane, t_ray *ray)
{
	double		t;
	double		dn;
	t_vector	op;

	op = vector_sub(plane->point, ray->origin);
	dn = vector_dot(ray->direction, plane->normal);
	if (dn == 0)
		return (0);
	t = vector_dot(op, plane->normal) / dn;
	return (t);
}
