/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 02:47:32 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/29 17:41:42 by ymiao            ###   ########.fr       */
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
	// sp->center = vector_init(random_double(-50, 30),
	// 						random_double(-5, 20),
	// 						random_double(-100, -30));
	sp->radius = random_double(0.5, 1.5);
	// sp->color = color_init_d(random_double(0, 1),
	// 						random_double(0, 1),
	// 						random_double(0, 1));
	sp->center = vector_init(3, -5, -25);
	sp->color = color_init_d(0.7, 0.75, 0.7);
	obj_lstadd_back(objs, obj_lstnew(sp, SPHERE, sp->color));
}

static void	add_plane(t_object **objs)
{
	t_plane	*pl;
	
	pl = mem_manager(MALLOC, sizeof(t_plane), NULL);
	pl->point = vector_init(0, 10, 0);
	pl->normal = vector_init(0, -1, 0);
	pl->color = color_init_d(0.85, 0.82, 0.78);
	obj_lstadd_back(objs, obj_lstnew(pl, PLANE, pl->color));
}

static void	add_plane2(t_object **objs)
{
	t_plane	*pl;
	
	pl = mem_manager(MALLOC, sizeof(t_plane), NULL);
	pl->point = vector_init(0, 0, -50);
	pl->normal = vector_init(0, 0, 1);
	pl->color = color_init_d(0.65, 0.82, 0.78);
	obj_lstadd_back(objs, obj_lstnew(pl, PLANE, pl->color));
}

static void	add_cylinder(t_object **objs)
{
	t_cylinder	*cy;
	
	cy = mem_manager(MALLOC, sizeof(t_cylinder), NULL);
	cy->center = vector_init(-12, -10, -30);
	cy->normal = vector_init(0, 1, 0);
	cy->diameter = 5;
	cy->height = 20;
	cy->color = color_init_d(0.6, 0.7, 0.6);
	obj_lstadd_back(objs, obj_lstnew(cy, CYLINDER, cy->color));
}

/**
 * @brief set up all the elements in the scene
 */
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

	sphere->center = vector_init(0, 0, -40);
	sphere->radius = 3;
	sphere->color = color_init_d(0.7, 0.75, 0.7);

	plane->point = vector_init(0, -10, 0);
	plane->normal = vector_init(0, 1, 0);
	plane->color = color_init_d(0.85, 0.82, 0.78);

	cylinder->center = vector_init(12, -10, -30);
	cylinder->normal = vector_init(0, 1, 0);
	cylinder->diameter = 5;
	cylinder->height = 20;
	cylinder->color = color_init_d(0.8, 0.6, 0.5);

	rt->camera.viewpoint = vector_init(0, 0, 0);
	rt->camera.direction = vector_init(0, 0, -1);
	rt->camera.fov = 50;
	rt->ambient.ratio = 0.15;
	rt->ambient.color = color_init_d(1.0, 1.0, 1.0);
	rt->light.position = vector_init(0, 8, -30);
	rt->light.ratio = 0.6;

	t_object	*objs;
	objs = NULL;
	// objs = obj_lstnew(sphere, SPHERE, sphere->color);
	obj_lstadd_back(&objs, obj_lstnew(sphere, SPHERE, sphere->color));
	obj_lstadd_back(&objs, obj_lstnew(plane, PLANE, plane->color));
	obj_lstadd_back(&objs, obj_lstnew(cylinder, CYLINDER, cylinder->color));
	add_sphere(&objs);
	// add_sphere(&objs);
	add_plane(&objs);
	add_plane2(&objs);
	add_cylinder(&objs);
	// add_sphere(&objs);
	// add_sphere(&objs);
	rt->object = objs;
}
