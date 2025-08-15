enum _TypeMeta
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
	_MT_unsigned_long,
	_MT_unsigned_long_long,
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
	_MT__StructMetaData,
	_MT__MemberMetaData,
	_MT__StructMetaDataNode,
	_MT__MemberNode,
	_MT__DecomposedBasicType,
	_MT__TypeFormDifference,
	_MT__MemberDifference,
	_MT__TypeToCheckInfo,
	_MT__TypeToCheck,
	_MT__MemoryDetail,
	_MT__PersistMemory,
	_MT_PointerRecordNode,
	_MT__TemporayMemory,
	_MT_AppWindow,
	_MT_LineData,
	_MT_QuadVertexPosition,
	_MT__Rect,
	_MT__Quad,
	_MT_D_GlyphInfo,
	_MT_D_GlyphInfoNode,
	_MT__D_Font_Info,
	_MT_D_App_Data,
	_MT_GLFWwindow,
	_MT_GameLoadFunction,
	_MT_GameLoopFunction,
	_MT_GameUnloadFunction,
	_MT__MouseButtonBlocker,
	_MT_Type_Count
};

int _TypeMetaSize[] = 
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
sizeof(unsigned long),//unsigned long 
sizeof(unsigned long long),//unsigned long long 
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
sizeof(_StructMetaData),//_StructMetaData 
sizeof(_MemberMetaData),//_MemberMetaData 
sizeof(_StructMetaDataNode),//_StructMetaDataNode 
sizeof(_MemberNode),//_MemberNode 
sizeof(_DecomposedBasicType),//_DecomposedBasicType 
sizeof(_TypeFormDifference),//_TypeFormDifference 
sizeof(_MemberDifference),//_MemberDifference 
sizeof(_TypeToCheckInfo),//_TypeToCheckInfo 
sizeof(_TypeToCheck),//_TypeToCheck 
sizeof(_MemoryDetail),//_MemoryDetail 
sizeof(_PersistMemory),//_PersistMemory 
sizeof(PointerRecordNode),//PointerRecordNode 
sizeof(_TemporayMemory),//_TemporayMemory 
sizeof(AppWindow),//AppWindow 
sizeof(LineData),//LineData 
sizeof(QuadVertexPosition),//QuadVertexPosition 
sizeof(_Rect),//_Rect 
sizeof(_Quad),//_Quad 
sizeof(D_GlyphInfo),//D_GlyphInfo 
sizeof(D_GlyphInfoNode),//D_GlyphInfoNode 
sizeof(_D_Font_Info),//_D_Font_Info 
sizeof(D_App_Data),//D_App_Data 
0,//GLFWwindow 
0,//GameLoadFunction 
0,//GameLoopFunction 
0,//GameUnloadFunction 
0,//_MouseButtonBlocker 
};

const char * _TypeMetaName[] = 
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
	"unsigned long",
	"unsigned long long",
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
	"_StructMetaData",
	"_MemberMetaData",
	"_StructMetaDataNode",
	"_MemberNode",
	"_DecomposedBasicType",
	"_TypeFormDifference",
	"_MemberDifference",
	"_TypeToCheckInfo",
	"_TypeToCheck",
	"_MemoryDetail",
	"_PersistMemory",
	"PointerRecordNode",
	"_TemporayMemory",
	"AppWindow",
	"LineData",
	"QuadVertexPosition",
	"_Rect",
	"_Quad",
	"D_GlyphInfo",
	"D_GlyphInfoNode",
	"_D_Font_Info",
	"D_App_Data",
	"GLFWwindow",
	"GameLoadFunction",
	"GameLoopFunction",
	"GameUnloadFunction",
	"_MouseButtonBlocker",
};

bool _IsTypeEnum[] = 
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
false,
false,
false,
false,
false,
false,
};

global const char * AppWindow_String[] = 
{
 "MainWindow",
 "WindowCount",
};

global const char * QuadVertexPosition_String[] = 
{
 "Vertex_TopRight",
 "Vertex_TopLeft",
 "Vertex_BottomRight",
 "Vertex_BottomLeft",
 "Vertex_Count",
};

enum IntrospectedStruct
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
	IS__StructMetaData, 
	IS__MemberMetaData, 
	IS__StructMetaDataNode, 
	IS__MemberNode, 
	IS__DecomposedBasicType, 
	IS__TypeFormDifference, 
	IS__MemberDifference, 
	IS__TypeToCheckInfo, 
	IS__TypeToCheck, 
	IS__MemoryDetail, 
	IS__PersistMemory, 
	IS_PointerRecordNode, 
	IS__TemporayMemory, 
	IS_LineData, 
	IS__Rect, 
	IS__Quad, 
	IS_D_GlyphInfo, 
	IS_D_GlyphInfoNode, 
	IS__D_Font_Info, 
	IS_D_App_Data, 
};

global int MemberMetaCount_Color = 4;
global _MemberMetaData MemberMeta_Color[4] =
{
	{ "r" , false,false,false,0,_MT_unsigned_char,"unsigned_char",(int)&((Color *)0)->r ,_TypeMetaSize[ _MT_unsigned_char],}, 
	{ "g" , false,false,false,0,_MT_unsigned_char,"unsigned_char",(int)&((Color *)0)->g ,_TypeMetaSize[ _MT_unsigned_char],}, 
	{ "b" , false,false,false,0,_MT_unsigned_char,"unsigned_char",(int)&((Color *)0)->b ,_TypeMetaSize[ _MT_unsigned_char],}, 
	{ "a" , false,false,false,0,_MT_unsigned_char,"unsigned_char",(int)&((Color *)0)->a ,_TypeMetaSize[ _MT_unsigned_char],}, 
};

global int MemberMetaCount_Vector4 = 4;
global _MemberMetaData MemberMeta_Vector4[4] =
{
	{ "x" , false,false,false,0,_MT_float,"float",(int)&((Vector4 *)0)->x ,_TypeMetaSize[ _MT_float],}, 
	{ "y" , false,false,false,0,_MT_float,"float",(int)&((Vector4 *)0)->y ,_TypeMetaSize[ _MT_float],}, 
	{ "z" , false,false,false,0,_MT_float,"float",(int)&((Vector4 *)0)->z ,_TypeMetaSize[ _MT_float],}, 
	{ "w" , false,false,false,0,_MT_float,"float",(int)&((Vector4 *)0)->w ,_TypeMetaSize[ _MT_float],}, 
};

global int MemberMetaCount_Vector2 = 2;
global _MemberMetaData MemberMeta_Vector2[2] =
{
	{ "x" , false,false,false,0,_MT_float,"float",(int)&((Vector2 *)0)->x ,_TypeMetaSize[ _MT_float],}, 
	{ "y" , false,false,false,0,_MT_float,"float",(int)&((Vector2 *)0)->y ,_TypeMetaSize[ _MT_float],}, 
};

