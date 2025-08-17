/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_lights.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 00:11:02 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/17 02:26:07 by ymiao            ###   ########.fr       */
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
static t_color	calculate_ambient_light(t_ambient ambient, t_color obj_color)
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
static t_color	calculate_diffuse_light(t_light light, t_vector hit_point,
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
	result.r = light_intensity * obj_color.r;
	result.g = light_intensity * obj_color.g;
	result.b = light_intensity * obj_color.b;
	return (result);
}

/**
 * @brief Calculates all lighting effects
 * 
 * @param ambient
 * @param light
 * @param hit_point
 * @param normal
 * @param object_color
 * @return The final combined color
 */
t_color	calculate_combined_light(t_minirt *rt,
						t_vector hit_point, t_vector normal, t_color obj_color)
{
	t_color	ambient_color;
	t_color	diffuse_color;
	t_color	final_color;

	ambient_color = calculate_ambient_light(rt->ambient, obj_color);
	diffuse_color = calculate_diffuse_light(rt->light, hit_point,
			normal, obj_color);
	final_color.r = ambient_color.r + diffuse_color.r;
	final_color.g = ambient_color.g + diffuse_color.g;
	final_color.b = ambient_color.b + diffuse_color.b;
	if (final_color.r > 1.0)
		final_color.r = 1.0;
	if (final_color.g > 1.0)
		final_color.g = 1.0;
	if (final_color.b > 1.0)
		final_color.b = 1.0;
	return (final_color);
}
