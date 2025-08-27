/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assgniment.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhuang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 19:27:21 by jinhuang          #+#    #+#             */
/*   Updated: 2025/08/27 19:46:06 by jinhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vector	parse_vector(char *str)
{
	t_vector	v;
	char	**coords;

	coords = ft_split(str, ',');
	if (!coords)
		ft_error("failed to allocate memory");
	if (count_tokens(coords) != 3)
	{
		free_tokens(coords);
		ft_error("wrong vector");
	}
	v.x = ft_atof(coords[0]);
	v.y = ft_atof(coords[1]);
	v.z = ft_atof(coords[2]);
	free_tokens(coords);
	return (v);
}

bool	in_range(double val, double min, double max)
{
	return (val >= min && val <= max);
}

t_vector	normalize_vector(t_vector v)
{
	double	length;

	length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (length == 0)
		ft_error("cannot normalize");
	return (vector_div(v, length));
}

void	parse_camera(char **tokens, t_minirt *rt)
{
	double	fov_double;
	double	fov_radians;

	if (count_tokens(tokens) != 4)
		ft_error("wrong elements");
	if (rt->camera)
		ft_error("Repeated elements");

	rt->camera.viewpoint = parse_vector(tokens[1]);
	rt->camera.direction = normalize_vector(tokens[2]);
	fov_double = ft_atof(tokens[3]);
	if (!in_range(fov_double, 0, 180))
		ft_error("FOV out of range");
	rt->camera.fov = (int)fov_double;
	setup_cam_coords(&rt->camera);
}


