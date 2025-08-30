/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:56:13 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/30 06:12:37 by ymiao            ###   ########.fr       */
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
# include <ctype.h>

# include "../mlx_linux/mlx.h"

# include "math_tool/math_tool.h"
# include "objects/objects.h"
# include "render/scene.h"
# include "utils/utils.h"

# define ACL_ERROR "Scene must have exactly one ambient light(A), \
one camera(C), and one light(L)"

# define EPSILON 1e-6

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

	int			a_count;
	int			c_count;
	int			l_count;
}	t_minirt;

//event
void	init_event(t_minirt *rt);

// render - mlx helper function
void	put_pixel(t_img *img, int x, int y, int color);

// (render -- parser )setup the whole scene
// (this func should be replaced by parser)
void	setup_scene(t_minirt *rt);

// render - camera projection
void	setup_cam_coords(t_camera *camera);
t_ray	gen_cam_ray(t_camera *camera, int x, int y);

// render - calculate shadows
bool	is_in_shadow(t_object *objs, t_vector hit_point, t_vector light_pos);

// render - calculate final color(light)
t_color	combine_light(t_minirt *rt,
			t_vector hit_point, t_vector normal, t_color obj_color);

// test for the entry point
void	test_sphere(t_minirt *rt);

//parsing
t_color	parse_color(char *str);
void	parse_ambient(char **tokens, t_minirt *rt);
void	parse_light(char **tokens, t_minirt *rt);
t_vector	parse_vector(char *str);
bool	in_range(double val, double min, double max);
t_vector	normalize_vector(t_vector v);
void	parse_camera(char **tokens, t_minirt *rt);
void	parse_sphere(char **tokens, t_minirt *rt);
void	parse_plane(char **tokens, t_minirt *rt);
void	parse_cylinder(char **tokens, t_minirt *rt);
void	loading_file(char *filename, t_minirt *rt);
bool	is_empty_or_comment(char *line);
void	free_tokens(char **tokens);
void	parse_line(char *line, t_minirt *rt);
double	ft_atof(const char *str);
void	ft_error(const char *msg);
int	count_tokens(char **tokens);
bool	is_normalized_vector(t_vector v);

#endif
