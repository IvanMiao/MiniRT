/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:57:45 by jinhuang          #+#    #+#             */
/*   Updated: 2025/08/29 23:46:42 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"
#include <fcntl.h>

void	loading_file(char *filename, t_minirt *rt)
{
	int		fd;
	char	*line;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		ft_error("cannot read file");
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (!is_empty_or_comment(line))
			parse_line(line, rt);
		//free(line);
		line = get_next_line(fd);
	}
	close(fd);
}

bool	is_empty_or_comment(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '#')
			return (true);
		if (!ft_isspace(line[i]))
			return (false);
		i++;
	}
	return (true);
}

void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

void	parse_line(char *line, t_minirt *rt)
{
	char	**tokens;

	tokens = ft_split(line, ' ');
	mem_manager(FREE, 0, line);
	if (!tokens || !tokens[0])
		return ;
	if (ft_strcmp(tokens[0], "A") == 0)
		parse_ambient(tokens, rt);
	else if (ft_strcmp(tokens[0], "C") == 0)
		parse_camera(tokens, rt);
	else if (ft_strcmp(tokens[0], "L") == 0)
		parse_light(tokens, rt);
	else if (ft_strcmp(tokens[0], "sp") == 0)
		parse_sphere(tokens, rt);
	else if (ft_strcmp(tokens[0], "pl") == 0)
		parse_plane(tokens, rt);
	else if (ft_strcmp(tokens[0], "cy") == 0)
		parse_cylinder(tokens, rt);
	else
		ft_error("invalid type");
	//free_tokens(tokens);
}