global int MemberMetaCount_Vector3 = 3;
global _MemberMetaData MemberMeta_Vector3[3] =
{
	{ "x" , false,false,false,0,_MT_float,"float",(int)&((Vector3 *)0)->x ,_TypeMetaSize[ _MT_float],}, 
	{ "y" , false,false,false,0,_MT_float,"float",(int)&((Vector3 *)0)->y ,_TypeMetaSize[ _MT_float],}, 
	{ "z" , false,false,false,0,_MT_float,"float",(int)&((Vector3 *)0)->z ,_TypeMetaSize[ _MT_float],}, 
};

global int MemberMetaCount_Quaternion = 4;
global _MemberMetaData MemberMeta_Quaternion[4] =
{
	{ "x" , false,false,false,0,_MT_float,"float",(int)&((Quaternion *)0)->x ,_TypeMetaSize[ _MT_float],}, 
	{ "y" , false,false,false,0,_MT_float,"float",(int)&((Quaternion *)0)->y ,_TypeMetaSize[ _MT_float],}, 
	{ "z" , false,false,false,0,_MT_float,"float",(int)&((Quaternion *)0)->z ,_TypeMetaSize[ _MT_float],}, 
	{ "w" , false,false,false,0,_MT_float,"float",(int)&((Quaternion *)0)->w ,_TypeMetaSize[ _MT_float],}, 
};

global int MemberMetaCount_D_Rectangle = 4;
global _MemberMetaData MemberMeta_D_Rectangle[4] =
{
	{ "x" , false,false,false,0,_MT_float,"float",(int)&((D_Rectangle *)0)->x ,_TypeMetaSize[ _MT_float],}, 
	{ "y" , false,false,false,0,_MT_float,"float",(int)&((D_Rectangle *)0)->y ,_TypeMetaSize[ _MT_float],}, 
	{ "width" , false,false,false,0,_MT_float,"float",(int)&((D_Rectangle *)0)->width ,_TypeMetaSize[ _MT_float],}, 
	{ "height" , false,false,false,0,_MT_float,"float",(int)&((D_Rectangle *)0)->height ,_TypeMetaSize[ _MT_float],}, 
};

global int MemberMetaCount_Image = 5;
global _MemberMetaData MemberMeta_Image[5] =
{
	{ "data" , false,true,false,0,_MT_void,"void",(int)&((Image *)0)->data ,_TypeMetaSize[ _MT_void],}, 
	{ "width" , false,false,false,0,_MT_int,"int",(int)&((Image *)0)->width ,_TypeMetaSize[ _MT_int],}, 
	{ "height" , false,false,false,0,_MT_int,"int",(int)&((Image *)0)->height ,_TypeMetaSize[ _MT_int],}, 
	{ "mipmaps" , false,false,false,0,_MT_int,"int",(int)&((Image *)0)->mipmaps ,_TypeMetaSize[ _MT_int],}, 
	{ "format" , false,false,false,0,_MT_int,"int",(int)&((Image *)0)->format ,_TypeMetaSize[ _MT_int],}, 
};

global int MemberMetaCount_Texture = 5;
global _MemberMetaData MemberMeta_Texture[5] =
{
	{ "id" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((Texture *)0)->id ,_TypeMetaSize[ _MT_unsigned_int],}, 
	{ "width" , false,false,false,0,_MT_int,"int",(int)&((Texture *)0)->width ,_TypeMetaSize[ _MT_int],}, 
	{ "height" , false,false,false,0,_MT_int,"int",(int)&((Texture *)0)->height ,_TypeMetaSize[ _MT_int],}, 
	{ "mipmaps" , false,false,false,0,_MT_int,"int",(int)&((Texture *)0)->mipmaps ,_TypeMetaSize[ _MT_int],}, 
	{ "format" , false,false,false,0,_MT_int,"int",(int)&((Texture *)0)->format ,_TypeMetaSize[ _MT_int],}, 
};

global int MemberMetaCount_Camera3D = 5;
global _MemberMetaData MemberMeta_Camera3D[5] =
{
	{ "position" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((Camera3D *)0)->position ,_TypeMetaSize[ _MT_Vector3],}, 
	{ "target" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((Camera3D *)0)->target ,_TypeMetaSize[ _MT_Vector3],}, 
	{ "up" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((Camera3D *)0)->up ,_TypeMetaSize[ _MT_Vector3],}, 
	{ "fovy" , false,false,false,0,_MT_float,"float",(int)&((Camera3D *)0)->fovy ,_TypeMetaSize[ _MT_float],}, 
	{ "projection" , false,false,false,0,_MT_int,"int",(int)&((Camera3D *)0)->projection ,_TypeMetaSize[ _MT_int],}, 
};

global int MemberMetaCount_Matrix = 16;
global _MemberMetaData MemberMeta_Matrix[16] =
{
	{ "m0" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m0 ,_TypeMetaSize[ _MT_float],}, 
	{ "m4" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m4 ,_TypeMetaSize[ _MT_float],}, 
	{ "m8" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m8 ,_TypeMetaSize[ _MT_float],}, 
	{ "m12" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m12 ,_TypeMetaSize[ _MT_float],}, 
	{ "m1" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m1 ,_TypeMetaSize[ _MT_float],}, 
	{ "m5" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m5 ,_TypeMetaSize[ _MT_float],}, 
	{ "m9" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m9 ,_TypeMetaSize[ _MT_float],}, 
	{ "m13" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m13 ,_TypeMetaSize[ _MT_float],}, 
	{ "m2" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m2 ,_TypeMetaSize[ _MT_float],}, 
	{ "m6" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m6 ,_TypeMetaSize[ _MT_float],}, 
	{ "m10" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m10 ,_TypeMetaSize[ _MT_float],}, 
	{ "m14" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m14 ,_TypeMetaSize[ _MT_float],}, 
	{ "m3" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m3 ,_TypeMetaSize[ _MT_float],}, 
	{ "m7" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m7 ,_TypeMetaSize[ _MT_float],}, 
	{ "m11" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m11 ,_TypeMetaSize[ _MT_float],}, 
	{ "m15" , false,false,false,0,_MT_float,"float",(int)&((Matrix *)0)->m15 ,_TypeMetaSize[ _MT_float],}, 
};

global int MemberMetaCount_Ray = 2;
global _MemberMetaData MemberMeta_Ray[2] =
{
	{ "position" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((Ray *)0)->position ,_TypeMetaSize[ _MT_Vector3],}, 
	{ "direction" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((Ray *)0)->direction ,_TypeMetaSize[ _MT_Vector3],}, 
};

