/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 16:23:27 by jinhuang          #+#    #+#             */
/*   Updated: 2025/09/01 19:59:59 by jinhuang         ###   ########.fr       */
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
		if (result > (INT_MAX - (*str - '0')) / 10)
			ft_error("Integer overflow");
		result = (result * 10) + *str - '0';
		++str;
	}
	result *= count;
	if (result > INT_MAX || result < INT_MIN)
		ft_error("Integer overflow");
	return (result);
}
