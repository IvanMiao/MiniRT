/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assignment_obj_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:27:17 by jinhuang          #+#    #+#             */
/*   Updated: 2025/09/02 19:22:24 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt_bonus.h"

void	parse_sphere(char **tokens, t_minirt *rt)
{
	t_sphere	*sphere;
	t_vector	center;
	double		diameter;
	t_color		color;

	if (count_tokens(tokens) != 4)
		ft_error("Wrong format of sphere");
	center = parse_vector(tokens[1]);
	diameter = ft_atof(tokens[2]);
	if (diameter < 0)
		ft_error("Wrong input of sphere");
	color = parse_color(tokens[3]);
	sphere = mem_manager(MALLOC, sizeof(t_sphere), NULL);
	sphere->center = center;
	sphere->radius = diameter / 2.0;
	sphere->diameter = diameter;
	sphere->color = color;
	obj_lstadd_back(&rt->object, obj_lstnew(sphere, SPHERE, color));
}

void	parse_plane(char **tokens, t_minirt *rt)
{
	t_plane		*plane;
	t_vector	pos;
	t_vector	normal;
	t_color		color;

	if (count_tokens(tokens) != 4)
		ft_error("Wrong format of plane");
	pos = parse_vector(tokens[1]);
	normal = parse_vector(tokens[2]);
	if (!is_normalized_vector(normal))
		ft_error("Wrong input of plane");
	normal = vector_normalize(normal);
	color = parse_color(tokens[3]);
	plane = mem_manager(MALLOC, sizeof(t_plane), NULL);
	plane->point = pos;
	plane->normal = normal;
	plane->color = color;
	obj_lstadd_back(&rt->object, obj_lstnew(plane, PLANE, color));
}

static void	parse_cylinder2(char **tokens, t_cylinder *cyl)
{
	t_vector	center;
	t_vector	normal;
	double		diameter;
	double		height;
	t_color		color;

	if (count_tokens(tokens) != 6)
		ft_error("Wrong format of cylinder");
	center = parse_vector(tokens[1]);
	normal = parse_vector(tokens[2]);
	if (!is_normalized_vector(normal))
		ft_error("Wrong input of cylinder");
	diameter = ft_atof(tokens[3]);
	height = ft_atof(tokens[4]);
	if (diameter < 0 || height < 0)
		ft_error("Diameter and height cannot be negative");
	color = parse_color(tokens[5]);
	cyl->center = center;
	cyl->normal = vector_normalize(normal);
	cyl->diameter = diameter;
	cyl->height = height;
	cyl->color = color;
}

void	parse_cylinder(char **tokens, t_minirt *rt)
{
	t_cylinder	*cyl;

	cyl = mem_manager(MALLOC, sizeof(t_cylinder), NULL);
	parse_cylinder2(tokens, cyl);
	obj_lstadd_back(&rt->object, obj_lstnew(cyl, CYLINDER, cyl->color));
}

// code arguments: co <center> <normal> <height> <angle> <color>
// angle input: degree -> rad
void	parse_cone(char **tokens, t_minirt *rt)
{
	t_cone		*cone;
	t_vector	normal;
	double		height;
	double		angle;

	if (count_tokens(tokens) != 6)
		ft_error("Wrong format of cone");
	normal = parse_vector(tokens[2]);
	if (!is_normalized_vector(normal))
		ft_error("Wrong input of cone normal");
	normal = vector_normalize(normal);
	height = ft_atof(tokens[3]);
	if (height <= 0)
		ft_error("Invalid cone height");
	angle = ft_atof(tokens[4]);
	if (angle <= 0 || angle >= 90)
		ft_error("Invalid cone angle");
	angle = angle * (M_PI / 180.0);
	cone = mem_manager(MALLOC, sizeof(t_cone), NULL);
	cone->center = parse_vector(tokens[1]);
	cone->normal = normal;
	cone->height = height;
	cone->angle = angle;
	cone->color = parse_color(tokens[5]);
	obj_lstadd_back(&rt->object, obj_lstnew(cone, CONE, cone->color));
}
