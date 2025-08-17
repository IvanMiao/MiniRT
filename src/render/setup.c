/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 02:47:32 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/17 03:23:30 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

/**
 * @brief 设置测试场景的所有元素
 */
// 设置相机：位于原点，看向负Z方向，FOV=60度
// 设置球体
// 设置环境光：20% 白色环境光
// 设置点光源：80% 光源强度
void	setup_scene(t_minirt *rt, t_sphere *sphere)
{
	rt->camera.viewpoint = vector_init(0, 0, 0);
	rt->camera.direction = vector_init(0, 0, -1);
	rt->camera.fov = 60;
	sphere->center = vector_init(0, 0, -5);
	sphere->radius = 1.0;
	sphere->color = color_init_d(0.8, 0.2, 0.2);
	rt->ambient.ratio = 0.2;
	rt->ambient.color = color_init_d(1.0, 1.0, 1.0);
	rt->light.position = vector_init(-2, 2, -3);
	rt->light.ratio = 0.8;
}