global int MemberMetaCount_stbtt__buf = 3;
global _MemberMetaData MemberMeta_stbtt__buf[3] =
{
	{ "data" , false,true,false,0,_MT_unsigned_char,"unsigned_char",(int)&((stbtt__buf *)0)->data ,_TypeMetaSize[ _MT_unsigned_char],}, 
	{ "cursor" , false,false,false,0,_MT_int,"int",(int)&((stbtt__buf *)0)->cursor ,_TypeMetaSize[ _MT_int],}, 
	{ "size" , false,false,false,0,_MT_int,"int",(int)&((stbtt__buf *)0)->size ,_TypeMetaSize[ _MT_int],}, 
};

global int MemberMetaCount_stbtt_fontinfo = 20;
global _MemberMetaData MemberMeta_stbtt_fontinfo[20] =
{
	{ "userdata" , false,true,false,0,_MT_void,"void",(int)&((stbtt_fontinfo *)0)->userdata ,_TypeMetaSize[ _MT_void],}, 
	{ "data" , false,true,false,0,_MT_unsigned_char,"unsigned_char",(int)&((stbtt_fontinfo *)0)->data ,_TypeMetaSize[ _MT_unsigned_char],}, 
	{ "fontstart" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->fontstart ,_TypeMetaSize[ _MT_int],}, 
	{ "numGlyphs" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->numGlyphs ,_TypeMetaSize[ _MT_int],}, 
	{ "loca" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->loca ,_TypeMetaSize[ _MT_int],}, 
	{ "head" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->head ,_TypeMetaSize[ _MT_int],}, 
	{ "glyf" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->glyf ,_TypeMetaSize[ _MT_int],}, 
	{ "hhea" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->hhea ,_TypeMetaSize[ _MT_int],}, 
	{ "hmtx" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->hmtx ,_TypeMetaSize[ _MT_int],}, 
	{ "kern" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->kern ,_TypeMetaSize[ _MT_int],}, 
	{ "gpos" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->gpos ,_TypeMetaSize[ _MT_int],}, 
	{ "svg" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->svg ,_TypeMetaSize[ _MT_int],}, 
	{ "index_map" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->index_map ,_TypeMetaSize[ _MT_int],}, 
	{ "indexToLocFormat" , false,false,false,0,_MT_int,"int",(int)&((stbtt_fontinfo *)0)->indexToLocFormat ,_TypeMetaSize[ _MT_int],}, 
	{ "cff" , false,false,false,0,_MT_stbtt__buf,"stbtt__buf",(int)&((stbtt_fontinfo *)0)->cff ,_TypeMetaSize[ _MT_stbtt__buf],}, 
	{ "charstrings" , false,false,false,0,_MT_stbtt__buf,"stbtt__buf",(int)&((stbtt_fontinfo *)0)->charstrings ,_TypeMetaSize[ _MT_stbtt__buf],}, 
	{ "gsubrs" , false,false,false,0,_MT_stbtt__buf,"stbtt__buf",(int)&((stbtt_fontinfo *)0)->gsubrs ,_TypeMetaSize[ _MT_stbtt__buf],}, 
	{ "subrs" , false,false,false,0,_MT_stbtt__buf,"stbtt__buf",(int)&((stbtt_fontinfo *)0)->subrs ,_TypeMetaSize[ _MT_stbtt__buf],}, 
	{ "fontdicts" , false,false,false,0,_MT_stbtt__buf,"stbtt__buf",(int)&((stbtt_fontinfo *)0)->fontdicts ,_TypeMetaSize[ _MT_stbtt__buf],}, 
	{ "fdselect" , false,false,false,0,_MT_stbtt__buf,"stbtt__buf",(int)&((stbtt_fontinfo *)0)->fdselect ,_TypeMetaSize[ _MT_stbtt__buf],}, 
};

global int MemberMetaCount_stbrp_node = 3;
global _MemberMetaData MemberMeta_stbrp_node[3] =
{
	{ "x" , false,false,false,0,_MT_stbrp_coord,"stbrp_coord",(int)&((stbrp_node *)0)->x ,_TypeMetaSize[ _MT_stbrp_coord],}, 
	{ "y" , false,false,false,0,_MT_stbrp_coord,"stbrp_coord",(int)&((stbrp_node *)0)->y ,_TypeMetaSize[ _MT_stbrp_coord],}, 
	{ "next" , false,true,false,0,_MT_stbrp_node,"stbrp_node",(int)&((stbrp_node *)0)->next ,_TypeMetaSize[ _MT_stbrp_node],}, 
};

global int MemberMetaCount_stbrp_context = 9;
global _MemberMetaData MemberMeta_stbrp_context[9] =
{
	{ "width" , false,false,false,0,_MT_int,"int",(int)&((stbrp_context *)0)->width ,_TypeMetaSize[ _MT_int],}, 
	{ "height" , false,false,false,0,_MT_int,"int",(int)&((stbrp_context *)0)->height ,_TypeMetaSize[ _MT_int],}, 
	{ "align" , false,false,false,0,_MT_int,"int",(int)&((stbrp_context *)0)->align ,_TypeMetaSize[ _MT_int],}, 
	{ "init_mode" , false,false,false,0,_MT_int,"int",(int)&((stbrp_context *)0)->init_mode ,_TypeMetaSize[ _MT_int],}, 
	{ "heuristic" , false,false,false,0,_MT_int,"int",(int)&((stbrp_context *)0)->heuristic ,_TypeMetaSize[ _MT_int],}, 
	{ "num_nodes" , false,false,false,0,_MT_int,"int",(int)&((stbrp_context *)0)->num_nodes ,_TypeMetaSize[ _MT_int],}, 
	{ "active_head" , false,true,false,0,_MT_stbrp_node,"stbrp_node",(int)&((stbrp_context *)0)->active_head ,_TypeMetaSize[ _MT_stbrp_node],}, 
	{ "free_head" , false,true,false,0,_MT_stbrp_node,"stbrp_node",(int)&((stbrp_context *)0)->free_head ,_TypeMetaSize[ _MT_stbrp_node],}, 
	{ "extra" , false,false,true,2,_MT_stbrp_node,"stbrp_node",(int)&((stbrp_context *)0)->extra ,_TypeMetaSize[ _MT_stbrp_node],}, 
};

global int MemberMetaCount__StructMetaData = 2;
global _MemberMetaData MemberMeta__StructMetaData[2] =
{
	{ "MembersArray" , false,true,false,0,_MT__MemberMetaData,"_MemberMetaData",(int)&((_StructMetaData *)0)->MembersArray ,_TypeMetaSize[ _MT__MemberMetaData],}, 
	{ "MemberCount" , false,false,false,0,_MT_int,"int",(int)&((_StructMetaData *)0)->MemberCount ,_TypeMetaSize[ _MT_int],}, 
};

