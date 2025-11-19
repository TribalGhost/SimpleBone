typedef enum _type_meta _type_meta;
enum _type_meta
{
	_MT_float,
	_MT_double,
	_MT_bool,
	_MT_int,
	_MT_short,
	_MT_long,
	_MT_long_long,
	_MT_char,
	_MT_wchar_t,
	_MT_unsigned_int,
	_MT_unsigned_short,
	_MT_unsigned_long_long,
	_MT_unsigned_long,
	_MT_unsigned_char,
	_MT_void,
	_MT_HDC,
	_MT_Color,
	_MT_Vector4,
	_MT_Vector2,
	_MT_Vector3,
	_MT_Quaternion,
	_MT_D_Rectangle,
	_MT_Image,
	_MT_Texture,
	_MT_Texture2D,
	_MT_Camera3D,
	_MT_Matrix,
	_MT_Ray,
	_MT_stbtt__buf,
	_MT_stbtt_fontinfo,
	_MT_stbrp_coord,
	_MT_stbrp_node,
	_MT_stbrp_context,
	_MT_MemberMetaData,
	_MT_StructMetaData,
	_MT_StructMetaDataNode,
	_MT_FixedString,
	_MT_FixedStringW,
	_MT_DataHeader,
	_MT_TemporayMemory,
	_MT_AppWindow,
	_MT_ListNode,
	_MT_DummyNodeTag,
	_MT_List,
	_MT_Array,
	_MT_HashTableEntry,
	_MT_HashTableSlot,
	_MT_HashTable,
	_MT_QuadVertexPosition,
	_MT_Rect,
	_MT_Quad,
	_MT_D_GlyphInfo,
	_MT_D_GlyphInfoNode,
	_MT_D_FontContext,
	_MT_D_App_Data,
	_MT_D_ShaderUniform,
	_MT_D_ShaderType,
	_MT_D_ShaderCode,
	_MT_GLShaderType,
	_MT_ShaderInput,
	_MT_ShaderBuffer,
	_MT_String,
	_MT_DrawFlag,
	_MT_RenderState,
	_MT_D_Light_GPU_Data,
	_MT_D_Light,
	_MT_D_Vertex_Data,
	_MT_BoxFace,
	_MT_BoxVertex,
	_MT_GameMenuType,
	_MT_DrawingMenu,
	_MT_BoneState,
	_MT_KeyFrame,
	_MT_RotationAxis,
	_MT_RotationAxisData,
	_MT_Bone,
	_MT_AnimationTag,
	_MT_Clip,
	_MT_ClipBone,
	_MT_BoneSelection,
	_MT_Box,
	_MT_Face,
	_MT_DeformVertexSlice,
	_MT_DeformVertex,
	_MT_BoneSelectionResultData,
	_MT_BoneSelectionResult,
	_MT_D_Model,
	_MT_EditorType,
	_MT_EditorData,
	_MT_SplitViewport,
	_MT_GLFWwindow,
	_MT_GameLoadFunction,
	_MT_GameLoopFunction,
	_MT_GameUnloadFunction,
	_MT_type_count
};

const int _type_meta_size[] = 
{
sizeof(float),//float 
sizeof(double),//double 
sizeof(bool),//bool 
sizeof(int),//int 
sizeof(short),//short 
sizeof(long),//long 
sizeof(long long),//long long 
sizeof(char),//char 
sizeof(wchar_t),//wchar_t 
sizeof(unsigned int),//unsigned int 
sizeof(unsigned short),//unsigned short 
sizeof(unsigned long long),//unsigned long long 
sizeof(unsigned long),//unsigned long 
sizeof(unsigned char),//unsigned char 
0,//void 
sizeof(HDC),//HDC 
sizeof(Color),//Color 
sizeof(Vector4),//Vector4 
sizeof(Vector2),//Vector2 
sizeof(Vector3),//Vector3 
sizeof(Quaternion),//Quaternion 
sizeof(D_Rectangle),//D_Rectangle 
sizeof(Image),//Image 
sizeof(Texture),//Texture 
sizeof(Texture),//Texture2D 
sizeof(Camera3D),//Camera3D 
sizeof(Matrix),//Matrix 
sizeof(Ray),//Ray 
sizeof(stbtt__buf),//stbtt__buf 
sizeof(stbtt_fontinfo),//stbtt_fontinfo 
sizeof(int),//stbrp_coord 
sizeof(stbrp_node),//stbrp_node 
sizeof(stbrp_context),//stbrp_context 
sizeof(MemberMetaData),//MemberMetaData 
sizeof(StructMetaData),//StructMetaData 
sizeof(StructMetaDataNode),//StructMetaDataNode 
sizeof(FixedString),//FixedString 
sizeof(FixedStringW),//FixedStringW 
sizeof(DataHeader),//DataHeader 
sizeof(TemporayMemory),//TemporayMemory 
sizeof(AppWindow),//AppWindow 
sizeof(ListNode),//ListNode 
sizeof(DummyNodeTag),//DummyNodeTag 
sizeof(List),//List 
sizeof(Array),//Array 
sizeof(HashTableEntry),//HashTableEntry 
sizeof(HashTableSlot),//HashTableSlot 
sizeof(HashTable),//HashTable 
sizeof(QuadVertexPosition),//QuadVertexPosition 
sizeof(Rect),//Rect 
sizeof(Quad),//Quad 
sizeof(D_GlyphInfo),//D_GlyphInfo 
sizeof(D_GlyphInfoNode),//D_GlyphInfoNode 
sizeof(D_FontContext),//D_FontContext 
sizeof(D_App_Data),//D_App_Data 
sizeof(D_ShaderUniform),//D_ShaderUniform 
sizeof(D_ShaderType),//D_ShaderType 
sizeof(D_ShaderCode),//D_ShaderCode 
sizeof(GLShaderType),//GLShaderType 
sizeof(ShaderInput),//ShaderInput 
sizeof(ShaderBuffer),//ShaderBuffer 
sizeof(String),//String 
sizeof(DrawFlag),//DrawFlag 
sizeof(RenderState),//RenderState 
sizeof(D_Light_GPU_Data),//D_Light_GPU_Data 
sizeof(D_Light),//D_Light 
sizeof(D_Vertex_Data),//D_Vertex_Data 
sizeof(BoxFace),//BoxFace 
sizeof(BoxVertex),//BoxVertex 
sizeof(GameMenuType),//GameMenuType 
sizeof(DrawingMenu),//DrawingMenu 
sizeof(BoneState),//BoneState 
sizeof(KeyFrame),//KeyFrame 
sizeof(RotationAxis),//RotationAxis 
sizeof(RotationAxisData),//RotationAxisData 
sizeof(Bone),//Bone 
sizeof(AnimationTag),//AnimationTag 
sizeof(Clip),//Clip 
sizeof(ClipBone),//ClipBone 
sizeof(BoneSelection),//BoneSelection 
sizeof(Box),//Box 
sizeof(Face),//Face 
sizeof(DeformVertexSlice),//DeformVertexSlice 
sizeof(DeformVertex),//DeformVertex 
sizeof(BoneSelectionResultData),//BoneSelectionResultData 
sizeof(BoneSelectionResult),//BoneSelectionResult 
sizeof(D_Model),//D_Model 
sizeof(EditorType),//EditorType 
sizeof(EditorData),//EditorData 
sizeof(SplitViewport),//SplitViewport 
0,//GLFWwindow 
0,//GameLoadFunction 
0,//GameLoopFunction 
0,//GameUnloadFunction 
};

const char * _type_meta_name[] = 
{
	"float",
	"double",
	"bool",
	"int",
	"short",
	"long",
	"long long",
	"char",
	"wchar_t",
	"unsigned int",
	"unsigned short",
	"unsigned long long",
	"unsigned long",
	"unsigned char",
	"void",
	"HDC",
	"Color",
	"Vector4",
	"Vector2",
	"Vector3",
	"Quaternion",
	"D_Rectangle",
	"Image",
	"Texture",
	"Texture2D",
	"Camera3D",
	"Matrix",
	"Ray",
	"stbtt__buf",
	"stbtt_fontinfo",
	"stbrp_coord",
	"stbrp_node",
	"stbrp_context",
	"MemberMetaData",
	"StructMetaData",
	"StructMetaDataNode",
	"FixedString",
	"FixedStringW",
	"DataHeader",
	"TemporayMemory",
	"AppWindow",
	"ListNode",
	"DummyNodeTag",
	"List",
	"Array",
	"HashTableEntry",
	"HashTableSlot",
	"HashTable",
	"QuadVertexPosition",
	"Rect",
	"Quad",
	"D_GlyphInfo",
	"D_GlyphInfoNode",
	"D_FontContext",
	"D_App_Data",
	"D_ShaderUniform",
	"D_ShaderType",
	"D_ShaderCode",
	"GLShaderType",
	"ShaderInput",
	"ShaderBuffer",
	"String",
	"DrawFlag",
	"RenderState",
	"D_Light_GPU_Data",
	"D_Light",
	"D_Vertex_Data",
	"BoxFace",
	"BoxVertex",
	"GameMenuType",
	"DrawingMenu",
	"BoneState",
	"KeyFrame",
	"RotationAxis",
	"RotationAxisData",
	"Bone",
	"AnimationTag",
	"Clip",
	"ClipBone",
	"BoneSelection",
	"Box",
	"Face",
	"DeformVertexSlice",
	"DeformVertex",
	"BoneSelectionResultData",
	"BoneSelectionResult",
	"D_Model",
	"EditorType",
	"EditorData",
	"SplitViewport",
	"GLFWwindow",
	"GameLoadFunction",
	"GameLoopFunction",
	"GameUnloadFunction",
};

bool _is_type_enum[] = 
{
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
true,
false,
true,
false,
false,
false,
false,
false,
true,
false,
false,
false,
false,
false,
false,
true,
true,
false,
true,
false,
false,
false,
true,
false,
false,
false,
false,
true,
true,
true,
false,
false,
false,
true,
false,
false,
true,
false,
false,
false,
false,
false,
false,
false,
false,
false,
false,
true,
false,
false,
false,
false,
false,
false,
};

global const char * AppWindow_String[] = 
{
 "main_window",
 "window_count",
};

global const char * DummyNodeTag_String[] = 
{
 "N_node_head",
 "N_node_tail",
 "N_recycled_node_head",
 "N_recycled_node_tail",
 "invalid_node",
};

global const char * QuadVertexPosition_String[] = 
{
 "vertex_top_right",
 "vertex_top_left",
 "vertex_bottom_right",
 "vertex_bottom_left",
 "quad_vertex_count",
};

global const char * D_ShaderUniform_String[] = 
{
 "SU_MVP",
 "SU_texture_0",
 "SU_screen_size",
 "SU_grid_size",
 "SU_spread",
 "SU_elapsed_time",
 "SU_camera_position",
 "SU_camera_direction",
 "SU_draw_flag",
 "SU_count",
};

global const char * D_ShaderType_String[] = 
{
 "S_point_light",
 "S_rect",
 "S_quad",
 "S_line",
 "S_text",
 "S_quad_MS",
 "S_bloom",
 "S_blur_H",
 "S_blur_V",
 "S_spring",
 "S_count",
};

global const char * GLShaderType_String[] = 
{
 "GS_vert",
 "GS_geo",
 "GS_frag",
 "GS_count",
};

global const char * DrawFlag_String[] = 
{
 "DF_none",
 "DF_contour",
};

global const char * BoxFace_String[] = 
{
 "face_top",
 "face_bottom",
 "face_right",
 "face_left",
 "face_front",
 "face_back",
 "face_count",
};

global const char * BoxVertex_String[] = 
{
 "bv_A",
 "bv_B",
 "bv_C",
 "bv_D",
 "bv_E",
 "bv_F",
 "bv_G",
 "bv_H",
 "box_vertex_count",
};

global const char * GameMenuType_String[] = 
{
 "GMT_descend",
 "GMT_to_the_right",
};

global const char * RotationAxis_String[] = 
{
 "R_None",
 "R_Z",
 "R_Y",
 "R_X",
 "R_count",
};

global const char * AnimationTag_String[] = 
{
 "AT_foward",
 "AT_backward",
 "AT_right_forward",
 "AT_right_side",
 "AT_right_backward",
 "AT_left_forward",
 "AT_left_side",
 "AT_left_backward",
 "AT_idle",
 "AT_forward_right_offset",
 "AT_forward_left_offset",
 "AT_backward_right_offset",
 "AT_backward_left_offset",
 "AT_being_slap",
 "AT_slap",
 "animation_tag_count",
};

global const char * EditorType_String[] = 
{
 "edit_base_pose",
 "edit_animation",
 "edit_map",
 "edit_type_count",
};

