/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_tool_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:33:28 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/02 19:21:31 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_TOOL_BONUS_H
# define MATH_TOOL_BONUS_H

# include <math.h>
# define WIDTH 1200
# define HEIGHT 900

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

typedef struct s_vector
{
	double	x;
	double	y;
	double	z;
}	t_vector;

// attention: elements in t_color are 3 normalized double numbers,
// instead of 3 int numbers
// we need initialize the color with `color_init(int r, int g, int b)`
typedef struct s_color
{
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
t_color		color_add(t_color a, t_color b);
t_color		color_div(t_color a, double scalar);
int			color_to_int(t_color color);

#endif