global int MemberMetaCount__MemberMetaData = 9;
global _MemberMetaData MemberMeta__MemberMetaData[9] =
{
	{ "Name" , false,false,true,64,_MT_char,"char",(int)&((_MemberMetaData *)0)->Name ,_TypeMetaSize[ _MT_char],}, 
	{ "MultiplePointer" , false,false,false,0,_MT_bool,"bool",(int)&((_MemberMetaData *)0)->MultiplePointer ,_TypeMetaSize[ _MT_bool],}, 
	{ "IsPointer" , false,false,false,0,_MT_bool,"bool",(int)&((_MemberMetaData *)0)->IsPointer ,_TypeMetaSize[ _MT_bool],}, 
	{ "IsArray" , false,false,false,0,_MT_bool,"bool",(int)&((_MemberMetaData *)0)->IsArray ,_TypeMetaSize[ _MT_bool],}, 
	{ "ArraySize" , false,false,false,0,_MT_int,"int",(int)&((_MemberMetaData *)0)->ArraySize ,_TypeMetaSize[ _MT_int],}, 
	{ "MemberType" , false,false,false,0,_MT_int,"int",(int)&((_MemberMetaData *)0)->MemberType ,_TypeMetaSize[ _MT_int],}, 
	{ "TypeName" , false,false,true,64,_MT_char,"char",(int)&((_MemberMetaData *)0)->TypeName ,_TypeMetaSize[ _MT_char],}, 
	{ "MemberOffset" , false,false,false,0,_MT_int,"int",(int)&((_MemberMetaData *)0)->MemberOffset ,_TypeMetaSize[ _MT_int],}, 
	{ "MemberSize" , false,false,false,0,_MT_int,"int",(int)&((_MemberMetaData *)0)->MemberSize ,_TypeMetaSize[ _MT_int],}, 
};

global int MemberMetaCount__StructMetaDataNode = 6;
global _MemberMetaData MemberMeta__StructMetaDataNode[6] =
{
	{ "Name" , false,true,false,0,_MT_char,"char",(int)&((_StructMetaDataNode *)0)->Name ,_TypeMetaSize[ _MT_char],}, 
	{ "TypeIndex" , false,false,false,0,_MT_int,"int",(int)&((_StructMetaDataNode *)0)->TypeIndex ,_TypeMetaSize[ _MT_int],}, 
	{ "Size" , false,false,false,0,_MT_int,"int",(int)&((_StructMetaDataNode *)0)->Size ,_TypeMetaSize[ _MT_int],}, 
	{ "Introspected" , false,false,false,0,_MT_bool,"bool",(int)&((_StructMetaDataNode *)0)->Introspected ,_TypeMetaSize[ _MT_bool],}, 
	{ "StructMeta" , false,false,false,0,_MT__StructMetaData,"_StructMetaData",(int)&((_StructMetaDataNode *)0)->StructMeta ,_TypeMetaSize[ _MT__StructMetaData],}, 
	{ "Next" , false,true,false,0,_MT__StructMetaDataNode,"_StructMetaDataNode",(int)&((_StructMetaDataNode *)0)->Next ,_TypeMetaSize[ _MT__StructMetaDataNode],}, 
};

global int MemberMetaCount__MemberNode = 3;
global _MemberMetaData MemberMeta__MemberNode[3] =
{
	{ "Member" , false,true,false,0,_MT__MemberMetaData,"_MemberMetaData",(int)&((_MemberNode *)0)->Member ,_TypeMetaSize[ _MT__MemberMetaData],}, 
	{ "DataIndex" , false,false,false,0,_MT_int,"int",(int)&((_MemberNode *)0)->DataIndex ,_TypeMetaSize[ _MT_int],}, 
	{ "Previous" , false,true,false,0,_MT__MemberNode,"_MemberNode",(int)&((_MemberNode *)0)->Previous ,_TypeMetaSize[ _MT__MemberNode],}, 
};

global int MemberMetaCount__DecomposedBasicType = 10;
global _MemberMetaData MemberMeta__DecomposedBasicType[10] =
{
	{ "Name" , false,true,false,0,_MT_char,"char",(int)&((_DecomposedBasicType *)0)->Name ,_TypeMetaSize[ _MT_char],}, 
	{ "Type" , false,false,false,0,_MT_int,"int",(int)&((_DecomposedBasicType *)0)->Type ,_TypeMetaSize[ _MT_int],}, 
	{ "Offset" , false,false,false,0,_MT_int,"int",(int)&((_DecomposedBasicType *)0)->Offset ,_TypeMetaSize[ _MT_int],}, 
	{ "IsPointer" , false,false,false,0,_MT_bool,"bool",(int)&((_DecomposedBasicType *)0)->IsPointer ,_TypeMetaSize[ _MT_bool],}, 
	{ "DataCount" , false,false,false,0,_MT_int,"int",(int)&((_DecomposedBasicType *)0)->DataCount ,_TypeMetaSize[ _MT_int],}, 
	{ "Taken" , false,false,false,0,_MT_bool,"bool",(int)&((_DecomposedBasicType *)0)->Taken ,_TypeMetaSize[ _MT_bool],}, 
	{ "PreviousMemoryLocation" , false,false,false,0,_MT_int,"int",(int)&((_DecomposedBasicType *)0)->PreviousMemoryLocation ,_TypeMetaSize[ _MT_int],}, 
	{ "CurrentMemoryLocation" , false,false,false,0,_MT_int,"int",(int)&((_DecomposedBasicType *)0)->CurrentMemoryLocation ,_TypeMetaSize[ _MT_int],}, 
	{ "ReverseMemberList" , false,true,false,0,_MT__MemberNode,"_MemberNode",(int)&((_DecomposedBasicType *)0)->ReverseMemberList ,_TypeMetaSize[ _MT__MemberNode],}, 
	{ "Next" , false,true,false,0,_MT__DecomposedBasicType,"_DecomposedBasicType",(int)&((_DecomposedBasicType *)0)->Next ,_TypeMetaSize[ _MT__DecomposedBasicType],}, 
};

