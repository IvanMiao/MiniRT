/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 17:42:25 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/18 20:31:20 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

# include "../math_tool/math_tool.h"
# include "../objects/objects.h"

// struct for the ambient lighting
// ratio: [0.0, 1.0]
typedef struct s_ambient
{
	double		ratio;
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
	int			initialized;

	t_vector	u;
	t_vector	v;
	t_vector	w;

	double		aspect_ratio;
	double		half_width;
	double		half_height;
}	t_camera;

// srtuct for the light
// ratio: [0.0, 1.0]
typedef struct s_light
{
	t_vector	position;
	double		ratio;
	int			initialized;
}	t_light;

typedef struct s_ray
{
	t_vector	origin;
	t_vector	direction;
}	t_ray;

#endif
