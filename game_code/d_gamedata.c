//===game===

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

#define EPSILON 0.000001f

#ifdef BUILD_D_WINDOWS
global char * map_save_name = "Game\\map_data.ma";
#endif

#ifdef BUILD_D_LINUX
global char * map_save_name = "Game/map_data.ma";
#endif

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
	int upper_bound;
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

typedef struct D_Memory D_Memory;
struct D_Memory
{
	unsigned char* start_memory;
	unsigned char* current_memory;
	int size;
};

global D_Memory run_time_memory = {};
global D_Memory frame_time_memory = {};

typedef struct Chunk Chunk;
struct Chunk
{
    int allocated_size;
    int chunk_count;
    int previous;
};

BUFFER(ChunkBuffer , Chunk);

typedef struct ChunkPool ChunkPool;
struct ChunkPool
{
    int allocate_type;
    int chunk_size;
    int pool_size;
    ChunkBuffer buffer;
    Array array;
    unsigned char * memory_start;
};

global ChunkPool * current_pool_to_allocate = 0;
global int * current_chunk_index = 0;

typedef enum AllocationType AllocationType;
enum AllocationType
{
    AT_temp,
    AT_frame,
    AT_pool,
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
	Vector4 vertex_color[quad_vertex_count];// do i need you here?
};

BUFFER(QuadBuffer , Quad);

//TODO: the front face and back front are wrong
//no idea why and it is cold, i can barely move my figures
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

typedef enum D_Direction D_Direction;
enum D_Direction
{
    D_right,
    D_up,
    D_forward,
    D_count,
};

typedef struct Box Box;
struct Box
{
    Vector3 position;
    Vector3 size;
    Quaternion rotation;
    
    float top_front;
    float top_right;
    float right_top;
    float right_front;
    float front_top;
    float front_right;
};

BUFFER(BoxBuffer , Box);

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
    
    int simplex_index[4];
};

typedef struct TriangleIndex TriangleIndex;
struct TriangleIndex
{
    int a_index;
    int b_index;
    int c_index;
};

BUFFER(TriangleIndexBuffer , TriangleIndex);

typedef struct NewEdge NewEdge;
struct NewEdge
{
    int a_index;
    int b_index;
};

BUFFER(NewEdgeBuffer , NewEdge);

typedef enum ShapeType ShapeType;
enum ShapeType
{
    ST_invalid,
    ST_quad,
    ST_box,
    ST_capsule,//TODO: didn't work, try again
};

typedef struct EntityHandle EntityHandle;
struct EntityHandle
{
    int entity_index;
    int generation_index;
};

BUFFER(EntityHandleBuffer , EntityHandle);

typedef struct ShapeOwner ShapeOwner;
struct ShapeOwner
{
    bool is_entity;
    bool is_player;
    
    union
    {
        EntityHandle entity_handle;
        //no need to use handle yet
        int player_index;
    };
};

typedef struct ShapeUnion ShapeUnion;
struct ShapeUnion
{
    ShapeType type;
    union
    {
        Box box;
        Quad quad;
    };
    
    Vector3 velocity;
    ShapeOwner owner;
};

BUFFER(ShapeUnionBuffer , ShapeUnion);

typedef struct ConvexShape ConvexShape;
struct ConvexShape
{
    //TODO: i don't think capsule's support function is correct
    //if it is i don't know what to do
    //try sphere?
    
    ShapeUnion shape;
    Vector3 velocity;
    Vector3 position;
    
    bool collect_all_collision;
    bool capture_collision;
    
    Vector3 * shape_vertices;
    int shape_vertices_count;
};

typedef struct RayCastResult RayCastResult;
struct RayCastResult
{
    Vector3 surface_normal;
    float hit_time;
    ShapeOwner shape_owner;
};

BUFFER(RayCastResultBuffer , RayCastResult);

typedef struct CollisionResult CollisionResult;
struct CollisionResult
{
    Vector3 velocity;
    Vector3 offset;
    bool collided;
    bool stucked;
};

