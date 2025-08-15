//all of it is external code
//some of the struct defination was moved here for generate meta data

//i can't precompile it for no fucking reason
#define WIN32_LEAN_AND_MEAN
#include "Windows.h" //just Windows and its big window
#include "float.h"
#include "timeapi.h"
#include "direct.h"

#define GLAD_API_CALL_EXPORT

/*! @brief Opaque window object.
 *
 *  Opaque window object.
 *
 *  @see @ref window_object
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
struct GLFWwindow;

//linking gl function, i don't even know what gl version i'm using here lol
//i guess it was 3 or higher? i think i called some gl 4.0 function
//but it was compiled by raylib.lib so i can't tell
#include "external_include/glad.h" 
#include "external_include/glfw3.h" //me trying avoiding win32

#define GLFW_EXPOSE_NATIVE_WIN32
#include "external_include/glfw3native.h" //using win32 inside glfw (bitch then why don't you just use win32)

#include <wchar.h> //for chinese characters
#include <stdio.h> //just file io and printing stuff

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};


struct Vector4
{
	float x;
	float y;
	float z;
	float w;
};

struct Vector2
{
	float x;
	float y;
};

struct Vector3
{
	float x;
	float y;
	float z;
};

struct Quaternion
{
	float x;
	float y;
	float z;
	float w;
};

struct D_Rectangle {
    float x;
    float y;
    float width;
    float height;
};

struct Image {
    void *data;
    int width;
    int height;
    int mipmaps;
    int format;
};

// Texture, tex data stored in GPU memory (VRAM)
struct Texture {
    unsigned int id;        // OpenGL texture id
    int width;              // Texture base width
    int height;             // Texture base height
    int mipmaps;            // Mipmap levels, 1 by default
    int format;             // Data format (PixelFormat type)
};

// Texture2D, same as Texture
typedef Texture Texture2D;

// Camera, defines position/orientation in 3d space
struct Camera3D {
    Vector3 position;       // Camera position
    Vector3 target;         // Camera target it looks-at
    Vector3 up;             // Camera up vector (rotation over its axis)
    float fovy;             // Camera field-of-view aperture in Y (degrees) in perspective, used as near plane width in orthographic
    int projection;         // Camera projection: CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC
};

// Matrix, 4x4 components, column major, OpenGL style, right-handed
struct Matrix {
    float m0;
    float m4;
    float m8;
    float m12;  // Matrix first row (4 components)
    
    
    float m1;
    float m5;
    float m9;
    float m13;  // Matrix second row (4 components)
    
    
    float m2;
    float m6;
    float m10;
    float m14; // Matrix third row (4 components)
    
    
    
    float m3;
    float m7;
    float m11;
    float m15; // Matrix fourth row (4 components)
};

// Ray, ray for raycasting
struct Ray {
    Vector3 position;       // Ray position (origin)
    Vector3 direction;      // Ray direction
};

#include "external_include/raylib.h" //handy function for game programming
#include "external_include/rlgl.h" //handy function for opengl programming
#define RAYMATH_STATIC_INLINE
#include "external_include/raymath.h" //handy function for vector and math (don't fail your math examine)

struct stbtt__buf
{
    unsigned char *data;
    int cursor;
    int size;
};

struct stbtt_fontinfo
{
    void           * userdata;
    unsigned char  * data;              // pointer to .ttf file
    int              fontstart;         // offset of start of font
    
    int numGlyphs;                     // number of glyphs, needed for range checking
    
    int loca;
    int head;
    int glyf;
    int hhea; 
    int hmtx;
    int kern;
    int gpos;
    int svg; // table locations as offset from start of .ttf
    
    int index_map;                     // a cmap mapping for our chosen character encoding
    int indexToLocFormat;              // format needed to map from glyph index to glyph
    
    stbtt__buf cff;                    // cff font data
    stbtt__buf charstrings;            // the charstring index
    stbtt__buf gsubrs;                 // global charstring subroutines index
    stbtt__buf subrs;                  // private charstring subroutines index
    stbtt__buf fontdicts;              // array of font dicts
    stbtt__buf fdselect;               // map from glyph to fontdict
};

//////////////////////////////////////////////////////////////////////////////
//
// the details of the following structures don't matter to you, but they must
// be visible so you can handle the memory allocations for them

typedef int stbrp_coord;

struct stbrp_node
{
    stbrp_coord  x;
    stbrp_coord y;
    stbrp_node* next;
};

struct stbrp_context
{
    int width;
    int height;
    int align;
    int init_mode;
    int heuristic;
    int num_nodes;
    stbrp_node* active_head;
    stbrp_node* free_head;
    stbrp_node extra[2]; // we allocate two extra nodes so optimal user-node-count is 'width' not 'width+2'
};

#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h" //packing texture into texture atlas
#include "stb_truetype.h" //handling text
