/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 03:07:10 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/02 19:23:02 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt_bonus.h"

static double	hit_for_shadow(t_object *obj, t_ray *ray)
{
	if (obj->type == SPHERE)
		return (hit_sphere((t_sphere *)obj->obj, ray));
	if (obj->type == PLANE)
		return (hit_plane((t_plane *)obj->obj, ray));
	if (obj->type == CYLINDER)
		return (hit_cylinder((t_cylinder *)obj->obj, ray));
	if (obj->type == CONE)
		return (hit_cone((t_cone *)obj->obj, ray));
	return (INFINITY);
}

bool	is_in_shadow(t_object *objs, t_vector hit_point, t_vector light_pos)
{
	t_ray		shadow_ray;
	double		dist_to_light;
	t_object	*curr_obj;
	double		t;

	shadow_ray.direction = vector_normalize(vector_sub(light_pos, hit_point));
	shadow_ray.origin = vector_add(hit_point,
			vector_mult(shadow_ray.direction, 1e-4));
	dist_to_light = vector_length(vector_sub(light_pos, hit_point));
	curr_obj = objs;
	while (curr_obj)
	{
		t = hit_for_shadow(curr_obj, &shadow_ray);
		if (t > 0 && t < dist_to_light)
			return (true);
		curr_obj = curr_obj->next;
	}
	return (false);
}
