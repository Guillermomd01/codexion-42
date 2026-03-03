#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>


typedef struct s_data t_data;
typedef struct s_coder t_coder;
typedef struct s_dongle t_dongle;


struct s_data
{
    long            n_coders;
    long            t_burnout;
    long            t_compile;
    long            t_debug;
    long            t_refactor;
    long            n_compiles;
    long            t_cooldown;
    int             scheduler;
    long            start;
    t_coder         *coder;
    t_dongle        *dongle;
    pthread_mutex_t write_lock;
    pthread_mutex_t lock;
    int             sim_error;
    int             sim_finished;
    pthread_t       monitor;
};

struct s_coder
{
    int             id;
    pthread_t       thread;
    t_data          *data;
    t_dongle        *rd;
    t_dongle        *ld;
    long            request_time;
    long            last_compile;
    long            n_compiles;
    pthread_mutex_t lock;
    int             dongle_flag;
    t_coder         *next_coder;
};

struct s_dongle
{
    int             id;
    pthread_mutex_t coder_lock;
    int             available;
    t_data          *data;
    t_coder         **heap;
    int             heap_len;
    pthread_cond_t  cond;
    pthread_mutex_t lock;
    long            next_time_available;
};

/*PARSING*/
int     parsing(t_data *data, int ac, char **argv);
long    ft_atol(char *str);
int check_data(t_data *data);

 /*init coders*/
 void    destroy_coders(t_data *data, int n);
 void    link_dongles(t_data *data, int i);
 int    init_coder(t_data *data);

//init dongles
void    destroy_dongles(t_data *data, int n);
int     dongle_parameters(t_dongle *dongle);
int init_dongles(t_data *data);

//init data
int	destroy_data(t_data *data, int code);
int	init_data(t_data *data);
int	init_codexion(t_data *data);

//heap.c
t_coder *heap_compare(t_coder *a, t_coder *b, int mode);
void bubble_up(t_dongle *dongle, int index);
void bubble_down(t_dongle *dongle, int index);
void heap_push(t_dongle *dongle, t_coder *coder);
t_coder *heap_pop(t_dongle *dongle);

//utils.c
void swap(t_coder **x, t_coder **y);
long long	get_time_in_ms(void);
void	ft_usleep(size_t milliseconds, t_data *data);


//dongle_manager
void	request_dongle(t_dongle *dongle, t_coder *coder);
void	release_dongle(t_dongle *dongle, t_coder *coder);
void	take_dongles(t_coder *coder);
void	release_dongles(t_coder *coder);

//monitor
void	*monitor_routine(void *arg);

//routine
int is_finished(t_coder *coder);
void    coder_routine(t_coder *coder);
void    refactoring_step(t_coder *coder);
void    compiling_step(t_coder *coder);
void    debuggin_step(t_coder *coder);

//main
int	main(int argc, char **argv);


#endif
