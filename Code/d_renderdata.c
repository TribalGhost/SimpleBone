
//doesn't feel nice
typedef enum D_ShaderUniform D_ShaderUniform;
enum D_ShaderUniform
{
	SU_MVP,
	SU_texture_0,
	SU_screen_size,
    
	SU_grid_size,
	SU_spread,
	
	SU_elapsed_time,
    
    SU_camera_position,
    SU_camera_direction,
    
    SU_draw_flag,
    
	SU_count,
};

typedef enum D_ShaderType D_ShaderType;
enum D_ShaderType
{
    S_point_light,
    
	S_rect,
	S_quad,
	S_line,
	S_text,
	
    S_quad_MS,
    
	S_bloom,
	
	S_blur_H,
	S_blur_V,
    
	S_spring,
    
    S_count,
};

typedef struct D_ShaderCode D_ShaderCode;
struct D_ShaderCode
{
	const char* fragment_shader_code;
	const char* vertex_shader_code;
};

typedef enum GLShaderType GLShaderType;
enum GLShaderType
{
	
    GS_vert,
	GS_geo, //there is so much bad news about you
	GS_frag,
    
	GS_count,
    
};

typedef struct ShaderInput ShaderInput;
struct ShaderInput
{
    
    D_ShaderType shader_type;
    
    char * shader_source_file_name[GS_count];
    
	unsigned int shader;
	unsigned int vertex_array_object;
    
	int * vertex_buffer_location_array;
	int uniform_location[SU_count];
    
	int vertices_count;
	int indices_count;
    
};

global ShaderInput all_shader_inputs[S_count] = {};

typedef struct ShaderBuffer ShaderBuffer;
struct ShaderBuffer
{
	const char* name;
	int primitive_data_type;
	int components_count;
	int max_data_size;
	int primitive_data_size;
	
    bool is_uniform_buffer;
    bool is_indices_buffer;
    
	unsigned char* data;
    int buffer_size;
    
	unsigned int buffer_handle;
};

//actually there is no limit for buffer
#define MAX_GPU_BUFFER 128
global ShaderBuffer* all_GPU_buffers[MAX_GPU_BUFFER] ={};
global int shader_buffer_count = 0;

#define SHADER_INPUT_NAME_SIZE 64

typedef struct String String;
struct String
{
	wchar_t* start;
	int count;
};

typedef enum DrawFlag DrawFlag;
enum DrawFlag
{
    DF_none,
    DF_contour,
};

typedef struct RenderState RenderState;
struct RenderState
{
	
    unsigned int current_texture;
	D_ShaderType previous_shader_input;
	D_ShaderType current_shader_input;
    
    unsigned int default_white_image;
    unsigned int missing_texture;
    
    int draw_flag;
    
    unsigned int screen_frame_buffer;
    
	unsigned int interface_texture;
    unsigned int game_world_texture;
	unsigned int game_world_depth_texture;
    
	//unsigned int lighting_texture;
	//unsigned int bloom_texture;
	//unsigned int first_blur_texture;
	//unsigned int blur_texture;
    
    //TODO : refactor these
	const char* shader_uniform_name[SU_count];
	int shader_uniform_type[SU_count];
	unsigned char * shader_uniform_data[SU_count];
    
	float elapsed_time;
	float fake_depth;
    
    List texture_list_to_delete;
};

global RenderState render_state = {};

#define MAX_LIGHT 2048
typedef struct D_Light_GPU_Data D_Light_GPU_Data;
struct D_Light_GPU_Data
{
    int light_count;
    
    Vector3 * light_position;
    Vector4 * light_color;
    Vector2 * light_size;
    float * light_mul;
    Vector2 * light_tex_coord[quad_vertex_count];
};

global D_Light_GPU_Data light_data ={};

typedef struct D_Light D_Light;
struct D_Light
{
    Vector3 light_position;
    Vector2 light_size;
    Vector4 light_color;
    
	float light_strength;
};

typedef struct D_Vertex_Data D_Vertex_Data;
struct D_Vertex_Data
{
	unsigned short * indices;
    
	Vector3 * position;
	Vector3 * normal;
    Vector4 * rotation;
	Vector4 * color;
	Vector2 * tex_coord;
    
	Vector3 * line_start;
	Vector3 * line_end;
	float * line_size;
	float * fake_depth;
    
	Vector2 * size;
	Vector4 * corner_radius;
    
	Vector4 * corner_color[quad_vertex_count];
	Vector2 * corner_tex_coord[quad_vertex_count];
    
};

global D_Vertex_Data vertex_data = {};