/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 02:13:08 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/13 18:52:12 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECTS_H
# define OBJECTS_H

# include "../math_tool/math_tool.h"

typedef enum e_obj_type
{
	SPHERE,
	PLANE,
	CYLINDER
}	t_obj_type;

typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

typedef struct s_sphere
{
	t_vector	center;
	double		radius;
}	t_sphere;

double		hit_sphere(const t_sphere *sp, const t_ray *ray);
t_vector	sphere_normal_at(t_sphere *sp, t_vector p);

#endif