global int MemberMetaCount__TypeFormDifference = 9;
global _MemberMetaData MemberMeta__TypeFormDifference[9] =
{
	{ "CurrentStruct" , false,false,false,0,_MT__StructMetaData,"_StructMetaData",(int)&((_TypeFormDifference *)0)->CurrentStruct ,_TypeMetaSize[ _MT__StructMetaData],}, 
	{ "PreviousStruct" , false,false,false,0,_MT__StructMetaData,"_StructMetaData",(int)&((_TypeFormDifference *)0)->PreviousStruct ,_TypeMetaSize[ _MT__StructMetaData],}, 
	{ "CurrentIsStruct" , false,false,false,0,_MT_bool,"bool",(int)&((_TypeFormDifference *)0)->CurrentIsStruct ,_TypeMetaSize[ _MT_bool],}, 
	{ "CurrentIsEnumType" , false,false,false,0,_MT_bool,"bool",(int)&((_TypeFormDifference *)0)->CurrentIsEnumType ,_TypeMetaSize[ _MT_bool],}, 
	{ "CurrentIsBasicType" , false,false,false,0,_MT_bool,"bool",(int)&((_TypeFormDifference *)0)->CurrentIsBasicType ,_TypeMetaSize[ _MT_bool],}, 
	{ "PreviousIsStruct" , false,false,false,0,_MT_bool,"bool",(int)&((_TypeFormDifference *)0)->PreviousIsStruct ,_TypeMetaSize[ _MT_bool],}, 
	{ "PreviousIsEnumType" , false,false,false,0,_MT_bool,"bool",(int)&((_TypeFormDifference *)0)->PreviousIsEnumType ,_TypeMetaSize[ _MT_bool],}, 
	{ "PreviousIsBasicType" , false,false,false,0,_MT_bool,"bool",(int)&((_TypeFormDifference *)0)->PreviousIsBasicType ,_TypeMetaSize[ _MT_bool],}, 
	{ "DifferentFormOfType" , false,false,false,0,_MT_bool,"bool",(int)&((_TypeFormDifference *)0)->DifferentFormOfType ,_TypeMetaSize[ _MT_bool],}, 
};

global int MemberMetaCount__MemberDifference = 11;
global _MemberMetaData MemberMeta__MemberDifference[11] =
{
	{ "Same" , false,false,false,0,_MT_bool,"bool",(int)&((_MemberDifference *)0)->Same ,_TypeMetaSize[ _MT_bool],}, 
	{ "DifferentName" , false,false,false,0,_MT_bool,"bool",(int)&((_MemberDifference *)0)->DifferentName ,_TypeMetaSize[ _MT_bool],}, 
	{ "DifferentType" , false,false,false,0,_MT_bool,"bool",(int)&((_MemberDifference *)0)->DifferentType ,_TypeMetaSize[ _MT_bool],}, 
	{ "DifferentPointer" , false,false,false,0,_MT_bool,"bool",(int)&((_MemberDifference *)0)->DifferentPointer ,_TypeMetaSize[ _MT_bool],}, 
	{ "DifferentArray" , false,false,false,0,_MT_bool,"bool",(int)&((_MemberDifference *)0)->DifferentArray ,_TypeMetaSize[ _MT_bool],}, 
	{ "DifferentArraySize" , false,false,false,0,_MT_bool,"bool",(int)&((_MemberDifference *)0)->DifferentArraySize ,_TypeMetaSize[ _MT_bool],}, 
	{ "CurrentName" , false,true,false,0,_MT_char,"char",(int)&((_MemberDifference *)0)->CurrentName ,_TypeMetaSize[ _MT_char],}, 
	{ "CurrentTypeName" , false,true,false,0,_MT_char,"char",(int)&((_MemberDifference *)0)->CurrentTypeName ,_TypeMetaSize[ _MT_char],}, 
	{ "PreviousName" , false,true,false,0,_MT_char,"char",(int)&((_MemberDifference *)0)->PreviousName ,_TypeMetaSize[ _MT_char],}, 
	{ "PreviousTypeName" , false,true,false,0,_MT_char,"char",(int)&((_MemberDifference *)0)->PreviousTypeName ,_TypeMetaSize[ _MT_char],}, 
	{ "TypeFormDifference" , false,false,false,0,_MT__TypeFormDifference,"_TypeFormDifference",(int)&((_MemberDifference *)0)->TypeFormDifference ,_TypeMetaSize[ _MT__TypeFormDifference],}, 
};

global int MemberMetaCount__TypeToCheckInfo = 4;
global _MemberMetaData MemberMeta__TypeToCheckInfo[4] =
{
	{ "Start" , false,false,false,0,_MT_long_long,"long_long",(int)&((_TypeToCheckInfo *)0)->Start ,_TypeMetaSize[ _MT_long_long],}, 
	{ "DataOffset" , false,false,false,0,_MT_int,"int",(int)&((_TypeToCheckInfo *)0)->DataOffset ,_TypeMetaSize[ _MT_int],}, 
	{ "IsPointer" , false,false,false,0,_MT_bool,"bool",(int)&((_TypeToCheckInfo *)0)->IsPointer ,_TypeMetaSize[ _MT_bool],}, 
	{ "_Type" , false,false,false,0,_MT_int,"int",(int)&((_TypeToCheckInfo *)0)->_Type ,_TypeMetaSize[ _MT_int],}, 
};

global int MemberMetaCount__TypeToCheck = 4;
global _MemberMetaData MemberMeta__TypeToCheck[4] =
{
	{ "StackDepth" , false,false,false,0,_MT_int,"int",(int)&((_TypeToCheck *)0)->StackDepth ,_TypeMetaSize[ _MT_int],}, 
	{ "Current" , false,false,false,0,_MT__TypeToCheckInfo,"_TypeToCheckInfo",(int)&((_TypeToCheck *)0)->Current ,_TypeMetaSize[ _MT__TypeToCheckInfo],}, 
	{ "Previous" , false,false,false,0,_MT__TypeToCheckInfo,"_TypeToCheckInfo",(int)&((_TypeToCheck *)0)->Previous ,_TypeMetaSize[ _MT__TypeToCheckInfo],}, 
	{ "ArrayCount" , false,false,false,0,_MT_int,"int",(int)&((_TypeToCheck *)0)->ArrayCount ,_TypeMetaSize[ _MT_int],}, 
};

