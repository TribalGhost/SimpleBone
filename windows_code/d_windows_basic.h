#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include "float.h"
#include "timeapi.h"
#include "direct.h"

#define GLAD_API_CALL_EXPORT

//linking gl function, i don't even know what gl version i'm using here lol
//i guess it was 3 or higher? i think i called some gl 4.0 function
//but it was compiled by raylib.lib so i can't tell
#include "include/glad.h" 
#include "include/glfw3.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "include/glfw3native.h"

#include <wchar.h> //for chinese characters
#include <stdio.h> //just file io and printing stuff

#include "include/raylib.h"
#include "include/rlgl.h"
#define RAYMATH_STATIC_INLINE
#include "raymath.h"

#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#include "include/stb_rect_pack.h" //packing texture into texture atlas
#include "include/stb_truetype.h" //handling text

#include "errno.h"//this doesn't work
#include "winsock2.h"
#include "ws2tcpip.h"

typedef ADDRINFOA addrinfo;
#define get_socket_error() WSAGetLastError()

#define CATCH ((*(int *)0)=1)

#define internal static
#define local_persist static
#define global static

#define D_UPDATE_RATE 60

#define DeltaTime 1.0f/D_UPDATE_RATE

//return an array of string
#define enum_to_string(Enum) Enum##_String

global int CurrentGLError = 0;
#define GL_CATCH if((CurrentGLError =glGetError() )!= GL_NO_ERROR) CATCH

//it was mean to have multiple windows
//but multiple viewport is easy to write
typedef enum AppWindow AppWindow;
enum AppWindow
{
	main_window,
	window_count,
};

global GLFWwindow* all_windows[window_count] = {};
global bool foucsed_windows[window_count] = {};
global bool is_window_focused = false;
global int current_window_index = 0;

typedef struct D_GlyphInfo D_GlyphInfo;
struct D_GlyphInfo
{
	int value;
	int offsetX;
	int offsetY;
	int advanceX;
	R_Rectangle glyph_rect;
};

#define GLYPH_INFO_LEGTH 2048
typedef struct D_GlyphInfoNode D_GlyphInfoNode;
struct D_GlyphInfoNode
{
	D_GlyphInfo info;
	D_GlyphInfoNode* next_node;
};

#define GLYPH_INFO_HASHMAP_LENGTH 128
#define STBRP_NODE_SIZE 256
typedef struct D_FontContext D_FontContext;
struct D_FontContext
{
	stbtt_fontinfo font_info;
	Image glyph_atlas_image;
	int glyph_atlas_padding;
	int glyph_resolution;
    
	D_GlyphInfoNode * all_glyph_info;
	int all_glyph_info_count;
    
	D_GlyphInfoNode** glyph_hash_map;
    
	stbrp_context context;
	//is this how the packing data store?
	stbrp_node *nodes;
	Texture2D glyph_atlas_texture;
};

typedef struct D_App_Data D_App_Data;

#define GAME_LOAD(Name) void Name(D_App_Data * _AppData)
typedef GAME_LOAD((GameLoadFunction));

#define GAME_LOOP(Name) void Name()
typedef GAME_LOOP((GameLoopFunction));

#define GAME_UNLOAD(Name) void Name()
typedef GAME_UNLOAD((GameUnloadFunction));

struct D_App_Data
{
    int loop_count;
    
	GameUnloadFunction * game_unload;
	GameLoopFunction * game_loop;
    
	double loop_time;
    
	double top;
	double right;
    
	Vector2 mouse_position;
    
	D_FontContext default_font;
    
	int codepoint_queue_indedx;
	int codepoint_queue_count;
	unsigned int codepoint_queue[64];
    
	GLFWwindow* current_window;
	GLFWwindow* current_focus_window;
	Vector2 window_size;
    
	bool exit_window;
    
	//i forgot putting this in appdata but it work fine before i switch away from visual studio
	//wtf?
	bool update_atlas;
	float mouse_scroll_delta;
    
	float window_text_size;
    
#define MAX_FILE_PATH 1024
    
	char application_path[MAX_FILE_PATH];
    char * host_name;
    char * user_name;
    
    HDC _HDC;
    
    bool is_server;
    bool is_client;
};

global D_App_Data* app_data = 0;

internal void char_callback(GLFWwindow* window, unsigned int codepoint)
{
	app_data->codepoint_queue[app_data->codepoint_queue_count] = codepoint;
	app_data->codepoint_queue_count++;
}

internal unsigned int char_pressed()
{
	if (app_data->codepoint_queue_indedx == app_data->codepoint_queue_count)
		return 0;
    
	unsigned int result = app_data->codepoint_queue[app_data->codepoint_queue_indedx];
	app_data->codepoint_queue_indedx++;
	return result;
    
}

internal void mosue_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	app_data->mouse_scroll_delta += yoffset;
}

internal void focus_callback(GLFWwindow* window, int focused)
{
	if (focused == GLFW_TRUE)
	{
		app_data->current_focus_window = window;
	}
    
	for (int i = 0; i < window_count; i++)
	{
		if ( all_windows[i] == window)
		{
			foucsed_windows[i] = focused;
			break;
		}
	}
}

internal void refresh_callback(GLFWwindow* window)
{
	//TODO: fix real time windows resizing
	//not sure what happening here but i wanna clean up the "windows focus" things
	//before i work on this
}