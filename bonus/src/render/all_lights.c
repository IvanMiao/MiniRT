/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_lights.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 00:11:02 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/31 23:54:28 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

/**
 * @brief Calculates the ambient lighting
 * 
 * @param ambient
 * @param obj_color
 * @return The resulting ambient light color
 */
static t_color	cal_ambient_light(t_ambient ambient, t_color obj_color)
{
	t_color	result;

	result.r = ambient.ratio * ambient.color.r * obj_color.r;
	result.g = ambient.ratio * ambient.color.g * obj_color.g;
	result.b = ambient.ratio * ambient.color.b * obj_color.b;
	return (result);
}

/**
 * @brief Calculates the diffuse lighting
 * 
 * @param light
 * @param hit_point
 * @param normal
 * @param obje_color
 * @return The resulting diffuse light color
 */
static t_color	cal_diff_light(t_light light, t_vector hit_point,
								t_vector normal, t_color obj_color)
{
	t_vector	light_dir;
	double		light_intensity;
	t_color		result;

	light_dir = vector_sub(light.position, hit_point);
	light_dir = vector_normalize(light_dir);
	light_intensity = vector_dot(normal, light_dir);
	if (light_intensity < 0.0)
		light_intensity = 0.0;
	light_intensity *= light.ratio;
	result.r = light_intensity * obj_color.r * light.color.r;
	result.g = light_intensity * obj_color.g * light.color.g;
	result.b = light_intensity * obj_color.b * light.color.b;
	return (result);
}

static t_color	cal_spec_color(t_minirt *rt,
						t_light light, t_vector hit_point, t_vector normal)
{
	t_vector	light_dir;
	t_vector	view_dir;
	t_vector	reflect_dir;
	double		spec_intensity;
	t_color		res;

	light_dir = vector_normalize(vector_sub(light.position, hit_point));
	view_dir = vector_normalize(vector_sub(rt->camera.viewpoint, hit_point));
	reflect_dir = vector_sub(
			vector_mult(normal, 2 * vector_dot(normal, light_dir)), light_dir);
	spec_intensity = pow(fmax(vector_dot(view_dir, reflect_dir), 0.0), 64.0);
	res.r = light.ratio * spec_intensity * light.color.r;
	res.g = light.ratio * spec_intensity * light.color.g;
	res.b = light.ratio * spec_intensity * light.color.b;
	return (res);
}

static void	check_final_color(t_color *final_color)
{
	if (final_color->r > 1.0)
		final_color->r = 1.0;
	if (final_color->g > 1.0)
		final_color->g = 1.0;
	if (final_color->b > 1.0)
		final_color->b = 1.0;
}

/**
 * @brief Calculates all lighting effects
 * 
 * @param t_minirt including ambient and light
 * @param hit_point
 * @param normal
 * @param object_color
 * @return The final combined color
 */
t_color	combine_light(t_minirt *rt,
						t_vector hit_point, t_vector normal, t_color obj_color)
{
	t_color	diff_color;
	t_color	final_color;
	t_color	spec_color;
	t_light	*curr_light;

	spec_color = color_init_d(0, 0, 0);
	final_color = cal_ambient_light(rt->ambient, obj_color);
	curr_light = rt->light;
	while (curr_light)
	{
		if (!is_in_shadow(rt->object, hit_point, curr_light->position))
		{
			diff_color = cal_diff_light(*curr_light, hit_point,
					normal, obj_color);
			spec_color = cal_spec_color(rt, *curr_light, hit_point, normal);
			final_color = color_add(final_color, diff_color);
			final_color = color_add(final_color, spec_color);
		}
		curr_light = curr_light->next;
	}
	check_final_color(&final_color);
	return (final_color);
}
