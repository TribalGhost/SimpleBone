#define EPSILON 0.000001f

global StructMetaData * _type_struct_meta_ = 0;

global const char* game_texture_data_name = "Game\\GameTexture.gts";

global char * game_state_save_name = "Game\\GameState";
global char * game_animation_save_name = "Game\\Animation.txt";

global bool shader_compile_failed = false;

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
global int start_color_seed = 121365623;

typedef enum GameMenuType GameMenuType;
enum GameMenuType
{
	GMT_descend,
	GMT_to_the_right,
};

typedef struct DrawingMenu DrawingMenu;
struct DrawingMenu
{
	
    Vector2 current_button_position;
	bool on_screen;
    
	float current_button_left;
    
	bool button_hover;
	bool button_clicked;
    
	int menu_type;
	float button_extra_offset;
    
};

typedef struct BoneState BoneState;
struct BoneState
{
    
	Vector3 local_position;
	Quaternion local_rotation;
	Vector3 end_point_offset;
    
};

typedef struct KeyFrame KeyFrame;
struct KeyFrame
{
    int bone_index;
    
    BoneState bone_state;
	int frame_index;
    
    //i don't think i should name it like this
    Vector3 previous_bezier_offset;
    Vector3 next_bezier_offset;
    
    //int hash_table_by_bone_slot_index;
    //int hash_table_slot_index;
};

typedef enum RotationAxis RotationAxis;
enum RotationAxis
{
    R_None,
    
    R_Z,
    R_Y,
    R_X,
    
    R_count,
};

typedef struct RotationAxisData RotationAxisData;
struct RotationAxisData
{
    Rect rect;
    
    Color target_axis_color;
    float target_axis_line_size;
    
    Color axis_color;
    float axis_line_size;
};

typedef struct Bone Bone;
struct Bone
{
    
    bool from_blend_file;
    
    bool IK_enable;
    int IK_chain_length;
    
    int IK_target_bone_index;
    int IK_pole_bone_index;
    
    //TODO : these things should be specifiy by clip
    bool free_position;
	
    //use for look up parent and children bone only
    //try not forget updating it
    int bone_index;
    int parent_bone_index;
    
    FixedStringW bone_name;
    
	BoneState state;
	
	Vector3 position;
	Vector3 end_position;
	Quaternion rotation;
    
    //it can be edit when the current frame is the key frame belong to this bone
    bool free_bone;
};

global float camera_current_zoom = 1.0;

#define FRAME_PER_SECOND 60
#define FRAME_TIME 1.0 / ((double)FRAME_PER_SECOND)

#define GetKeyFrameHash(FrameIndex , BoneIndex) (hash_int(16 * BoneIndex + FrameIndex))

typedef struct Clip Clip;
struct Clip
{
    
    FixedString clip_name;//no idea why this exist
    
    int clip_index;
    
    //AnimationBlendTag blend_tag;
    
    bool not_loop;
    
    //TODO : Add clip time to it
    //i just gonna assume all clip length to 1 
    
    //for querying keyframe
    //probably useless
    
    List key_frame_active_list;
    //too wacky
    //i can't tell the difference between them
    HashTable key_frame_hash_table_by_bone;
    HashTable key_frame_hash_table;
    
    List dragging_key_frame_list;
    
    int key_frame_start_offset;
    int key_frame_count_to_save;
    
};

//this should be a temporary thing
//bone interaction are modifying these instead of the actual thing

//TODO: rename this
typedef struct ClipBone ClipBone;
struct ClipBone
{
    Bone * bone_pose_offset;
    
    //these two is really confusing
    Bone * final_bone_pose;
    
    //wait this isn't a temporary thing
    bool * selected_bone;
    bool * hovered_bone;
    
    int clip_index;
    
};

typedef struct BoneSelection BoneSelection;
struct BoneSelection
{
    int clip_bone_stack_index;//?? what is this
    int clip_index;
    int bone_index;
};

#define MAX_CLIP_STACK 64
#define CLIP_START_CAPACITY 16

global ClipBone clip_bone_stack[MAX_CLIP_STACK] = {};
global int clip_bone_stack_count = 0;
global bool add_clip_bone_at_next_frame = false;

#define MAX_CLIP 64
global Clip * clip_array[MAX_CLIP] = {};

#define KEY_FRAME_CAPACITY 64
global KeyFrame * all_key_frame = 0;
global int all_key_frame_capacity = KEY_FRAME_CAPACITY;
global int all_key_frame_count = 0;

typedef struct Box Box;
struct Box
{
    Vector3 position;
    Vector3 size;
    Quaternion rotation;
};

typedef struct Face Face;
struct Face
{
    Vector3 position;
    Vector2 size;
	Quaternion rotation;
    Vector3 vertex_position[quad_vertex_count];
};

typedef struct DeformVertexSlice DeformVertexSlice;
struct DeformVertexSlice
{
    int start;
    int count;
};

typedef struct DeformVertex DeformVertex;
struct DeformVertex
{
    int bone_index;
    float weight;
};

typedef struct BoneSelectionResultData BoneSelectionResultData;
struct BoneSelectionResultData
{
    int bone_index;
    Vector3 hit_point;
};

typedef struct BoneSelectionResult BoneSelectionResult;
struct BoneSelectionResult
{
    BoneSelectionResultData * data;
    int data_count;
};

typedef struct D_Model D_Model;
struct D_Model
{
    
    DeformVertexSlice * deform_vertex_slice;
    Vector3 * vertices;
    int vertex_count;
    
    Vector3 * normals;
    unsigned short * indices;
    int index_count;
    
    DeformVertex * all_deform_vertex;
    int deform_vertex_count;
    
