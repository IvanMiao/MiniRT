/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:56:13 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/05 01:06:53 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_BONUS_H
# define MINIRT_BONUS_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>
# include <X11/X.h>
# include <X11/keysym.h>
# include <ctype.h>
# include <time.h>

# include "../mlx_linux/mlx.h"

# include "math_tool/math_tool_bonus.h"
# include "parsing/parsing_bonus.h"
# include "objects/objects_bonus.h"
# include "render/scene_bonus.h"
# include "utils/utils_bonus.h"

# define ACL_ERROR "Scene must have exactly one ambient light(A), \
one camera(C), and at least one light(L)"

# define EPSILON 1e-2f
# define SAMPELS_PER_PIXEL 2
# define SHININESS 64.0f

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
	t_light		*light;
	t_object	*object;

	int			a_count;
	int			c_count;
	int			l_count;
}	t_minirt;

//event
void	init_event(t_minirt *rt);

// render
void	render(t_minirt *rt);

// render - camera projection
void	setup_cam_coords(t_camera *camera);
t_ray	gen_cam_ray(t_camera *camera, int x, int y);

// render - calculate shadows
bool	is_in_shadow(t_object *objs, t_vector hit_point, t_vector light_pos);

// render - calculate final color(light)
t_color	combine_light(t_minirt *rt,
			t_vector hit_point, t_vector normal, t_color obj_color);

//parsing
void	loading_file(char *filename, t_minirt *rt);
void	parse_line(char *line, t_minirt *rt);

void	parse_ambient(char **tokens, t_minirt *rt);
void	parse_light(char **tokens, t_minirt *rt);
void	parse_camera(char **tokens, t_minirt *rt);
void	parse_sphere(char **tokens, t_minirt *rt);
void	parse_plane(char **tokens, t_minirt *rt);
void	parse_cylinder(char **tokens, t_minirt *rt);
void	parse_cone(char **tokens, t_minirt *rt);

#endif
