/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 23:04:30 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/29 17:55:13 by jinhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <unistd.h>
# include <string.h>
# include <stddef.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>

# define MALLOC		1
# define FREE		2
# define FREEALL	3

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif


typedef struct s_list
{
	char			*str_buf;
	struct s_list	*next;
}				t_list;


typedef struct s_node
{
	void			*ptr;
	struct s_node	*next;
}	t_node;

void	ft_putstr_fd(char *s, int fd);
char	**ft_split(char const *s, char c);

void	*mem_manager(int mode, size_t size, void *ptr);

int		ft_atoi(const char *str);
int		ft_strcmp(char *s1, char *s2);
int		ft_isspace(char c);

size_t	ft_strlen(const char *s);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strlcpy(const char *src, size_t size);
char	*get_next_line(int fd);

#endif
