/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 03:17:29 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/02 03:31:44 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdbool.h>
# include "../math_tool/math_tool.h"

t_color		parse_color(char *str);
t_vector	parse_vector(char *str);

void		free_tokens(char **tokens);
double		ft_atof(const char *str);
void		ft_error(const char *msg);
int			count_tokens(char **tokens);
bool		is_normalized_vector(t_vector v);
void		trim_newline(char *str);

#endif
