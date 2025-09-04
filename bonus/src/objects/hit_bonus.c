/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 00:55:02 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/02 19:21:49 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects_bonus.h"

static float	hit_obj(t_object *obj, t_ray *ray)
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

static void	cal_point_and_normal(t_hit_record *record, t_ray *ray)
{
	record->point = vector_add(ray->origin,
			vector_mult(ray->direction, record->t));
	if (record->obj_ptr->type == SPHERE)
		record->normal = sphere_normal_at((t_sphere *)record->obj_ptr->obj,
				record->point);
	else if (record->obj_ptr->type == PLANE)
		record->normal = ((t_plane *)record->obj_ptr->obj)->normal;
	else if (record->obj_ptr->type == CYLINDER)
		record->normal = cylinder_normal_at((t_cylinder *)record->obj_ptr->obj,
				record->point);
	else if (record->obj_ptr->type == CONE)
		record->normal = cone_normal_at((t_cone *)record->obj_ptr->obj,
				record->point);
}

t_hit_record	trace_ray(t_object *objs, t_ray *ray)
{
	t_hit_record	record;
	t_object		*curr_obj;
	float			t;

	record.hit = false;
	record.t = INFINITY;
	curr_obj = objs;
	while (curr_obj)
	{
		t = hit_obj(curr_obj, ray);
		if (t > 1e-6 && t < record.t)
		{
			record.hit = true;
			record.t = t;
			record.obj_ptr = curr_obj;
			record.obj_color = curr_obj->color;
		}
		curr_obj = curr_obj->next;
	}
	if (record.hit)
		cal_point_and_normal(&record, ray);
	return (record);
}
