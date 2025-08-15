#ifndef global

#define global static
#define internal static

#define persist __based(_StartMemory_)

#define Array(Type) D_Array_##Type
#define Hash(Type) D_Hash_##Type

#endif

//doesn't feel nice
enum D_ShaderUniform
{
	SU_MVP,
	SU_Texture0,
	SU_ScreenSize,

	SU_GridSize,
	SU_Spread,
	
	SU_ElapsedTime,

	SU_COUNT,
};

enum D_ShaderType
{
    S_Point_Light,
    
	S_Rect,
	S_Quad,
	S_Line,
	S_Text,
	
	S_Bloom,
	
	S_Blur_H,
	S_Blur_V,

	S_Spring,

    S_COUNT,
};

struct D_ShaderCode
{
	const char* FragmentShaderCode;
	const char* VertexShaderCode;
};

enum GLShaderType
{
	GS_Vert,
	GS_Geo, //there is so much bad news about you
	GS_Frag,
    
	GS_Count,
};

struct _ShaderInput
{
    D_ShaderType ShaderType;
	const char* ShaderSourceFileName[GS_Count];

	unsigned int Shader;
	unsigned int VertexArrayObject;
    
	int * VertexBufferLocationArray;
	int UniformLocation[SU_COUNT];
    
	int VerticesCount;
	int IndicesCount;
};

global _ShaderInput AllShaderInputs[S_COUNT] = {};

struct _ShaderBuffer
{
	const char* Name;
	int PrimitiveDataType;
	int ComponentsCount;
	int MaxDataSize;
	int PrimitiveDataSize;
	
    bool IsUniformBuffer;
    bool IsIndicesBuffer;
    
	unsigned char* Data;
    int BufferSize;
    
	unsigned int BufferHandle;
};

//actually there is no limit for buffer
#define MAX_GPU_BUFFER 128
global _ShaderBuffer* All_GPU_Buffers[MAX_GPU_BUFFER] ={};
global int ShaderBufferCount = 0;

#define SHADER_INPUT_NAME_SIZE 64

struct String
{
	wchar_t* Start;
	int Count;
};

#define MAX_TEXTURE 2048

struct TextureMetaNode
{
    int Index;
    TextureMetaNode * Next;
};

struct D_Render_State
{
	
    unsigned int CurrentTexture;
	D_ShaderType PreviousShaderInput;
	D_ShaderType CurrentShaderInput;
    
    int TextureCount;
	Texture * AllTextures;
    String * AllTexturesName;
    
    unsigned int ScreenFrameBuffer;

    unsigned int GameWorldTexture;
	unsigned int GUITexture;
	unsigned int GameWorldDepthTexture;

	unsigned int LightingTexture;
	unsigned int BloomTexture;
	unsigned int FirstBlurTexture;
	unsigned int BlurTexture;

    //TODO : refactor these
	const char* ShaderUniformName[SU_COUNT];
	int ShaderUniformType[SU_COUNT];
	unsigned char * ShaderUniformData[SU_COUNT];

	float ElapsedTime;
	float FakeDepth;
};

global D_Render_State RenderState = {};

#define MAX_LIGHT 2048
struct D_Light_GPU_Data
{
    int LightCount;
    
    Vector3 * LightPosition;
    Vector4 *LightColor;
    Vector2 * LightSize;
    float * LightMul;
    Vector2 *LightTexCoord[Vertex_Count];
};

global D_Light_GPU_Data LightData ={};

struct D_Light
{
    Vector3 LightPosition;
    Vector2 LightSize;
    Vector4 LightColor;

	float LightStrength;
};

struct D_Vertex_Data
{
	unsigned short * Indices;
    
	Vector3 * Position;
	Vector4 * Rotation;
	Vector4 * Color;
	Vector2 * TexCoord;

	Vector3 * LineStart;
	Vector3 * LineEnd;
	float * LineSize;
	float * FakeDepth;

	Vector2 * Size;
	Vector4 * CornerRadius;

	Vector4 * CornerColor[Vertex_Count];
	Vector2 * CornerTexCoord[Vertex_Count];
};

global D_Vertex_Data VertexData = {};