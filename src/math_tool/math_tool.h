/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_tool.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:33:28 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/16 05:42:49 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_TOOL_H
# define MATH_TOOL_H

# include <math.h>

typedef struct s_vector
{
	double	x;
	double	y;
	double	z;
}	t_vector;

typedef struct s_color
{
	// double or int?
	double	r;
	double	g;
	double	b;
}	t_color;

typedef struct s_ray
{
	t_vector	origin;
	t_vector	direction;
}	t_ray;

t_vector	vector_init(double x, double y, double z);
t_vector	vector_add(t_vector v1, t_vector v2);
t_vector	vector_sub(t_vector v1, t_vector v2);
t_vector	vector_mult(t_vector v, double scalar);
t_vector	vector_div(t_vector v, double scalar);

double		vector_length(t_vector v);
double		vector_length_square(t_vector v);
t_vector	vector_normalize(t_vector v);
double		vector_dot(t_vector v1, t_vector v2);
t_vector	vector_cross(t_vector v1, t_vector v2);

#endif
