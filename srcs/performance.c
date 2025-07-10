#include "../includes/cub3d.h"

double get_time(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec + tv.tv_usec / 1000000.0);
}

void init_timing(t_game *game)
{
    
}