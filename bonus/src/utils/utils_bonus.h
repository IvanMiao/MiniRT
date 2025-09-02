/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 23:04:30 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/02 19:23:35 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_BONUS_H
# define UTILS_BONUS_H

# include <unistd.h>
# include <string.h>
# include <stddef.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <limits.h>

# define MALLOC		1
# define FREE		2
# define FREEALL	3
# define STORE_FD	4
# define CLOSE_FD	5

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

typedef struct s_node
{
	void			*ptr;
	struct s_node	*next;
}	t_node;

void	ft_putstr_fd(char *s, int fd);
char	**ft_split(char const *s, char c);
char	**ft_split_space(char const *s);

void	*mem_manager(int mode, size_t size, void *ptr);

int		ft_atoi(const char *str);
int		ft_strcmp(char *s1, char *s2);
int		ft_isspace(char c);
int		ft_isdigit(char c);

size_t	ft_strlen(const char *s);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strlcpy(const char *src, size_t size);
char	*get_next_line(int fd);

void	ft_error(const char *msg);

#endif
