/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_atof.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:54:23 by jinhuang          #+#    #+#             */
/*   Updated: 2025/08/31 01:49:07 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

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

static double	parse_integer_part(const char **str)
{
	double	res;

	res = 0.0;
	while (**str >= '0' && **str <= '9')
	{
		res = res * 10.0 + (**str - '0');
		(*str)++;
	}
	return (res);
}

static double	parse_fraction_part(const char **str)
{
	double	res;
	double	div;

	res = 0.0;
	div = 10.0;
	if (**str == '.')
	{
		(*str)++;
		while (**str >= '0' && **str <= '9')
		{
			res += (**str - '0') / div;
			div *= 10.0;
			(*str)++;
		}
	}
	return (res);
}

double	ft_atof(const char *str)
{
	int		sign;
	double	int_part;
	double	frac_part;
	double	result;

	str = skip_whitespace_and_sign(str, &sign);
	int_part = parse_integer_part(&str);
	frac_part = parse_fraction_part(&str);
	if (*str != '\0')
		ft_error("Invalid number format for a double number");
	result = sign * (int_part + frac_part);
	if (!isfinite(result))
		ft_error("Double overflow");
	return (result);
}
