




#include "codexion.h"

int	is_finished(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->lock);
	if (coder->data->sim_finished)
	{
		pthread_mutex_unlock(&coder->data->lock);
		return (0);
	}
	pthread_mutex_unlock(&coder->data->lock);
	return (1);
}

void	coder_routine(t_coder *coder)
{
	pthread_mutex_lock(&coder->lock);
	coder->last_compile = coder->data->start;
	pthread_mutex_unlock(&coder->lock);
	if (coder->id % 2 != 0)
		ft_usleep(50, coder->data);
	while (is_finished(coder))
	{
		pthread_mutex_lock(&coder->lock);
		if (coder->n_compiles == coder->data->n_compiles)
		{
			pthread_mutex_unlock(&coder->lock);
			break ;
		}
		pthread_mutex_unlock(&coder->lock);
		compiling_step(coder);
		if (!is_finished(coder))
			break ;
		debuggin_step(coder);
		if (!is_finished(coder))
			break ;
		refactoring_step(coder);
		if (!is_finished(coder))
			break ;
	}
}

void	refactoring_step(t_coder *coder)
{
	long	now;

	if (!is_finished(coder))
		return ;
	now = get_time_in_ms() - coder->data->start;
	pthread_mutex_lock(&coder->data->write_lock);
	printf("%ld %i is refactoring\n", now, coder->id + 1);
	pthread_mutex_unlock(&coder->data->write_lock);
	ft_usleep(coder->data->t_refactor, coder->data);
}

void	compiling_step(t_coder *coder)
{
	long	now;

	take_dongles(coder);
	if (!is_finished(coder))
	{
		release_dongles(coder);
		return ;
	}
	now = get_time_in_ms() - coder->data->start;
	pthread_mutex_lock(&coder->data->write_lock);
	printf("%ld %i is compiling\n", now, coder->id + 1);
	pthread_mutex_unlock(&coder->data->write_lock);
	pthread_mutex_lock(&coder->lock);
	coder->last_compile = get_time_in_ms();
	pthread_mutex_unlock(&coder->lock);
	ft_usleep(coder->data->t_compile, coder->data);
	pthread_mutex_lock(&coder->lock);
	coder->n_compiles++;
	pthread_mutex_unlock(&coder->lock);
	release_dongles(coder);
}

void	debuggin_step(t_coder *coder)
{
	long	now;

	now = get_time_in_ms() - coder->data->start;
	if (!is_finished(coder))
		return ;
	pthread_mutex_lock(&coder->data->write_lock);
	printf("%ld %i is debugging\n", now, coder->id + 1);
	pthread_mutex_unlock(&coder->data->write_lock);
	ft_usleep(coder->data->t_debug, coder->data);
}
