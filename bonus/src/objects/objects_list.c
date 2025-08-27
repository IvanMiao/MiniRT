/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 01:57:04 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/22 20:49:21 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "../utils/utils.h"

t_object	*obj_lstnew(void *obj, t_obj_type type, t_color color)
{
	t_object	*new;

	new = (t_object *)mem_manager(MALLOC, sizeof(t_object), NULL);
	new->obj = obj;
	new->type = type;
	new->color = color;
	new->next = NULL;
	return (new);
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

void	obj_lstadd_back(t_object **lst, t_object *new)
{
	t_object	*last;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last = obj_lstlsat(*lst);
	last->next = new;
}