typedef enum introspected_struct introspected_struct;
enum introspected_struct
{
	IS_Color, 
	IS_Vector4, 
	IS_Vector2, 
	IS_Vector3, 
	IS_Quaternion, 
	IS_D_Rectangle, 
	IS_Image, 
	IS_Texture, 
	IS_Camera3D, 
	IS_Matrix, 
	IS_Ray, 
	IS_stbtt__buf, 
	IS_stbtt_fontinfo, 
	IS_stbrp_node, 
	IS_stbrp_context, 
	IS_MemberMetaData, 
	IS_StructMetaData, 
	IS_StructMetaDataNode, 
	IS_FixedString, 
	IS_FixedStringW, 
	IS_DataHeader, 
	IS_TemporayMemory, 
	IS_ListNode, 
	IS_List, 
	IS_Array, 
	IS_HashTableEntry, 
	IS_HashTableSlot, 
	IS_HashTable, 
	IS_Rect, 
	IS_Quad, 
	IS_D_GlyphInfo, 
	IS_D_GlyphInfoNode, 
	IS_D_FontContext, 
	IS_D_App_Data, 
	IS_D_ShaderCode, 
	IS_ShaderInput, 
	IS_ShaderBuffer, 
	IS_String, 
	IS_RenderState, 
	IS_D_Light_GPU_Data, 
	IS_D_Light, 
	IS_D_Vertex_Data, 
	IS_DrawingMenu, 
	IS_BoneState, 
	IS_KeyFrame, 
	IS_RotationAxisData, 
	IS_Bone, 
	IS_Clip, 
	IS_ClipBone, 
	IS_BoneSelection, 
	IS_Box, 
	IS_Face, 
	IS_DeformVertexSlice, 
	IS_DeformVertex, 
	IS_BoneSelectionResultData, 
	IS_BoneSelectionResult, 
	IS_D_Model, 
	IS_EditorData, 
	IS_SplitViewport, 
};

global int member_meta_count_Color = 4;
global const MemberMetaData member_meta_Color[4] =
{
	{ "r" , false,false,false,0,_MT_unsigned_char,"unsigned_char",(int)&((Color *)0)->r ,sizeof(unsigned char),}, 
	{ "g" , false,false,false,0,_MT_unsigned_char,"unsigned_char",(int)&((Color *)0)->g ,sizeof(unsigned char),}, 
	{ "b" , false,false,false,0,_MT_unsigned_char,"unsigned_char",(int)&((Color *)0)->b ,sizeof(unsigned char),}, 
	{ "a" , false,false,false,0,_MT_unsigned_char,"unsigned_char",(int)&((Color *)0)->a ,sizeof(unsigned char),}, 
};

global int member_meta_count_Vector4 = 4;
global const MemberMetaData member_meta_Vector4[4] =
{
	{ "x" , false,false,false,0,_MT_float,"float",(int)&((Vector4 *)0)->x ,sizeof(float),}, 
	{ "y" , false,false,false,0,_MT_float,"float",(int)&((Vector4 *)0)->y ,sizeof(float),}, 
	{ "z" , false,false,false,0,_MT_float,"float",(int)&((Vector4 *)0)->z ,sizeof(float),}, 
	{ "w" , false,false,false,0,_MT_float,"float",(int)&((Vector4 *)0)->w ,sizeof(float),}, 
};

global int member_meta_count_Vector2 = 2;
global const MemberMetaData member_meta_Vector2[2] =
{
	{ "x" , false,false,false,0,_MT_float,"float",(int)&((Vector2 *)0)->x ,sizeof(float),}, 
	{ "y" , false,false,false,0,_MT_float,"float",(int)&((Vector2 *)0)->y ,sizeof(float),}, 
};

global int member_meta_count_Vector3 = 3;
global const MemberMetaData member_meta_Vector3[3] =
{
	{ "x" , false,false,false,0,_MT_float,"float",(int)&((Vector3 *)0)->x ,sizeof(float),}, 
	{ "y" , false,false,false,0,_MT_float,"float",(int)&((Vector3 *)0)->y ,sizeof(float),}, 
	{ "z" , false,false,false,0,_MT_float,"float",(int)&((Vector3 *)0)->z ,sizeof(float),}, 
};

global int member_meta_count_Quaternion = 4;
global const MemberMetaData member_meta_Quaternion[4] =
{
	{ "x" , false,false,false,0,_MT_float,"float",(int)&((Quaternion *)0)->x ,sizeof(float),}, 
	{ "y" , false,false,false,0,_MT_float,"float",(int)&((Quaternion *)0)->y ,sizeof(float),}, 
	{ "z" , false,false,false,0,_MT_float,"float",(int)&((Quaternion *)0)->z ,sizeof(float),}, 
	{ "w" , false,false,false,0,_MT_float,"float",(int)&((Quaternion *)0)->w ,sizeof(float),}, 
};

global int member_meta_count_D_Rectangle = 4;
global const MemberMetaData member_meta_D_Rectangle[4] =
{
	{ "x" , false,false,false,0,_MT_float,"float",(int)&((D_Rectangle *)0)->x ,sizeof(float),}, 
	{ "y" , false,false,false,0,_MT_float,"float",(int)&((D_Rectangle *)0)->y ,sizeof(float),}, 
	{ "width" , false,false,false,0,_MT_float,"float",(int)&((D_Rectangle *)0)->width ,sizeof(float),}, 
	{ "height" , false,false,false,0,_MT_float,"float",(int)&((D_Rectangle *)0)->height ,sizeof(float),}, 
};

global int member_meta_count_Image = 5;
global const MemberMetaData member_meta_Image[5] =
{
	{ "data" , false,true,false,0,_MT_void,"void",(int)&((Image *)0)->data ,sizeof(void *),}, 
	{ "width" , false,false,false,0,_MT_int,"int",(int)&((Image *)0)->width ,sizeof(int),}, 
	{ "height" , false,false,false,0,_MT_int,"int",(int)&((Image *)0)->height ,sizeof(int),}, 
	{ "mipmaps" , false,false,false,0,_MT_int,"int",(int)&((Image *)0)->mipmaps ,sizeof(int),}, 
	{ "format" , false,false,false,0,_MT_int,"int",(int)&((Image *)0)->format ,sizeof(int),}, 
};

global int member_meta_count_Texture = 5;
global const MemberMetaData member_meta_Texture[5] =
{
	{ "id" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((Texture *)0)->id ,sizeof(unsigned int),}, 
	{ "width" , false,false,false,0,_MT_int,"int",(int)&((Texture *)0)->width ,sizeof(int),}, 
	{ "height" , false,false,false,0,_MT_int,"int",(int)&((Texture *)0)->height ,sizeof(int),}, 
	{ "mipmaps" , false,false,false,0,_MT_int,"int",(int)&((Texture *)0)->mipmaps ,sizeof(int),}, 
	{ "format" , false,false,false,0,_MT_int,"int",(int)&((Texture *)0)->format ,sizeof(int),}, 
};

global int member_meta_count_Camera3D = 5;
global const MemberMetaData member_meta_Camera3D[5] =
{
	{ "position" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((Camera3D *)0)->position ,sizeof(Vector3),}, 
	{ "target" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((Camera3D *)0)->target ,sizeof(Vector3),}, 
	{ "up" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((Camera3D *)0)->up ,sizeof(Vector3),}, 
	{ "fovy" , false,false,false,0,_MT_float,"float",(int)&((Camera3D *)0)->fovy ,sizeof(float),}, 
	{ "projection" , false,false,false,0,_MT_int,"int",(int)&((Camera3D *)0)->projection ,sizeof(int),}, 
};

global int member_meta_count_Matrix = 16;
global const MemberMetaData member_meta_Matrix[16] =
{
	{ "m0" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m0 ,sizeof(float),}, 
	{ "m4" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m4 ,sizeof(float),}, 
	{ "m8" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m8 ,sizeof(float),}, 
	{ "m12" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m12 ,sizeof(float),}, 
	{ "m1" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m1 ,sizeof(float),}, 
	{ "m5" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m5 ,sizeof(float),}, 
	{ "m9" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m9 ,sizeof(float),}, 
	{ "m13" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m13 ,sizeof(float),}, 
	{ "m2" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m2 ,sizeof(float),}, 
	{ "m6" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m6 ,sizeof(float),}, 
	{ "m10" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m10 ,sizeof(float),}, 
	{ "m14" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m14 ,sizeof(float),}, 
	{ "m3" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m3 ,sizeof(float),}, 
	{ "m7" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m7 ,sizeof(float),}, 
	{ "m11" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m11 ,sizeof(float),}, 
	{ "m15" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m15 ,sizeof(float),}, 
};

global int member_meta_count_Ray = 2;
global const MemberMetaData member_meta_Ray[2] =
{
	{ "position" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((Ray *)0)->position ,sizeof(Vector3),}, 
	{ "direction" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((Ray *)0)->direction ,sizeof(Vector3),}, 
};

global int member_meta_count_stbtt__buf = 3;
global const MemberMetaData member_meta_stbtt__buf[3] =
{
	{ "data" , false,true,false,0,_MT_unsigned_char,"unsigned_char",(int)&((stbtt__buf *)0)->data ,sizeof(void *),}, 
	{ "cursor" , false,false,false,0,_MT_int,"int",(int)&((stbtt__buf *)0)->cursor ,sizeof(int),}, 
	{ "size" , false,false,false,0,_MT_int,"int",(int)&((stbtt__buf *)0)->size ,sizeof(int),}, 
};

global int member_meta_count_stbtt_fontinfo = 20;
global const MemberMetaData member_meta_stbtt_fontinfo[20] =
{
	{ "userdata" , false,true,false,0,_MT_void,"void",(int)&((stbtt_fontinfo *)0)->userdata ,sizeof(void *),}, 
	{ "data" , false,true,false,0,_MT_unsigned_char,"unsigned_char",(int)&((stbtt_fontinfo *)0)->data ,sizeof(void *),}, 
	{ "fontstart" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->fontstart ,sizeof(int),}, 
	{ "numGlyphs" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->numGlyphs ,sizeof(int),}, 
	{ "loca" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->loca ,sizeof(int),}, 
	{ "head" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->head ,sizeof(int),}, 
	{ "glyf" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->glyf ,sizeof(int),}, 
	{ "hhea" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->hhea ,sizeof(int),}, 
	{ "hmtx" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->hmtx ,sizeof(int),}, 
	{ "kern" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->kern ,sizeof(int),}, 
	{ "gpos" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->gpos ,sizeof(int),}, 
	{ "svg" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->svg ,sizeof(int),}, 
	{ "index_map" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->index_map ,sizeof(int),}, 
	{ "indexToLocFormat" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->indexToLocFormat ,sizeof(int),}, 
	{ "cff" , false,false,false,0,_MT_stbtt__buf,"stbtt__buf",(int)&((stbtt_fontinfo *)0)->cff ,sizeof(stbtt__buf),}, 
	{ "charstrings" , false,false,false,0,_MT_stbtt__buf,"stbtt__buf",(int)&((stbtt_fontinfo *)0)->charstrings ,sizeof(stbtt__buf),}, 
	{ "gsubrs" , false,false,false,0,_MT_stbtt__buf,"stbtt__buf",(int)&((stbtt_fontinfo *)0)->gsubrs ,sizeof(stbtt__buf),}, 
	{ "subrs" , false,false,false,0,_MT_stbtt__buf,"stbtt__buf",(int)&((stbtt_fontinfo *)0)->subrs ,sizeof(stbtt__buf),}, 
	{ "fontdicts" , false,false,false,0,_MT_stbtt__buf,"stbtt__buf",(int)&((stbtt_fontinfo *)0)->fontdicts ,sizeof(stbtt__buf),}, 
	{ "fdselect" , false,false,false,0,_MT_stbtt__buf,"stbtt__buf",(int)&((stbtt_fontinfo *)0)->fdselect ,sizeof(stbtt__buf),}, 
};

global int member_meta_count_stbrp_node = 3;
global const MemberMetaData member_meta_stbrp_node[3] =
{
	{ "x" , false,false,false,0,_MT_stbrp_coord,"stbrp_coord",(int)&((stbrp_node *)0)->x ,sizeof(stbrp_coord),}, 
	{ "y" , false,false,false,0,_MT_stbrp_coord,"stbrp_coord",(int)&((stbrp_node *)0)->y ,sizeof(stbrp_coord),}, 
	{ "next" , false,true,false,0,_MT_stbrp_node,"stbrp_node",(int)&((stbrp_node *)0)->next ,sizeof(void *),}, 
};

global int member_meta_count_stbrp_context = 9;
global const MemberMetaData member_meta_stbrp_context[9] =
{
	{ "width" , false,false,false,0,_MT_int,"int",(int)&((stbrp_context *)0)->width ,sizeof(int),}, 
	{ "height" , false,false,false,0,_MT_int,"int",(int)&((stbrp_context *)0)->height ,sizeof(int),}, 
	{ "align" , false,false,false,0,_MT_int,"int",(int)&((stbrp_context *)0)->align ,sizeof(int),}, 
	{ "init_mode" , false,false,false,0,_MT_int,"int",(int)&((stbrp_context *)0)->init_mode ,sizeof(int),}, 
	{ "heuristic" , false,false,false,0,_MT_int,"int",(int)&((stbrp_context *)0)->heuristic ,sizeof(int),}, 
	{ "num_nodes" , false,false,false,0,_MT_int,"int",(int)&((stbrp_context *)0)->num_nodes ,sizeof(int),}, 
	{ "active_head" , false,true,false,0,_MT_stbrp_node,"stbrp_node",(int)&((stbrp_context *)0)->active_head ,sizeof(void *),}, 
	{ "free_head" , false,true,false,0,_MT_stbrp_node,"stbrp_node",(int)&((stbrp_context *)0)->free_head ,sizeof(void *),}, 
	{ "extra" , false,false,true,2,_MT_stbrp_node,"stbrp_node",(int)&((stbrp_context *)0)->extra ,sizeof(stbrp_node),}, 
};

