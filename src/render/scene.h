/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 17:42:25 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/05 01:06:53 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

# include "../math_tool/math_tool.h"

// struct for the ambient lighting
// ratio: [0.0, 1.0]
typedef struct s_ambient
{
	float		ratio;
	t_color		color;
}	t_ambient;

// struct for the camera
// direction: normalized orientation
// fov: Horizontal field of view in degrees in the range [0, 180]
typedef struct s_camera
{
	t_vector	viewpoint;
	t_vector	direction;
	int			fov;

	t_vector	u;
	t_vector	v;
	t_vector	w;

	float		aspect_ratio;
	float		half_width;
	float		half_height;
}	t_camera;

// srtuct for the light
// ratio: [0.0, 1.0]
typedef struct s_light
{
	t_vector	position;
	float		ratio;
}	t_light;

typedef struct s_ray
{
	t_vector	origin;
	t_vector	direction;
}	t_ray;

#endif