    Bone * all_bones;
	int bone_count;
    int bone_capacity;
    
    Bone * all_initial_bone;
    int initial_bone_count;
    int initial_bone_capacity;
    
    List root_bone_list;//when should i put you?
    
    //HashTable bone_parent_hash_table;
	HashTable bone_children_hash_table;
};

#define MAX_MODEL 64
global D_Model all_models[MAX_MODEL] = {};
global int model_count = 0;

//TODO:this should be inside clip bone
global int selected_model_index = -1;
global D_Model * selected_model = 0;

global Vector3 * all_reference_frame = 0;
global List reference_frame_list = {};

global int selected_reference_frame_index = -1;

global Vector3 right_direction = {1,0,0};
global Vector3 up_direction = {0,1,0};
global Vector3 forward_direction = {0,0,1};

typedef enum EditorType EditorType;
enum EditorType
{
    edit_base_pose,
    edit_animation,
    edit_world,
    demo,
    
    edit_type_count,
};

global int editor_type = edit_base_pose;

typedef struct RightClickMenu RightClickMenu;
struct RightClickMenu
{
    bool on;
    Vector2 position;
    
    int box_index;
};

global RightClickMenu right_click_menu = {};

typedef enum MapEditType MapEditType;
enum MapEditType
{
    MET_none,
    MET_quad,
    MET_box,
    MET_count,
};

global int current_map_edit_type = MET_none;

typedef struct DemoData DemoData;
struct DemoData
{
    Vector3 character_position;
    Vector3 character_velocity;
    Vector3 character_direction;
};

global DemoData demo_data = {};

typedef struct EditorData EditorData;
struct EditorData
{
    bool assigning_parent_bone;
    bool assigning_IK_target_bone;
    bool assigning_IK_pole_bone;
    
    int game_frame;
    
    int selected_clip_index;
    
    bool control_rig;
    bool turn_off_bezier_curve;
    
    bool selecting_bezier;
    
    int selected_bone_count;
    BoneSelection * selected_bone_stack;
    
	bool playing;
	float play_timer;
    
    int IK_iteration_count;
    
	float timeline_slider_offset;
    
	int current_frame_at_timeline;
    
	int start_frame_index;
	int timeline_frame_length;
    
	float timeline_scale;
    
	float target_camera_zoom;
	float current_camera_zoom;
	
    Vector3 right;
    Vector3 up;
    Quaternion billboard_rotation;
    
    Vector3 editor_camera_euler;
    Vector3 editor_camera_offset;
    
    float side_camera_zoom;
    
    Vector3 showcase_camera_euler;
    
	Vector2 current_mouse_position;
	Vector2 previous_mouse_position;
	Vector2 operate_menu_position;
    
	bool flat_color;
    
};

global EditorData * editor = 0;

//TODO: can model be different in clip?
//TODO: put this somewhere else
global Bone * base_pose_bone = 0;

global Vector2 * modifying_menu_position = 0;

global float screen_near_clip = 0;
global float screen_far_clip = 1;

global Matrix full_screen_pixel_matrix = {};
global Matrix viewport_screen_pixel_matrix = {};
global Matrix world_3D_matrix = {};
global Matrix view_matrix = {};
global Matrix project_matrix = {};

global Matrix world_3D_to_screen_matrix = {};

global R_Rectangle current_viewport = {};
global bool update_once = false;
global bool within_viewport = false;
global Vector2 mouse_position = {};

global int grid_normal_index = 0;

typedef struct SplitViewport SplitViewport;
struct SplitViewport
{
    R_Rectangle viewport;
    float camera_zoom;
    Vector3 camera_euler;
    Vector3 camera_offset;
    Vector3 camera_up;
    bool ortho;
};

global Ray mouse_ray_3D = {};

global Camera3D game_camera = {};

global Matrix current_matrix = {};

global long long game_update_count = 0;

#define SUBDIVISION (10)
#define GRID_SIZE (1.0)
#define UNIT_SIZE (GRID_SIZE / ((double)SUBDIVISION))

global Array box_in_map_array = {};
global Box * box_in_map = 0;

global Array quad_in_map_array = {};
global Quad * quad_in_map = 0;

global Vector3 * convex_shape_a_vertices = 0;
global int convex_shape_a_vertices_count = 0;

global Vector3 * convex_shape_b_vertices = 0; 
global int convex_shape_b_vertices_count = 0;

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

typedef struct BoundingBoxNode BoundingBoxNode;
struct BoundingBoxNode
{
    Vector3 right_top_forward;
    Vector3 left_bottom_backward;
    
    Shape shape;
    
    BoundingBoxNode * left;
    BoundingBoxNode * right;
};

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

typedef struct Player Player;
struct Player
{
    Vector3 position;
    Vector3 velocity;
    ShapeImpactData impact;
    Box box;
};

global Player player = {};

typedef struct Int3 Int3;
struct Int3
{
    int x;
    int y;
    int z;
};

typedef struct CellData CellData;
struct CellData
{
    bool blocked;
    
    unsigned int search_index;
    float cost;
    Int3 previous_cell;
};

global unsigned int search_index = 0;

global float nav_mesh_cell_size = GRID_SIZE * 0.5f;
global Box nav_mesh_whole_box = {};
global Box nav_mesh_start_box = {};

global Int3 nav_mesh_size = {};
global int nav_mesh_cell_capacity = 0;
global CellData * nav_mesh_cell = 0;

global int search_queue_capacity = 128;
global Int3 * search_queue = 0;

typedef struct PathResult PathResult;
struct PathResult
{
    bool path_found;
    
    Int3 * path;
    int count;
    int capacity;
};