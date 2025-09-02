/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assignment_cam_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 19:27:21 by jinhuang          #+#    #+#             */
/*   Updated: 2025/09/02 19:22:22 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt_bonus.h"

static bool	in_range(double val, double min, double max)
{
	return (val >= min && val <= max);
}

t_vector	parse_vector(char *str)
{
	t_vector	v;
	char		**coords;

	coords = ft_split(str, ',');
	if (!coords)
		ft_error("failed to allocate memory");
	if (count_tokens(coords) != 3)
		ft_error("wrong vector");
	v.x = ft_atof(coords[0]);
	v.y = ft_atof(coords[1]);
	v.z = ft_atof(coords[2]);
	free_tokens(coords);
	return (v);
}

void	parse_camera(char **tokens, t_minirt *rt)
{
	double	fov_double;

	if (count_tokens(tokens) != 4)
		ft_error("wrong elements");
	rt->camera.viewpoint = parse_vector(tokens[1]);
	rt->camera.direction = parse_vector(tokens[2]);
	if (!is_normalized_vector(rt->camera.direction))
		ft_error("Direction of camera should be normalized");
	fov_double = ft_atof(tokens[3]);
	if (!in_range(fov_double, 0, 180))
		ft_error("FOV out of range");
	rt->camera.fov = (int)fov_double;
	setup_cam_coords(&rt->camera);
}
