/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_part1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:36:01 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/13 01:50:38 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math_tool.h"
#include <unistd.h>

t_vector	new_vector(double x, double y, double z)
{
	t_vector	res;

	res.x = x;
	res.y = y;
	res.z = z;
	return (res);
}

t_vector	vector_add(t_vector v1, t_vector v2)
{
	t_vector	res;

	res.x = v1.x + v2.x;
	res.y = v1.y + v2.y;
	res.z = v1.z + v2.z;
	return (res);
}

t_vector	vector_sub(t_vector v1, t_vector v2)
{
	t_vector	res;

	res.x = v1.x - v2.x;
	res.y = v1.y - v2.y;
	res.z = v1.z - v2.z;
	return (res);
}

t_vector	vector_mult(t_vector v, double scalar)
{
	t_vector	res;

	res.x = v.x * scalar;
	res.y = v.y * scalar;
	res.z = v.z * scalar;
	return (res);
}

t_vector	vector_div(t_vector v, double scalar)
{
	t_vector	res;

	if (scalar == 0)
	{
		write(2, "Error: Division by zero\n", 24);
		return (v);
	}
	res.x = v.x / scalar;
	res.y = v.y / scalar;
	res.z = v.z / scalar;
	return (res);
}
