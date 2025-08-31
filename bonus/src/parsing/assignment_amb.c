/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assignment_amb.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 19:47:41 by jinhuang          #+#    #+#             */
/*   Updated: 2025/08/31 19:16:06 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

static void	check_color_tokens(char **tokens)
{
	int	i;
	int	j;

	i = 0;
	while (tokens[i])
	{
		j = 0;
		if (tokens[i][j] == '+' || tokens[i][j] == '-')
			j++;
		while (tokens[i][j])
		{
			if (!ft_isdigit(tokens[i][j]))
				ft_error("Invalid number format in color");
			j++;
		}
		i++;
	}
}

t_color	parse_color(char *str)
{
	char	**comp;
	int		r;
	int		g;
	int		b;

	comp = ft_split(str, ',');
	if (count_tokens(comp) != 3)
		ft_error("Wrong format of color");
	check_color_tokens(comp);
	r = ft_atoi(comp[0]);
	g = ft_atoi(comp[1]);
	b = ft_atoi(comp[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		ft_error("Out of range of RGB color");
	free_tokens(comp);
	return (color_init(r, g, b));
}

void	parse_ambient(char **tokens, t_minirt *rt)
{
	if (count_tokens(tokens) != 3)
		ft_error("wrong elements");
	rt->ambient.ratio = ft_atof(tokens[1]);
	if (rt->ambient.ratio < 0.0 || rt->ambient.ratio > 1.0)
		ft_error("Ambient out of range");
	rt->ambient.color = parse_color(tokens[2]);
}

void	parse_light(char **tokens, t_minirt *rt)
{
	t_vector	position;
	double		ratio;

	if (count_tokens(tokens) == 4)
		rt->light.color = parse_color(tokens[3]);
	else if (count_tokens(tokens) == 3)
		rt->light.color = color_init(1, 1, 1);
	else
		ft_error("wrong format of light");
	position = parse_vector(tokens[1]);
	ratio = ft_atof(tokens[2]);
	if (ratio < 0.0 || ratio > 1.0)
		ft_error("Light ratio out of range");
	rt->light.position = position;
	rt->light.ratio = ratio;
}
