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
	all_struct[_MT_GLFWwindow] = (StructMetaData){};
	all_struct[_MT_GameLoadFunction] = (StructMetaData){};
	all_struct[_MT_GameLoopFunction] = (StructMetaData){};
	all_struct[_MT_GameUnloadFunction] = (StructMetaData){};


return all_struct;
}

