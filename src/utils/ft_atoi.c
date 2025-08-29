/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhuang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 16:23:27 by jinhuang          #+#    #+#             */
/*   Updated: 2024/11/24 17:26:34 by jinhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	ft_atoi(const char *str)
{
	int	count;
	int	result;

	count = 1;
	result = 0;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		++str;
	if (*str == '-')
		count = -1;
	if (*str == '-' || *str == '+')
		++str;
	while (*str && *str >= '0' && *str <= '9')
	{
		result = (result * 10) + *str - '0';
		++str;
	}
	return (result * count);
}
/* #include <stdio.h>
#include <stdlib.h>

int	main()
{
	char *str = "-2334";
	char *str2 = " ---+--+1234ab567";
	char *str3 = "+2334";
	printf("%d\n", atoi(str));
	printf("%d\n", ft_atoi(str));
	printf("%d\n", atoi(str2));
	printf("%d\n", ft_atoi(str2));
	printf("%d\n", atoi(str3));
	printf("%d\n", ft_atoi(str3));
	return (0);
} */
