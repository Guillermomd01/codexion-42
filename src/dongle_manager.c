/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_manager.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gumunoz <gumunoz@student.42madrid.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 10:12:25 by gumunoz           #+#    #+#             */
/*   Updated: 2026/03/06 10:12:27 by gumunoz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	can_take(t_dongle *dongle, t_coder *coder)
{
	if (dongle->heap_len > 0 && dongle->heap[0] == coder && dongle->available)
		return (1);
	return (0);
}

void	request_dongle(t_dongle *dongle, t_coder *coder)
{
	long long	now;

	pthread_mutex_lock(&dongle->lock);
	coder->request_time = get_time_in_ms();
	heap_push(dongle, coder);
	while (!can_take(dongle, coder) && is_finished(coder))
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	if (is_finished(coder))
	{
		now = get_time_in_ms();
		if (now < dongle->next_time_available)
			ft_usleep(dongle->next_time_available - now, coder->data);
		heap_pop(dongle);
		dongle->available = 0;
		pthread_mutex_lock(&coder->data->write_lock);
		printf("%lld %d has taken a dongle\n",
			get_time_in_ms() - coder->data->start, coder->id + 1);
		pthread_mutex_unlock(&coder->data->write_lock);
	}
	else
		heap_pop(dongle);
	pthread_mutex_unlock(&dongle->lock);
}

// 3. Logic for releasing a dongle and activating the cooldown
void	release_dongle(t_dongle *dongle, t_coder *coder)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->available = 1;
	dongle->next_time_available = get_time_in_ms() + coder->data->t_cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
}

// 4. Take both dongles Ordered to avoid Deadlock
void	take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->ld == coder->rd)
	{
		request_dongle(coder->ld, coder);
		while (is_finished(coder))
			usleep(1000);
		return ;
	}
	if (coder->ld->id < coder->rd->id)
	{
		first = coder->ld;
		second = coder->rd;
	}
	else
	{
		first = coder->rd;
		second = coder->ld;
	}
	request_dongle(first, coder);
	request_dongle(second, coder);
}

// 5. Free both dongles
void	release_dongles(t_coder *coder)
{
	release_dongle(coder->ld, coder);
	release_dongle(coder->rd, coder);
}
