/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_tool.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:33:28 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/21 22:18:37 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_TOOL_H
# define MATH_TOOL_H

# include <math.h>
# define WIDTH 1200
# define HEIGHT 900

# define M_PI 3.14159265358979323846

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

t_vector	vector_init(double x, double y, double z);
t_vector	vector_add(t_vector v1, t_vector v2);
t_vector	vector_sub(t_vector v1, t_vector v2);
t_vector	vector_mult(t_vector v, double scalar);
t_vector	vector_div(t_vector v, double scalar);

double		vector_length(t_vector v);
double		vector_length_sq(t_vector v);
t_vector	vector_normalize(t_vector v);
double		vector_dot(t_vector v1, t_vector v2);
t_vector	vector_cross(t_vector v1, t_vector v2);

t_color		color_init(int r, int g, int b);
t_color		color_init_d(double r, double g, double b);
t_color		color_add(t_color c1, t_color c2);
t_color		color_scale(t_color c, double scalar);
t_color		color_mult(t_color c1, t_color c2);
int			color_to_int(t_color color);

#endif
