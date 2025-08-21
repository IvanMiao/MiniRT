/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 02:47:32 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/21 23:05:41 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

/**
 * @brief 设置场景的所有元素
 */
// 设置相机：位于原点，看向负Z方向，FOV=60度
// 设置球体
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

	sphere->center = vector_init(0, 0, -40);
	sphere->radius = 10;
	sphere->color = color_init_d(1, 0, 0);

	plane->point = vector_init(0, -10, 0);
	plane->normal = vector_init(0, 1, 0);
	plane->color = color_init_d(0.4, 0.6, 0.8);

	cylinder->center = vector_init(20, -10, -30);
	cylinder->normal = vector_init(0, 1, 0);
	cylinder->diameter = 10;
	cylinder->height = 20;
	cylinder->color = color_init_d(0.4, 0.5, 0.75);

	rt->camera.viewpoint = vector_init(0, 0, 0);
	rt->camera.direction = vector_init(0, 0, -1);
	rt->camera.fov = 70;
	rt->ambient.ratio = 0.15;
	rt->ambient.color = color_init_d(1.0, 1.0, 1.0);
	rt->light.position = vector_init(-80, 80, 10);
	rt->light.ratio = 0.7;

	rt->object = mem_manager(MALLOC, sizeof(t_object), NULL);
	rt->object->obj = sphere;
	rt->object->type = SPHERE;
	rt->object->color = color_init_d(1, 0, 0);

	rt->object->next = mem_manager(MALLOC, sizeof(t_object), NULL);
	rt->object->next->obj = plane;
	rt->object->next->type = PLANE;
	rt->object->next->color = color_init_d(0.4, 0.6, 0.8);

	t_object *obj = rt->object->next;
	obj->next = mem_manager(MALLOC, sizeof(t_object), NULL);
	obj->next->obj = cylinder;
	obj->next->type = CYLINDER;
	obj->next->color = cylinder->color;
	obj->next->next = NULL;
}
