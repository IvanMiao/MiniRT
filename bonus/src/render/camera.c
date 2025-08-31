/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 23:05:09 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/18 21:01:19 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

/**
 * @param camera Struct for camera
 * @var world_up "up" direciton of the world 
 * @var u right
 * @var v up
 * @var w behind
 */
void	setup_cam_coords(t_camera *camera)
{
	t_vector	world_up;
	double		theta;

	camera->aspect_ratio = (double)WIDTH / (double)HEIGHT;
	world_up = vector_init(0, 1, 0);
	if (fabs(vector_dot(camera->direction, world_up)) > 0.999)
		world_up = vector_init(0, 0, 1);
	camera->w = vector_mult(camera->direction, -1.0);
	camera->w = vector_normalize(camera->w);
	camera->u = vector_cross(world_up, camera->w);
	camera->u = vector_normalize(camera->u);
	camera->v = vector_cross(camera->w, camera->u);
	theta = camera->fov * M_PI / 180.0;
	camera->half_height = tan(theta / 2.0);
	camera->half_width = camera->half_height * camera->aspect_ratio;
}
