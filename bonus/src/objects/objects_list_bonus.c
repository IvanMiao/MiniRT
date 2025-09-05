/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_list_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 01:57:04 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/05 07:39:03 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects_bonus.h"
#include "../utils/utils_bonus.h"

t_object	*obj_lstnew(void *obj, t_obj_type type, t_color color)
{
	t_object	*new_obj;

	new_obj = (t_object *)mem_manager(MALLOC, sizeof(t_object), NULL);
	new_obj->obj = obj;
	new_obj->type = type;
	new_obj->color = color;
	new_obj->next = NULL;
	return (new_obj);
}

t_object	*obj_lstlsat(t_object *lst)
{
	t_object	*curr;

	if (!lst)
		return (NULL);
	curr = lst;
	while (curr->next)
		curr = curr->next;
	return (curr);
}

void	obj_lstadd_back(t_object **lst, t_object *new_obj)
{
	t_object	*last;

	if (!lst || !new_obj)
		return ;
	if (!*lst)
	{
		*lst = new_obj;
		return ;
	}
	last = obj_lstlsat(*lst);
	last->next = new_obj;
}