global int MemberMetaCount__MemoryDetail = 8;
global _MemberMetaData MemberMeta__MemoryDetail[8] =
{
	{ "Start" , false,false,false,0,_MT_long_long,"long_long",(int)&((_MemoryDetail *)0)->Start ,_TypeMetaSize[ _MT_long_long],}, 
	{ "End" , false,false,false,0,_MT_long_long,"long_long",(int)&((_MemoryDetail *)0)->End ,_TypeMetaSize[ _MT_long_long],}, 
	{ "DataName" , false,false,true,64,_MT_char,"char",(int)&((_MemoryDetail *)0)->DataName ,_TypeMetaSize[ _MT_char],}, 
	{ "IsPointer" , false,false,false,0,_MT_bool,"bool",(int)&((_MemoryDetail *)0)->IsPointer ,_TypeMetaSize[ _MT_bool],}, 
	{ "Type" , false,false,false,0,_MT_int,"int",(int)&((_MemoryDetail *)0)->Type ,_TypeMetaSize[ _MT_int],}, 
	{ "DataCount" , false,false,false,0,_MT_int,"int",(int)&((_MemoryDetail *)0)->DataCount ,_TypeMetaSize[ _MT_int],}, 
	{ "OldDetailFromPreviousMemory" , false,true,false,0,_MT__MemoryDetail,"_MemoryDetail",(int)&((_MemoryDetail *)0)->OldDetailFromPreviousMemory ,_TypeMetaSize[ _MT__MemoryDetail],}, 
	{ "NextNode" , false,true,false,0,_MT__MemoryDetail,"_MemoryDetail",(int)&((_MemoryDetail *)0)->NextNode ,_TypeMetaSize[ _MT__MemoryDetail],}, 
};

global int MemberMetaCount__PersistMemory = 7;
global _MemberMetaData MemberMeta__PersistMemory[7] =
{
	{ "StartMemoryAddress" , false,true,false,0,_MT_unsigned_char,"unsigned_char",(int)&((_PersistMemory *)0)->StartMemoryAddress ,_TypeMetaSize[ _MT_unsigned_char],}, 
	{ "StartOffset" , false,false,false,0,_MT_long_long,"long_long",(int)&((_PersistMemory *)0)->StartOffset ,_TypeMetaSize[ _MT_long_long],}, 
	{ "CurrentOffset" , false,false,false,0,_MT_long_long,"long_long",(int)&((_PersistMemory *)0)->CurrentOffset ,_TypeMetaSize[ _MT_long_long],}, 
	{ "Size" , false,false,false,0,_MT_long_long,"long_long",(int)&((_PersistMemory *)0)->Size ,_TypeMetaSize[ _MT_long_long],}, 
	{ "MemoryDetailHead" , false,true,false,0,_MT__MemoryDetail,"_MemoryDetail",(int)&((_PersistMemory *)0)->MemoryDetailHead ,_TypeMetaSize[ _MT__MemoryDetail],}, 
	{ "MemoryDetailTail" , false,true,false,0,_MT__MemoryDetail,"_MemoryDetail",(int)&((_PersistMemory *)0)->MemoryDetailTail ,_TypeMetaSize[ _MT__MemoryDetail],}, 
	{ "MemoryDetailCount" , false,false,false,0,_MT_int,"int",(int)&((_PersistMemory *)0)->MemoryDetailCount ,_TypeMetaSize[ _MT_int],}, 
};

global int MemberMetaCount_PointerRecordNode = 5;
global _MemberMetaData MemberMeta_PointerRecordNode[5] =
{
	{ "PointerStack" , true,true,false,0,_MT_void,"void",(int)&((PointerRecordNode *)0)->PointerStack ,_TypeMetaSize[ _MT_void],}, 
	{ "CurrentTypeStack" , false,true,false,0,_MT_int,"int",(int)&((PointerRecordNode *)0)->CurrentTypeStack ,_TypeMetaSize[ _MT_int],}, 
	{ "PreviousTypeStack" , false,true,false,0,_MT_int,"int",(int)&((PointerRecordNode *)0)->PreviousTypeStack ,_TypeMetaSize[ _MT_int],}, 
	{ "StackCount" , false,false,false,0,_MT_int,"int",(int)&((PointerRecordNode *)0)->StackCount ,_TypeMetaSize[ _MT_int],}, 
	{ "Next" , false,true,false,0,_MT_PointerRecordNode,"PointerRecordNode",(int)&((PointerRecordNode *)0)->Next ,_TypeMetaSize[ _MT_PointerRecordNode],}, 
};

global int MemberMetaCount__TemporayMemory = 3;
global _MemberMetaData MemberMeta__TemporayMemory[3] =
{
	{ "StartMemory" , false,true,false,0,_MT_unsigned_char,"unsigned_char",(int)&((_TemporayMemory *)0)->StartMemory ,_TypeMetaSize[ _MT_unsigned_char],}, 
	{ "CurrentMemory" , false,true,false,0,_MT_unsigned_char,"unsigned_char",(int)&((_TemporayMemory *)0)->CurrentMemory ,_TypeMetaSize[ _MT_unsigned_char],}, 
	{ "Size" , false,false,false,0,_MT_int,"int",(int)&((_TemporayMemory *)0)->Size ,_TypeMetaSize[ _MT_int],}, 
};

global int MemberMetaCount_LineData = 2;
global _MemberMetaData MemberMeta_LineData[2] =
{
	{ "Position" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((LineData *)0)->Position ,_TypeMetaSize[ _MT_Vector3],}, 
	{ "LineColor" , false,false,false,0,_MT_Color,"Color",(int)&((LineData *)0)->LineColor ,_TypeMetaSize[ _MT_Color],}, 
};

global int MemberMetaCount__Rect = 3;
global _MemberMetaData MemberMeta__Rect[3] =
{
	{ "Position" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((_Rect *)0)->Position ,_TypeMetaSize[ _MT_Vector3],}, 
	{ "Size" , false,false,false,0,_MT_Vector2,"Vector2",(int)&((_Rect *)0)->Size ,_TypeMetaSize[ _MT_Vector2],}, 
	{ "Rotation" , false,false,false,0,_MT_Quaternion,"Quaternion",(int)&((_Rect *)0)->Rotation ,_TypeMetaSize[ _MT_Quaternion],}, 
};

global int MemberMetaCount__Quad = 2;
global _MemberMetaData MemberMeta__Quad[2] =
{
	{ "VertexPosition" , false,false,true,Vertex_Count,_MT_Vector3,"Vector3",(int)&((_Quad *)0)->VertexPosition ,_TypeMetaSize[ _MT_Vector3],}, 
	{ "VertexColor" , false,false,true,Vertex_Count,_MT_Vector4,"Vector4",(int)&((_Quad *)0)->VertexColor ,_TypeMetaSize[ _MT_Vector4],}, 
};

global int MemberMetaCount_D_GlyphInfo = 5;
global _MemberMetaData MemberMeta_D_GlyphInfo[5] =
{
	{ "value" , false,false,false,0,_MT_int,"int",(int)&((D_GlyphInfo *)0)->value ,_TypeMetaSize[ _MT_int],}, 
	{ "offsetX" , false,false,false,0,_MT_int,"int",(int)&((D_GlyphInfo *)0)->offsetX ,_TypeMetaSize[ _MT_int],}, 
	{ "offsetY" , false,false,false,0,_MT_int,"int",(int)&((D_GlyphInfo *)0)->offsetY ,_TypeMetaSize[ _MT_int],}, 
	{ "advanceX" , false,false,false,0,_MT_int,"int",(int)&((D_GlyphInfo *)0)->advanceX ,_TypeMetaSize[ _MT_int],}, 
	{ "GlyphRect" , false,false,false,0,_MT_D_Rectangle,"D_Rectangle",(int)&((D_GlyphInfo *)0)->GlyphRect ,_TypeMetaSize[ _MT_D_Rectangle],}, 
};