global int member_meta_count_MemberMetaData = 9;
global const MemberMetaData member_meta_MemberMetaData[9] =
{
	{ "name" , false,false,true,64,_MT_char,"char",(int)&((MemberMetaData *)0)->name ,sizeof(char),}, 
	{ "multiple_pointer" , false,false,false,0,_MT_bool,"bool",(int)&((MemberMetaData *)0)->multiple_pointer ,sizeof(bool),}, 
	{ "is_pointer" , false,false,false,0,_MT_bool,"bool",(int)&((MemberMetaData *)0)->is_pointer ,sizeof(bool),}, 
	{ "is_array" , false,false,false,0,_MT_bool,"bool",(int)&((MemberMetaData *)0)->is_array ,sizeof(bool),}, 
	{ "array_size" , false,false,false,0,_MT_int,"int",(int)&((MemberMetaData *)0)->array_size ,sizeof(int),}, 
	{ "member_type" , false,false,false,0,_MT_int,"int",(int)&((MemberMetaData *)0)->member_type ,sizeof(int),}, 
	{ "type_name" , false,false,true,64,_MT_char,"char",(int)&((MemberMetaData *)0)->type_name ,sizeof(char),}, 
	{ "member_offset" , false,false,false,0,_MT_int,"int",(int)&((MemberMetaData *)0)->member_offset ,sizeof(int),}, 
	{ "member_size" , false,false,false,0,_MT_int,"int",(int)&((MemberMetaData *)0)->member_size ,sizeof(int),}, 
};

global int member_meta_count_StructMetaData = 2;
global const MemberMetaData member_meta_StructMetaData[2] =
{
	{ "member_array" , false,true,false,0,_MT_MemberMetaData,"MemberMetaData",(int)&((StructMetaData *)0)->member_array ,sizeof(void *),}, 
	{ "member_count" , false,false,false,0,_MT_int,"int",(int)&((StructMetaData *)0)->member_count ,sizeof(int),}, 
};

global int member_meta_count_StructMetaDataNode = 6;
global const MemberMetaData member_meta_StructMetaDataNode[6] =
{
	{ "name" , false,true,false,0,_MT_char,"char",(int)&((StructMetaDataNode *)0)->name ,sizeof(void *),}, 
	{ "type_index" , false,false,false,0,_MT_int,"int",(int)&((StructMetaDataNode *)0)->type_index ,sizeof(int),}, 
	{ "size" , false,false,false,0,_MT_int,"int",(int)&((StructMetaDataNode *)0)->size ,sizeof(int),}, 
	{ "introspected" , false,false,false,0,_MT_bool,"bool",(int)&((StructMetaDataNode *)0)->introspected ,sizeof(bool),}, 
	{ "struct_meta" , false,false,false,0,_MT_StructMetaData,"StructMetaData",(int)&((StructMetaDataNode *)0)->struct_meta ,sizeof(StructMetaData),}, 
	{ "next" , false,true,false,0,_MT_StructMetaDataNode,"StructMetaDataNode",(int)&((StructMetaDataNode *)0)->next ,sizeof(void *),}, 
};

global int member_meta_count_FixedString = 1;
global const MemberMetaData member_meta_FixedString[1] =
{
	{ "string" , false,false,true,FIXED_STRING_SIZE,_MT_char,"char",(int)&((FixedString *)0)->string ,sizeof(char),}, 
};

global int member_meta_count_FixedStringW = 1;
global const MemberMetaData member_meta_FixedStringW[1] =
{
	{ "string" , false,false,true,FIXED_STRING_SIZE,_MT_wchar_t,"wchar_t",(int)&((FixedStringW *)0)->string ,sizeof(wchar_t),}, 
};

global int member_meta_count_DataHeader = 2;
global const MemberMetaData member_meta_DataHeader[2] =
{
	{ "name" , false,false,false,0,_MT_FixedString,"FixedString",(int)&((DataHeader *)0)->name ,sizeof(FixedString),}, 
	{ "data_offset" , false,false,false,0,_MT_int,"int",(int)&((DataHeader *)0)->data_offset ,sizeof(int),}, 
};

global int member_meta_count_TemporayMemory = 3;
global const MemberMetaData member_meta_TemporayMemory[3] =
{
	{ "start_memory" , false,true,false,0,_MT_unsigned_char,"unsigned_char",(int)&((TemporayMemory *)0)->start_memory ,sizeof(void *),}, 
	{ "current_memory" , false,true,false,0,_MT_unsigned_char,"unsigned_char",(int)&((TemporayMemory *)0)->current_memory ,sizeof(void *),}, 
	{ "size" , false,false,false,0,_MT_int,"int",(int)&((TemporayMemory *)0)->size ,sizeof(int),}, 
};

global int member_meta_count_ListNode = 3;
global const MemberMetaData member_meta_ListNode[3] =
{
	{ "data_index" , false,false,false,0,_MT_int,"int",(int)&((ListNode *)0)->data_index ,sizeof(int),}, 
	{ "next" , false,false,false,0,_MT_int,"int",(int)&((ListNode *)0)->next ,sizeof(int),}, 
	{ "previous" , false,false,false,0,_MT_int,"int",(int)&((ListNode *)0)->previous ,sizeof(int),}, 
};

global int member_meta_count_List = 4;
global const MemberMetaData member_meta_List[4] =
{
	{ "all_node" , false,true,false,0,_MT_ListNode,"ListNode",(int)&((List *)0)->all_node ,sizeof(void *),}, 
	{ "node_array" , false,true,false,0,_MT_ListNode,"ListNode",(int)&((List *)0)->node_array ,sizeof(void *),}, 
	{ "unuse_index" , false,false,false,0,_MT_int,"int",(int)&((List *)0)->unuse_index ,sizeof(int),}, 
	{ "count" , false,false,false,0,_MT_int,"int",(int)&((List *)0)->count ,sizeof(int),}, 
};

global int member_meta_count_Array = 4;
global const MemberMetaData member_meta_Array[4] =
{
	{ "valid_array" , false,true,false,0,_MT_bool,"bool",(int)&((Array *)0)->valid_array ,sizeof(void *),}, 
	{ "capacity" , false,false,false,0,_MT_int,"int",(int)&((Array *)0)->capacity ,sizeof(int),}, 
	{ "count" , false,false,false,0,_MT_int,"int",(int)&((Array *)0)->count ,sizeof(int),}, 
	{ "lowest_index" , false,false,false,0,_MT_int,"int",(int)&((Array *)0)->lowest_index ,sizeof(int),}, 
};

global int member_meta_count_HashTableEntry = 2;
global const MemberMetaData member_meta_HashTableEntry[2] =
{
	{ "head_index" , false,false,false,0,_MT_int,"int",(int)&((HashTableEntry *)0)->head_index ,sizeof(int),}, 
	{ "tail_index" , false,false,false,0,_MT_int,"int",(int)&((HashTableEntry *)0)->tail_index ,sizeof(int),}, 
};

global int member_meta_count_HashTableSlot = 5;
global const MemberMetaData member_meta_HashTableSlot[5] =
{
	{ "valid" , false,false,false,0,_MT_bool,"bool",(int)&((HashTableSlot *)0)->valid ,sizeof(bool),}, 
	{ "slot_value" , false,false,false,0,_MT_int,"int",(int)&((HashTableSlot *)0)->slot_value ,sizeof(int),}, 
	{ "data_index" , false,false,false,0,_MT_int,"int",(int)&((HashTableSlot *)0)->data_index ,sizeof(int),}, 
	{ "next_index" , false,false,false,0,_MT_int,"int",(int)&((HashTableSlot *)0)->next_index ,sizeof(int),}, 
	{ "previous_index" , false,false,false,0,_MT_int,"int",(int)&((HashTableSlot *)0)->previous_index ,sizeof(int),}, 
};

global int member_meta_count_HashTable = 3;
global const MemberMetaData member_meta_HashTable[3] =
{
	{ "entry_array" , false,true,false,0,_MT_HashTableEntry,"HashTableEntry",(int)&((HashTable *)0)->entry_array ,sizeof(void *),}, 
	{ "slot_array" , false,true,false,0,_MT_HashTableSlot,"HashTableSlot",(int)&((HashTable *)0)->slot_array ,sizeof(void *),}, 
	{ "count" , false,false,false,0,_MT_int,"int",(int)&((HashTable *)0)->count ,sizeof(int),}, 
};

global int member_meta_count_Rect = 3;
global const MemberMetaData member_meta_Rect[3] =
{
	{ "position" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((Rect *)0)->position ,sizeof(Vector3),}, 
	{ "size" , false,false,false,0,_MT_Vector2,"Vector2",(int)&((Rect *)0)->size ,sizeof(Vector2),}, 
	{ "rotation" , false,false,false,0,_MT_Quaternion,"Quaternion",(int)&((Rect *)0)->rotation ,sizeof(Quaternion),}, 
};

global int member_meta_count_Quad = 2;
global const MemberMetaData member_meta_Quad[2] =
{
	{ "vertex_position" , false,false,true,quad_vertex_count,_MT_Vector3,"Vector3",(int)&((Quad *)0)->vertex_position ,sizeof(Vector3),}, 
	{ "vertex_color" , false,false,true,quad_vertex_count,_MT_Vector4,"Vector4",(int)&((Quad *)0)->vertex_color ,sizeof(Vector4),}, 
};

global int member_meta_count_D_GlyphInfo = 5;
global const MemberMetaData member_meta_D_GlyphInfo[5] =
{
	{ "value" , false,false,false,0,_MT_int,"int",(int)&((D_GlyphInfo *)0)->value ,sizeof(int),}, 
	{ "offsetX" , false,false,false,0,_MT_int,"int",(int)&((D_GlyphInfo *)0)->offsetX ,sizeof(int),}, 
	{ "offsetY" , false,false,false,0,_MT_int,"int",(int)&((D_GlyphInfo *)0)->offsetY ,sizeof(int),}, 
	{ "advanceX" , false,false,false,0,_MT_int,"int",(int)&((D_GlyphInfo *)0)->advanceX ,sizeof(int),}, 
	{ "glyph_rect" , false,false,false,0,_MT_D_Rectangle,"D_Rectangle",(int)&((D_GlyphInfo *)0)->glyph_rect ,sizeof(D_Rectangle),}, 
};

global int member_meta_count_D_GlyphInfoNode = 2;
global const MemberMetaData member_meta_D_GlyphInfoNode[2] =
{
	{ "info" , false,false,false,0,_MT_D_GlyphInfo,"D_GlyphInfo",(int)&((D_GlyphInfoNode *)0)->info ,sizeof(D_GlyphInfo),}, 
	{ "next_node" , false,true,false,0,_MT_D_GlyphInfoNode,"D_GlyphInfoNode",(int)&((D_GlyphInfoNode *)0)->next_node ,sizeof(void *),}, 
};

global int member_meta_count_D_FontContext = 10;
global const MemberMetaData member_meta_D_FontContext[10] =
{
	{ "font_info" , false,false,false,0,_MT_stbtt_fontinfo,"stbtt_fontinfo",(int)&((D_FontContext *)0)->font_info ,sizeof(stbtt_fontinfo),}, 
	{ "glyph_atlas_image" , false,false,false,0,_MT_Image,"Image",(int)&((D_FontContext *)0)->glyph_atlas_image ,sizeof(Image),}, 
	{ "glyph_atlas_padding" , false,false,false,0,_MT_int,"int",(int)&((D_FontContext *)0)->glyph_atlas_padding ,sizeof(int),}, 
	{ "glyph_resolution" , false,false,false,0,_MT_int,"int",(int)&((D_FontContext *)0)->glyph_resolution ,sizeof(int),}, 
	{ "all_glyph_info" , false,true,false,0,_MT_D_GlyphInfoNode,"D_GlyphInfoNode",(int)&((D_FontContext *)0)->all_glyph_info ,sizeof(void *),}, 
	{ "all_glyph_info_count" , false,false,false,0,_MT_int,"int",(int)&((D_FontContext *)0)->all_glyph_info_count ,sizeof(int),}, 
	{ "glyph_hash_map" , true,true,false,0,_MT_D_GlyphInfoNode,"D_GlyphInfoNode",(int)&((D_FontContext *)0)->glyph_hash_map ,sizeof(void *),}, 
	{ "context" , false,false,false,0,_MT_stbrp_context,"stbrp_context",(int)&((D_FontContext *)0)->context ,sizeof(stbrp_context),}, 
	{ "nodes" , false,true,false,0,_MT_stbrp_node,"stbrp_node",(int)&((D_FontContext *)0)->nodes ,sizeof(void *),}, 
	{ "glyph_atlas_texture" , false,false,false,0,_MT_Texture2D,"Texture2D",(int)&((D_FontContext *)0)->glyph_atlas_texture ,sizeof(Texture2D),}, 
};

