#include "utils.h"


static size_t	check_c(char letter)
{
	if (ft_isspace(letter))
		return (1);
	return (0);
}

static size_t	count_word(char const *s)
{
	size_t	i;

	i = 0;
	while (s[i] && !check_c(s[i]))
		i++;
	return (i);
}

static size_t	count_array(char const *s)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && check_c(s[i]))
			i++;
		if (s[i])
			count++;
		while (s[i] && !check_c(s[i]))
			i++;
	}
	return (count);
}

static char	*get_word(char const *s)
{
	char	*word;
	size_t	i;
	size_t	nb;

	i = -1;
	nb = count_word(s);
	word = (char *)mem_manager(MALLOC, sizeof(char) * (nb + 1), NULL);
	if (!word)
		return (NULL);
	while (++i < nb)
		word[i] = s[i];
	word[i] = '\0';
	return (word);
}

char	**ft_split_space(char const *s)
{
	char	**ans;
	size_t	nb;
	size_t	j;

	j = 0;
	nb = count_array(s);
	ans = (char **)mem_manager(MALLOC, sizeof(char *) * (nb + 1), NULL);
	while (*s)
	{
		while (*s && check_c(*s))
			s++;
		if (*s)
		{
			nb = count_word(s);
			ans[j] = get_word(s);
			s += nb;
			j++;
		}
	}
	ans[j] = NULL;
	return (ans);
}
