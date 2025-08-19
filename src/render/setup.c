/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 02:47:32 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/20 01:29:53 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

/**
 * @brief 设置场景的所有元素
 */
// 设置相机：位于原点，看向负Z方向，FOV=60度
// 设置球体
// 设置环境光：20% 白色环境光
// 设置点光源：80% 光源强度
void	setup_scene(t_minirt *rt)
{
	t_sphere	*sphere;
	t_plane		*plane;

	sphere = mem_manager(MALLOC, sizeof(t_sphere), NULL);
	plane = mem_manager(MALLOC, sizeof(t_plane), NULL);
	sphere->center = vector_init(0, 0, 20);
	sphere->radius = 10;
	sphere->color = color_init_d(1, 0, 0);
	plane->point = vector_init(0, -10, 0);
	plane->normal = vector_init(0, 1, 0);
	plane->color = color_init_d(0.4, 0.6, 0.8);

	rt->camera.viewpoint = vector_init(-50, 0, 20);
	rt->camera.direction = vector_init(1, 0, 0);
	rt->camera.fov = 70;
	rt->ambient.ratio = 0.2;
	rt->ambient.color = color_init_d(1.0, 1.0, 1.0);
	rt->light.position = vector_init(-40, 0, 30);
	rt->light.ratio = 0.7;

	rt->object = mem_manager(MALLOC, sizeof(t_object), NULL);
	rt->object->obj = sphere;
	rt->object->type = SPHERE;
	rt->object->color = color_init_d(1, 0, 0);
	rt->object->next = mem_manager(MALLOC, sizeof(t_object), NULL);
	rt->object->next->obj = plane;
	rt->object->next->type = PLANE;
	rt->object->next->color = color_init_d(0.4, 0.6, 0.8);
	rt->object->next->next = NULL;
}
