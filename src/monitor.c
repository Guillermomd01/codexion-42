#include "codexion.h"


static void	stop_simulation(t_data *data)
{
	int	j;

	pthread_mutex_lock(&data->lock);
	data->sim_finished = 1;
	pthread_mutex_unlock(&data->lock);
	j = -1;
	while (++j < data->n_coders)
	{
		pthread_mutex_lock(&data->dongle[j].lock);
		pthread_cond_broadcast(&data->dongle[j].cond);
		pthread_mutex_unlock(&data->dongle[j].lock);
	}
}

static int	is_dead(t_coder *coder, t_data *data)
{
	pthread_mutex_lock(&coder->lock);
	if (get_time_in_ms() - coder->last_compile > data->t_burnout)
	{
		pthread_mutex_lock(&data->write_lock);
		printf("%lld %d burned out\n",
			get_time_in_ms() - data->start, coder->id + 1);
		pthread_mutex_unlock(&data->write_lock);
		stop_simulation(data);
		pthread_mutex_unlock(&coder->lock);
		return (1);
	}
	pthread_mutex_unlock(&coder->lock);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;
	int		all_done;

	data = (t_data *)arg;
	while (1)
	{
		all_done = 1;
		i = -1;
		while (++i < data->n_coders)
		{
			if (is_dead(&data->coder[i], data))
				return (NULL);
			pthread_mutex_lock(&data->coder[i].lock);
			if (data->n_compiles != -1
				&& data->coder[i].n_compiles < data->n_compiles)
				all_done = 0;
			pthread_mutex_unlock(&data->coder[i].lock);
		}
		pthread_mutex_lock(&data->lock);
		if (data->n_compiles != -1 && all_done)
			return (data->sim_finished = 1,
				pthread_mutex_unlock(&data->lock), NULL);
		pthread_mutex_unlock(&data->lock);
		usleep(500);
	}
}
