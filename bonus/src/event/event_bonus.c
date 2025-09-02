/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 20:24:38 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/02 19:21:10 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt_bonus.h"

static int	f_close(t_minirt *rt)
{
	mlx_destroy_image(rt->mlx, rt->img.img);
	mlx_destroy_window(rt->mlx, rt->mlx_win);
	mlx_destroy_display(rt->mlx);
	free(rt->mlx);
	mem_manager(FREEALL, 0, NULL);
	exit(EXIT_SUCCESS);
}

static int	f_key(int keycode, t_minirt *rt)
{
	if (keycode == XK_Escape)
		f_close(rt);
	return (0);
}

void	init_event(t_minirt *rt)
{
	mlx_hook(rt->mlx_win, KeyPress, KeyPressMask, f_key, rt);
	mlx_hook(rt->mlx_win, DestroyNotify,
		StructureNotifyMask, f_close, rt);
}
