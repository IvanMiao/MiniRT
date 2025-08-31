/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 20:38:07 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/31 20:41:59 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

t_light	*light_lstnew(t_vector position, double ratio, t_color color)
{
	t_light *new_light;
	
	new_light = mem_manager(MALLOC, sizeof(t_light), NULL);
	new_light->position = position;
	new_light->ratio = ratio;
	new_light->color = color;
	new_light->next = NULL;
	return (new_light);
}

void	light_lstadd_back(t_light **lst, t_light *new_light)
{
	t_light	*curr;
	
	if (!lst || !new_light)
		return ;
	if (!*lst)
	{
		*lst = new_light;
		return ;
	}
	curr = *lst;
	while (curr->next)
		curr = curr->next;
	curr->next = new_light;
}