global int MemberMetaCount_D_GlyphInfoNode = 2;
global _MemberMetaData MemberMeta_D_GlyphInfoNode[2] =
{
	{ "Info" , false,false,false,0,_MT_D_GlyphInfo,"D_GlyphInfo",(int)&((D_GlyphInfoNode *)0)->Info ,_TypeMetaSize[ _MT_D_GlyphInfo],}, 
	{ "NextNode" , false,true,false,0,_MT_D_GlyphInfoNode,"D_GlyphInfoNode",(int)&((D_GlyphInfoNode *)0)->NextNode ,_TypeMetaSize[ _MT_D_GlyphInfoNode],}, 
};

global int MemberMetaCount__D_Font_Info = 10;
global _MemberMetaData MemberMeta__D_Font_Info[10] =
{
	{ "FontInfo" , false,false,false,0,_MT_stbtt_fontinfo,"stbtt_fontinfo",(int)&((_D_Font_Info *)0)->FontInfo ,_TypeMetaSize[ _MT_stbtt_fontinfo],}, 
	{ "GlyphAtlasImage" , false,false,false,0,_MT_Image,"Image",(int)&((_D_Font_Info *)0)->GlyphAtlasImage ,_TypeMetaSize[ _MT_Image],}, 
	{ "GlyphAltasPadding" , false,false,false,0,_MT_int,"int",(int)&((_D_Font_Info *)0)->GlyphAltasPadding ,_TypeMetaSize[ _MT_int],}, 
	{ "GlyphResolution" , false,false,false,0,_MT_int,"int",(int)&((_D_Font_Info *)0)->GlyphResolution ,_TypeMetaSize[ _MT_int],}, 
	{ "AllGlyphInfo" , false,true,false,0,_MT_D_GlyphInfoNode,"D_GlyphInfoNode",(int)&((_D_Font_Info *)0)->AllGlyphInfo ,_TypeMetaSize[ _MT_D_GlyphInfoNode],}, 
	{ "AllGlyphInfoCount" , false,false,false,0,_MT_int,"int",(int)&((_D_Font_Info *)0)->AllGlyphInfoCount ,_TypeMetaSize[ _MT_int],}, 
	{ "GlyphHashmap" , true,true,false,0,_MT_D_GlyphInfoNode,"D_GlyphInfoNode",(int)&((_D_Font_Info *)0)->GlyphHashmap ,_TypeMetaSize[ _MT_D_GlyphInfoNode],}, 
	{ "STB_RP_Context" , false,false,false,0,_MT_stbrp_context,"stbrp_context",(int)&((_D_Font_Info *)0)->STB_RP_Context ,_TypeMetaSize[ _MT_stbrp_context],}, 
	{ "_Nodes" , false,true,false,0,_MT_stbrp_node,"stbrp_node",(int)&((_D_Font_Info *)0)->_Nodes ,_TypeMetaSize[ _MT_stbrp_node],}, 
	{ "GlyphAtlasTexture" , false,false,false,0,_MT_Texture2D,"Texture2D",(int)&((_D_Font_Info *)0)->GlyphAtlasTexture ,_TypeMetaSize[ _MT_Texture2D],}, 
};

