



#include "codexion.h"

int	destroy_data(t_data *data, int code)
{
	if (code > 0)
	{
		pthread_mutex_destroy(&data->lock);
		pthread_mutex_destroy(&data->write_lock);
	}
	if (code > 1)
		destroy_dongles(data, data->n_coders);
	if (code > 2)
		destroy_coders(data, data->n_coders);
	return (1);
}

int	init_data(t_data *data)
{
	data->start = 0;
	data->sim_error = 0;
	data->sim_finished = 0;
	if (pthread_mutex_init(&data->lock, NULL))
		return (1);
	if (pthread_mutex_init(&data->write_lock, NULL))
	{
		pthread_mutex_destroy(&data->lock);
		return (1);
	}
	return (0);
}

int	init_codexion(t_data *data)
{
	if (init_data(data))
		return (1);
	if (init_dongles(data))
		return (destroy_data(data, 1));
	if (init_coder(data))
		return (destroy_data(data, 2));
	return (0);
}
