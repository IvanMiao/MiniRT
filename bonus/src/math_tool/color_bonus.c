/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 17:54:58 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/05 01:06:53 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math_tool_bonus.h"

/**
 * @brief make int to float to get more accurencies
 */
t_color	color_init(int r, int g, int b)
{
	t_color	res;

	res.r = (float)r / 255.0f;
	res.g = (float)g / 255.0f;
	res.b = (float)b / 255.0f;
	return (res);
}

t_color	color_init_d(float r, float g, float b)
{
	t_color	res;

	res.r = r;
	res.g = g;
	res.b = b;
	return (res);
}

t_color	color_add(t_color a, t_color b)
{
	t_color	res;

	res.r = a.r + b.r;
	res.g = a.g + b.g;
	res.b = a.b + b.b;
	return (res);
}

t_color	color_div(t_color a, float scalar)
{
	t_color	res;

	res.r = a.r / scalar;
	res.g = a.g / scalar;
	res.b = a.b / scalar;
	return (res);
}

int	color_to_int(t_color color)
{
	int	r;
	int	g;
	int	b;

	r = (int)(fminf(color.r, 1.0f) * 255.0f);
	g = (int)(fminf(color.g, 1.0f) * 255.0f);
	b = (int)(fminf(color.b, 1.0f) * 255.0f);
	if (r < 0)
		r = 0;
	if (g < 0)
		g = 0;
	if (b < 0)
		b = 0;
	return (r << 16 | g << 8 | b);
}
