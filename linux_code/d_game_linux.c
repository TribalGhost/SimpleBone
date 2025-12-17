#include "stdio.h"
#include "stdbool.h"
#include "unistd.h"
#include "errno.h"
#include "time.h"

#include "d_main_linux.h"

#define BUILD_LINUX_NET
#include "d_net.c"

internal double time_stamp()
{
    struct timespec now = {};
    clock_gettime(CLOCK_MONOTONIC_RAW , &now);
    double nano_second = now.tv_nsec;
    nano_second /= 1000000000.0;
    return ((double)now.tv_sec) + nano_second;
}

extern GAME_UNLOAD(game_unload)
{
    end_connection();
}

internal void game_update()
{
    
}

#define DELTA_TIME ((double)(1.0 / 60.0)) 

global int update_count = 0;

extern GAME_LOOP(game_loop)
{
    local_persist double elapsed_time = 0;
    
    local_persist double previous_time = 0;
    double current_time = time_stamp();
    elapsed_time += current_time - previous_time;
    previous_time = current_time;
    
    if(elapsed_time > DELTA_TIME * 10.0) elapsed_time = 0;
    for(;elapsed_time > DELTA_TIME; elapsed_time -= DELTA_TIME)
    {
        update_count++;
        server_update();
        game_update();
        //printf("update : %d \n" , update_count);
    }
    
    double update_time = time_stamp() - current_time;
    double sleep_time = (1.0 / 60.0) - update_time;
    usleep(sleep_time * 1000.0 * 1000.0);
    //printf("sleep : %f <-- %f\n" , sleep_time , update_time);
}

extern GAME_LOAD(game_load)
{
    app_data = app_data_in;
    app_data->game_unload_function = game_unload;
    app_data->game_loop_function = game_loop;
    
    net_state = (NetState){};
    net_state.server_socket = -1;
    net_state.is_client = false;
    net_state.is_server = true;
    
    start_connection();
}
