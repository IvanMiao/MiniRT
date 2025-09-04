/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_atof_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:54:23 by jinhuang          #+#    #+#             */
/*   Updated: 2025/09/05 01:06:53 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt_bonus.h"

static const char	*skip_whitespace_and_sign(const char *str, int *sign)
{
	*sign = 1;
	while (*str == ' ' || *str == '\t' || *str == '\n')
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			*sign = -1;
		str++;
	}
	return (str);
}

static float	parse_integer_part(const char **str)
{
	float	res;

	res = 0.0f;
	while (**str >= '0' && **str <= '9')
	{
		res = res * 10.0f + (**str - '0');
		(*str)++;
	}
	return (res);
}

static float	parse_fraction_part(const char **str)
{
	float	res;
	float	div;

	res = 0.0f;
	div = 10.0f;
	if (**str == '.')
	{
		(*str)++;
		while (**str >= '0' && **str <= '9')
		{
			res += (**str - '0') / div;
			div *= 10.0f;
			(*str)++;
		}
	}
	return (res);
}

float	ft_atof(const char *str)
{
	int		sign;
	float	int_part;
	float	frac_part;

	str = skip_whitespace_and_sign(str, &sign);
	int_part = parse_integer_part(&str);
	frac_part = parse_fraction_part(&str);
	if (*str != '\0')
		ft_error("Invalid number format for a float number");
	return (sign * (int_part + frac_part));
}
