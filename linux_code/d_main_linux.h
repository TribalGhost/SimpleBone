#define internal static
#define local_persist static
#define global static
#define CATCH ((*(int *)0)=1)

typedef struct AppData AppData;

#define GAME_LOAD(name) void name(AppData * app_data_in)
typedef GAME_LOAD(GameLoadFunction);

#define GAME_UNLOAD(name) void (name)()
typedef GAME_UNLOAD(GameUnloadFunction);

#define GAME_LOOP(name) void (name)()
typedef GAME_LOOP(GameLoopFunction);

struct AppData
{
    bool running;
    GameUnloadFunction * game_unload_function;
    GameLoopFunction * game_loop_function;
};

global AppData * app_data = 0;
