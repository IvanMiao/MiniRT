/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 04:07:45 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/02 19:23:17 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils_bonus.h"

static size_t	check_c(char letter, char c)
{
	if (letter == c)
		return (1);
	return (0);
}

static size_t	count_word(char const *s, char c)
{
	size_t	i;

	i = 0;
	while (s[i] && !check_c(s[i], c))
		i++;
	return (i);
}

static size_t	count_array(char const *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && check_c(s[i], c))
			i++;
		if (s[i])
			count++;
		while (s[i] && !check_c(s[i], c))
			i++;
	}
	return (count);
}

static char	*get_word(char const *s, char c)
{
	char	*word;
	size_t	i;
	size_t	nb;

	i = -1;
	nb = count_word(s, c);
	word = (char *)mem_manager(MALLOC, sizeof(char) * (nb + 1), NULL);
	if (!word)
		return (NULL);
	while (++i < nb)
		word[i] = s[i];
	word[i] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**ans;
	size_t	nb;
	size_t	j;

	j = 0;
	nb = count_array(s, c);
	ans = (char **)mem_manager(MALLOC, sizeof(char *) * (nb + 1), NULL);
	if (!ans)
		return (NULL);
	while (*s)
	{
		while (*s && check_c(*s, c))
			s++;
		if (*s)
		{
			nb = count_word(s, c);
			ans[j] = get_word(s, c);
			s += nb;
			j++;
		}
	}
	ans[j] = NULL;
	return (ans);
}
