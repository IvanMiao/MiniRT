/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_part2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 01:42:45 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/13 02:10:51 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math_tool.h"

// calculate the length/module
double	vector_length(t_vector v)
{
	double	res;

	res = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return (res);
}

// normalize: transform the vector to a unit vector
t_vector	vector_normalize(t_vector v)
{
	t_vector	res;
	double		v_length;

	v_length = vector_length(v);
	res = vector_div(v, v_length);
	return (res);
}

// calculate the dot product of two vectors
double	vector_dot(t_vector v1, t_vector v2)
{
	double	res;

	res = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	return (res);
}

// calculate the cross product of two vectors
t_vector	vector_cross(t_vector v1, t_vector v2)
{
	t_vector	res;

	res.x = (v1.y * v2.z) - (v1.z * v2.y);
	res.y = (v1.z * v2.x) - (v1.x * v2.z);
	res.z = (v1.x * v2.y) - (v1.y * v2.x);
	return (res);
}
