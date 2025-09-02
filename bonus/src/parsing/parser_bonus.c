/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:57:45 by jinhuang          #+#    #+#             */
/*   Updated: 2025/09/02 19:22:26 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt_bonus.h"

static void	check_extention(char *filename)
{
	int	i;

	i = 0;
	while (filename[i])
		i++;
	if (i < 3)
		ft_error("File extention must be .rt");
	i--;
	if (filename[i] != 't' || filename[i - 1] != 'r' || filename[i - 2] != '.')
		ft_error("File extention must be .rt");
}

static bool	is_empty_or_comment(char *line)
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

static bool	parse_acl(char **tokens, t_minirt *rt)
{
	if (ft_strcmp(tokens[0], "A") == 0)
	{
		rt->a_count++;
		parse_ambient(tokens, rt);
		return (true);
	}
	else if (ft_strcmp(tokens[0], "C") == 0)
	{
		rt->c_count++;
		parse_camera(tokens, rt);
		return (true);
	}
	else if (ft_strcmp(tokens[0], "L") == 0)
	{
		rt->l_count++;
		parse_light(tokens, rt);
		return (true);
	}
	return (false);
}

void	parse_line(char *line, t_minirt *rt)
{
	char	**tokens;

	tokens = ft_split_space(line);
	mem_manager(FREE, 0, line);
	if (!tokens || !tokens[0])
		return ;
	if (parse_acl(tokens, rt))
		return ;
	else if (ft_strcmp(tokens[0], "sp") == 0)
		parse_sphere(tokens, rt);
	else if (ft_strcmp(tokens[0], "pl") == 0)
		parse_plane(tokens, rt);
	else if (ft_strcmp(tokens[0], "cy") == 0)
		parse_cylinder(tokens, rt);
	else if (ft_strcmp(tokens[0], "co") == 0)
		parse_cone(tokens, rt);
	else
		ft_error("Invalid type");
	free_tokens(tokens);
}

void	loading_file(char *filename, t_minirt *rt)
{
	int		fd;
	char	*line;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		ft_error("Cannot read file");
	mem_manager(STORE_FD, (size_t)fd, NULL);
	check_extention(filename);
	line = get_next_line(fd);
	while (line != NULL)
	{
		trim_newline(line);
		if (!is_empty_or_comment(line))
			parse_line(line, rt);
		line = get_next_line(fd);
	}
	close(fd);
	if (rt->a_count != 1 || rt->c_count != 1 || rt->l_count == 0)
		ft_error(ACL_ERROR);
}