typedef struct BoundingBoxNode BoundingBoxNode;
struct BoundingBoxNode
{
    Vector3 right_top_forward;
    Vector3 left_bottom_backward;
    
    ShapeUnion shape;
    
    BoundingBoxNode * left;
    BoundingBoxNode * right;
};

BUFFER(BoundingBoxNodeBuffer , BoundingBoxNode);
BUFFER(BoundingBoxNodePointerBuffer , BoundingBoxNode *);

typedef enum SplitType SplitType;
enum SplitType
{
    split_yz,
    split_xz,
    split_xy,
    split_count,
};

global BoundingBoxNode * bounding_box_in_map_root = 0;
global BoundingBoxNodePointerBuffer bounding_box_root_stack = {};

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

typedef struct CameraTrigger CameraTrigger;
struct CameraTrigger
{
    Vector3 camera_target_offset;
    bool player_within;
};

BUFFER(CameraTriggerBuffer , CameraTrigger);

#define INPUT_MAX_KEY 16

typedef struct InputArray InputArray;
struct InputArray
{
    int count;
    int array[INPUT_MAX_KEY];
    bool consumed_array[INPUT_MAX_KEY];
};

typedef enum InputArrayType InputArrayType;
enum InputArrayType
{
    IA_pressing_key,
    IA_pressed_key,
    IA_released_key,
    IA_pressing_mouse,
    IA_pressed_mouse,
    IA_released_mouse,
    IA_count,
};

typedef struct InputState InputState;
struct InputState
{
    InputArray input_array[IA_count];
};

typedef enum ChargeDirection ChargeDirection;
enum ChargeDirection
{
    CD_forward,
    CD_backward,
    CD_right,
    CD_left,
    CD_up,
    CD_bottom,
    
    CD_count,
};

typedef enum EntityType EntityType;
enum EntityType
{
    E_moving_wall,
    E_small_block,
    E_hook,
    E_bubble,
    
    E_count,
};

typedef struct Entity Entity;
struct Entity
{
    int generation_index;
    int entity_index;
    
    int type;
    
    Vector3 position;
    Vector3 velocity;
    Vector3 previous_position;
    
    float velocity_multipler;
    float gravity_force;
    
    Box box;
    
    bool respawnable;
    bool unpickable;
    
    bool clearable;
    bool cleared;
    float clear_timer;
    bool override_clear_time;
    float clear_time;
    
    bool respawning;
    float respawn_timer;
    bool override_respawn_time;
    float respawn_time;
    
    float charge_timer;
    bool charging;
    Vector3 charge_direction;
    
    bool hook_on_surface;
    
    bool expanding;
    bool stop_expanding;
    
    bool solid;
    float solid_timer;
    
    bool detonable;
};

global Color entity_color_array[E_count] = {};

BUFFER(EntityBuffer , Entity);

typedef struct Player Player;
struct Player
{
    int chunk_index;
    
    bool it_is_me;
    int connection_index;
    
    Vector3 camera_target;
    Vector3 camera_position;
    
    Vector3 position;
    Vector3 velocity;
    
    Vector3 interact_direction;
    Vector2 interact_input;
    bool interact;
    
    float no_both_direction_timer;
    Vector3 character_position;
    Box box;
    Box collide_box;
    
    float ground_time;
    
    float spring_time;
    bool is_holding_entity;
    EntityHandle holding_entity_handle;
    
    bool floating_in_bubble;
    bool grounded;
    bool moving_upward;
    
    bool floatable;
    bool floating;
    float float_timer;
    
    EntityHandleBuffer hook_buffer;
    Array hook_array;
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

typedef struct CollisionVisual CollisionVisual;
struct CollisionVisual
{
    Vector3 a;
    Vector3 b;
    Vector3 c;
    
    Vector3 closest_point;
    Vector3 ray_end;
    
