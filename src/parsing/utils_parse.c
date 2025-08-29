/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhuang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:51:29 by jinhuang          #+#    #+#             */
/*   Updated: 2025/08/27 21:01:15 by jinhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

#define EPSILON 1e-6

void	ft_error(const char *msg)
{
	write(2, "Error: ", 7);
	while (*msg)
		write(2, msg++, 1);
	write(2, "\n", 1);
	mem_manager(FREEALL, 0, NULL);
	exit(EXIT_FAILURE);
}

int	count_tokens(char **tokens)
{
	int	count;

	count = 0;
	while (tokens[count])
		count++;
	return (count);
}

bool	is_normalized_vector(t_vector v)
{
	double	length_squared;
	length_squared = v.x * v.x + v.y * v.y + v.z * v.z;
	return (fabs(length_squared - 1.0) < EPSILON);
}
