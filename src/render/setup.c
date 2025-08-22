/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 02:47:32 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/22 20:59:29 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

/**
 * @brief 设置场景的所有元素
 */
// 设置相机：位于原点，看向负Z方向，FOV=60度
// 设置平面、球体、圆柱体
// 设置环境光
// 设置点光源
void	setup_scene(t_minirt *rt)
{
	t_sphere	*sphere;
	t_plane		*plane;
	t_cylinder	*cylinder;

	sphere = mem_manager(MALLOC, sizeof(t_sphere), NULL);
	plane = mem_manager(MALLOC, sizeof(t_plane), NULL);
	cylinder = mem_manager(MALLOC, sizeof(t_cylinder), NULL);

	sphere->center = vector_init(-10, 0, -40);
	sphere->radius = 10;
	sphere->color = color_init_d(1, 0, 0);

	plane->point = vector_init(0, -10, 0);
	plane->normal = vector_init(0, 1, 0);
	plane->color = color_init_d(0.4, 0.6, 0.8);

	cylinder->center = vector_init(20, -10, -40);
	cylinder->normal = vector_init(0, 1, 0);
	cylinder->diameter = 10;
	cylinder->height = 20;
	cylinder->color = color_init_d(0.4, 0.5, 0.75);

	rt->camera.viewpoint = vector_init(0, 0, 0);
	rt->camera.direction = vector_init(0, 0, -1);
	rt->camera.fov = 60;
	rt->ambient.ratio = 0.15;
	rt->ambient.color = color_init_d(1.0, 1.0, 1.0);
	rt->light.position = vector_init(-80, 80, 10);
	rt->light.ratio = 0.7;

	t_object	*objs;
	objs = NULL;
	// objs = obj_lstnew(sphere, SPHERE, sphere->color);
	obj_lstadd_back(&objs, obj_lstnew(sphere, SPHERE, sphere->color));
	obj_lstadd_back(&objs, obj_lstnew(plane, PLANE, plane->color));
	obj_lstadd_back(&objs, obj_lstnew(cylinder, CYLINDER, cylinder->color));
	rt->object = objs;
}
