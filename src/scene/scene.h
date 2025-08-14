/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 17:42:25 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/14 18:16:39 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

# include "../math_tool/math_tool.h"

// struct for the ambient lighting
typedef struct s_ambient
{
	double		ratio;
	t_color		color;
}	t_ambient;

// struct for the camera
// fov: Horizontal field of view in degrees in the range [0, 180]
typedef struct s_camera
{
	t_vector	viewpoint;
	t_vector	norm_orient;
	int			fov;
}	t_camera;

// srtuct for the light
typedef struct s_light
{
	t_vector	position;
	double		brightness;
}	t_light;

#endif