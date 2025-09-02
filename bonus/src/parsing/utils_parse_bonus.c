/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parse_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:51:29 by jinhuang          #+#    #+#             */
/*   Updated: 2025/09/02 19:22:40 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt_bonus.h"

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
	return (fabs(length_squared - 1.0) < 1e-4);
}

void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		mem_manager(FREE, 0, tokens[i]);
		i++;
	}
	mem_manager(FREE, 0, tokens);
}

void	trim_newline(char *str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
		i++;
	if (i > 0 && str[i - 1] == '\n')
		str[i - 1] = '\0';
}
