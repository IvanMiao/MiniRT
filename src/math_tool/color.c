/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 17:54:58 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/16 05:38:44 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "math_tool.h"

/**
 * @brief make int to double to get more accurencies
 */
t_color	color_init(int r, int g, int b)
{
	t_color	res;

	res.r = (double)r / 255.0;
	res.g = (double)g / 255.0;
	res.b = (double)b / 255.0;
	return (res);
}

t_color	color_add(t_color c1, t_color c2)
{
	t_color	res;

	res.r = c1.r + c2.r;
	res.g = c1.g + c2.g;
	res.b = c1.b + c2.b;
	return (res);
}

t_color	color_scale(t_color c, double scalar)
{
	t_color	res;

	res.r = c.r * scalar;
	res.g = c.g * scalar;
	res.b = c.b * scalar;
	return (res);
}

t_color	color_mult(t_color c1, t_color c2)
{
	t_color	res;

	res.r = c1.r * c2.r;
	res.g = c1.g * c2.g;
	res.b = c1.b * c2.b;
	return (res);
}

int	color_to_int(t_color color)
{
	int	r;
	int	g;
	int	b;

	r = (int)(fmin(color.r, 1.0) * 255.0);
	g = (int)(fmin(color.g, 1.0) * 255.0);
	b = (int)(fmin(color.b, 1.0) * 255.0);
	if (r < 0)
		r = 0;
	if (g < 0)
		g = 0;
	if (b < 0)
		b = 0;
	return (r << 16 | g << 8 | b);
}
