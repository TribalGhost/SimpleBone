#include "d_linux_basic.h"
#define BUILD_D_LINUX
#include "d_gamedata.c"
#include "d_gamefunction.c"

extern GAME_UNLOAD(game_unload)
{
    free(run_time_memory.start_memory);
    free(frame_time_memory.start_memory);
    free(arena_memory.start_memory);
    end_connection();
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
        frame_time_memory.current_memory = frame_time_memory.start_memory;
        server_update();
        //printf("update : %d \n" , update_count);
    }
    
    double update_time = time_stamp() - current_time;
    double sleep_time = (1.0 / 60.0) - update_time;
    usleep(sleep_time * 1000.0 * 1000.0);
    //printf("sleep : %f <-- %f\n" , sleep_time , update_time);
}

extern GAME_LOAD(game_load)
{
    run_time_memory.size = 1024 * 1024 * 8;
    frame_time_memory.size = 1024 * 1024 * 8;
    arena_memory.size = 1024 * 1024 * 8;
    
    run_time_memory.start_memory = malloc(run_time_memory.size);
    frame_time_memory.start_memory = malloc(frame_time_memory.size);
    arena_memory.start_memory = malloc(arena_memory.size);
    
    run_time_memory.current_memory = run_time_memory.start_memory;
    frame_time_memory.current_memory = frame_time_memory.start_memory;
    arena_memory.current_memory = arena_memory.start_memory;
    
    app_data = app_data_in;
    app_data->game_unload_function = game_unload;
    app_data->game_loop_function = game_loop;
    
    //TODO: get application path
    app_data->application_path = allocate_temp(char , 128 );
    
    readlink("/proc/self/exe", app_data->application_path , 128);
    
    int exec_start = 0;
    for(int char_index = 0; app_data->application_path[char_index]; char_index++ ) if(app_data->application_path[char_index] == '/') exec_start = char_index;
    exec_start++;
    for(int char_index = exec_start; app_data->application_path[char_index]; char_index++) app_data->application_path[char_index] = 0;
    
    net_state = (NetState){};
    net_state.server_socket = -1;
    net_state.is_client = false;
    net_state.is_server = true;
    
    player_array = allocate_array(16 , AT_temp);
    allocate_buffer(&player_buffer , Player , 16 , AT_temp);
    
    if(!load_map()) CATCH;
    
    start_connection();
}