global int member_meta_count_D_App_Data = 32;
global const MemberMetaData member_meta_D_App_Data[32] =
{
	{ "run_time_memory" , false,false,false,0,_MT_TemporayMemory,"TemporayMemory",(int)&((D_App_Data *)0)->run_time_memory ,sizeof(TemporayMemory),}, 
	{ "frame_time_memory" , false,false,false,0,_MT_TemporayMemory,"TemporayMemory",(int)&((D_App_Data *)0)->frame_time_memory ,sizeof(TemporayMemory),}, 
	{ "loop_count" , false,false,false,0,_MT_int,"int",(int)&((D_App_Data *)0)->loop_count ,sizeof(int),}, 
	{ "game_unload" , false,true,false,0,_MT_GameUnloadFunction,"GameUnloadFunction",(int)&((D_App_Data *)0)->game_unload ,sizeof(void *),}, 
	{ "game_loop" , false,true,false,0,_MT_GameLoopFunction,"GameLoopFunction",(int)&((D_App_Data *)0)->game_loop ,sizeof(void *),}, 
	{ "loop_time" , false,false,false,0,_MT_double,"double",(int)&((D_App_Data *)0)->loop_time ,sizeof(double),}, 
	{ "top" , false,false,false,0,_MT_double,"double",(int)&((D_App_Data *)0)->top ,sizeof(double),}, 
	{ "right" , false,false,false,0,_MT_double,"double",(int)&((D_App_Data *)0)->right ,sizeof(double),}, 
	{ "mouse_position" , false,false,false,0,_MT_Vector2,"Vector2",(int)&((D_App_Data *)0)->mouse_position ,sizeof(Vector2),}, 
	{ "default_font" , false,false,false,0,_MT_D_FontContext,"D_FontContext",(int)&((D_App_Data *)0)->default_font ,sizeof(D_FontContext),}, 
	{ "all_pressing_key" , false,false,true,ALL_KEY,_MT_bool,"bool",(int)&((D_App_Data *)0)->all_pressing_key ,sizeof(bool),}, 
	{ "all_pressing_mouse" , false,false,true,ALL_MOUSE_KEY,_MT_bool,"bool",(int)&((D_App_Data *)0)->all_pressing_mouse ,sizeof(bool),}, 
	{ "released_mouse_array" , false,false,true,MAX_KEY,_MT_int,"int",(int)&((D_App_Data *)0)->released_mouse_array ,sizeof(int),}, 
	{ "released_mouse_array_count" , false,false,false,0,_MT_int,"int",(int)&((D_App_Data *)0)->released_mouse_array_count ,sizeof(int),}, 
	{ "pressed_mouse_array" , false,false,true,MAX_KEY,_MT_int,"int",(int)&((D_App_Data *)0)->pressed_mouse_array ,sizeof(int),}, 
	{ "pressed_mouse_array_count" , false,false,false,0,_MT_int,"int",(int)&((D_App_Data *)0)->pressed_mouse_array_count ,sizeof(int),}, 
	{ "pressed_key_array" , false,false,true,MAX_KEY,_MT_int,"int",(int)&((D_App_Data *)0)->pressed_key_array ,sizeof(int),}, 
	{ "pressed_key_array_count" , false,false,false,0,_MT_int,"int",(int)&((D_App_Data *)0)->pressed_key_array_count ,sizeof(int),}, 
	{ "released_key_array" , false,false,true,MAX_KEY,_MT_int,"int",(int)&((D_App_Data *)0)->released_key_array ,sizeof(int),}, 
	{ "released_key_array_count" , false,false,false,0,_MT_int,"int",(int)&((D_App_Data *)0)->released_key_array_count ,sizeof(int),}, 
	{ "codepoint_queue_indedx" , false,false,false,0,_MT_int,"int",(int)&((D_App_Data *)0)->codepoint_queue_indedx ,sizeof(int),}, 
	{ "codepoint_queue_count" , false,false,false,0,_MT_int,"int",(int)&((D_App_Data *)0)->codepoint_queue_count ,sizeof(int),}, 
	{ "codepoint_queue" , false,false,true,64,_MT_unsigned_int,"unsigned_int",(int)&((D_App_Data *)0)->codepoint_queue ,sizeof(unsigned int),}, 
	{ "current_window" , false,true,false,0,_MT_GLFWwindow,"GLFWwindow",(int)&((D_App_Data *)0)->current_window ,sizeof(void *),}, 
	{ "current_focus_window" , false,true,false,0,_MT_GLFWwindow,"GLFWwindow",(int)&((D_App_Data *)0)->current_focus_window ,sizeof(void *),}, 
	{ "window_size" , false,false,false,0,_MT_Vector2,"Vector2",(int)&((D_App_Data *)0)->window_size ,sizeof(Vector2),}, 
	{ "exit_window" , false,false,false,0,_MT_bool,"bool",(int)&((D_App_Data *)0)->exit_window ,sizeof(bool),}, 
	{ "update_atlas" , false,false,false,0,_MT_bool,"bool",(int)&((D_App_Data *)0)->update_atlas ,sizeof(bool),}, 
	{ "mouse_scroll_delta" , false,false,false,0,_MT_float,"float",(int)&((D_App_Data *)0)->mouse_scroll_delta ,sizeof(float),}, 
	{ "window_text_size" , false,false,false,0,_MT_float,"float",(int)&((D_App_Data *)0)->window_text_size ,sizeof(float),}, 
	{ "aplication_path" , false,false,true,MAX_FILE_PATH,_MT_char,"char",(int)&((D_App_Data *)0)->aplication_path ,sizeof(char),}, 
	{ "_HDC" , false,false,false,0,_MT_HDC,"HDC",(int)&((D_App_Data *)0)->_HDC ,sizeof(HDC),}, 
};

global int member_meta_count_D_ShaderCode = 2;
global const MemberMetaData member_meta_D_ShaderCode[2] =
{
	{ "fragment_shader_code" , false,true,false,0,_MT_char,"char",(int)&((D_ShaderCode *)0)->fragment_shader_code ,sizeof(void *),}, 
	{ "vertex_shader_code" , false,true,false,0,_MT_char,"char",(int)&((D_ShaderCode *)0)->vertex_shader_code ,sizeof(void *),}, 
};

global int member_meta_count_ShaderInput = 8;
global const MemberMetaData member_meta_ShaderInput[8] =
{
	{ "shader_type" , false,false,false,0,_MT_D_ShaderType,"D_ShaderType",(int)&((ShaderInput *)0)->shader_type ,sizeof(D_ShaderType),}, 
	{ "shader_source_file_name" , false,true,true,GS_count,_MT_char,"char",(int)&((ShaderInput *)0)->shader_source_file_name ,sizeof(void *),}, 
	{ "shader" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((ShaderInput *)0)->shader ,sizeof(unsigned int),}, 
	{ "vertex_array_object" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((ShaderInput *)0)->vertex_array_object ,sizeof(unsigned int),}, 
	{ "vertex_buffer_location_array" , false,true,false,0,_MT_int,"int",(int)&((ShaderInput *)0)->vertex_buffer_location_array ,sizeof(void *),}, 
	{ "uniform_location" , false,false,true,SU_count,_MT_int,"int",(int)&((ShaderInput *)0)->uniform_location ,sizeof(int),}, 
	{ "vertices_count" , false,false,false,0,_MT_int,"int",(int)&((ShaderInput *)0)->vertices_count ,sizeof(int),}, 
	{ "indices_count" , false,false,false,0,_MT_int,"int",(int)&((ShaderInput *)0)->indices_count ,sizeof(int),}, 
};

global int member_meta_count_ShaderBuffer = 10;
global const MemberMetaData member_meta_ShaderBuffer[10] =
{
	{ "name" , false,true,false,0,_MT_char,"char",(int)&((ShaderBuffer *)0)->name ,sizeof(void *),}, 
	{ "primitive_data_type" , false,false,false,0,_MT_int,"int",(int)&((ShaderBuffer *)0)->primitive_data_type ,sizeof(int),}, 
	{ "components_count" , false,false,false,0,_MT_int,"int",(int)&((ShaderBuffer *)0)->components_count ,sizeof(int),}, 
	{ "max_data_size" , false,false,false,0,_MT_int,"int",(int)&((ShaderBuffer *)0)->max_data_size ,sizeof(int),}, 
	{ "primitive_data_size" , false,false,false,0,_MT_int,"int",(int)&((ShaderBuffer *)0)->primitive_data_size ,sizeof(int),}, 
	{ "is_uniform_buffer" , false,false,false,0,_MT_bool,"bool",(int)&((ShaderBuffer *)0)->is_uniform_buffer ,sizeof(bool),}, 
	{ "is_indices_buffer" , false,false,false,0,_MT_bool,"bool",(int)&((ShaderBuffer *)0)->is_indices_buffer ,sizeof(bool),}, 
	{ "data" , false,true,false,0,_MT_unsigned_char,"unsigned_char",(int)&((ShaderBuffer *)0)->data ,sizeof(void *),}, 
	{ "buffer_size" , false,false,false,0,_MT_int,"int",(int)&((ShaderBuffer *)0)->buffer_size ,sizeof(int),}, 
	{ "buffer_handle" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((ShaderBuffer *)0)->buffer_handle ,sizeof(unsigned int),}, 
};

global int member_meta_count_String = 2;
global const MemberMetaData member_meta_String[2] =
{
	{ "start" , false,true,false,0,_MT_wchar_t,"wchar_t",(int)&((String *)0)->start ,sizeof(void *),}, 
	{ "count" , false,false,false,0,_MT_int,"int",(int)&((String *)0)->count ,sizeof(int),}, 
};

global int member_meta_count_RenderState = 16;
global const MemberMetaData member_meta_RenderState[16] =
{
	{ "current_texture" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((RenderState *)0)->current_texture ,sizeof(unsigned int),}, 
	{ "previous_shader_input" , false,false,false,0,_MT_D_ShaderType,"D_ShaderType",(int)&((RenderState *)0)->previous_shader_input ,sizeof(D_ShaderType),}, 
	{ "current_shader_input" , false,false,false,0,_MT_D_ShaderType,"D_ShaderType",(int)&((RenderState *)0)->current_shader_input ,sizeof(D_ShaderType),}, 
	{ "default_white_image" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((RenderState *)0)->default_white_image ,sizeof(unsigned int),}, 
	{ "missing_texture" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((RenderState *)0)->missing_texture ,sizeof(unsigned int),}, 
	{ "draw_flag" , false,false,false,0,_MT_int,"int",(int)&((RenderState *)0)->draw_flag ,sizeof(int),}, 
	{ "screen_frame_buffer" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((RenderState *)0)->screen_frame_buffer ,sizeof(unsigned int),}, 
	{ "interface_texture" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((RenderState *)0)->interface_texture ,sizeof(unsigned int),}, 
	{ "game_world_texture" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((RenderState *)0)->game_world_texture ,sizeof(unsigned int),}, 
	{ "game_world_depth_texture" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((RenderState *)0)->game_world_depth_texture ,sizeof(unsigned int),}, 
	{ "shader_uniform_name" , false,true,true,SU_count,_MT_char,"char",(int)&((RenderState *)0)->shader_uniform_name ,sizeof(void *),}, 
	{ "shader_uniform_type" , false,false,true,SU_count,_MT_int,"int",(int)&((RenderState *)0)->shader_uniform_type ,sizeof(int),}, 
	{ "shader_uniform_data" , false,true,true,SU_count,_MT_unsigned_char,"unsigned_char",(int)&((RenderState *)0)->shader_uniform_data ,sizeof(void *),}, 
	{ "elapsed_time" , false,false,false,0,_MT_float,"float",(int)&((RenderState *)0)->elapsed_time ,sizeof(float),}, 
	{ "fake_depth" , false,false,false,0,_MT_float,"float",(int)&((RenderState *)0)->fake_depth ,sizeof(float),}, 
	{ "texture_list_to_delete" , false,false,false,0,_MT_List,"List",(int)&((RenderState *)0)->texture_list_to_delete ,sizeof(List),}, 
};

global int member_meta_count_D_Light_GPU_Data = 6;
global const MemberMetaData member_meta_D_Light_GPU_Data[6] =
{
	{ "light_count" , false,false,false,0,_MT_int,"int",(int)&((D_Light_GPU_Data *)0)->light_count ,sizeof(int),}, 
	{ "light_position" , false,true,false,0,_MT_Vector3,"Vector3",(int)&((D_Light_GPU_Data *)0)->light_position ,sizeof(void *),}, 
	{ "light_color" , false,true,false,0,_MT_Vector4,"Vector4",(int)&((D_Light_GPU_Data *)0)->light_color ,sizeof(void *),}, 
	{ "light_size" , false,true,false,0,_MT_Vector2,"Vector2",(int)&((D_Light_GPU_Data *)0)->light_size ,sizeof(void *),}, 
	{ "light_mul" , false,true,false,0,_MT_float,"float",(int)&((D_Light_GPU_Data *)0)->light_mul ,sizeof(void *),}, 
	{ "light_tex_coord" , false,true,true,quad_vertex_count,_MT_Vector2,"Vector2",(int)&((D_Light_GPU_Data *)0)->light_tex_coord ,sizeof(void *),}, 
};

