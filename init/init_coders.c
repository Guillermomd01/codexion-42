



#include "codexion.h"

void	destroy_coders(t_data *data, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&data->coder[i].lock);
		i++;
	}
	free(data->coder);
}

void	link_dongles(t_data *data, int i)
{
	if (i % 2 == 0)
	{
		data->coder[i].rd = &data->dongle[i];
		data->coder[i].ld = &data->dongle[
			(i + data->n_coders - 1) % data->n_coders
		];
	}
	else
	{
		data->coder[i].rd = &data->dongle[i - 1];
		data->coder[i].ld = &data->dongle[i];
	}
}

int	init_coder(t_data *data)
{
	long	i;

	data->coder = malloc(data->n_coders * sizeof(t_coder));
	if (!data->coder)
		return (1);
	i = 0;
	while (i < data->n_coders)
	{
		data->coder[i].id = i;
		data->coder[i].last_compile = 0;
		data->coder[i].n_compiles = 0;
		data->coder[i].dongle_flag = 0;
		data->coder[i].next_coder = NULL;
		data->coder[i].data = data;
		link_dongles(data, i);
		if (pthread_mutex_init(&data->coder[i].lock, NULL))
		{
			destroy_coders(data, i);
			return (1);
		}
		i++;
	}
	return (0);
}