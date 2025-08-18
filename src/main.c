/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:19:30 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/18 20:32:12 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	init_minirt(t_minirt *rt)
{
	rt->mlx = mlx_init();
	rt->mlx_win = mlx_new_window(rt->mlx, WIDTH, HEIGHT, "MiniRT");
	rt->img.img = mlx_new_image(rt->mlx, WIDTH, HEIGHT);
	rt->img.addr = mlx_get_data_addr(rt->img.img,
			&rt->img.bits_per_pixel, &rt->img.line_length, &rt->img.endian);
	init_event(rt);
}

int	main(void)
{
	t_minirt	rt;

	init_minirt(&rt);
	test_sphere(&rt);
	return (0);
}