global int member_meta_count_D_Light = 4;
global const MemberMetaData member_meta_D_Light[4] =
{
	{ "light_position" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((D_Light *)0)->light_position ,sizeof(Vector3),}, 
	{ "light_size" , false,false,false,0,_MT_Vector2,"Vector2",(int)&((D_Light *)0)->light_size ,sizeof(Vector2),}, 
	{ "light_color" , false,false,false,0,_MT_Vector4,"Vector4",(int)&((D_Light *)0)->light_color ,sizeof(Vector4),}, 
	{ "light_strength" , false,false,false,0,_MT_float,"float",(int)&((D_Light *)0)->light_strength ,sizeof(float),}, 
};

global int member_meta_count_D_Vertex_Data = 14;
global const MemberMetaData member_meta_D_Vertex_Data[14] =
{
	{ "indices" , false,true,false,0,_MT_unsigned_short,"unsigned_short",(int)&((D_Vertex_Data *)0)->indices ,sizeof(void *),}, 
	{ "position" , false,true,false,0,_MT_Vector3,"Vector3",(int)&((D_Vertex_Data *)0)->position ,sizeof(void *),}, 
	{ "normal" , false,true,false,0,_MT_Vector3,"Vector3",(int)&((D_Vertex_Data *)0)->normal ,sizeof(void *),}, 
	{ "rotation" , false,true,false,0,_MT_Vector4,"Vector4",(int)&((D_Vertex_Data *)0)->rotation ,sizeof(void *),}, 
	{ "color" , false,true,false,0,_MT_Vector4,"Vector4",(int)&((D_Vertex_Data *)0)->color ,sizeof(void *),}, 
	{ "tex_coord" , false,true,false,0,_MT_Vector2,"Vector2",(int)&((D_Vertex_Data *)0)->tex_coord ,sizeof(void *),}, 
	{ "line_start" , false,true,false,0,_MT_Vector3,"Vector3",(int)&((D_Vertex_Data *)0)->line_start ,sizeof(void *),}, 
	{ "line_end" , false,true,false,0,_MT_Vector3,"Vector3",(int)&((D_Vertex_Data *)0)->line_end ,sizeof(void *),}, 
	{ "line_size" , false,true,false,0,_MT_float,"float",(int)&((D_Vertex_Data *)0)->line_size ,sizeof(void *),}, 
	{ "fake_depth" , false,true,false,0,_MT_float,"float",(int)&((D_Vertex_Data *)0)->fake_depth ,sizeof(void *),}, 
	{ "size" , false,true,false,0,_MT_Vector2,"Vector2",(int)&((D_Vertex_Data *)0)->size ,sizeof(void *),}, 
	{ "corner_radius" , false,true,false,0,_MT_Vector4,"Vector4",(int)&((D_Vertex_Data *)0)->corner_radius ,sizeof(void *),}, 
	{ "corner_color" , false,true,true,quad_vertex_count,_MT_Vector4,"Vector4",(int)&((D_Vertex_Data *)0)->corner_color ,sizeof(void *),}, 
	{ "corner_tex_coord" , false,true,true,quad_vertex_count,_MT_Vector2,"Vector2",(int)&((D_Vertex_Data *)0)->corner_tex_coord ,sizeof(void *),}, 
};

global int member_meta_count_DrawingMenu = 7;
global const MemberMetaData member_meta_DrawingMenu[7] =
{
	{ "current_button_position" , false,false,false,0,_MT_Vector2,"Vector2",(int)&((DrawingMenu *)0)->current_button_position ,sizeof(Vector2),}, 
	{ "on_screen" , false,false,false,0,_MT_bool,"bool",(int)&((DrawingMenu *)0)->on_screen ,sizeof(bool),}, 
	{ "current_button_left" , false,false,false,0,_MT_float,"float",(int)&((DrawingMenu *)0)->current_button_left ,sizeof(float),}, 
	{ "button_hover" , false,false,false,0,_MT_bool,"bool",(int)&((DrawingMenu *)0)->button_hover ,sizeof(bool),}, 
	{ "button_clicked" , false,false,false,0,_MT_bool,"bool",(int)&((DrawingMenu *)0)->button_clicked ,sizeof(bool),}, 
	{ "menu_type" , false,false,false,0,_MT_int,"int",(int)&((DrawingMenu *)0)->menu_type ,sizeof(int),}, 
	{ "button_extra_offset" , false,false,false,0,_MT_float,"float",(int)&((DrawingMenu *)0)->button_extra_offset ,sizeof(float),}, 
};

global int member_meta_count_BoneState = 3;
global const MemberMetaData member_meta_BoneState[3] =
{
	{ "local_position" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((BoneState *)0)->local_position ,sizeof(Vector3),}, 
	{ "local_rotation" , false,false,false,0,_MT_Quaternion,"Quaternion",(int)&((BoneState *)0)->local_rotation ,sizeof(Quaternion),}, 
	{ "end_point_offset" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((BoneState *)0)->end_point_offset ,sizeof(Vector3),}, 
};

global int member_meta_count_KeyFrame = 5;
global const MemberMetaData member_meta_KeyFrame[5] =
{
	{ "bone_index" , false,false,false,0,_MT_int,"int",(int)&((KeyFrame *)0)->bone_index ,sizeof(int),}, 
	{ "bone_state" , false,false,false,0,_MT_BoneState,"BoneState",(int)&((KeyFrame *)0)->bone_state ,sizeof(BoneState),}, 
	{ "frame_index" , false,false,false,0,_MT_int,"int",(int)&((KeyFrame *)0)->frame_index ,sizeof(int),}, 
	{ "previous_bezier_offset" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((KeyFrame *)0)->previous_bezier_offset ,sizeof(Vector3),}, 
	{ "next_bezier_offset" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((KeyFrame *)0)->next_bezier_offset ,sizeof(Vector3),}, 
};

global int member_meta_count_RotationAxisData = 5;
global const MemberMetaData member_meta_RotationAxisData[5] =
{
	{ "rect" , false,false,false,0,_MT_Rect,"Rect",(int)&((RotationAxisData *)0)->rect ,sizeof(Rect),}, 
	{ "target_axis_color" , false,false,false,0,_MT_Color,"Color",(int)&((RotationAxisData *)0)->target_axis_color ,sizeof(Color),}, 
	{ "target_axis_line_size" , false,false,false,0,_MT_float,"float",(int)&((RotationAxisData *)0)->target_axis_line_size ,sizeof(float),}, 
	{ "axis_color" , false,false,false,0,_MT_Color,"Color",(int)&((RotationAxisData *)0)->axis_color ,sizeof(Color),}, 
	{ "axis_line_size" , false,false,false,0,_MT_float,"float",(int)&((RotationAxisData *)0)->axis_line_size ,sizeof(float),}, 
};

global int member_meta_count_Bone = 14;
global const MemberMetaData member_meta_Bone[14] =
{
	{ "from_blend_file" , false,false,false,0,_MT_bool,"bool",(int)&((Bone *)0)->from_blend_file ,sizeof(bool),}, 
	{ "IK_enable" , false,false,false,0,_MT_bool,"bool",(int)&((Bone *)0)->IK_enable ,sizeof(bool),}, 
	{ "IK_chain_length" , false,false,false,0,_MT_int,"int",(int)&((Bone *)0)->IK_chain_length ,sizeof(int),}, 
	{ "IK_target_bone_index" , false,false,false,0,_MT_int,"int",(int)&((Bone *)0)->IK_target_bone_index ,sizeof(int),}, 
	{ "IK_pole_bone_index" , false,false,false,0,_MT_int,"int",(int)&((Bone *)0)->IK_pole_bone_index ,sizeof(int),}, 
	{ "free_position" , false,false,false,0,_MT_bool,"bool",(int)&((Bone *)0)->free_position ,sizeof(bool),}, 
	{ "bone_index" , false,false,false,0,_MT_int,"int",(int)&((Bone *)0)->bone_index ,sizeof(int),}, 
	{ "parent_bone_index" , false,false,false,0,_MT_int,"int",(int)&((Bone *)0)->parent_bone_index ,sizeof(int),}, 
	{ "bone_name" , false,false,false,0,_MT_FixedStringW,"FixedStringW",(int)&((Bone *)0)->bone_name ,sizeof(FixedStringW),}, 
	{ "state" , false,false,false,0,_MT_BoneState,"BoneState",(int)&((Bone *)0)->state ,sizeof(BoneState),}, 
	{ "position" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((Bone *)0)->position ,sizeof(Vector3),}, 
	{ "end_position" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((Bone *)0)->end_position ,sizeof(Vector3),}, 
	{ "rotation" , false,false,false,0,_MT_Quaternion,"Quaternion",(int)&((Bone *)0)->rotation ,sizeof(Quaternion),}, 
	{ "free_bone" , false,false,false,0,_MT_bool,"bool",(int)&((Bone *)0)->free_bone ,sizeof(bool),}, 
};

global int member_meta_count_Clip = 9;
global const MemberMetaData member_meta_Clip[9] =
{
	{ "clip_name" , false,false,false,0,_MT_FixedString,"FixedString",(int)&((Clip *)0)->clip_name ,sizeof(FixedString),}, 
	{ "clip_index" , false,false,false,0,_MT_int,"int",(int)&((Clip *)0)->clip_index ,sizeof(int),}, 
	{ "not_loop" , false,false,false,0,_MT_bool,"bool",(int)&((Clip *)0)->not_loop ,sizeof(bool),}, 
	{ "key_frame_active_list" , false,false,false,0,_MT_List,"List",(int)&((Clip *)0)->key_frame_active_list ,sizeof(List),}, 
	{ "key_frame_hash_table_by_bone" , false,false,false,0,_MT_HashTable,"HashTable",(int)&((Clip *)0)->key_frame_hash_table_by_bone ,sizeof(HashTable),}, 
	{ "key_frame_hash_table" , false,false,false,0,_MT_HashTable,"HashTable",(int)&((Clip *)0)->key_frame_hash_table ,sizeof(HashTable),}, 
	{ "dragging_key_frame_list" , false,false,false,0,_MT_List,"List",(int)&((Clip *)0)->dragging_key_frame_list ,sizeof(List),}, 
	{ "key_frame_start_offset" , false,false,false,0,_MT_int,"int",(int)&((Clip *)0)->key_frame_start_offset ,sizeof(int),}, 
	{ "key_frame_count_to_save" , false,false,false,0,_MT_int,"int",(int)&((Clip *)0)->key_frame_count_to_save ,sizeof(int),}, 
};

global int member_meta_count_ClipBone = 5;
global const MemberMetaData member_meta_ClipBone[5] =
{
	{ "bone_pose_offset" , false,true,false,0,_MT_Bone,"Bone",(int)&((ClipBone *)0)->bone_pose_offset ,sizeof(void *),}, 
	{ "final_bone_pose" , false,true,false,0,_MT_Bone,"Bone",(int)&((ClipBone *)0)->final_bone_pose ,sizeof(void *),}, 
	{ "selected_bone" , false,true,false,0,_MT_bool,"bool",(int)&((ClipBone *)0)->selected_bone ,sizeof(void *),}, 
	{ "hovered_bone" , false,true,false,0,_MT_bool,"bool",(int)&((ClipBone *)0)->hovered_bone ,sizeof(void *),}, 
	{ "clip_index" , false,false,false,0,_MT_int,"int",(int)&((ClipBone *)0)->clip_index ,sizeof(int),}, 
};

global int member_meta_count_BoneSelection = 3;
global const MemberMetaData member_meta_BoneSelection[3] =
{
	{ "clip_bone_stack_index" , false,false,false,0,_MT_int,"int",(int)&((BoneSelection *)0)->clip_bone_stack_index ,sizeof(int),}, 
	{ "clip_index" , false,false,false,0,_MT_int,"int",(int)&((BoneSelection *)0)->clip_index ,sizeof(int),}, 
	{ "bone_index" , false,false,false,0,_MT_int,"int",(int)&((BoneSelection *)0)->bone_index ,sizeof(int),}, 
};

global int member_meta_count_Box = 3;
global const MemberMetaData member_meta_Box[3] =
{
	{ "position" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((Box *)0)->position ,sizeof(Vector3),}, 
	{ "size" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((Box *)0)->size ,sizeof(Vector3),}, 
	{ "rotation" , false,false,false,0,_MT_Quaternion,"Quaternion",(int)&((Box *)0)->rotation ,sizeof(Quaternion),}, 
};

