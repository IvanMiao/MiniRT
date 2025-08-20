/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:56:13 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/20 03:19:17 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>
# include <X11/X.h>
# include <X11/keysym.h>

# include "../mlx_linux/mlx.h"

# include "math_tool/math_tool.h"
# include "objects/objects.h"
# include "render/scene.h"
# include "utils/utils.h"

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_img;

typedef struct s_minirt
{
	void		*mlx;
	void		*mlx_win;
	t_img		img;
	t_ambient	ambient;
	t_camera	camera;
	t_light		light;
	t_object	*object;
}	t_minirt;

//event
void	init_event(t_minirt *rt);

// render - mlx helper function
void	put_pixel(t_img *img, int x, int y, int color);

// test
// camera projection
void	setup_scene(t_minirt *rt);

void	setup_cam_coords(t_camera *camera);
t_ray	gen_cam_ray(t_camera *camera, int x, int y);

bool	is_in_shadow(t_object *objs, t_vector hit_point, t_vector light_pos);

t_color	calculate_combined_light(t_minirt *rt,
			t_vector hit_point, t_vector normal, t_color obj_color);

void	test_sphere(t_minirt *rt);

#endif