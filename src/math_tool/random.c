/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 21:30:00 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/04 21:44:02 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math_tool.h"

static int	g_random_init = 0;

// Global MSAA samples configuration
int g_msaa_samples = MSAA_SAMPLES;

/**
 * @brief Generate a random double between 0.0 and 1.0
 */
double	random_double(void)
{
	if (!g_random_init)
	{
		srand(time(NULL));
		g_random_init = 1;
	}
	return ((double)rand() / RAND_MAX);
}

/**
 * @brief Generate a random double between min and max
 */
double	random_double_range(double min, double max)
{
	return (min + (max - min) * random_double());
}
