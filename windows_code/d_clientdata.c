global char * game_state_save_name = "Game\\game_state_data.da";

global bool shader_compile_failed = false;

global int start_color_seed = 121365623;

BUFFER(FixedStringWBuffer , FixedStringW);

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

BUFFER(KeyFrameBuffer , KeyFrame);

typedef enum RotationAxis RotationAxis;
enum RotationAxis
{
    R_X,
    R_Y,
    R_Z,
    
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

BUFFER(BoneBuffer , Bone);

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
global KeyFrameBuffer all_key_frame_buffer = {};

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

BUFFER(DeformVertexBuffer , DeformVertex);

typedef struct BoneSelectionResultData BoneSelectionResultData;
struct BoneSelectionResultData
{
    int bone_index;
    Vector3 hit_point;
};

BUFFER(BoneSelectionResultDataBuffer , BoneSelectionResultData);

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
    
    BoneBuffer bone_buffer;
    BoneBuffer initial_bone_buffer;
    
    List root_bone_list;
    HashTable bone_children_hash_table;
};

#define MAX_MODEL 64
global D_Model all_models[MAX_MODEL] = {};
global int model_count = 0;

//TODO:this should be inside clip bone
global int selected_model_index = -1;
global D_Model * selected_model = 0;

global int selected_reference_frame_index = -1;

typedef enum EditorType EditorType;
enum EditorType
{
    edit_base_pose,
    edit_animation,
    edit_world,
    demo,
    
    editor_type_count,
};

global int editor_type = edit_base_pose;

typedef enum MapEditType MapEditType;
enum MapEditType
{
    MET_none,
    MET_quad,
    MET_box,
    MET_camera_trigger,
    MET_entity,
    MET_count,
};

typedef enum EditType EditType;
enum EditType
{
    edit_move,
    edit_rotate,
    edit_stretch,
    edit_drag_new_box,
    edit_camera_offset,
    edit_add_entity,
    edit_count,
};

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

global Vector3Buffer reference_frame_buffer = {};
global List reference_frame_list = {};

global Ray mouse_ray_3D = {};
global Matrix current_matrix = {};
global InputState client_input_state = {};

global int current_map_edit_type = MET_none;
global int current_edit_type = edit_move;

global Box * last_clicked_box = 0;
global Box * hovering_box = 0;

global Vector3 game_camera_position = {};
global Vector3 game_camera_offset = {};

global Array camera_array = {};
global CameraTriggerBuffer camera_buffer = {};
global BoxBuffer camera_zone_buffer = {};

global List camera_within_list = {};
global Vector3 last_camera_offset = {};