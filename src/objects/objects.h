/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 02:13:08 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/05 01:51:49 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECTS_H
# define OBJECTS_H

# include "../math_tool/math_tool.h"
# include "../render/scene.h"
# include <stdbool.h>

# ifndef EPSILON
#  define EPSILON 1e-2f
# endif

typedef enum e_obj_type
{
	SPHERE,
	PLANE,
	CYLINDER
}	t_obj_type;

typedef struct s_sphere
{
	t_vector	center;
	float		diameter;
	float		radius;
	t_color		color;
}	t_sphere;

typedef struct s_plane
{
	t_vector	point;
	t_vector	normal;
	t_color		color;
}	t_plane;

typedef struct s_cylinder
{
	t_vector	center;
	t_vector	normal;
	float		diameter;
	float		height;
	t_color		color;
}	t_cylinder;

typedef struct s_object
{
	t_obj_type		type;
	void			*obj;
	t_color			color;
	struct s_object	*next;
}	t_object;

// intersection record
typedef struct s_hit_record
{
	bool		hit;
	t_object	*obj_ptr;
	float		t;
	t_vector	point;
	t_vector	normal;
	t_color		obj_color;
}	t_hit_record;

// helper struct, just for cylinder.c
typedef struct s_cy_info
{
	t_vector	oc;
	float		a;
	float		b;
	float		c;
	float		discri;
	float		radius;
	t_vector	normal;
}	t_cy_info;

// Calculate object hit/normal
float			hit_sphere(const t_sphere *sp, const t_ray *ray);
t_vector		sphere_normal_at(t_sphere *sp, t_vector p);

float			hit_plane(t_plane *plane, t_ray *ray);

float			hit_cylinder(t_cylinder *cy, t_ray *ray);
t_vector		cylinder_normal_at(t_cylinder *cy, t_vector point);

// Stores hit stats
t_hit_record	trace_ray(t_object *objs, t_ray *ray);

// object linked list
t_object		*obj_lstnew(void *obj, t_obj_type type, t_color color);
t_object		*obj_lstlsat(t_object *lst);
void			obj_lstadd_back(t_object **lst, t_object *new);

#endif
