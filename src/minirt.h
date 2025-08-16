/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:56:13 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/16 05:37:13 by ymiao            ###   ########.fr       */
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

# include "../mlx_linux/mlx.h"

# include "math_tool/math_tool.h"
# include "objects/objects.h"
# include "scene/scene.h"
# include "utils/utils.h"

# define WIDTH 800
# define HEIGHT 800

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
	t_node		*objects;
}	t_minirt;

// mlx helper function
void	put_pixel(t_img *img, int x, int y, int color);

// test
void	test_sphere(void);

#endif