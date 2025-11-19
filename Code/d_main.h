#define CATCH ((*(int *)0)=1)

#define internal static
#define local_persist static
#define global static

global int CurrentGLError = 0;
#define GL_CATCH if((CurrentGLError =glGetError() )!= GL_NO_ERROR) CATCH

#define D_UPDATE_RATE 60

#define DeltaTime 1.0f/D_UPDATE_RATE

typedef struct MemberMetaData MemberMetaData;
struct MemberMetaData
{
	char name[64];
    
	bool multiple_pointer;
	bool is_pointer;
	bool is_array;
	
    int array_size;
    
    int member_type;
	char type_name[64];
	int member_offset;
	int member_size;
};

typedef struct StructMetaData StructMetaData;
struct StructMetaData
{
	MemberMetaData * member_array;
	int member_count;
};

typedef struct StructMetaDataNode StructMetaDataNode;
struct StructMetaDataNode
{
    const char* name;
    int type_index;
    int size;
    bool introspected;
    StructMetaData struct_meta;
    
    StructMetaDataNode* next;
};

//can't remember why you exist
#define STRUCT_META_HASH_MAX 64 
global StructMetaDataNode ** _struct_meta_hash = 0;

//StructMetaData GetStructMeta(StructName)
#define GetStructMeta(Struct) {member_meta_##Struct,member_meta_count_##Struct}
#define GetMember(Struct ,MemberName) _ME_##Struct##_##MemberName
#define GetType(Name) _MT_##Name
#define GetNameByType(Type) _TypeMetaName[Type]

//return an array of string
#define enum_to_string(Enum) Enum##_String

#define FIXED_STRING_SIZE 64
typedef struct FixedString FixedString;
struct FixedString
{
    char string[FIXED_STRING_SIZE];
};

typedef struct FixedStringW FixedStringW;
struct FixedStringW
{
    wchar_t string[FIXED_STRING_SIZE];
};

typedef struct DataHeader DataHeader;
struct DataHeader
{
    //i wanna put a data_type in it
    //no don't do that
    FixedString name;
    int data_offset;
    
};

#define MAX_SAVE_SIZE (1024 * 1024 * 16)
global unsigned char * save_memory = 0;
global unsigned char * current_save_memory_location = 0;

#define MAX_HEADER_SIZE (1024 * 1024 * 4)
global DataHeader * data_header_array = 0;
global DataHeader * current_data_header = 0;
global DataHeader reading_data_header = {};
global int save_header_count = 0;

//i think this worth being a macro
//if this can be replace to something sane i will be the happiest man alive
//wait i can generate it
#define REALLOCATE_BUFFER_IF_TOO_SMALL( type , buffer  , capacity , count , allocate_func) \
if(capacity == count) \
{ \
\
if(count > capacity) CATCH;\
capacity *= 2; \
type * new_buffer = (type *)allocate_func(sizeof(type) * capacity);\
for(int _buffer_index = 0; _buffer_index < count ; _buffer_index++)\
{\
new_buffer[_buffer_index] = buffer[_buffer_index];\
}\
buffer = new_buffer; \
}

//oh no there is one more
#define REALLOCATE_LIST_IF_TOO_SMALL(type , buffer , list )\
if(list_full(list))\
{\
type * new_buffer = allocate_temp(type , (list)->count * 2);\
List new_list = allocate_list((list)->count * 2);\
for(int buffer_index = 0 ; buffer_index < (list)->count ; buffer_index++)\
{\
new_buffer[buffer_index] = buffer[buffer_index];\
}\
\
for(int node_index = 0 ; node_index < (list)->count + DUMMY_NODE_COUNT ; node_index++)\
{\
new_list.all_node[node_index] = (list)->all_node[node_index];\
}\
new_list.unuse_index = (list)->unuse_index;\
new_list.count = (list)->count * 2;\
buffer = new_buffer;\
(*(list)) = new_list;\
}

//TODO : generate these instead of macro
#define REALLOCATE_ARRAY_IF_TOO_SMALL(type , buffer , array )\
if(is_array_full(array))\
{\
type * new_buffer = allocate_temp(type , (array)->capacity * 2);\
Array new_array = allocate_array((array)->capacity * 2);\
for(int buffer_index = 0 ; buffer_index < (array)->count ; buffer_index++)\
{\
new_buffer[buffer_index] = buffer[buffer_index];\
}\
\
for(int index = 0 ; index < (array)->capacity ; index++)\
{\
new_array.valid_array[index] = (array)->valid_array[index];\
}\
new_array.capacity = (array)->capacity * 2;\
new_array.count = (array)->count;\
new_array.lowest_index = (array)->lowest_index;\
buffer = new_buffer;\
(*(array)) = new_array;\
}

#define HASH_DEBUG 0

typedef struct TemporayMemory TemporayMemory;
struct TemporayMemory
{
	unsigned char* start_memory;
	unsigned char* current_memory;
	int size;
};

typedef enum AppWindow AppWindow;
enum AppWindow
{
	main_window,
	
	window_count,
};

//wow you can do that?!
//so stupid

//just fucking use enum you dumbass
global GLFWwindow* all_windows[window_count] = {};
global bool foucsed_windows[window_count] = {};
global bool is_window_focused = false;
global int current_window_index = 0;

global bool DEBUG_window = true;

internal void set_window(GLFWwindow* new_window, int target_window)
{
	all_windows[target_window] = new_window;
}

typedef struct ListNode ListNode;
struct ListNode
{
	int data_index;
    
	int next;
	int previous;
};

typedef enum DummyNodeTag DummyNodeTag;
enum DummyNodeTag
{
    N_node_head = -1,
    N_node_tail = -2,
    
    N_recycled_node_head = -3,
    N_recycled_node_tail = -4,
    
