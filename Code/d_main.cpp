#include "d_header.h"
#include "d_main.h"
#include "d_main_meta_generated.cpp" // i can't figure out how to make offset an actual number
#include "d_function.cpp"

//#include "ui_editor_function_and_data.cpp"
//#include "ui_editor.cpp"

global D_App_Data _GlobalData = {};

internal GAME_LOOP(GameLoopStub) {}

extern "C" int main()
{
	timeBeginPeriod(1);
    
	AppData = &_GlobalData;
    
	AppData->WindowSize = { 1280,720 };
    
	sprintf(AppData->ApplicationPath ,GetApplicationDirectory());
    
	glfwInit();
    
    glfwWindowHint(GLFW_SAMPLES,8);
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DEPTH_BITS, GL_TRUE);
	
	SetWindow(glfwCreateWindow(AppData->WindowSize.x, AppData->WindowSize.y, "BulletTime",0, 0), MainWindow);
        
	glfwMakeContextCurrent(GetWindow(MainWindow));
	
	gladLoadGL(glfwGetProcAddress);
    
    glEnable(GL_MULTISAMPLE);

	//glEnable( GL_LINE_SMOOTH );
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	GL_CATCH;

	//useless
	//TODO : clean it
	AppData->CurrentFoucsWindow = GetWindow(MainWindow);
	WindowsFocused[MainWindow] = true;
    
	glfwMakeContextCurrent(GetWindow(MainWindow));
    
	for (int WindowIndex = 0; WindowIndex < WindowCount; WindowIndex++)
	{
		GLFWwindow* WindowToCallback = GetWindow(WindowIndex);
		glfwSetScrollCallback(WindowToCallback, MouseScrollCallback);
		glfwSetCharCallback(WindowToCallback, CharCallback);
		glfwSetKeyCallback(WindowToCallback, KeyCallback);
		glfwSetMouseButtonCallback(WindowToCallback, MouseCallback);
		glfwSetWindowFocusCallback(WindowToCallback, FocusCallback);
		glfwSetWindowRefreshCallback(WindowToCallback, RefreshCallback);
	}
    
	AppData->GameCamera.position = { -10,0,0 };
	AppData->GameCamera.target = AppData->GameCamera.position;
	AppData->GameCamera.target.z += 1;
	AppData->GameCamera.projection = CAMERA_PERSPECTIVE;
	AppData->GameCamera.up = { 0,1,0 };
	AppData->GameCamera.fovy = 90;
    
	int FontDataSize = 0;
    
	AppData->DefaultFont.GlyphResolution = 64;
	AppData->DefaultFont.AllGlyphInfo = AllocateMemory(D_GlyphInfoNode, GLYPH_INFO_LEGTH);
	AppData->DefaultFont.GlyphHashmap = AllocateMemory(D_GlyphInfoNode*, GLYPH_INFO_HASHMAP_LENGTH);
	AppData->DefaultFont._Nodes= AllocateMemory(stbrp_node, STBRP_NODE_SIZE);
    
	AppData->DefaultFont.GlyphAtlasImage.width = 2048;
	AppData->DefaultFont.GlyphAtlasImage.height = 2048;
	AppData->DefaultFont.GlyphAtlasImage.mipmaps = 1;
	AppData->DefaultFont.GlyphAtlasImage.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;
	AppData->DefaultFont.GlyphAtlasImage.data = RL_CALLOC(1, AppData->DefaultFont.GlyphAtlasImage.width * AppData->DefaultFont.GlyphAtlasImage.height);
    
	char FontDirectory[256] = {};
	char* WorkDirectory = (char*)GetApplicationDirectory();
	strcpy(FontDirectory, WorkDirectory);
	strcat(FontDirectory, "Asset\\DefaultFont.ttf");
	unsigned char* FontData = LoadFileData(FontDirectory, &FontDataSize);
    
	AppData->DefaultFont.GlyphAltasPadding = 1;
    
	stbtt_InitFont(&AppData->DefaultFont.FontInfo, FontData, 0);
    
	stbrp_init_target(
                      &AppData->DefaultFont.STB_RP_Context,
                      AppData->DefaultFont.GlyphAtlasImage.width,
                      AppData->DefaultFont.GlyphAtlasImage.height,
                      AppData->DefaultFont._Nodes,
                      STBRP_NODE_SIZE);
    
	GL_CATCH;
    
	AppData->_HDC = GetDC(glfwGetWin32Window(glfwGetCurrentContext()));
    
	HMODULE GameModule = 0;
	long ModuleModTime = 0;
    
	GameLoadFunction * GameLoad = 0;
    
	char DLLPath[MAX_FILE_PATH] = {};
	const char* DLLName = "DGame.dll";
    
	char CopyDLLPath[MAX_FILE_PATH] = {};
	const char* CopyDLLName = "C_DGame.dll";
	const char* LoadGameName = "GameLoad";
    
	CombineFilePath(DLLName, DLLPath);
	CombineFilePath(CopyDLLName, CopyDLLPath);
    
	if (FileExists(DLLPath))
	{
		ModuleModTime = GetFileModTime(DLLPath);
		if (!CopyFileA(DLLPath, CopyDLLPath, false))
			ASSERT;
		GameModule = LoadLibraryA(CopyDLLPath);
	}
    
	if (GameModule)
	{
		GameLoad = (GameLoadFunction * )GetProcAddress(GameModule, LoadGameName);
		GameLoad(AppData);
	}
    

	AppData->LoopTime = TimeStamp();

	GL_CATCH;
	while (1)
	{
		AppData->LoopCount++;
        
		if (ModuleModTime != GetFileModTime(DLLPath) && !FileExists("Lock.tmp"))
		{
			ModuleModTime = GetFileModTime(DLLPath);
			
			if (AppData->_GameUnload)
				AppData->_GameUnload();
            
			if (!FreeLibrary(GameModule))
			{
				ASSERT;
			}
            
			//TODO: try other debugger or force visual studio unload dll immediately
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
					GameLoad(AppData);
				}
                
				break;
			}
		}
        
		if (glfwWindowShouldClose(GetWindow(MainWindow)))
		{
            if (AppData->_GameUnload)
                AppData->_GameUnload();
			
            glfwDestroyWindow(GetWindow(MainWindow));
			break;
		}
        
