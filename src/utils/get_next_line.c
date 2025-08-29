/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:15:54 by ymiao             #+#    #+#             */
/*   Updated: 2024/12/06 16:32:00 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

char	*read_file(int fd, char *bufcat)
{
	int		bytes;
	char	*buf;

	if (!bufcat)
		bufcat = ft_strdup("");
	bytes = 1;
	while (bytes && !ft_strchr(bufcat, '\n'))
	{
		buf = mem_manager(MALLOC, (BUFFER_SIZE + 1), NULL);
		if (!buf)
			return (NULL);
		bytes = read(fd, buf, BUFFER_SIZE);
		if ((bytes <= 0 && bufcat[0] == '\0') || bytes == -1)
		{
			mem_manager(FREE, 0, bufcat);
			mem_manager(FREE, 0, buf);
			return (NULL);
		}
		buf[bytes] = '\0';
		bufcat = ft_strjoin(bufcat, buf);
	}
	return (bufcat);
}

char	*check_buf(char *buf)
{
	char	*ans;
	char	*pos_n;

	if (!buf)
		return (NULL);
	pos_n = NULL;
	pos_n = ft_strchr(buf, '\n');
	if (pos_n)
	{
		ans = ft_strdup(pos_n + 1);
		mem_manager(FREE, 0, buf);
	}
	else
	{
		mem_manager(FREE, 0, buf);
		buf = NULL;
		ans = NULL;
	}
	return (ans);
}

char	*get_one_line(char *buf, char *line)
{
	char	*pos_n;

	if (!buf)
		return (NULL);
	pos_n = ft_strchr(buf, '\n');
	if (pos_n)
		line = ft_strlcpy(buf, pos_n - buf + 1);
	else
		line = ft_strdup(buf);
	return (line);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*buf;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = read_file(fd, buf);
	line = get_one_line(buf, line);
	buf = check_buf(buf);
	return (line);
}

/*
  int    main(void)
{
    char *line;
    char *name = "read_error.txt";
    int fd = open(name, O_RDONLY);
    line = get_next_line(fd);
    printf("%s\n", line);
    free(line);
    line = get_next_line(fd);
    printf("%s\n", line);
    free(line);
    line = get_next_line(80);
    printf("%s\n", line);
    free(line);
    close(fd);
    fd = open(name, O_RDONLY);
    line = get_next_line(fd);
    printf("%s\n", line);
    free(line);
    return (0);
}


int	main(void)
{
	char	*s;
	int		fd;
	int		i;

	fd = open("test.txt", O_RDONLY);
	i = -1;
	while (++i < 500)
	{
		s = get_next_line(fd);
		printf("Find the next line [%d]: %s\n", i, s);
		free(s);
	}
	close(fd);
	return (0);
}
*/