global int MemberMetaCount_D_App_Data = 37;
global _MemberMetaData MemberMeta_D_App_Data[37] =
{
	{ "RunTimeMemory" , false,false,false,0,_MT__TemporayMemory,"_TemporayMemory",(int)&((D_App_Data *)0)->RunTimeMemory ,_TypeMetaSize[ _MT__TemporayMemory],}, 
	{ "FrameTimeMemory" , false,false,false,0,_MT__TemporayMemory,"_TemporayMemory",(int)&((D_App_Data *)0)->FrameTimeMemory ,_TypeMetaSize[ _MT__TemporayMemory],}, 
	{ "GameEditorData" , false,false,false,0,_MT__PersistMemory,"_PersistMemory",(int)&((D_App_Data *)0)->GameEditorData ,_TypeMetaSize[ _MT__PersistMemory],}, 
	{ "LoopCount" , false,false,false,0,_MT_int,"int",(int)&((D_App_Data *)0)->LoopCount ,_TypeMetaSize[ _MT_int],}, 
	{ "_GameUnload" , false,true,false,0,_MT_GameUnloadFunction,"GameUnloadFunction",(int)&((D_App_Data *)0)->_GameUnload ,_TypeMetaSize[ _MT_GameUnloadFunction],}, 
	{ "_GameLoop" , false,true,false,0,_MT_GameLoopFunction,"GameLoopFunction",(int)&((D_App_Data *)0)->_GameLoop ,_TypeMetaSize[ _MT_GameLoopFunction],}, 
	{ "LoopTime" , false,false,false,0,_MT_double,"double",(int)&((D_App_Data *)0)->LoopTime ,_TypeMetaSize[ _MT_double],}, 
	{ "GameCamera" , false,false,false,0,_MT_Camera3D,"Camera3D",(int)&((D_App_Data *)0)->GameCamera ,_TypeMetaSize[ _MT_Camera3D],}, 
	{ "MVP_Matrix" , false,false,false,0,_MT_Matrix,"Matrix",(int)&((D_App_Data *)0)->MVP_Matrix ,_TypeMetaSize[ _MT_Matrix],}, 
	{ "Top" , false,false,false,0,_MT_double,"double",(int)&((D_App_Data *)0)->Top ,_TypeMetaSize[ _MT_double],}, 
	{ "Right" , false,false,false,0,_MT_double,"double",(int)&((D_App_Data *)0)->Right ,_TypeMetaSize[ _MT_double],}, 
	{ "MousePosition" , false,false,false,0,_MT_Vector2,"Vector2",(int)&((D_App_Data *)0)->MousePosition ,_TypeMetaSize[ _MT_Vector2],}, 
	{ "MouseRay3D" , false,false,false,0,_MT_Ray,"Ray",(int)&((D_App_Data *)0)->MouseRay3D ,_TypeMetaSize[ _MT_Ray],}, 
	{ "WindowScreenViewportPanel" , false,true,false,0,_MT__Rect,"_Rect",(int)&((D_App_Data *)0)->WindowScreenViewportPanel ,_TypeMetaSize[ _MT__Rect],}, 
	{ "DefaultFont" , false,false,false,0,_MT__D_Font_Info,"_D_Font_Info",(int)&((D_App_Data *)0)->DefaultFont ,_TypeMetaSize[ _MT__D_Font_Info],}, 
	{ "PressingKeyList" , false,false,true,ALL_MOUSE_KEY,_MT_bool,"bool",(int)&((D_App_Data *)0)->PressingKeyList ,_TypeMetaSize[ _MT_bool],}, 
	{ "PressingMouseList" , false,false,true,ALL_KEY,_MT_bool,"bool",(int)&((D_App_Data *)0)->PressingMouseList ,_TypeMetaSize[ _MT_bool],}, 
	{ "ReleasedMouseArray" , false,false,true,MAX_KEY,_MT_int,"int",(int)&((D_App_Data *)0)->ReleasedMouseArray ,_TypeMetaSize[ _MT_int],}, 
	{ "ReleasedMouseArrayCount" , false,false,false,0,_MT_int,"int",(int)&((D_App_Data *)0)->ReleasedMouseArrayCount ,_TypeMetaSize[ _MT_int],}, 
	{ "PressedMouseArray" , false,false,true,MAX_KEY,_MT_int,"int",(int)&((D_App_Data *)0)->PressedMouseArray ,_TypeMetaSize[ _MT_int],}, 
	{ "PressedMouseArrayCount" , false,false,false,0,_MT_int,"int",(int)&((D_App_Data *)0)->PressedMouseArrayCount ,_TypeMetaSize[ _MT_int],}, 
	{ "CodepointQueueIndex" , false,false,false,0,_MT_int,"int",(int)&((D_App_Data *)0)->CodepointQueueIndex ,_TypeMetaSize[ _MT_int],}, 
	{ "CodepointQueueCount" , false,false,false,0,_MT_int,"int",(int)&((D_App_Data *)0)->CodepointQueueCount ,_TypeMetaSize[ _MT_int],}, 
	{ "CodepointQueue" , false,false,true,64,_MT_unsigned_int,"unsigned_int",(int)&((D_App_Data *)0)->CodepointQueue ,_TypeMetaSize[ _MT_unsigned_int],}, 
	{ "PressedKeyArray" , false,false,true,MAX_KEY,_MT_int,"int",(int)&((D_App_Data *)0)->PressedKeyArray ,_TypeMetaSize[ _MT_int],}, 
	{ "PressedKeyArrayCount" , false,false,false,0,_MT_int,"int",(int)&((D_App_Data *)0)->PressedKeyArrayCount ,_TypeMetaSize[ _MT_int],}, 
	{ "ReleasedKeyArray" , false,false,true,MAX_KEY,_MT_int,"int",(int)&((D_App_Data *)0)->ReleasedKeyArray ,_TypeMetaSize[ _MT_int],}, 
	{ "ReleasedKeyArrayCount" , false,false,false,0,_MT_int,"int",(int)&((D_App_Data *)0)->ReleasedKeyArrayCount ,_TypeMetaSize[ _MT_int],}, 
	{ "CurrentWindow" , false,true,false,0,_MT_GLFWwindow,"GLFWwindow",(int)&((D_App_Data *)0)->CurrentWindow ,_TypeMetaSize[ _MT_GLFWwindow],}, 
	{ "CurrentFoucsWindow" , false,true,false,0,_MT_GLFWwindow,"GLFWwindow",(int)&((D_App_Data *)0)->CurrentFoucsWindow ,_TypeMetaSize[ _MT_GLFWwindow],}, 
	{ "WindowSize" , false,false,false,0,_MT_Vector2,"Vector2",(int)&((D_App_Data *)0)->WindowSize ,_TypeMetaSize[ _MT_Vector2],}, 
	{ "ExitWindow" , false,false,false,0,_MT_bool,"bool",(int)&((D_App_Data *)0)->ExitWindow ,_TypeMetaSize[ _MT_bool],}, 
	{ "UpdateAtlas" , false,false,false,0,_MT_bool,"bool",(int)&((D_App_Data *)0)->UpdateAtlas ,_TypeMetaSize[ _MT_bool],}, 
	{ "MouseScrollDelta" , false,false,false,0,_MT_float,"float",(int)&((D_App_Data *)0)->MouseScrollDelta ,_TypeMetaSize[ _MT_float],}, 
	{ "WindowTextSize" , false,false,false,0,_MT_float,"float",(int)&((D_App_Data *)0)->WindowTextSize ,_TypeMetaSize[ _MT_float],}, 
	{ "ApplicationPath" , false,false,true,MAX_FILE_PATH,_MT_char,"char",(int)&((D_App_Data *)0)->ApplicationPath ,_TypeMetaSize[ _MT_char],}, 
	{ "_HDC" , false,false,false,0,_MT_HDC,"HDC",(int)&((D_App_Data *)0)->_HDC ,_TypeMetaSize[ _MT_HDC],}, 
};

global _StructMetaData _TypeStructMeta_[] = 
{
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	GetStructMeta(Color),
	GetStructMeta(Vector4),
	GetStructMeta(Vector2),
	GetStructMeta(Vector3),
	GetStructMeta(Quaternion),
	GetStructMeta(D_Rectangle),
	GetStructMeta(Image),
	GetStructMeta(Texture),
	{},
	GetStructMeta(Camera3D),
	GetStructMeta(Matrix),
	GetStructMeta(Ray),
	GetStructMeta(stbtt__buf),
	GetStructMeta(stbtt_fontinfo),
	{},
	GetStructMeta(stbrp_node),
	GetStructMeta(stbrp_context),
	GetStructMeta(_StructMetaData),
	GetStructMeta(_MemberMetaData),
	GetStructMeta(_StructMetaDataNode),
	GetStructMeta(_MemberNode),
	GetStructMeta(_DecomposedBasicType),
	GetStructMeta(_TypeFormDifference),
	GetStructMeta(_MemberDifference),
	GetStructMeta(_TypeToCheckInfo),
	GetStructMeta(_TypeToCheck),
	GetStructMeta(_MemoryDetail),
	GetStructMeta(_PersistMemory),
	GetStructMeta(PointerRecordNode),
	GetStructMeta(_TemporayMemory),
	{},
	GetStructMeta(LineData),
	{},
	GetStructMeta(_Rect),
	GetStructMeta(_Quad),
	GetStructMeta(D_GlyphInfo),
	GetStructMeta(D_GlyphInfoNode),
	GetStructMeta(_D_Font_Info),
	GetStructMeta(D_App_Data),
	{},
	{},
	{},
	{},
	{},
};

