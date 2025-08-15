#define ASSERT *(int *)0=1

#define internal static
#define localPersist static
#define global static

#define DISABLE_POST_PROCESS 0

global int CurrentGLError = 0;
#define GL_CATCH if((CurrentGLError =glGetError() )!= GL_NO_ERROR) ASSERT

#define D_UPDATE_RATE 60

#define DeltaTime 1.0f/D_UPDATE_RATE

struct _MemberMetaData;

struct _StructMetaData
{
	_MemberMetaData *MembersArray;
	int MemberCount;
};

struct _MemberMetaData
{
	char Name[64];
    
	bool MultiplePointer;
	bool IsPointer;
	bool IsArray;
	
    int ArraySize;
    
    int MemberType;
	char TypeName[64];
	int MemberOffset;
	int MemberSize;
};

struct _StructMetaDataNode
{
    const char* Name;
    int TypeIndex;
    int Size;
    bool Introspected;
    _StructMetaData StructMeta;
    
    _StructMetaDataNode* Next;
};

//can't remember why you exist
#define STRUCT_META_HASH_MAX 64 
global _StructMetaDataNode ** _StructMetaHash = {};

//_StructMetaData GetStructMeta(StructName)
#define GetStructMeta(Struct) {MemberMeta_##Struct,MemberMetaCount_##Struct}
#define GetMember(Struct ,MemberName) _ME_##Struct##_##MemberName
#define GetType(Name) _MT_##Name

//return an array of string
#define EnumToString(Enum) Enum##_String

//magic keyword
//turn pointer to offset of an other pointer
//now all "offset" pointer can be save on disk
global unsigned char * _StartMemory_ = 0;
#define persist __based(_StartMemory_)

global unsigned char * PersistMemoryBlock = 0;

#define HASH_DEBUG 0

struct _MemberNode
{
    _MemberMetaData * Member;
    
    int DataIndex;
    
    _MemberNode * Previous;
};

//now we can start composting ^^
struct _DecomposedBasicType
{
    char * Name;
    
    int Type;
    int Offset;
    
    bool IsPointer;
    
    int DataCount;
    
    bool Taken;
    
    int PreviousMemoryLocation;
    int CurrentMemoryLocation;
    
    _MemberNode * ReverseMemberList;
    _DecomposedBasicType * Next;
};

struct _TypeFormDifference
{
    _StructMetaData CurrentStruct;
    _StructMetaData PreviousStruct;
    
    bool CurrentIsStruct;
    bool CurrentIsEnumType;
    bool CurrentIsBasicType;
    
    bool PreviousIsStruct;
    bool PreviousIsEnumType;
    bool PreviousIsBasicType;
    
    bool DifferentFormOfType;
};

struct _MemberDifference
{
    bool Same;
    
    bool DifferentName;
    bool DifferentType;
    bool DifferentPointer;
    bool DifferentArray;
    bool DifferentArraySize;
    
    char * CurrentName;
    char * CurrentTypeName;
    
    char * PreviousName;
    char * PreviousTypeName;
    
    _TypeFormDifference TypeFormDifference;
};

struct _TypeToCheckInfo
{
    long long Start;
    
    int DataOffset;
    bool IsPointer;
    
    int _Type;
};

struct _TypeToCheck
{
    int StackDepth;
    
    _TypeToCheckInfo Current;
    _TypeToCheckInfo Previous;
    
    int ArrayCount;
};

//TODO : create and dispose memory dynamicly

//2 way to do it
//use id or memory section as hash key to do look up
//or allocate 2 memory detail , one for saving it self, one for saving the actual data
struct _MemoryDetail
{
    
    long long Start;
	long long End;
    
	char DataName[64];
    
    bool IsPointer;
    
    int Type;
	int DataCount;
    
	_MemoryDetail * OldDetailFromPreviousMemory;
    
    _MemoryDetail * NextNode;
    
};

struct _PersistMemory
{
	unsigned char * StartMemoryAddress;
    
	long long StartOffset;
	long long CurrentOffset;
    
	long long Size;
    
	_MemoryDetail* MemoryDetailHead;
	_MemoryDetail* MemoryDetailTail;
    
	int MemoryDetailCount;
};

global _PersistMemory PreviousMemoryInfo = {};
global int TypeToCheckStackCount = 0 ;

global long long PreviousMemorySize = 0;
global unsigned char* PreviousSaveData = 0;

global char ** PreviousTypeNameArray = 0;
global _StructMetaData * PreviousStructMetaArray = 0;
global bool * PreviousIsEnum = 0;
global int * PreviousTypeSize = 0;
global int PreviousVoidTypeIndex = 0;
global int PreviousTypeCount = 0;

#define MEMBER_MAX_STACK 256
global _MemberMetaData * PreviousNestedMemberStack[MEMBER_MAX_STACK] = {};
global int PreviousNestedMemberStackCount = 0 ;

#define MAX_POINTER_STACK 2048

