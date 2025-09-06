/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_tool.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:33:28 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/06 19:17:52 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_TOOL_H
# define MATH_TOOL_H

# include <math.h>
# include <stdlib.h>
# include <time.h>
# define WIDTH 1600
# define HEIGHT 1200

// MSAA settings - default value, can be overridden
# ifndef MSAA_SAMPLES
#  define MSAA_SAMPLES 256
# endif

// Global variable for runtime MSAA configuration
extern int g_msaa_samples;

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

typedef struct s_vector
{
	float	x;
	float	y;
	float	z;
}	t_vector;

// attention: elements in t_color are 3 normalized float numbers,
// instead of 3 int numbers
// we need initialize the color with `color_init(int r, int g, int b)`
typedef struct s_color
{
	float	r;
	float	g;
	float	b;
}	t_color;

t_vector	vector_init(float x, float y, float z);
t_vector	vector_add(t_vector v1, t_vector v2);
t_vector	vector_sub(t_vector v1, t_vector v2);
t_vector	vector_mult(t_vector v, float scalar);
t_vector	vector_div(t_vector v, float scalar);

float		vector_length(t_vector v);
float		vector_length_sq(t_vector v);
t_vector	vector_normalize(t_vector v);
float		vector_dot(t_vector v1, t_vector v2);
t_vector	vector_cross(t_vector v1, t_vector v2);

// Random number generation for MSAA
double		random_double(void);
double		random_double_range(double min, double max);

t_color		color_init(int r, int g, int b);
t_color		color_init_d(float r, float g, float b);
t_color		color_add(t_color a, t_color b);
t_color		color_div(t_color a, float scalar);
int			color_to_int(t_color color);

#endif
