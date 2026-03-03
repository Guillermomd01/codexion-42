



#include "codexion.h"

void    destroy_dongles(t_data *data, int n)
{
    int i;
    i = 0;
    while (i < n)
    {
        pthread_mutex_destroy(&data->dongle[i].lock);
        pthread_cond_destroy(&data->dongle[i].cond);
        if (data->dongle[i].heap)
            free(data->dongle[i].heap);
        i++;
    }
    free(data->dongle);
}
int dongle_parameters(t_dongle *dongle)
{
    dongle->available = 1;
    dongle->next_time_available = 0;
    dongle->heap = malloc(sizeof(t_coder *) * dongle->data->n_coders); 
    if (!dongle->heap)
        return (1);
    dongle->heap_len = 0;
    pthread_mutex_init(&dongle->coder_lock, NULL);
    if (pthread_mutex_init(&dongle->lock, NULL))
        return (1);
    if (pthread_cond_init(&dongle->cond, NULL))
    {
        pthread_mutex_destroy(&dongle->lock);
        return (1);
    }
    return (0);
}

int init_dongles(t_data *data)
{
    int i;

    data->dongle = malloc(data->n_coders * sizeof(t_dongle));
    if (!data->dongle)
        return (1);
    i = 0;
    while (i < data->n_coders)
    {
        data->dongle[i].id = i;
        data->dongle[i].data = data;
        if (dongle_parameters(&data->dongle[i]))
        {
            destroy_dongles(data, i);
            return (1);
        }
        i++;
    }
    return (0);
}
