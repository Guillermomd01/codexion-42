



#include "codexion.h"

long	ft_atol(char *str)
{
	long	result;
	int		i;

	i = 0;
	result = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+')
		i++;
	if (!str[i])
		return (-1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (result > (INT_MAX - str[i] + '0') / 10)
			return (-1);
		result = 10 * result + (str[i] - '0');
		i++;
	}
	if (str[i])
		return (-1);
	return (result);
}

int	parsing(t_data *data, int ac, char **argv)
{
	if (ac != 9)
		return (-1);
	data->n_coders = ft_atol(argv[1]);
	data->t_burnout = ft_atol(argv[2]);
	data->t_compile = ft_atol(argv[3]);
	data->t_debug = ft_atol(argv[4]);
	data->t_refactor = ft_atol(argv[5]);
	data->n_compiles = ft_atol(argv[6]);
	data->t_cooldown = ft_atol(argv[7]);
	if (check_data(data))
		return (-1);
	if (strcmp(argv[8], "edf") == 0)
		data->scheduler = 1;
	else if (strcmp(argv[8], "fifo") == 0)
		data->scheduler = 0;
	else
		return (1);
	return (0);
}

int	check_data(t_data *data)
{
	if (data->n_coders < 1)
		return (1);
	if (data->t_burnout < 0)
		return (1);
	if (data->t_compile < 0)
		return (1);
	if (data->t_debug < 0)
		return (1);
	if (data->t_refactor < 0)
		return (1);
	if (data->n_compiles < 0)
		return (1);
	if (data->t_cooldown < 0)
		return (1);
	return (0);
}