global int member_meta_count_Face = 4;
global const MemberMetaData member_meta_Face[4] =
{
	{ "position" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((Face *)0)->position ,sizeof(Vector3),}, 
	{ "size" , false,false,false,0,_MT_Vector2,"Vector2",(int)&((Face *)0)->size ,sizeof(Vector2),}, 
	{ "rotation" , false,false,false,0,_MT_Quaternion,"Quaternion",(int)&((Face *)0)->rotation ,sizeof(Quaternion),}, 
	{ "vertex_position" , false,false,true,quad_vertex_count,_MT_Vector3,"Vector3",(int)&((Face *)0)->vertex_position ,sizeof(Vector3),}, 
};

global int member_meta_count_DeformVertexSlice = 2;
global const MemberMetaData member_meta_DeformVertexSlice[2] =
{
	{ "start" , false,false,false,0,_MT_int,"int",(int)&((DeformVertexSlice *)0)->start ,sizeof(int),}, 
	{ "count" , false,false,false,0,_MT_int,"int",(int)&((DeformVertexSlice *)0)->count ,sizeof(int),}, 
};

global int member_meta_count_DeformVertex = 2;
global const MemberMetaData member_meta_DeformVertex[2] =
{
	{ "bone_index" , false,false,false,0,_MT_int,"int",(int)&((DeformVertex *)0)->bone_index ,sizeof(int),}, 
	{ "weight" , false,false,false,0,_MT_float,"float",(int)&((DeformVertex *)0)->weight ,sizeof(float),}, 
};

global int member_meta_count_BoneSelectionResultData = 2;
global const MemberMetaData member_meta_BoneSelectionResultData[2] =
{
	{ "bone_index" , false,false,false,0,_MT_int,"int",(int)&((BoneSelectionResultData *)0)->bone_index ,sizeof(int),}, 
	{ "hit_point" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((BoneSelectionResultData *)0)->hit_point ,sizeof(Vector3),}, 
};

global int member_meta_count_BoneSelectionResult = 2;
global const MemberMetaData member_meta_BoneSelectionResult[2] =
{
	{ "data" , false,true,false,0,_MT_BoneSelectionResultData,"BoneSelectionResultData",(int)&((BoneSelectionResult *)0)->data ,sizeof(void *),}, 
	{ "data_count" , false,false,false,0,_MT_int,"int",(int)&((BoneSelectionResult *)0)->data_count ,sizeof(int),}, 
};

global int member_meta_count_D_Model = 16;
global const MemberMetaData member_meta_D_Model[16] =
{
	{ "deform_vertex_slice" , false,true,false,0,_MT_DeformVertexSlice,"DeformVertexSlice",(int)&((D_Model *)0)->deform_vertex_slice ,sizeof(void *),}, 
	{ "vertices" , false,true,false,0,_MT_Vector3,"Vector3",(int)&((D_Model *)0)->vertices ,sizeof(void *),}, 
	{ "vertex_count" , false,false,false,0,_MT_int,"int",(int)&((D_Model *)0)->vertex_count ,sizeof(int),}, 
	{ "normals" , false,true,false,0,_MT_Vector3,"Vector3",(int)&((D_Model *)0)->normals ,sizeof(void *),}, 
	{ "indices" , false,true,false,0,_MT_unsigned_short,"unsigned_short",(int)&((D_Model *)0)->indices ,sizeof(void *),}, 
	{ "index_count" , false,false,false,0,_MT_int,"int",(int)&((D_Model *)0)->index_count ,sizeof(int),}, 
	{ "all_deform_vertex" , false,true,false,0,_MT_DeformVertex,"DeformVertex",(int)&((D_Model *)0)->all_deform_vertex ,sizeof(void *),}, 
	{ "deform_vertex_count" , false,false,false,0,_MT_int,"int",(int)&((D_Model *)0)->deform_vertex_count ,sizeof(int),}, 
	{ "all_bones" , false,true,false,0,_MT_Bone,"Bone",(int)&((D_Model *)0)->all_bones ,sizeof(void *),}, 
	{ "bone_count" , false,false,false,0,_MT_int,"int",(int)&((D_Model *)0)->bone_count ,sizeof(int),}, 
	{ "bone_capacity" , false,false,false,0,_MT_int,"int",(int)&((D_Model *)0)->bone_capacity ,sizeof(int),}, 
	{ "all_initial_bone" , false,true,false,0,_MT_Bone,"Bone",(int)&((D_Model *)0)->all_initial_bone ,sizeof(void *),}, 
	{ "initial_bone_count" , false,false,false,0,_MT_int,"int",(int)&((D_Model *)0)->initial_bone_count ,sizeof(int),}, 
	{ "initial_bone_capacity" , false,false,false,0,_MT_int,"int",(int)&((D_Model *)0)->initial_bone_capacity ,sizeof(int),}, 
	{ "root_bone_list" , false,false,false,0,_MT_List,"List",(int)&((D_Model *)0)->root_bone_list ,sizeof(List),}, 
	{ "bone_children_hash_table" , false,false,false,0,_MT_HashTable,"HashTable",(int)&((D_Model *)0)->bone_children_hash_table ,sizeof(HashTable),}, 
};

global int member_meta_count_EditorData = 31;
global const MemberMetaData member_meta_EditorData[31] =
{
	{ "assigning_parent_bone" , false,false,false,0,_MT_bool,"bool",(int)&((EditorData *)0)->assigning_parent_bone ,sizeof(bool),}, 
	{ "assigning_IK_target_bone" , false,false,false,0,_MT_bool,"bool",(int)&((EditorData *)0)->assigning_IK_target_bone ,sizeof(bool),}, 
	{ "assigning_IK_pole_bone" , false,false,false,0,_MT_bool,"bool",(int)&((EditorData *)0)->assigning_IK_pole_bone ,sizeof(bool),}, 
	{ "game_frame" , false,false,false,0,_MT_int,"int",(int)&((EditorData *)0)->game_frame ,sizeof(int),}, 
	{ "selected_clip_index" , false,false,false,0,_MT_int,"int",(int)&((EditorData *)0)->selected_clip_index ,sizeof(int),}, 
	{ "control_rig" , false,false,false,0,_MT_bool,"bool",(int)&((EditorData *)0)->control_rig ,sizeof(bool),}, 
	{ "turn_off_bezier_curve" , false,false,false,0,_MT_bool,"bool",(int)&((EditorData *)0)->turn_off_bezier_curve ,sizeof(bool),}, 
	{ "selecting_bezier" , false,false,false,0,_MT_bool,"bool",(int)&((EditorData *)0)->selecting_bezier ,sizeof(bool),}, 
	{ "selected_bone_count" , false,false,false,0,_MT_int,"int",(int)&((EditorData *)0)->selected_bone_count ,sizeof(int),}, 
	{ "selected_bone_stack" , false,true,false,0,_MT_BoneSelection,"BoneSelection",(int)&((EditorData *)0)->selected_bone_stack ,sizeof(void *),}, 
	{ "playing" , false,false,false,0,_MT_bool,"bool",(int)&((EditorData *)0)->playing ,sizeof(bool),}, 
	{ "play_timer" , false,false,false,0,_MT_float,"float",(int)&((EditorData *)0)->play_timer ,sizeof(float),}, 
	{ "IK_iteration_count" , false,false,false,0,_MT_int,"int",(int)&((EditorData *)0)->IK_iteration_count ,sizeof(int),}, 
	{ "timeline_slider_offset" , false,false,false,0,_MT_float,"float",(int)&((EditorData *)0)->timeline_slider_offset ,sizeof(float),}, 
	{ "current_frame_at_timeline" , false,false,false,0,_MT_int,"int",(int)&((EditorData *)0)->current_frame_at_timeline ,sizeof(int),}, 
	{ "start_frame_index" , false,false,false,0,_MT_int,"int",(int)&((EditorData *)0)->start_frame_index ,sizeof(int),}, 
	{ "timeline_frame_length" , false,false,false,0,_MT_int,"int",(int)&((EditorData *)0)->timeline_frame_length ,sizeof(int),}, 
	{ "timeline_scale" , false,false,false,0,_MT_float,"float",(int)&((EditorData *)0)->timeline_scale ,sizeof(float),}, 
	{ "target_camera_zoom" , false,false,false,0,_MT_float,"float",(int)&((EditorData *)0)->target_camera_zoom ,sizeof(float),}, 
	{ "current_camera_zoom" , false,false,false,0,_MT_float,"float",(int)&((EditorData *)0)->current_camera_zoom ,sizeof(float),}, 
	{ "right" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((EditorData *)0)->right ,sizeof(Vector3),}, 
	{ "up" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((EditorData *)0)->up ,sizeof(Vector3),}, 
	{ "billboard_rotation" , false,false,false,0,_MT_Quaternion,"Quaternion",(int)&((EditorData *)0)->billboard_rotation ,sizeof(Quaternion),}, 
	{ "editor_camera_euler" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((EditorData *)0)->editor_camera_euler ,sizeof(Vector3),}, 
	{ "editor_camera_offset" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((EditorData *)0)->editor_camera_offset ,sizeof(Vector3),}, 
	{ "side_camera_zoom" , false,false,false,0,_MT_float,"float",(int)&((EditorData *)0)->side_camera_zoom ,sizeof(float),}, 
	{ "showcase_camera_euler" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((EditorData *)0)->showcase_camera_euler ,sizeof(Vector3),}, 
	{ "current_mouse_position" , false,false,false,0,_MT_Vector2,"Vector2",(int)&((EditorData *)0)->current_mouse_position ,sizeof(Vector2),}, 
	{ "previous_mouse_position" , false,false,false,0,_MT_Vector2,"Vector2",(int)&((EditorData *)0)->previous_mouse_position ,sizeof(Vector2),}, 
	{ "operate_menu_position" , false,false,false,0,_MT_Vector2,"Vector2",(int)&((EditorData *)0)->operate_menu_position ,sizeof(Vector2),}, 
	{ "flat_color" , false,false,false,0,_MT_bool,"bool",(int)&((EditorData *)0)->flat_color ,sizeof(bool),}, 
};

global int member_meta_count_SplitViewport = 6;
global const MemberMetaData member_meta_SplitViewport[6] =
{
	{ "viewport" , false,false,false,0,_MT_D_Rectangle,"D_Rectangle",(int)&((SplitViewport *)0)->viewport ,sizeof(D_Rectangle),}, 
	{ "camera_zoom" , false,false,false,0,_MT_float,"float",(int)&((SplitViewport *)0)->camera_zoom ,sizeof(float),}, 
	{ "camera_euler" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((SplitViewport *)0)->camera_euler ,sizeof(Vector3),}, 
	{ "camera_offset" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((SplitViewport *)0)->camera_offset ,sizeof(Vector3),}, 
	{ "camera_up" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((SplitViewport *)0)->camera_up ,sizeof(Vector3),}, 
	{ "ortho" , false,false,false,0,_MT_bool,"bool",(int)&((SplitViewport *)0)->ortho ,sizeof(bool),}, 
};

