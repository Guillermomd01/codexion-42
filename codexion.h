#ifndef CODEXION_H
# define CODEXION_H


typedef struct s_data t_data;



struct s_data
{
    long    n_coders;
    long    t_burnout;
    long    T_compile;
    long    t_debug;
    long    t_refactor;
    long    n_compiles;
    long    t_cooldown;
    int     scheduler;
};



#endif