struct PointerRecordNode
{
    void *** PointerStack;
    int *CurrentTypeStack;
    int *PreviousTypeStack;
    
    int StackCount = 0;
    PointerRecordNode* Next;
};

global PointerRecordNode * PointerStackHeader = 0;
global PointerRecordNode * CurrentPointerStack = 0;

global int StackDepth = 0;

global _DecomposedBasicType ** CurrentBasicTypeHeadList = 0;
global _DecomposedBasicType ** CurrentBasicTypeTail = 0; 

global _DecomposedBasicType ** PreviousBasicTypeHeadList = 0;
global _DecomposedBasicType ** PreviousBasicTypeTail = 0; 


struct _TemporayMemory
{
	unsigned char* StartMemory;
	unsigned char* CurrentMemory;
	int Size;
};

enum AppWindow
{
	MainWindow,
	//SubWindow,
	//EditorWindow,
    
	WindowCount,
};

//wow you can do that?!
//so stupid

//just fucking use enum you dumbass
global GLFWwindow* AllWindows[WindowCount] = {};
global bool WindowsFocused[WindowCount] = {};
global bool IsWindowFocus = false;
global int CurrentWindowIndex = 0;

global bool DEBUG_Window = true;

internal GLFWwindow* GetWindow(int TargetWindow)
{
	return(AllWindows[TargetWindow]);
}

internal void SetWindow(GLFWwindow* NewWindow, int TargetWindow)
{
	AllWindows[TargetWindow] = NewWindow;
}

struct LineData
{
	Vector3 Position;
	Color LineColor;
};

enum QuadVertexPosition
{
	Vertex_TopRight,
	Vertex_TopLeft,
	Vertex_BottomRight,
	Vertex_BottomLeft,
	Vertex_Count,
};

global QuadVertexPosition VertexBoxOrder[Vertex_Count] =
{
	Vertex_TopLeft, 
	Vertex_TopRight,
	Vertex_BottomRight,
	Vertex_BottomLeft,
};

//TODO : maybe don't put color and coord in it?
struct _Rect
{
	Vector3 Position;
    
    Vector2 Size;
	Quaternion Rotation;//why you still here?
};

struct _Quad
{
	Vector3 VertexPosition[Vertex_Count];
	Vector4 VertexColor[Vertex_Count];// do i even what you here?
};

struct D_GlyphInfo
{
	int value;
	int offsetX;
	int offsetY;
	int advanceX;
	D_Rectangle GlyphRect;
};

#define GLYPH_INFO_LEGTH 2048
struct D_GlyphInfoNode
{
	D_GlyphInfo Info;
	D_GlyphInfoNode* NextNode;
};

#define GLYPH_INFO_HASHMAP_LENGTH 128
#define STBRP_NODE_SIZE 256
struct _D_Font_Info
{
	stbtt_fontinfo FontInfo;
	Image GlyphAtlasImage;
	int GlyphAltasPadding;
	int GlyphResolution;
    
	D_GlyphInfoNode * AllGlyphInfo;
	int AllGlyphInfoCount;
    
	D_GlyphInfoNode** GlyphHashmap;
    
	stbrp_context STB_RP_Context;
	//is this how the packing data store?
	stbrp_node *_Nodes;
	Texture2D GlyphAtlasTexture;
};

struct D_App_Data;

#define GAME_LOAD(Name) void Name(D_App_Data * _AppData)
typedef GAME_LOAD((GameLoadFunction));

#define GAME_LOOP(Name) void Name()
typedef GAME_LOOP((GameLoopFunction));

#define GAME_UNLOAD(Name) void Name()
typedef GAME_UNLOAD((GameUnloadFunction));

struct D_App_Data
{
	//you shall join the other!
	_TemporayMemory RunTimeMemory;
    _TemporayMemory FrameTimeMemory;
    
	_PersistMemory GameEditorData;
    
	int LoopCount;
    
	GameUnloadFunction * _GameUnload;
	GameLoopFunction * _GameLoop;
    
	double LoopTime;
    
	//TODO: remove this
	Camera3D GameCamera;
    
    //move this to game data
	Matrix MVP_Matrix;
    
	double Top;
	double Right;
    
	Vector2 MousePosition;
    
	Ray MouseRay3D;
    
	_Rect* WindowScreenViewportPanel;
    
	_D_Font_Info DefaultFont;
    
#define ALL_MOUSE_KEY 64
	bool PressingKeyList[ALL_MOUSE_KEY];
#define ALL_KEY 512
	bool PressingMouseList[ALL_KEY];
    
#define MAX_KEY 16
	
	int ReleasedMouseArray[MAX_KEY];
	int ReleasedMouseArrayCount;
    
	int PressedMouseArray[MAX_KEY];
	int PressedMouseArrayCount;
    
	int CodepointQueueIndex;
	int CodepointQueueCount;
	unsigned int CodepointQueue[64];
    
	int PressedKeyArray[MAX_KEY];
	int PressedKeyArrayCount;
    
