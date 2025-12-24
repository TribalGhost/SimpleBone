#include "d_linux_basic.h"

global AppData global_app_data = {};

internal bool compare_time(struct timespec time_a ,struct  timespec time_b)
{
    if(time_a.tv_sec != time_b.tv_sec) return false;
    
    //something wacky happening here
    //the nano second changed twice
    //if(time_a.tv_nsec != time_b.tv_nsec) return false;
    
    return true;
}

extern int main()
{
    app_data = &global_app_data;
    app_data->host_name = "192.168.10.8";
    
    const char * dll_path = "DGAME";
    const char * build_finish = "build_finish";
    
    void * game_dll = dlopen(dll_path , RTLD_LAZY);
    if(!game_dll) CATCH;
    
    GameLoadFunction * game_load = (GameLoadFunction *)dlsym(game_dll, "game_load");
    game_load(app_data);
    
    struct stat previous_file_attribute = {};
    stat(dll_path , &previous_file_attribute);
    
    remove(build_finish);
    
    for(;;)
    {
        struct stat current_file_attribute = {};
        
        if(access(dll_path, F_OK) == 0)
        {
            stat(dll_path , &current_file_attribute);
            //TODO:hear me out
            //why don't i put a stomp file to indicate new dll is made
            if(access("build_finish", F_OK) == 0)
            {
                printf("load new dll\n");
                previous_file_attribute = current_file_attribute;
                app_data->game_unload_function();
                
                app_data->game_unload_function = 0;
                app_data->game_loop_function = 0;
                
                dlclose(game_dll);
                game_dll = 0;
                game_dll = dlopen(dll_path , RTLD_LAZY);
                if(!game_dll) CATCH;
                game_load = (GameLoadFunction *)dlsym(game_dll, "game_load");
                game_load(app_data);
                
                remove(build_finish);
            }
            
            app_data->game_loop_function();
        }
    }
    
    app_data->game_unload_function();
}