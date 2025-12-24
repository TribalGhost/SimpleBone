//===game===
typedef struct GameMemory GameMemory;
struct GameMemory
{
	unsigned char* start_memory;
	unsigned char* current_memory;
	int size;
};

global GameMemory run_time_memory = {};
global GameMemory frame_time_memory = {};
global GameMemory arena_memory = {};

#define EPSILON 0.000001f

#ifdef BUILD_D_WINDOWS
global char * map_save_name = "Game\\map_data.ma";
#endif

#ifdef BUILD_D_LINUX
global char * map_save_name = "Game/map_data.ma";
#endif

//so far this is better
//i have type instead of void pointer 
//so that i can rely on type checking when refactorying
//no need to convert void pointer(and debug)
#define BUFFER(name , type) \
typedef struct name name; \
struct name \
{ \
type * data; \
int data_size; \
int count; \
int capacity; \
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
    int capacity;
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
    int capacity;
};

BUFFER(Vector3Buffer , Vector3);

typedef enum AllocatorType AllocatorType;
enum AllocatorType
{
    AT_temp,
    AT_frame,
};

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
    FixedString name;
    int data_offset;
    //data type, data size and data count can be add to it if i needed it
};

global unsigned char * scratch_buffer_for_read = 0;

#define MAX_SAVE_SIZE (1024 * 1024 * 16)
global unsigned char * save_memory = 0;
global unsigned char * current_save_memory_location = 0;

#define MAX_HEADER_SIZE (1024 * 1024 * 4)
global DataHeader * data_header_array = 0;
global DataHeader * current_data_header = 0;
global DataHeader reading_data_header = {};
global int save_header_count = 0;

typedef struct Rect Rect;
struct Rect
{
	Vector3 position;
    Vector2 size;
	Quaternion rotation;
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

typedef struct Quad Quad;
struct Quad
{
	Vector3 vertex_position[quad_vertex_count];
	Vector4 vertex_color[quad_vertex_count];// do i even what you here?
};

BUFFER(QuadBuffer , Quad);

typedef struct ShapeImpactData ShapeImpactData;
struct ShapeImpactData
{
    Vector3 * shape_a_vertices;
    int shape_a_vertices_count;
    Vector3 * shape_b_vertices;
    int shape_b_vertices_count;
    
    Vector3 ray_direction;
    bool stop_if_too_far;
    
    float time_of_impact;
    Vector3 impact_point;
    Vector3 impact_normal;
};

typedef struct GJK_State GJK_State;
struct GJK_State
{
    Vector3 simplex[4];
    int simplex_count;
    Vector3 search_direction;
    Vector3 origin;
};

typedef struct ConvexShape ConvexShape;
struct ConvexShape
{
    Vector3 * vertices; 
    int vertices_count; 
    Vector3 velocity;
    Vector3 position;
};

typedef struct RayCastResult RayCastResult;
struct RayCastResult
{
    bool impacted;
    Vector3 surface_normal;
    float closest_hit_time;
};

typedef enum BoxFace BoxFace;
enum BoxFace
{
    face_top,
    face_bottom,
    face_right,
    face_left,
    face_front,
    face_back,
    face_count,
};

typedef enum BoxVertex BoxVertex;
enum BoxVertex
{
    bv_A, //top_forward_left
    bv_B, //top_forward_right
    bv_C, //top_backward_left
    bv_D, //top_backward_right
    bv_E, //bottom_forward_left
    bv_F, //bottom_forward_right
    bv_G, //bottom_backward_left
    bv_H, //bottom_backward_right
    
    box_vertex_count
};

global Rect box_rect[face_count] = {};

typedef struct Box Box;
struct Box
{
    Vector3 position;
    Vector3 size;
    Quaternion rotation;
};

BUFFER(BoxBuffer , Box);

typedef enum ShapeType ShapeType;
enum ShapeType
{
    ST_invalid,
    ST_quad,
    ST_box,
};

typedef struct Shape Shape;
struct Shape
{
    int type;
    int index;
};

BUFFER(ShapeBuffer , Shape);

typedef struct BoundingBoxNode BoundingBoxNode;
struct BoundingBoxNode
{
    Vector3 right_top_forward;
    Vector3 left_bottom_backward;
    
    Shape shape;
    
    BoundingBoxNode * left;
    BoundingBoxNode * right;
};

BUFFER(BoundingBoxNodeBuffer , BoundingBoxNode);

typedef enum SplitType SplitType;
enum SplitType
{
    split_yz,
    split_xz,
    split_xy,
    split_count,
};

global BoundingBoxNode * bounding_box_root = 0;

typedef struct CellIterator CellIterator;
struct CellIterator
{
    bool initialized;
    
    int cell_x;
    int cell_y;
    int cell_z;
    
    int cell_left;
    int cell_bottom;
    int cell_backward;
    
    int cell_right;
    int cell_top;
    int cell_forward;
};

#define INPUT_MAX_KEY 16

typedef struct InputState InputState;
struct InputState
{
    int pressing_key[INPUT_MAX_KEY];
	int pressing_key_count;
    