#if 0
		if (GetWindow(EditorWindow) && glfwWindowShouldClose(GetWindow(EditorWindow)))
		{
			glfwDestroyWindow(GetWindow(EditorWindow));
			SetWindow(0, EditorWindow);
		}
#endif
        
		if (AppData->ExitWindow)
			break;
        
		bool NoWindowFocus = true;
        
		for (int i = 0; i < WindowCount; i++)
		{
			if (WindowsFocused[i])
				NoWindowFocus = false;
		}
        
		AppData->MouseScrollDelta = 0;
		AppData->CodepointQueueCount = 0;
		AppData->CodepointQueueIndex = 0;
		AppData->PressedMouseArrayCount = 0;
		AppData->PressedKeyArrayCount = 0;
        AppData->ReleasedKeyArrayCount = 0;
        
		AppData->ReleasedMouseArrayCount = 0;
        
		for (int WindowIndex = 0; WindowIndex < WindowCount; WindowIndex++)
		{
			AppData->CurrentWindow = GetWindow(WindowIndex);
			CurrentWindowIndex = WindowIndex;
			glfwMakeContextCurrent(AppData->CurrentWindow);
            
			if (!AppData->CurrentWindow)
				continue;
            
			int WindowSizeWidth = 0;
			int WindowSizeHeight = 0;
			glfwGetWindowSize(AppData->CurrentWindow, &WindowSizeWidth, &WindowSizeHeight);
            
			if (WindowIndex == MainWindow)
			{
				if (AppData->WindowSize.x != WindowSizeWidth || AppData->WindowSize.y != WindowSizeHeight)
				{
					AppData->WindowSize.x = WindowSizeWidth;
					AppData->WindowSize.y = WindowSizeHeight;
					glViewport(0, 0, WindowSizeWidth, WindowSizeHeight);
				}
			}
            
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            
			AppData->WindowTextSize = mode->height / ((float)AppData->WindowSize.y);
			GL_CATCH;
            
			if (CurrentWindowIndex == MainWindow)
			{
				AppData->_GameLoop();
			}
		}

	}
}