/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_normal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhuang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 20:47:28 by jinhuang          #+#    #+#             */
/*   Updated: 2025/08/31 20:47:48 by jinhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"

t_vector cone_normal_at(t_cone *co, t_vector point)
{
	t_vector	v;        // 单位法线方向
	//t_vector	tip;      // 圆锥顶点
	t_vector	oc;       // 点到顶点的向量
	double		m;        // 点在轴向上的投影长度
	double		k;        // tan(angle)
	t_vector	normal;
	double		epsilon = 1e-6;

	v = vector_normalize(co->normal);
	//tip = vector_add(co->center, vector_mult(v, co->height));
	oc = vector_sub(point, co->center);

	m = vector_dot(oc, v);
	k = tan(co->angle);

	// 计算法线：法线 = (点到顶点的向量) - (1+k^2)*投影部分
	normal = vector_sub(oc, vector_mult(v, m * (1 + k * k)));

	normal = vector_normalize(normal);

	// 处理点在顶点附近的情况，避免数值问题
	if (vector_length_sq(normal) < epsilon)
		return (vector_mult(v, -1)); // 顶点处法线方向近似为朝下法向量

	return (normal);
}

