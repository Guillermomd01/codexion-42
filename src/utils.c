

# include "codexion.h"
void swap(t_coder **x, t_coder **y)
{
    t_coder *temp;

    temp = *x;
    *x = *y;
    *y = temp;
}

long long	get_time_in_ms(void)
{
	struct timeval	tv;
	if (gettimeofday(&tv, NULL) == -1)
		return (0);
	
	return ((long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000);
}
void	ft_usleep(size_t milliseconds, t_data *data)
{
	size_t	start;

	start = get_time_in_ms();
	while ((get_time_in_ms() - start) < milliseconds)
	{
		pthread_mutex_lock(&data->lock);
		if (data->sim_finished)
		{
			pthread_mutex_unlock(&data->lock);
			break ;
		}
		pthread_mutex_unlock(&data->lock);
		usleep(100);
	}
}