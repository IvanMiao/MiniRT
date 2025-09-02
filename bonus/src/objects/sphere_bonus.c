/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 18:07:27 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/02 19:22:12 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects_bonus.h"

/**
 * @brief Calculates the intersection of a ray with a sphere.
 * 
 * @param sp A pointer to the sphere struct to check for intersection.
 * @param ray A pointer to the ray srtuct being cast.
 * @return	If the ray intersects the sphere, 
 * 			it returns the smallest positive 't', representing the distance 
 * 			from the ray's origin to the closest intersection point.
 * 			If no intersection, it returns -1.0.
*/
double	hit_sphere(const t_sphere *sp, const t_ray *ray)
{
	t_vector	oc;
	double		a;
	double		b;
	double		c;
	double		discriminant;

	oc = vector_sub(ray->origin, sp->center);
	a = vector_dot(ray->direction, ray->direction);
	b = 2.0 * vector_dot(oc, ray->direction);
	c = vector_dot(oc, oc) - sp->radius * sp->radius;
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (-1.0);
	else
		return ((-b - sqrt(discriminant)) / (2.0 * a));
}

/**
 * @brief Calculates the surface normal vector *at* a given point on a sphere.
 * 
 * @param sp A pointer to the sphere struct.
 * @param p The point on the sphere's surface where the noraml is calculated.
 * @return	A normalized vector representing the surface normal at point p,
 * 			pointint outward from the sphere's center.
 */
t_vector	sphere_normal_at(t_sphere *sp, t_vector p)
{
	t_vector	normal;

	normal = vector_sub(p, sp->center);
	return (vector_normalize(normal));
}
