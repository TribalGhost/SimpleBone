#include "d_header.h"
#include "d_main.h"
//#include "d_main_meta_generated.cpp" // i can't figure out how to make offset an actual number


#include "d_function.c"

//#include "ui_editor_function_and_data.cpp"
//#include "ui_editor.cpp"

global D_App_Data _GlobalData = {};

internal GAME_LOOP(GameLoopStub) {}

internal void APIENTRY DebugCallback(
                                     GLenum source, GLenum type, GLuint id, GLenum severity,
                                     GLsizei length, const GLchar* message, const void* user)
{
    
    if(type != GL_DEBUG_TYPE_PERFORMANCE)
    {
        
        if (severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM)
        {
            printf(message);
            printf("\n");
            
            if (IsDebuggerPresent())
            {
                
                printf("OpenGL error - check the callstack in debugger");
                CATCH;
            }
            
            printf("OpenGL API usage error! Use debugger to examine call stack!");
            CATCH;
            
        }
        
    }
    
}


int main()
{
	timeBeginPeriod(1);
    
	app_data = &_GlobalData;
    
	app_data->window_size = (Vector2){ 1280,720 };
    
	sprintf(app_data->aplication_path ,GetApplicationDirectory());
    
	glfwInit();
    
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    //glfwWindowHint(GLFW_SAMPLES,4);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DEPTH_BITS, GL_TRUE);
    
    GLFWwindow * _MainWindow = glfwCreateWindow(app_data->window_size.x, app_data->window_size.y, "BulletTime",0, 0);
    
	set_window(_MainWindow, main_window);
    
	glfwMakeContextCurrent(all_windows[main_window]);
	
	gladLoadGL(glfwGetProcAddress);
    
    glEnable(GL_MULTISAMPLE);
    //glDisable(GL_MULTISAMPLE);
    
    GL_CATCH;
    
    //useless
	//TODO : clean it
	app_data->current_focus_window = all_windows[main_window];
	foucsed_windows[main_window] = true;
    
	glfwMakeContextCurrent(all_windows[main_window]);
    
	for (int WindowIndex = 0; WindowIndex < window_count; WindowIndex++)
	{
		GLFWwindow* WindowToCallback = all_windows[WindowIndex];
		glfwSetScrollCallback(WindowToCallback, mosue_scroll_callback);
		glfwSetCharCallback(WindowToCallback, char_callback);
		glfwSetKeyCallback(WindowToCallback, key_callback);
		glfwSetMouseButtonCallback(WindowToCallback, MouseCallback);
		glfwSetWindowFocusCallback(WindowToCallback, focus_callback);
		glfwSetWindowRefreshCallback(WindowToCallback, refresh_callback);
	}
    
	int FontDataSize = 0;
    
	app_data->default_font.glyph_resolution = 64;
	app_data->default_font.all_glyph_info = AllocateMemory(D_GlyphInfoNode, GLYPH_INFO_LEGTH);
	app_data->default_font.glyph_hash_map = AllocateMemory(D_GlyphInfoNode*, GLYPH_INFO_HASHMAP_LENGTH);
	app_data->default_font.nodes= AllocateMemory(stbrp_node, STBRP_NODE_SIZE);
    
	app_data->default_font.glyph_atlas_image.width = 2048;
	app_data->default_font.glyph_atlas_image.height = 2048;
	app_data->default_font.glyph_atlas_image.mipmaps = 1;
	app_data->default_font.glyph_atlas_image.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;
	app_data->default_font.glyph_atlas_image.data = RL_CALLOC(1, app_data->default_font.glyph_atlas_image.width * app_data->default_font.glyph_atlas_image.height);
    
	char FontDirectory[256] = {};
	char* WorkDirectory = (char*)GetApplicationDirectory();
	strcpy(FontDirectory, WorkDirectory);
	strcat(FontDirectory, "Asset\\DefaultFont.ttf");
	unsigned char* FontData = LoadFileData(FontDirectory, &FontDataSize);
    
	app_data->default_font.glyph_atlas_padding = 1;
    
	stbtt_InitFont(&app_data->default_font.font_info, FontData, 0);
    
	stbrp_init_target(
                      &app_data->default_font.context,
                      app_data->default_font.glyph_atlas_image.width,
                      app_data->default_font.glyph_atlas_image.height,
                      app_data->default_font.nodes,
                      STBRP_NODE_SIZE);
    
	GL_CATCH;
    
	app_data->_HDC = GetDC(glfwGetWin32Window(glfwGetCurrentContext()));
    
    glDebugMessageCallback(&DebugCallback, NULL);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    
	HMODULE GameModule = 0;
	long ModuleModTime = 0;
    
	GameLoadFunction * GameLoad = 0;
    
	char DLLPath[MAX_FILE_PATH] = {};
	const char* DLLName = "DGame.dll";
    
	char CopyDLLPath[MAX_FILE_PATH] = {};
	const char* CopyDLLName = "C_DGame.dll";
	const char* LoadGameName = "game_load";
    
	combine_file_path(DLLName, DLLPath);
	combine_file_path(CopyDLLName, CopyDLLPath);
    
	if (FileExists(DLLPath))
	{
		ModuleModTime = GetFileModTime(DLLPath);
		if (!CopyFileA(DLLPath, CopyDLLPath, false))
			CATCH;
		GameModule = LoadLibraryA(CopyDLLPath);
	}
    
	if (GameModule)
	{
		GameLoad = (GameLoadFunction * )GetProcAddress(GameModule, LoadGameName);
		GameLoad(app_data);
	}
    
    
	app_data->loop_time = time_stamp();
    
	GL_CATCH;
	while (1)
	{
		app_data->loop_count++;
        
		if (ModuleModTime != GetFileModTime(DLLPath) && !FileExists("Lock.tmp"))
		{
			ModuleModTime = GetFileModTime(DLLPath);
			
			if (app_data->game_unload)
				app_data->game_unload();
            
			if (!FreeLibrary(GameModule))
			{
				CATCH;
			}
            
			//TODO: try other debugger or force visual studio unload dll immediately
            
            //there is no other debugger that i wanna use other than VS or raddbg ):
			//just leave it be
            while (1)
			{
				if (!CopyFileA(DLLPath, CopyDLLPath, false))
				{
					DWORD CopyFailed = GetLastError();
					continue;
				}
                
				GameModule = LoadLibraryA(CopyDLLPath);
                
				if (GameModule)
				{
					GameLoad = (GameLoadFunction *)GetProcAddress(GameModule, LoadGameName);
					GameLoad(app_data);
				}
                
				break;
			}
		}
        
		if (glfwWindowShouldClose(all_windows[main_window]))
		{
            if (app_data->game_unload)
                app_data->game_unload();
			
            glfwDestroyWindow(all_windows[main_window]);
			break;
		}
        
#if 0
		if (GetWindow(EditorWindow) && glfwWindowShouldClose(GetWindow(EditorWindow)))
		{
			glfwDestroyWindow(GetWindow(EditorWindow));
			set_window(0, EditorWindow);
		}
#endif
        
		if (app_data->exit_window)
			break;
        
		bool NoWindowFocus = true;
        
		for (int i = 0; i < window_count; i++)
		{
			if (foucsed_windows[i])
				NoWindowFocus = false;
		}
        
		app_data->mouse_scroll_delta = 0;
		app_data->codepoint_queue_count = 0;
		app_data->codepoint_queue_indedx = 0;
		app_data->pressed_mouse_array_count = 0;
		app_data->pressed_key_array_count = 0;
        app_data->released_key_array_count = 0;
        
		app_data->released_mouse_array_count = 0;
        
		for (int WindowIndex = 0; WindowIndex < window_count; WindowIndex++)
		{
			
            app_data->current_window = all_windows[WindowIndex];
			current_window_index = WindowIndex;
			glfwMakeContextCurrent(app_data->current_window);
            
			if (!app_data->current_window)
				continue;
            
			int WindowSizeWidth = 0;
			int WindowSizeHeight = 0;
			glfwGetWindowSize(app_data->current_window, &WindowSizeWidth, &WindowSizeHeight);
            
			if (WindowIndex == main_window)
			{
				if (app_data->window_size.x != WindowSizeWidth || app_data->window_size.y != WindowSizeHeight)
				{
					app_data->window_size.x = WindowSizeWidth;
					app_data->window_size.y = WindowSizeHeight;
				}
			}
            
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            
			app_data->window_text_size = mode->height / ((float)app_data->window_size.y);
			GL_CATCH;
            
			if (current_window_index == main_window)
			{
				app_data->game_loop();
			}
		}
        
	}
}