	int ReleasedKeyArray[MAX_KEY];
	int ReleasedKeyArrayCount;
    
	GLFWwindow* CurrentWindow ;
	GLFWwindow* CurrentFoucsWindow ;
	Vector2 WindowSize;
    
	bool ExitWindow ;
    
	//i forgot putting this in appdata but it work fine before i switch away from visual studio
	//wtf?
	bool UpdateAtlas;
	float MouseScrollDelta;
    
	float WindowTextSize;
    
#define MAX_FILE_PATH 1024
    
	char ApplicationPath[MAX_FILE_PATH];
    
    HDC _HDC;
};

global D_App_Data* AppData = 0;
global bool MenuInteruped = true;

internal void KeyCallback(GLFWwindow* window, int Key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE)
	{
		AppData->PressingKeyList[Key] = false;
		AppData->ReleasedKeyArray[AppData->ReleasedKeyArrayCount++] = Key;
	}
    
	if (action == GLFW_PRESS)
	{
		AppData->PressingKeyList[Key] = true;
		AppData->PressedKeyArray[AppData->PressedKeyArrayCount++] = Key;
	}
}

//TODO : it got trigger by pressing mouse 
internal bool KeyPressing(int Key)
{
	return(AppData->PressingKeyList[Key]);
}

internal bool KeyPressed(int Key)
{
	for (int i = 0; i < AppData->PressedKeyArrayCount; i++)
	{
		int PressedKey = AppData->PressedKeyArray[i];
		if (PressedKey == Key)
		{
			return true;
		}
	}
    
	return false;
}

internal bool KeyReleased(int Key)
{
    
	for (int i = 0; i < AppData->ReleasedKeyArrayCount; i++)
	{
		int ReleasedKey = AppData->ReleasedKeyArray[i];
		if (ReleasedKey == Key)
		{
			return true;
		}
	}
    
	return false;
}

internal void CharCallback(GLFWwindow* window, unsigned int codepoint)
{
	AppData->CodepointQueue[AppData->CodepointQueueCount] = codepoint;
	AppData->CodepointQueueCount++;
}

internal unsigned int CharPressed()
{
	if (AppData->CodepointQueueIndex == AppData->CodepointQueueCount)
		return 0;
    
	unsigned int Result = AppData->CodepointQueue[AppData->CodepointQueueIndex];
	AppData->CodepointQueueIndex++;
	return Result;
}

internal void MouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_RELEASE)
	{
		AppData->ReleasedMouseArray[AppData->ReleasedMouseArrayCount++] = button;
		AppData->PressingMouseList[button] = false;
	}
    
	if (action == GLFW_PRESS)
	{
		AppData->PressedMouseArray[AppData->PressedMouseArrayCount++] = button;
		AppData->PressingMouseList[button] = true;
	}
}

internal bool MouseButtonPressing(int button)
{
	return(AppData->PressingMouseList[button]);
}

#define MOUSE_BUTTON_CHECK(Name) bool (Name)(int button)
typedef MOUSE_BUTTON_CHECK(* _MouseButtonBlocker);
global _MouseButtonBlocker MouseButtonBlocker = 0;

//cool trick but not much use
internal bool MouseButtonPressingWithCheck(int button)
{
    if(MouseButtonBlocker)
    {
        if(!MouseButtonBlocker(button)) return false;
    }
    
    return MouseButtonPressing(button);
}

internal bool MouseButtonPressed(int button)
{
	for (int i = 0; i < AppData->PressedMouseArrayCount; i++)
	{
		int PressedKey= AppData->PressedMouseArray[i];
		if (PressedKey ==button)
		{
			return true;
		}
	}
    
	return false;
}

internal bool MouseButtonPressedWithCheck(int button)
{
    if(MouseButtonBlocker)
    {
        if(!MouseButtonBlocker(button)) return false;
    }
    
    return MouseButtonPressed(button);
}

internal bool MouseButtonReleased(int button)
{
	for (int i = 0; i < AppData->ReleasedMouseArrayCount; i++)
	{
		int ReleasedKey= AppData->ReleasedMouseArray[i];
		if (ReleasedKey == button)
		{
			return true;
		}
	}
    
	return false;
}

internal bool MouseButtonReleasedWithCheck(int button)
{
    if(MouseButtonBlocker)
    {
        if(!MouseButtonBlocker(button)) return false;
    }
    
    return MouseButtonReleased(button);
}

internal void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	AppData->MouseScrollDelta += yoffset;
}

internal void FocusCallback(GLFWwindow* window, int focused)
{
	if (focused == GLFW_TRUE)
	{
		AppData->CurrentFoucsWindow = window;
	}
    
	for (int i = 0; i < WindowCount; i++)
	{
		if (GetWindow(i) == window)
		{
			WindowsFocused[i] = focused;
			break;
		}
	}
}

internal void RefreshCallback(GLFWwindow* Window)
{
	//TODO: fix real time windows resizing
	//not sure what happening here but i wanna clean up the "windows focus" things
	//before i work on this
}