    bool collided;
    Vector3 collision_point;
    Vector3 collision_normal;
    Vector3 offset;
    Vector3 direction;
    
    ShapeUnion shape_a;
    ShapeUnion shape_b;
};

BUFFER(CollisionVisualBuffer , CollisionVisual);

typedef enum CollisionType CollisionType;
enum CollisionType
{
    CT_raycast,
    CT_collision,
};

typedef struct FrameCollision FrameCollision;
struct FrameCollision
{
    int update_index;
    int collision_type;
    Vector3 collision_visual_offset;
    //TODO: something wrong, this index is 1 less
    int slice_start;
    int slice_end;
    
    Vector3 start;
    Vector3 velocity;
};

BUFFER(FrameCollisionBuffer , FrameCollision);

BUFFER(FloatBuffer , float);
BUFFER(Int32Buffer , int);
BUFFER(BoolBuffer , bool);
BUFFER(ByteBuffer , unsigned char);

//global long long previous_update_count = -1;

//change this to 8
#define SUBDIVISION (8)
#define GRID_SIZE (1.0)
#define UNIT_SIZE (GRID_SIZE / ((double)SUBDIVISION))

global float default_entity_clear_time = 5.0f;
global float default_entity_respawn_time = 1.0f;

//global Vector3 player_box_size = {GRID_SIZE * 0.4 , GRID_SIZE * 0.6 , GRID_SIZE * 0.4};
global Vector3 player_box_size = {GRID_SIZE * 0.6 , GRID_SIZE * 0.96 , GRID_SIZE * 0.6};
global Vector3 entity_box_size = {GRID_SIZE * 1.0 , GRID_SIZE * 1.0 , GRID_SIZE * 1.0};

global Camera3D world_camera = {};

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

global float player_spring_percent = 0.8f;
global Vector3 player_spring_ray = {0, -UNIT_SIZE * 5, 0};

global Vector3 * convex_shape_a_vertices = 0;
global int convex_shape_a_vertices_count = 0;

global Vector3 * convex_shape_b_vertices = 0; 
global int convex_shape_b_vertices_count = 0;

global bool store_multiple_frame_collision = false;
global int current_frame_collision_index = 0;
global FrameCollisionBuffer frame_collision_buffer = {};
global FrameCollision * current_frame_collision = 0;

global bool show_bounding_box = false;
global bool capture_collision = false;
global bool capture_collision_non_stop = false;
global bool display_all_visual = false;
global bool display_all_captured_collision = false;
global int current_collision_visual_index = 0;

global Vector3 collision_visual_offset = {};
global ShapeUnion shape_a_union;
global ShapeUnion shape_b_union;

global CollisionVisualBuffer collision_visual_buffer = {};

//===game data===

global InputState * input_state = 0;

global long long game_update_count = 0;
global bool game_paused = false;
global bool game_step = false;
global bool entity_initialized = false;

global ChunkPool player_pool = {};
global int player_chunk_size = 0;

//global BlockBuffer player_block_buffer = {};
//global Array player_block_array = {};
//global unsigned char * player_buffer = 0;

global EntityBuffer entity_layout_buffer = {};
global Array entity_layout_array = {};

global EntityBuffer entity_active_buffer = {};
global Array entity_active_array = {};

global Array box_in_map_array = {};
global BoxBuffer box_in_map_buffer = {};

global Array quad_in_map_array = {};
global QuadBuffer quad_in_map_buffer = {};

//===network===

#define CONNECTION_PORT 35222

typedef enum DataFlag DataFlag;
enum DataFlag
{
    DF_input_state,
    
    DF_camera_target,
    DF_camera_position,
    
    DF_player_count,
    DF_whole_player,
    DF_player_owned,
    
    DF_entity_count,
    DF_whole_entity,
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

global Array player_connection_array = {};
global PlayerConnectionBuffer player_connection_buffer = {};
global ReceiveState default_receive_state = {};
global NetState net_state = {};
