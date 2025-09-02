/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 16:23:27 by jinhuang          #+#    #+#             */
/*   Updated: 2025/09/02 18:52:31 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	ft_atoi(const char *str)
{
	int		count;
	long	result_long;
	int		result;

	count = 1;
	result_long = 0;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		++str;
	if (*str == '-')
		count = -1;
	if (*str == '-' || *str == '+')
		++str;
	while (*str && *str >= '0' && *str <= '9')
	{
		if (result_long > (INT_MAX - (*str - '0')) / 10)
			ft_error("Integer overflow");
		result_long = (result_long * 10) + *str - '0';
		++str;
	}
	if (result_long > INT_MAX || result_long < INT_MIN)
		ft_error("Integer overflow");
	result = result_long;
	return (result * count);
}
