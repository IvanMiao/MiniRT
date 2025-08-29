/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 02:47:32 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/27 21:10:39 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"


#include <time.h>
static double	random_double(double min, double max)
{
	return (min + ((double)rand() / RAND_MAX) * (max - min));
}

static void	add_sphere(t_object **objs)
{
	t_sphere	*sp;
	
	sp = mem_manager(MALLOC, sizeof(t_sphere), NULL);
	sp->center = vector_init(random_double(-50, 30),
							random_double(-5, 20),
							random_double(-100, -30));
	sp->radius = random_double(2, 7);
	sp->color = color_init_d(random_double(0, 1),
							random_double(0, 1),
							random_double(0, 1));
	obj_lstadd_back(objs, obj_lstnew(sp, SPHERE, sp->color));
}

static void	add_plane(t_object **objs)
{
	t_plane	*pl;
	
	pl = mem_manager(MALLOC, sizeof(t_plane), NULL);
	// pl->point = vector_init(random_double(-50, 30),
	// 						random_double(10, 40),
	// 						random_double(-100, -30));
	// pl->normal = vector_init(random_double(0, 0.5),
	// 						random_double(0, 1),
	// 						random_double(0, 0.5));
	pl->point = vector_init(30, -10, 0);
	pl->normal = vector_init(-1, 0.5, 0);
	pl->color = color_init_d(random_double(0, 1),
							random_double(0, 1),
							random_double(0, 1));
	obj_lstadd_back(objs, obj_lstnew(pl, PLANE, pl->color));
}

/**
 * @brief 设置场景的所有元素
 */
// 设置相机：位于原点，看向负Z方向，FOV=60度
// 设置平面、球体、圆柱体
// 设置环境光
// 设置点光源
void	setup_scene(t_minirt *rt)
{
	// 测试文件，待修改
	srand(time(NULL));
	t_sphere	*sphere;
	t_plane		*plane;
	t_cylinder	*cylinder;

	sphere = mem_manager(MALLOC, sizeof(t_sphere), NULL);
	plane = mem_manager(MALLOC, sizeof(t_plane), NULL);
	cylinder = mem_manager(MALLOC, sizeof(t_cylinder), NULL);

	sphere->center = vector_init(-10, 0, -40);
	sphere->radius = 5;
	sphere->color = color_init_d(1, 0, 0);

	plane->point = vector_init(0, -10, 0);
	plane->normal = vector_init(0, 1, 0);
	plane->color = color_init_d(0.4, 0.6, 0.8);

	cylinder->center = vector_init(15, -10, -40);
	cylinder->normal = vector_init(0, 1, 0);
	cylinder->diameter = 10;
	cylinder->height = 20;
	cylinder->color = color_init_d(0.4, 0.5, 0.75);

	rt->camera.viewpoint = vector_init(0, 0, 0);
	rt->camera.direction = vector_init(0, 0, -1);
	rt->camera.fov = 60;
	rt->ambient.ratio = 0.1;
	rt->ambient.color = color_init_d(1.0, 1.0, 1.0);
	rt->light.position = vector_init(-50, 100, 10);
	rt->light.ratio = 0.5;

	t_object	*objs;
	objs = NULL;
	// objs = obj_lstnew(sphere, SPHERE, sphere->color);
	obj_lstadd_back(&objs, obj_lstnew(sphere, SPHERE, sphere->color));
	obj_lstadd_back(&objs, obj_lstnew(plane, PLANE, plane->color));
	obj_lstadd_back(&objs, obj_lstnew(cylinder, CYLINDER, cylinder->color));
	add_sphere(&objs);
	add_sphere(&objs);
	add_plane(&objs);
	add_sphere(&objs);
	add_sphere(&objs);
	rt->object = objs;
}