internal StructMetaData * get_all_type_member_info()
{
StructMetaData * all_struct = malloc(sizeof(StructMetaData) * _MT_type_count);
	all_struct[_MT_float] = (StructMetaData){};
	all_struct[_MT_double] = (StructMetaData){};
	all_struct[_MT_bool] = (StructMetaData){};
	all_struct[_MT_int] = (StructMetaData){};
	all_struct[_MT_short] = (StructMetaData){};
	all_struct[_MT_long] = (StructMetaData){};
	all_struct[_MT_long_long] = (StructMetaData){};
	all_struct[_MT_char] = (StructMetaData){};
	all_struct[_MT_wchar_t] = (StructMetaData){};
	all_struct[_MT_unsigned_int] = (StructMetaData){};
	all_struct[_MT_unsigned_short] = (StructMetaData){};
	all_struct[_MT_unsigned_long_long] = (StructMetaData){};
	all_struct[_MT_unsigned_long] = (StructMetaData){};
	all_struct[_MT_unsigned_char] = (StructMetaData){};
	all_struct[_MT_void] = (StructMetaData){};
	all_struct[_MT_HDC] = (StructMetaData){};
	all_struct[_MT_Color] = (StructMetaData)GetStructMeta(Color);
	all_struct[_MT_Vector4] = (StructMetaData)GetStructMeta(Vector4);
	all_struct[_MT_Vector2] = (StructMetaData)GetStructMeta(Vector2);
	all_struct[_MT_Vector3] = (StructMetaData)GetStructMeta(Vector3);
	all_struct[_MT_Quaternion] = (StructMetaData)GetStructMeta(Quaternion);
	all_struct[_MT_D_Rectangle] = (StructMetaData)GetStructMeta(D_Rectangle);
	all_struct[_MT_Image] = (StructMetaData)GetStructMeta(Image);
	all_struct[_MT_Texture] = (StructMetaData)GetStructMeta(Texture);
	all_struct[_MT_Texture2D] = (StructMetaData){};
	all_struct[_MT_Camera3D] = (StructMetaData)GetStructMeta(Camera3D);
	all_struct[_MT_Matrix] = (StructMetaData)GetStructMeta(Matrix);
	all_struct[_MT_Ray] = (StructMetaData)GetStructMeta(Ray);
	all_struct[_MT_stbtt__buf] = (StructMetaData)GetStructMeta(stbtt__buf);
	all_struct[_MT_stbtt_fontinfo] = (StructMetaData)GetStructMeta(stbtt_fontinfo);
	all_struct[_MT_stbrp_coord] = (StructMetaData){};
	all_struct[_MT_stbrp_node] = (StructMetaData)GetStructMeta(stbrp_node);
	all_struct[_MT_stbrp_context] = (StructMetaData)GetStructMeta(stbrp_context);
	all_struct[_MT_MemberMetaData] = (StructMetaData)GetStructMeta(MemberMetaData);
	all_struct[_MT_StructMetaData] = (StructMetaData)GetStructMeta(StructMetaData);
	all_struct[_MT_StructMetaDataNode] = (StructMetaData)GetStructMeta(StructMetaDataNode);
	all_struct[_MT_FixedString] = (StructMetaData)GetStructMeta(FixedString);
	all_struct[_MT_FixedStringW] = (StructMetaData)GetStructMeta(FixedStringW);
	all_struct[_MT_DataHeader] = (StructMetaData)GetStructMeta(DataHeader);
	all_struct[_MT_TemporayMemory] = (StructMetaData)GetStructMeta(TemporayMemory);
	all_struct[_MT_AppWindow] = (StructMetaData){};
	all_struct[_MT_ListNode] = (StructMetaData)GetStructMeta(ListNode);
	all_struct[_MT_DummyNodeTag] = (StructMetaData){};
	all_struct[_MT_List] = (StructMetaData)GetStructMeta(List);
	all_struct[_MT_Array] = (StructMetaData)GetStructMeta(Array);
	all_struct[_MT_HashTableEntry] = (StructMetaData)GetStructMeta(HashTableEntry);
	all_struct[_MT_HashTableSlot] = (StructMetaData)GetStructMeta(HashTableSlot);
	all_struct[_MT_HashTable] = (StructMetaData)GetStructMeta(HashTable);
	all_struct[_MT_QuadVertexPosition] = (StructMetaData){};
	all_struct[_MT_Rect] = (StructMetaData)GetStructMeta(Rect);
	all_struct[_MT_Quad] = (StructMetaData)GetStructMeta(Quad);
	all_struct[_MT_D_GlyphInfo] = (StructMetaData)GetStructMeta(D_GlyphInfo);
	all_struct[_MT_D_GlyphInfoNode] = (StructMetaData)GetStructMeta(D_GlyphInfoNode);
	all_struct[_MT_D_FontContext] = (StructMetaData)GetStructMeta(D_FontContext);
	all_struct[_MT_D_App_Data] = (StructMetaData)GetStructMeta(D_App_Data);
	all_struct[_MT_D_ShaderUniform] = (StructMetaData){};
	all_struct[_MT_D_ShaderType] = (StructMetaData){};
	all_struct[_MT_D_ShaderCode] = (StructMetaData)GetStructMeta(D_ShaderCode);
	all_struct[_MT_GLShaderType] = (StructMetaData){};
	all_struct[_MT_ShaderInput] = (StructMetaData)GetStructMeta(ShaderInput);
	all_struct[_MT_ShaderBuffer] = (StructMetaData)GetStructMeta(ShaderBuffer);
	all_struct[_MT_String] = (StructMetaData)GetStructMeta(String);
	all_struct[_MT_DrawFlag] = (StructMetaData){};
	all_struct[_MT_RenderState] = (StructMetaData)GetStructMeta(RenderState);
	all_struct[_MT_D_Light_GPU_Data] = (StructMetaData)GetStructMeta(D_Light_GPU_Data);
	all_struct[_MT_D_Light] = (StructMetaData)GetStructMeta(D_Light);
	all_struct[_MT_D_Vertex_Data] = (StructMetaData)GetStructMeta(D_Vertex_Data);
	all_struct[_MT_BoxFace] = (StructMetaData){};
	all_struct[_MT_BoxVertex] = (StructMetaData){};
	all_struct[_MT_GameMenuType] = (StructMetaData){};
	all_struct[_MT_DrawingMenu] = (StructMetaData)GetStructMeta(DrawingMenu);
	all_struct[_MT_BoneState] = (StructMetaData)GetStructMeta(BoneState);
	all_struct[_MT_KeyFrame] = (StructMetaData)GetStructMeta(KeyFrame);
	all_struct[_MT_RotationAxis] = (StructMetaData){};
	all_struct[_MT_RotationAxisData] = (StructMetaData)GetStructMeta(RotationAxisData);
	all_struct[_MT_Bone] = (StructMetaData)GetStructMeta(Bone);
	all_struct[_MT_AnimationTag] = (StructMetaData){};
	all_struct[_MT_Clip] = (StructMetaData)GetStructMeta(Clip);
	all_struct[_MT_ClipBone] = (StructMetaData)GetStructMeta(ClipBone);
	all_struct[_MT_BoneSelection] = (StructMetaData)GetStructMeta(BoneSelection);
	all_struct[_MT_Box] = (StructMetaData)GetStructMeta(Box);
	all_struct[_MT_Face] = (StructMetaData)GetStructMeta(Face);
	all_struct[_MT_DeformVertexSlice] = (StructMetaData)GetStructMeta(DeformVertexSlice);
	all_struct[_MT_DeformVertex] = (StructMetaData)GetStructMeta(DeformVertex);
	all_struct[_MT_BoneSelectionResultData] = (StructMetaData)GetStructMeta(BoneSelectionResultData);
	all_struct[_MT_BoneSelectionResult] = (StructMetaData)GetStructMeta(BoneSelectionResult);
	all_struct[_MT_D_Model] = (StructMetaData)GetStructMeta(D_Model);
	all_struct[_MT_EditorType] = (StructMetaData){};
	all_struct[_MT_EditorData] = (StructMetaData)GetStructMeta(EditorData);
	all_struct[_MT_SplitViewport] = (StructMetaData)GetStructMeta(SplitViewport);
	all_struct[_MT_GLFWwindow] = (StructMetaData){};
	all_struct[_MT_GameLoadFunction] = (StructMetaData){};
	all_struct[_MT_GameLoopFunction] = (StructMetaData){};
	all_struct[_MT_GameUnloadFunction] = (StructMetaData){};


return all_struct;
}

