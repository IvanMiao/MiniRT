/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 23:04:30 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/16 00:25:35 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <unistd.h>
# include <string.h>
# include <stddef.h>
# include <stdlib.h>
# include <stdio.h>

# define MALLOC		1
# define FREE		2
# define FREEALL	3

typedef struct s_node
{
	void			*ptr;
	struct s_node	*next;
}	t_node;

void	ft_putstr_fd(char *s, int fd);
char	**ft_split(char const *s, char c);

void	*mem_manager(int mode, size_t size, void *ptr);

#endif
