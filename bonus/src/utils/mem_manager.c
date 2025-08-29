/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:41:58 by ymiao             #+#    #+#             */
/*   Updated: 2025/08/13 23:20:39 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

static int	track_malloc(void *ptr, t_node **head)
{
	t_node	*new;

	new = malloc(sizeof(t_node));
	if (!new)
		return (1);
	new->ptr = ptr;
	new->next = *head;
	*head = new;
	return (0);
}

static int	free_all_malloc(t_node **head)
{
	t_node	*curr;
	t_node	*next;

	curr = *head;
	while (curr)
	{
		next = curr->next;
		if (curr->ptr)
			free(curr->ptr);
		free(curr);
		curr = next;
	}
	*head = NULL;
	return (0);
}

static void	*safe_malloc(size_t size, t_node **head)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr || size == 0)
	{
		ft_putstr_fd("malloc error!\n", 2);
		free_all_malloc(head);
		exit(EXIT_FAILURE);
	}
	if (track_malloc(ptr, head) != 0)
	{
		ft_putstr_fd("malloc error!\n", 2);
		free(ptr);
		free_all_malloc(head);
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

static void	safe_free(void *ptr, t_node **head)
{
	t_node	*curr;
	t_node	*prev;

	if (ptr == NULL)
		return ;
	curr = *head;
	prev = NULL;
	while (curr)
	{
		if (curr->ptr == ptr)
		{
			if (prev == NULL)
				*head = curr->next;
			else
				prev->next = curr->next;
			free(curr->ptr);
			free(curr);
			break ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	*mem_manager(int mode, size_t size, void *ptr)
{
	static t_node	*head = NULL;

	if (mode == MALLOC)
		return (safe_malloc(size, &head));
	else if (mode == FREE)
		safe_free(ptr, &head);
	else if (mode == FREEALL)
		free_all_malloc(&head);
	return (NULL);
}