internal long long counter_stamp();
internal double time_stamp();
internal long long end_counter_stamp(long long begin_time);
internal double end_time_stamp(long long begin_time);
internal long long cycle_stamp();
internal Vector4 color_to_linear(Color color);
internal Color linear_to_color(Vector4 color);
internal Vector4 linear_fade(Vector4 linear_color ,float mul);
internal Color color_multiply_B(Color base , float value);
internal Color color_invert(Color base);
internal Color color_lerp(Color a , Color b , float t);
internal Color color_screen(Color base_color ,Color layer_color , float blend);
internal Color color_multiply(Color base , Color layer , float blend);
internal Vector4 vector4_lerp(Vector4 v1, Vector4 v2, float amount);
internal void color_quad(Vector4* quad_color , Vector4 target_color);
internal Ray get_screen_to_world_ray_EX(Vector2 position, Camera camera, int viewport_x , int viewport_y, int width, int height);
internal RayCollision get_ray_collision_triangle(Ray ray, Vector3 p1, Vector3 p2, Vector3 p3);
internal RayCollision get_collision_rect_3D(Ray ray, Rect rect);
internal Vector4 random_linear_color();
internal Rect get_rect();
internal Rect get_rect_B(int pixel_size_x, int pixel_size_y);
internal float noise(float seed);
internal unsigned int string_to_hash(const char * s);
internal unsigned char* allocate_temp_(int size);
internal unsigned char * allocate_frame_(int size);
internal DataHeader * add_data_header();
internal unsigned char* allocate_save_(int size);
internal void combine_file_path(const char* file_name,char * result_path);
internal wchar_t * combine_string_W(wchar_t * string_a , wchar_t * string_b);
internal char * combine_string(char * string_a , char * string_b);
internal char * get_app_file_path(const char* file_name);
internal char * get_level_file_path(const char * level_name);
internal char * get_level_file_path_B();
internal Vector4 quaternion_to_vector4(Quaternion _Quaternion);
internal float cubic_bezier(float x1 , float x2 , float x3 ,float x4, float t);
internal Vector3 vector3_cubic_bezier(Vector3 p1 , Vector3 p2 , Vector3 p3 , Vector3 p4  , float t);
internal float ease_in_out_sine(float x);
internal float ease_in_out_cubic(float x);
internal float ease_out_quint(float x );
internal float ease_in_back(float x , float c1);
internal float ease_out_back(float x  , float c1);
internal ShaderBuffer* get_GPU_buffer_info(void * data);
internal void * create_buffer_for_GPU(const char * buffer_name , int primitive_type,int component_count , int max_size);
internal void * create_GPU_vertex_buffer(const char * buffer_name , int primitive_type,int component_count , int buffer_size );
internal void * create_GPU_indics_buffer(const char * buffer_name , int primitive_type,int component_count , int buffer_size );
internal void render_state_init();
internal unsigned int D_load_texture(void * data , int width, int height, int format, int mipmapCount);
internal unsigned int D_load_texture_from_image(Image image);
internal void update_texture(unsigned int * id);
internal void update_texture_multi_sample(unsigned int * id);
internal void shader_init();
internal void update_all_GPU_vertex_buffer(ShaderInput * currentShader,int count);
internal void D_game_draw();
internal unsigned int load_texture_multi_sample(int width, int height);
internal unsigned int load_depth_texture_multi_sample(int width , int height);
internal Quad rect_to_quad(Rect rect);
internal Quad rect_to_rhombus(Rect rect);
internal void quad_draw_check(int target_texture);
internal void draw_model(D_Model * model , Bone * all_bone , Bone * all_initial_bone);
internal void draw_triangle(Vector3* all_vertices ,Vector4 vertex_color);
internal void draw_triangle_up(Quad quad , Color triangle_color );
internal void draw_triangle_right(Quad quad, Color triangle_color);
internal void draw_triangle_left(Quad quad, Color triangle_color);
internal void draw_triangle_bottom(Quad quad, Color triangle_color);
internal void draw_quad_with_texture(Quad quad,int target_texture);
internal void change_matrix(Matrix matrix);
internal void _D_DrawRect(Rect rect, Vector4 corner_radius, int target_texture , Vector4 * vertex_color , Vector2 * texture_coord);
internal void draw_rect(Rect rect , Vector4 rect_color, int texture_index);
internal void draw_rect_B(Rect rect , Vector4 rectColor);
internal void draw_rect_with_texture(Rect rect , Vector4 corners, Color rect_color, int texture_index);
internal void draw_rect_with_texture_B(Rect rect , Color rect_color, int texture);
internal void draw_quad_as_rect(Quad quad,int target_texture);
internal void draw_quad_as_rect_B(Quad * quad, int target_texture);
internal void draw_quad_as_rect_C(Quad quad,int target_texture,Color quad_color);
internal void draw_quad_as_rect_D(Quad * quad , int target_texture , Color quad_color);
internal void draw_quad(Quad quad, int target_texture);
internal void draw_quad_B(Quad quad);
internal void draw_quad_C(Vector3 position , float size , Quaternion rotation, Vector4 quad_color );
internal void draw_quad_D(Quad quad, Color quad_color);
internal void draw_rect_C(Rect rect, float corner_roundness);
internal void draw_rect_D(Rect rect, float corner_roundness, Color target_color);
internal void draw_circle(Rect rect, Color target_color);
internal Rect get_billboard_rect( Vector3 position , float size );
internal void draw_billboard_circle(Vector3 position , float size, Color target_color);
internal void draw_circle_B(Vector3 position,float size, Color target_color);
internal void draw_circle_C(Vector2 position,float size, Color target_color);
internal void draw_circle_D(Vector2 position,float size, Vector4 target_color);
internal void draw_circle_E(Vector3 position,float size, Vector4 target_color);
internal void draw_line_quad(Quad quad , Vector3 line_start , Vector3 line_end , float line_size);
internal void draw_line_quad_B(Quad quad , Color quad_color , Vector3 line_start , Vector3 line_end , float line_size);
internal void add_point_to_line(Vector3 point, Vector4 point_color , float point_size);
internal void add_point_to_line_B(Vector3 point, Color point_color , float point_size);
internal void add_point_to_line_C(Vector3 point, Color point_color);
internal void add_point_to_line_2D(Vector2 point, Vector4 point_color,float point_size);
internal void add_point_to_line_2D_B(Vector2 point, Vector4 point_color);
internal void add_point_to_line_2D_C(Vector2 point, Color point_color , float point_size);
internal void add_point_to_line_2D_D(Vector2 point, Color point_color);
internal unsigned int load_depth_texture(int width, int height);
internal float get_line_intersect_with_plane_time(Vector3 start , Vector3 end , Vector3 plane_normal , Vector3 plane_origin);
internal Vector3 transform_vector(Vector3 position , Matrix matrix);
internal float remove_prespective(Vector3 q);
internal Quad get_billboard_quad(Vector3 point, float width , float height);
internal void draw_round_line(Vector3 start , Vector3 end , float size , Color start_color , Color end_color);
internal void draw_round_line_B(Vector3 start , Vector3 end , float size , Color line_color);
internal void draw_rect_line(Vector2 start_position , Vector2 end_position , Vector4 line_color);
internal void draw_quad_line(Quad quad , Color line_color , float line_size);
internal void draw_rect_line_B(Vector2 start_position , Vector2 end_position , Color line_color);
internal void draw_rect_E(Vector3 position,float size, Color target_color , Quaternion rotation);
internal void draw_rect_F(Vector3 position, float size, Vector4 target_color , Quaternion rotation);
internal void draw_rect_line_C(Rect rect ,Vector4 line_color , float line_size);
internal void draw_rect_line_D(Rect rect, Vector4 line_color);
internal void draw_rect_line_E(Rect rect, Color line_color , float line_size );
internal void draw_rect_line_F( Rect rect , Color line_color);
internal void draw_quad_line_B( Quad quad, Color line_color);
internal void draw_rect_G( Rect rect,Color target_color);
internal void _draw_rect_text(Rect quad ,Vector4* color ,Vector2* texture_coord , Vector3 up ,Vector3 right);
internal void draw_background();
internal void draw_screen_flat(int texture_index , Vector4 color , bool multi_sample);
internal void draw_screen_flat_B(int texture_index ,Color color , bool multi_sample);
internal void draw_box_EX( Vector3 position , Vector3 size , Quaternion rotation , Color box_color);
internal void draw_box(Box box , Color color);
internal void char_to_wide_char(wchar_t * dst , char* src ,int length);
internal D_GlyphInfo D_get_glyph_from_codepoint(wchar_t current_codepoint,D_FontContext * font_context);
internal float get_glyph_width(D_GlyphInfo glyph_info , D_FontContext *font_info , float space_size_offset , float space_scaler , float scale);
internal void get_string_width(int count,wchar_t * codepoint_pointer,D_FontContext *font_info, float space_size_offset,float space_scaler,float scale,int * result_count,float * result_width);
internal float get_total_string_width(const wchar_t* text,float scale);
internal float get_total_string_width_B(char * text,float scale);
internal void draw_text_with_font(D_FontContext* font_info, Rect rect , Color text_color, wchar_t* string, float scale, float space_size_offset, float space_scaler, bool centre , Vector3 up , Vector3 right);
internal void D_draw_text_screen(Vector3 position ,char * text , float size, Color text_color , bool centre);
internal void D_draw_text(Rect rect , const wchar_t* string, Color text_color, float scale, float space_size_offset, float space_scaler, bool centre);
internal void D_draw_text_B(Rect rect, const wchar_t* string, Color text_color, bool centre);
internal void D_draw_text_C(Vector2 position,const wchar_t* string, Color textColor, float size, bool centre);
internal void D_draw_text_D(Vector3 position, const wchar_t* string, Color text_color, float size, bool centre);
internal void D_draw_text_E(Vector3 position , float size , char * string , Color text_color , bool centre);
internal void D_draw_text_F(Vector2 position , float size , char * string , Color text_color , bool centre);
internal void simple_float_input(wchar_t * target_string , float * target_float);
internal void get_float_input(float * target_float);
internal void simple_int_input(wchar_t * target_string , int * target_int);
internal void update_float_input();
internal Rect quad_position_left(Rect rect, float position_offset_x, float size_x, float size_offset_y);
internal Rect quad_position_left_B(float position_offset_x, float size_x, float size_offset_y);
internal Rect offset_from_bottom_left(float pixel_offset_x, float pixel_offset_y, float pixel_size_x,float pixel_size_y);
internal Rect offset_from_quad_top_left(Rect parent_rect, Rect child_rect, float pixel_offset_x,float pixel_offset_y);
internal Rect offset_from_top_left(Rect rect , float pixel_offset_x,float pixel_offset_y);
internal Rect quad_to_top_left(Rect rect);
internal Rect quad_to_bottom_left(Rect rect);
internal Rect quad_move_down_half(Rect rect);
internal Vector2 get_rect_top_right_corner(Rect rect);
internal Vector2 get_rect_bottom_left_corner(Rect rect);
internal Quad resize_block(Quad quad,float size);
internal Quad resize_block_B(Quad * quad, float size);
internal bool check_collision_rect(Rect rect , Vector2 point);
internal bool check_collision_rect_mouse(Rect rect);
internal bool check_collision_rect_mouse_B(Vector2 top_right , Vector2 bottom_left);
internal Vector3 get_edge_direction(Vector3 start,Vector3 end , Vector3 point);
internal bool check_collision_quad_point(Quad quad, Vector3 point);
internal bool check_collision_quad_to_rect(Quad quad , Rect rect);
internal bool check_collision_quad_mouse(Quad quad);
internal Vector4 selection_from_start_to_end_to_rect(Vector2 start_position, Vector2 end);
internal Vector2 get_menu_individual_item_position_end(Vector2 position, String* option_string, bool on_screen,int index,int offset_y);
internal Vector2 get_menu_individual_item_position_end_B(Vector2 position, String* option_string, bool on_screen, int index);
internal DrawingMenu start_draw_menu(Vector2 position , bool on_screen , GameMenuType menu_type);
internal DrawingMenu start_draw_dragging_menu(Vector2 * pixel_position , GameMenuType menu_type);
internal DrawingMenu start_draw_menu_mouse();
internal bool draw_menu_button_W_EX(DrawingMenu * menu, wchar_t * button_string , Color button_text_color , bool change_button_text_color);
internal bool draw_menu_button_EX(DrawingMenu * menu, char * button_string , Color button_text_color , bool change_button_text_color);
internal bool draw_menu_button_W(DrawingMenu * menu, wchar_t * button_string );
internal bool draw_menu_button(DrawingMenu * menu, char * button_string);
internal bool ray_line_segment_intersection_example(Vector3 o, Vector3 d, Vector3 a, Vector3 b);
internal Vector2 ray_line_segment_intersection(Vector3 o, Vector3 d, Vector3 a, Vector3 b);
internal Vector2 ray_line_segment_intersection_(Vector3 o, Vector3 d, Vector3 a, Vector3 b);
internal bool check_collision_rect_with_rect(Rect rect_A, Rect rect_B);
internal RayCollision get_collision_quad_3D(Quad quad_to_check);
internal bool check_collision_quad_3D_B(Quad quad_to_check);
internal RayCollision get_collision_rect_3D_B(Rect rect);
internal bool check_collision_rect_3D(Rect rect);
internal unsigned int string_to_hash_W(wchar_t *s);
internal int hash_int(int key);
internal void _print_hash_table(HashTableSlot * hash_slot_array , int count);
internal HashTable allocate_hash_table(int count);
internal int get_emty_slot_index_from_hash_table(int hash_value , HashTable * hash_table);
internal int get_hash_table_head_slot_index(int hash_value ,  HashTable * hash_table);
internal int get_data_index_from_slot_index(int slot_index , HashTable * hash_table);
internal int insert_to_hash_table(int slot_index_to_insert_after , int hash_value , int data_index , HashTable * hash_table);
internal int add_to_hash_table(int hash_value , int data_index , HashTable * hash_table);
internal void clear_hash_table(HashTable * hash_table);
internal bool delete_from_hash_table_by_slot_index(int slot_index , int hash_value , HashTable * hash_table);
internal bool delete_from_hash_table(int hash_value , int data_index , HashTable * hash_table);
internal int get_hash_table_tail_data_index(int hash_value , HashTable * hash_table);
internal void _iterate_hash_table( int hash_value , int * slot_index , int * data_index, HashTable * hash_table);
internal void _iterate_hash_table_reverse( int hash_value , int * slot_index , int * data_index, HashTable * hash_table);
internal List allocate_list(int count);
internal void clear_list(List * list);
internal bool list_full(List * list);
internal int create_new_node( List * list);
internal int get_next_node_index(int node_index , List * list);
internal int get_head_node_index(List * list);
internal int get_tail_node_index(List * list);
internal int get_previous_node_index(int node_index , List * list);
internal int get_data_index_from_node_index(int node_index , List * list);
internal int get_previous_data_index_from_node_index(int node_index , List * list);
internal int get_next_data_index_from_node_index(int node_index , List * list);
internal void insert_to_list
(
 bool connect_before_node , 
 int node_to_connect_index , 
 int node_index , 
 int data_index , 
 List * list
 );
internal int get_emty_node_from_list( List * list);
internal int create_and_insert_to_list(bool connect_before_node , int node_to_connect_index , int data_index , List * list);
internal int add_to_list_head(int data_index , List * list);
internal int add_to_list_tail( int data_index , List * list);
internal int add_to_list_tail_B(List * list);
internal bool delete_from_list(int node_index_to_delete , List * list);
internal Array allocate_array(int capacity);
internal void clear_array(Array * array);
internal void recheck_array(Array * array);
internal int add_to_array(Array * array);
internal bool delete_from_array(Array * array , int data_index);
internal bool is_array_full(Array * array);
internal bool iterate_array(int * data_index , Array * array);
internal void draw_arrow_line(Vector3 start_position , Vector3 end_position , Color start_color , Color end_color);
internal void draw_arrow_line_B(Vector3 start_position , Vector3 end_position , Color line_color);
internal void draw_arrow_ray_C(Vector3 start_position , Vector3 direction  ,Color start_color , Color end_color);
internal void draw_arrow_ray_D(Vector3 start_position , Vector3 direction  , Color line_color);
internal void lerp_bone_state(Bone * base_bone , Bone * blend_bone , int bone_index , float weight);
internal void lerp_multiple_bone_state( Bone * base_bone , Bone * blend_bone , int base_bone_count , float weight);
internal int get_key_frame_count( KeyFrame * start_key_frame , KeyFrame * end_key_frame , int target_start_frame , int target_frame_count);
internal bool is_rig(int bone_index);
internal void get_bone(Bone * bone_array , int bone_array_count);
internal Box get_box();
internal Color get_random_color();
internal void iterate_and_draw_bone_arrow( Bone * bone_array , Bone * bone , int stack_index);
internal void member_data_to_string(MemberMetaData* member_meta, unsigned char* data_head, wchar_t* string_buffer);
internal void _print_struct_member_info( StructMetaData struct_meta_data, unsigned char* struct_data, Rect text_quad);
internal void create_struct_name_string_hash();
internal int get_data_size(char * type_name);
internal int get_type_from_name(char * type_name);
internal void create_a_whole_new_world();
internal Vector3 mouse_on_plane(Vector3 plane_origin);
internal BoneSelectionResult bone_selection(Vector2 size , Color unactive_color , Color active_color);
internal void sort_bone_hash_table(int bone_index , HashTable * hash_table_by_bone);
internal Quad direction_to_quad(Vector3 direction , float width);
internal Vector3 * box_to_point(Box box);
internal bool box_collision_ray( Vector3 origin , Vector3 direction, Box box);
internal Vector3 get_furthest_point_by_direction( Vector3 direction , Vector3 * points , int point_count);
internal Vector3 position_to_grid(Vector3 position , int size);
internal bool check_selected_bone_rotation( Bone * final_bone_array_copy, int single_bone_index , Clip * clip_to_assign);
internal void bone_selection_and_edit_bone_state( int current_frame_index);
internal void bone_mouse_menu( Bone * single_editing_bone , Clip * clip , int current_frame_index);
internal void iterate_bone_structure(Bone * bone_array , Bone * root_bone);
internal void update_bone_structure( Bone * bone_array);
internal void bone_IK_update_B(Bone * bone_array , Bone * base_pose_bone_array , int target_bone_index , int pole_bone_index , int IK_bone_index , int iteration_count , int bone_chain_max_length);
internal void bone_IK_update( Bone * bone_array , Bone * base_pose_bone_array , Bone * target_bone , Bone * pole_bone , Bone * IK_bone , int iteration_count , int bone_chain_max_length);
internal void _bone_IK_update( Bone * bone_array , Bone * target_bone , Bone * pole_bone , Bone * IK_bone , int iteration_count , int bone_chain_max_length);
internal void draw_origin_grid(Vector3 origin);
internal void get_bone_from_existing_key_frame(Bone * bone_array , Clip * clip , int target_frame , int target_start_frame , int target_frame_count);
internal void add_bone_state(Bone * bone_array , Bone * add_bone_array , int bone_index);
internal void add_multiple_bone_state(Bone * bone_array , Bone * add_bone_array , int bone_array_count);
internal Bone * get_multiple_bone(int bone_array_count);
internal Bone * get_bone_pose_offset_from_clip( int clip_index , int target_frame , int target_frame_start, int target_frame_length);
internal float get_corner_weight(float vertical , float horizontal);
internal void viewport_update();
internal void game_update();
internal GAME_LOOP(game_loop);
internal bool compare_string_C(char * string_A, char * string_B , int count);
internal bool compare_string_W( wchar_t * string_A , wchar_t * string_B);
internal bool compare_string( char * string_A, char * string_B);
internal unsigned char * get_data_buffer_by_name(char * name);
internal unsigned char * allocate_to_file_(char * name , int size , int count);
internal void save_file();
internal int reassign_bone_index(int previous_bone_index);
internal void load_file();
internal GAME_UNLOAD(game_unload);
internal void game_init();
