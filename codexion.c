




#include "codexion.h"

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	monitor;
	int			i;

	if (parsing(&data, argc, argv) != 0)
	{
		printf("Use: ./codexion n_coders t_burnout");
		printf("t_compile t_debug t_refactor");
		printf("n_compiles t_cooldown scheduler\n");
		return (1);
	}
	if (init_codexion(&data) != 0)
		return (1);
	data.start = get_time_in_ms();
	i = -1;
	while (++i < data.n_coders)
		pthread_create(&data.coder[i].thread, NULL,
			(void *)coder_routine, &data.coder[i]);
	pthread_create(&monitor, NULL, monitor_routine, &data);
	i = -1;
	while (++i < data.n_coders)
		pthread_join(data.coder[i].thread, NULL);
	pthread_join(monitor, NULL);
	destroy_data(&data, 3);
	return (0);
}
