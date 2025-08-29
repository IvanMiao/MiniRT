/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:19:30 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/29 18:30:04 by jinhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	init_minirt(t_minirt *rt)
{
	rt->mlx = mlx_init();
	rt->mlx_win = mlx_new_window(rt->mlx, WIDTH, HEIGHT, "MiniRT");
	rt->img.img = mlx_new_image(rt->mlx, WIDTH, HEIGHT);
	rt->img.addr = mlx_get_data_addr(rt->img.img,
			&rt->img.bits_per_pixel, &rt->img.line_length, &rt->img.endian);
	init_event(rt);
}

static void	init_rt(t_minirt *rt)
{
	rt->mlx = NULL;
	rt->mlx_win = NULL;
	rt->img.img = NULL;
	rt->img.addr = NULL;
	rt->img.bits_per_pixel = 0;
	rt->img.line_length = 0;
	rt->img.endian = 0;
	rt->ambient.ratio = -1.0;
	rt->ambient.color.r = 0;
	rt->ambient.color.g = 0;
	rt->ambient.color.b = 0;
	rt->camera.viewpoint = (t_vector){0.0, 0.0, 0.0};
	rt->camera.direction = (t_vector){0.0, 0.0, -1.0};
	rt->camera.fov = 0;
	rt->camera.initialized = 0;
	rt->light.position = (t_vector){0.0, 0.0, 0.0};
	rt->light.ratio = 0.0;
	rt->light.initialized = 0;
	rt->object = NULL;
}


#include <stdio.h>

void print_color(const char *name, t_color color)
{
    printf("%s: R=%f, G=%f, B=%f\n", name, color.r, color.g, color.b);
}

void print_vector(const char *name, t_vector v)
{
    printf("%s: x=%.2f, y=%.2f, z=%.2f\n", name, v.x, v.y, v.z);
}

void print_rt_status(t_minirt *rt)
{
    printf("=== miniRT 状态检查 ===\n");

    printf("Ambient Light:\n");
    printf("  ratio = %.2f\n", rt->ambient.ratio);
    print_color("  color", rt->ambient.color);

    printf("Camera:\n");
    print_vector("  viewpoint", rt->camera.viewpoint);
    print_vector("  direction", rt->camera.direction);
    printf("  fov = %d\n", rt->camera.fov);
    printf("  initialized = %d\n", rt->camera.initialized);

    printf("Light:\n");
    print_vector("  position", rt->light.position);
    printf("  ratio = %.2f\n", rt->light.ratio);
    printf("  initialized = %d\n", rt->light.initialized);

    if (rt->object == NULL)
        printf("Objects: none\n");
    else
        printf("Objects: present (need custom print function to display)\n");

    printf("========================\n");
}

void print_objects(t_object *obj_list)
{
    t_object *curr = obj_list;

    while (curr)
    {
        printf("Object type: ");
        if (curr->type == SPHERE)
        {
            t_sphere *sp = (t_sphere *)curr->obj;
            printf("SPHERE\n");
            print_vector("  Center", sp->center);
            printf("\n");
            printf("  Diameter: %.2f\n", sp->diameter);
            printf("  Radius: %.2f\n", sp->radius);
            print_color("  Color", sp->color);
            printf("\n");
        }
        else if (curr->type == PLANE)
        {
            t_plane *pl = (t_plane *)curr->obj;
            printf("PLANE\n");
            print_vector("  Point", pl->point);
            printf("\n");
            print_vector("  Normal", pl->normal);
            printf("\n");
            print_color("  Color", pl->color);
            printf("\n");
        }
        else if (curr->type == CYLINDER)
        {
            t_cylinder *cy = (t_cylinder *)curr->obj;
            printf("CYLINDER\n");
            print_vector("  Center", cy->center);
            printf("\n");
            print_vector("  Normal", cy->normal);
            printf("\n");
            printf("  Diameter: %.2f\n", cy->diameter);
            printf("  Height: %.2f\n", cy->height);
            print_color("  Color", cy->color);
            printf("\n");
        }
        else
        {
            printf("UNKNOWN TYPE\n");
        }
        curr = curr->next;
    }
}

int	main(int argc, char **argv)
{
	t_minirt	rt;

	if (argc != 2)
		ft_error("input error");
	init_rt(&rt);
	loading_file(argv[1], &rt);
	print_rt_status(&rt);
	print_objects(rt.object);
	test_sphere(&rt);
	return (0);
}