	int pressed_key[INPUT_MAX_KEY];
	int pressed_key_count;
    
	int released_key[INPUT_MAX_KEY];
	int released_key_count;
    
    int pressing_mouse[INPUT_MAX_KEY];
    int pressing_mouse_count;
    
	int released_mouse[INPUT_MAX_KEY];
	int released_mouse_count;
    
	int pressed_mouse[INPUT_MAX_KEY];
	int pressed_mouse_count;
};

typedef struct Player Player;
struct Player
{
    Vector3 camera_target;
    Vector3 camera_position;
    
    Vector3 position;
    Vector3 velocity;
    Box box;
    
    bool grounded;
};

BUFFER(PlayerBuffer , Player);

typedef struct Int3 Int3;
struct Int3
{
    int x;
    int y;
    int z;
};

BUFFER(Int3Buffer , Int3);

typedef struct CellData CellData;
struct CellData
{
    bool blocked;
    
    unsigned int search_index;
    float cost;
    Int3 previous_cell;
};

typedef struct PathResult PathResult;
struct PathResult
{
    bool path_found;
    Int3Buffer buffer;
};

global long long previous_update_count = -1;
global Box debug_box = {};

#define SUBDIVISION (10)
#define GRID_SIZE (1.0)
#define UNIT_SIZE (GRID_SIZE / ((double)SUBDIVISION))

global Camera3D game_camera = {};

global unsigned int search_index = 0;

global float nav_mesh_cell_size = GRID_SIZE * 0.5f;
global Box nav_mesh_whole_box = {};
global Box nav_mesh_start_box = {};

global int search_queue_capacity = 128;
global Int3 * search_queue = 0;

global Int3 nav_mesh_size = {};
global int nav_mesh_cell_capacity = 0;
global CellData * nav_mesh_cell = 0;

global Vector3 right_direction = {1,0,0};
global Vector3 up_direction = {0,1,0};
global Vector3 forward_direction = {0,0,1};

global Vector3 * convex_shape_a_vertices = 0;
global int convex_shape_a_vertices_count = 0;

global Vector3 * convex_shape_b_vertices = 0; 
global int convex_shape_b_vertices_count = 0;

//===game data===

global bool stop_mouse_input = false;
global InputState * input_state = 0;

global long long game_update_count = 0;

global PlayerBuffer player_buffer = {};
global Array player_array = {};

global Array box_in_map_array = {};
global BoxBuffer box_in_map_buffer = {};

global Array quad_in_map_array = {};
global QuadBuffer quad_in_map_buffer = {};

global Vector3Buffer reference_frame_buffer = {};
global List reference_frame_list = {};

//===network===

#define CONNECTION_PORT 35222

typedef enum DataFlag DataFlag;
enum DataFlag
{
    DF_pressing_key_count,
    DF_pressed_key_count,
    DF_released_key_count,
    DF_pressing_mouse_count,
    DF_pressed_mouse_count,
    DF_released_mouse_count,
    
    DF_pressing_key,
    DF_pressed_key,
    DF_released_key,
    DF_pressing_mouse,
    DF_pressed_mouse,
    DF_released_mouse,
    
    DF_camera_target,
    DF_camera_position,
    
    DF_player_count,
    DF_player_position,
    DF_player_velocity,
    DF_player_grounded,
};

typedef enum ReceiveOrder ReceiveOrder;
enum ReceiveOrder
{
    RO_header_count,
    RO_header,
    RO_data_size,
    RO_data,
    
    RO_count,
};

typedef struct NetDataHeader NetDataHeader;
struct NetDataHeader
{
    int flag;
    int offset;
};

BUFFER(NetDataHeaderBuffer , NetDataHeader);
BUFFER(ByteBuffer , unsigned char);

#define MAX_RECEIVE_BUFFER 1024 * 16
#define MAX_RECEIVE_HEADER 128

typedef struct ReceiveState ReceiveState;
struct ReceiveState
{
    int last_update_index;
    
    bool connection_reseted;
    int available_buffer_index;
    
    unsigned char buffer[2][MAX_RECEIVE_BUFFER];
    NetDataHeader header_buffer[2][MAX_RECEIVE_HEADER];
    
    int receiving_buffer_index;
    
    ReceiveOrder state;
    int receiving_socket;
    
    int collected_byte;
    
    int data_size;
    int header_count;
};

#define MESSAGE_SIZE 64

typedef struct PlayerConnection PlayerConnection;
struct PlayerConnection
{
    char message[MESSAGE_SIZE];
    
    ReceiveState receive_state;
    int connection_socket;
    
    InputState input_state;
};

BUFFER(PlayerConnectionBuffer , PlayerConnection);

typedef struct NetState NetState;
struct NetState
{
    NetDataHeaderBuffer header_buffer;
    ByteBuffer send_buffer;
    
    ReceiveState client_receive_state;
    
    int listening_socket;
    int client_to_server_socket;
    
    bool connected_to_server;
    
    bool is_server;
    bool is_client;
};

//===network data===

global PlayerConnectionBuffer player_connection_buffer = {};
global ReceiveState default_receive_state = {};
global NetState net_state = {};