    invalid_node = -100000,
};

#define DUMMY_NODE_COUNT 4

//the first node is where the chain start
//the second node is recycled node chain
typedef struct List List;
struct List
{
	ListNode * all_node;
    ListNode * node_array;
	int unuse_index;
    int count;
};

//this thing suck
typedef struct Array Array;
struct Array
{
	bool * valid_array;
    
	int capacity;
	int count;
	int lowest_index;
};

//apparently you can move nearby slot to deleted slot
//sadly i can't move slot

//i choose link list because i can't think of a better way
//i can't swap slot --- what you mean you can't swap??

//this is terrible
typedef struct HashTableEntry HashTableEntry;
struct HashTableEntry
{
    int head_index;
	int tail_index;
};

typedef struct HashTableSlot HashTableSlot;
struct HashTableSlot
{
	bool valid;
    
	int slot_value;
    
	int data_index;
    
	int next_index;
	int previous_index;
    
};

typedef struct HashTable HashTable; 
struct HashTable
{
    
    HashTableEntry * entry_array;
	HashTableSlot * slot_array;
    int count;
    
};

typedef enum QuadVertexPosition QuadVertexPosition;
enum QuadVertexPosition
{
	vertex_top_right,
	vertex_top_left,
	vertex_bottom_right,
	vertex_bottom_left,
	quad_vertex_count,
};

global QuadVertexPosition VertexBoxOrder[quad_vertex_count] =
{
	vertex_top_left, 
	vertex_top_right,
	vertex_bottom_right,
	vertex_bottom_left,
};

typedef struct Rect Rect;
struct Rect
{
	Vector3 position;
    Vector2 size;
	Quaternion rotation;
};

typedef struct Quad Quad;
struct Quad
{
	Vector3 vertex_position[quad_vertex_count];
	Vector4 vertex_color[quad_vertex_count];// do i even what you here?
};

typedef struct D_GlyphInfo D_GlyphInfo;
struct D_GlyphInfo
{
	int value;
	int offsetX;
	int offsetY;
	int advanceX;
	D_Rectangle glyph_rect;
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
    
    //you shall join the other!
	TemporayMemory run_time_memory;
    TemporayMemory frame_time_memory;
    
	int loop_count;
    
	GameUnloadFunction * game_unload;
	GameLoopFunction * game_loop;
    
	double loop_time;
    
	double top;
	double right;
    
	Vector2 mouse_position;
    
	D_FontContext default_font;
    
#define ALL_KEY 512    
	bool all_pressing_key[ALL_KEY];
	
#define ALL_MOUSE_KEY 64
	bool all_pressing_mouse[ALL_MOUSE_KEY];
    
#define MAX_KEY 16
	
	int released_mouse_array[MAX_KEY];
	int released_mouse_array_count;
    
	int pressed_mouse_array[MAX_KEY];
	int pressed_mouse_array_count;
    
	int pressed_key_array[MAX_KEY];
	int pressed_key_array_count;
    
	int released_key_array[MAX_KEY];
	int released_key_array_count;
    
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
    
	char aplication_path[MAX_FILE_PATH];
    
    HDC _HDC;
};

global D_App_Data* app_data = 0;
global bool stop_mouse_input = false;

internal void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE)
	{
		app_data->all_pressing_key[key] = false;
		app_data->released_key_array[app_data->released_key_array_count++] = key;
	}
    
	if (action == GLFW_PRESS)
	{
		app_data->all_pressing_key[key] = true;
		app_data->pressed_key_array[app_data->pressed_key_array_count++] = key;
	}
}

//TODO : Key D got trigger by pressing left mouse button 
internal bool key_pressing(int key)
{
    return(app_data->all_pressing_key[key]);
}

internal bool key_pressed(int key)
{
	for (int i = 0; i < app_data->pressed_key_array_count; i++)
	{
		int PressedKey = app_data->pressed_key_array[i];
		if (PressedKey == key)
		{
			return true;
		}
	}
    
	return false;
}

internal bool key_released(int key)
{
    
	for (int i = 0; i < app_data->released_key_array_count; i++)
	{
		int ReleasedKey = app_data->released_key_array[i];
		if (ReleasedKey == key)
		{
			return true;
		}
	}
    
	return false;
}

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

internal void MouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_RELEASE)
	{
		app_data->released_mouse_array[app_data->released_mouse_array_count++] = button;
		app_data->all_pressing_mouse[button] = false;
	}
    
	if (action == GLFW_PRESS)
	{
		app_data->pressed_mouse_array[app_data->pressed_mouse_array_count++] = button;
		app_data->all_pressing_mouse[button] = true;
	}
}

internal bool mouse_button_pressing_no_check(int button)
{
	return(app_data->all_pressing_mouse[button]);
}

internal bool mouse_button_pressing(int Button)
{
    if(stop_mouse_input) return false;
    
    return mouse_button_pressing_no_check(Button);
}


internal bool mouse_button_pressed_no_check(int button)
{
	for (int i = 0; i < app_data->pressed_mouse_array_count; i++)
	{
		int pressed_key = app_data->pressed_mouse_array[i];
		if (pressed_key == button)
		{
			return true;
		}
	}
    
	return false;
}

internal bool mouse_button_pressed(int button)
{
    if(stop_mouse_input) return false;
    
    return mouse_button_pressed_no_check(button);
}

internal bool mouse_button_released_no_check(int button)
{
	for (int i = 0; i < app_data->released_mouse_array_count; i++)
	{
		int released_key= app_data->released_mouse_array[i];
		if (released_key == button)
		{
			return true;
		}
	}
    
	return false;
}

internal bool mouse_button_released(int button)
{
    if(stop_mouse_input) return false;
    
    return mouse_button_released_no_check(button);
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