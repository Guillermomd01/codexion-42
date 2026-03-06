/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gumunoz <gumunoz@student.42madrid.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 10:12:43 by gumunoz           #+#    #+#             */
/*   Updated: 2026/03/06 10:12:45 by gumunoz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_coder	*heap_compare(t_coder *a, t_coder *b, int mode)
{
	long	da;
	long	db;

	if (mode == 1)
	{
		da = a->last_compile + a->data->t_burnout;
		db = b->last_compile + b->data->t_burnout;
		if (da != db)
		{
			if (da < db)
				return (a);
			return (b);
		}
	}
	if (a->request_time != b->request_time)
	{
		if (a->request_time < b->request_time)
			return (a);
		return (b);
	}
	if (a->id < b->id)
		return (a);
	return (b);
}

void	bubble_up(t_dongle *dongle, int index)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (heap_compare(dongle->heap[index], dongle->heap[parent],
				(int)dongle->data->scheduler) == dongle->heap[index])
		{
			swap(&dongle->heap[index], &dongle->heap[parent]);
			index = parent;
		}
		else
			break ;
	}
}

void	bubble_down(t_dongle *dongle, int index)
{
	int	top;
	int	left;
	int	right;

	while (1)
	{
		left = 2 * index + 1;
		right = 2 * index + 2;
		top = index;
		if (left < dongle->heap_len && heap_compare(dongle->heap[left],
				dongle->heap[top],
				(int)dongle->data->scheduler) == dongle->heap[left])
			top = left;
		if (right < dongle->heap_len && heap_compare(dongle->heap[right],
				dongle->heap[top],
				(int)dongle->data->scheduler) == dongle->heap[right])
			top = right;
		if (top == index)
			break ;
		swap(&dongle->heap[index], &dongle->heap[top]);
		index = top;
	}
}

void	heap_push(t_dongle *dongle, t_coder *coder)
{
	dongle->heap[dongle->heap_len] = coder;
	bubble_up(dongle, dongle->heap_len);
	dongle->heap_len++;
}

t_coder	*heap_pop(t_dongle *dongle)
{
	t_coder	*top;

	if (dongle->heap_len == 0)
		return (NULL);
	top = dongle->heap[0];
	dongle->heap[0] = dongle->heap[dongle->heap_len - 1];
	dongle->heap_len--;
	if (dongle->heap_len > 0)
		bubble_down(dongle, 0);
	return (top);
}
