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
	_MT_D_ShaderUniform,
	_MT_D_ShaderType,
	_MT_D_ShaderCode,
	_MT_GLShaderType,
	_MT__ShaderInput,
	_MT__ShaderBuffer,
	_MT_String,
	_MT_TextureMetaNode,
	_MT_D_Render_State,
	_MT_D_Light_GPU_Data,
	_MT_D_Light,
	_MT_D_Vertex_Data,
	_MT_BoneTag,
	_MT__BoneState,
	_MT__KeyFrame,
	_MT__HashTableEntry,
	_MT__HashTableSlot,
	_MT__ListNode,
	_MT_NodeTag,
	_MT__List,
	_MT__HashTable,
	_MT_RotationAxis,
	_MT_RotationAxisData,
	_MT__Bone,
	_MT_GameMenuType,
	_MT_DrawingMenu,
	_MT_Editor_Data,
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
sizeof(D_ShaderUniform),//D_ShaderUniform 
sizeof(D_ShaderType),//D_ShaderType 
sizeof(D_ShaderCode),//D_ShaderCode 
sizeof(GLShaderType),//GLShaderType 
sizeof(_ShaderInput),//_ShaderInput 
sizeof(_ShaderBuffer),//_ShaderBuffer 
sizeof(String),//String 
sizeof(TextureMetaNode),//TextureMetaNode 
sizeof(D_Render_State),//D_Render_State 
sizeof(D_Light_GPU_Data),//D_Light_GPU_Data 
sizeof(D_Light),//D_Light 
sizeof(D_Vertex_Data),//D_Vertex_Data 
sizeof(BoneTag),//BoneTag 
sizeof(_BoneState),//_BoneState 
sizeof(_KeyFrame),//_KeyFrame 
sizeof(_HashTableEntry),//_HashTableEntry 
sizeof(_HashTableSlot),//_HashTableSlot 
sizeof(_ListNode),//_ListNode 
sizeof(NodeTag),//NodeTag 
sizeof(_List),//_List 
sizeof(_HashTable),//_HashTable 
sizeof(RotationAxis),//RotationAxis 
sizeof(RotationAxisData),//RotationAxisData 
sizeof(_Bone),//_Bone 
sizeof(GameMenuType),//GameMenuType 
sizeof(DrawingMenu),//DrawingMenu 
sizeof(Editor_Data),//Editor_Data 
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
	"D_ShaderUniform",
	"D_ShaderType",
	"D_ShaderCode",
	"GLShaderType",
	"_ShaderInput",
	"_ShaderBuffer",
	"String",
	"TextureMetaNode",
	"D_Render_State",
	"D_Light_GPU_Data",
	"D_Light",
	"D_Vertex_Data",
	"BoneTag",
	"_BoneState",
	"_KeyFrame",
	"_HashTableEntry",
	"_HashTableSlot",
	"_ListNode",
	"NodeTag",
	"_List",
	"_HashTable",
	"RotationAxis",
	"RotationAxisData",
	"_Bone",
	"GameMenuType",
	"DrawingMenu",
	"Editor_Data",
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
true,
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
true,
false,
false,
false,
false,
false,
true,
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

global const char * D_ShaderUniform_String[] = 
{
 "SU_MVP",
 "SU_Texture0",
 "SU_ScreenSize",
 "SU_GridSize",
 "SU_Spread",
 "SU_ElapsedTime",
 "SU_COUNT",
};

global const char * D_ShaderType_String[] = 
{
 "S_Point_Light",
 "S_Rect",
 "S_Quad",
 "S_Line",
 "S_Text",
 "S_Bloom",
 "S_Blur_H",
 "S_Blur_V",
 "S_Spring",
 "S_COUNT",
};

global const char * GLShaderType_String[] = 
{
 "GS_Vert",
 "GS_Geo",
 "GS_Frag",
 "GS_Count",
};

global const char * BoneTag_String[] = 
{
 "B_Origin",
 "B_RightHandController",
 "B_RightArmPoleTarget",
 "B_LeftArmPoleTarget",
 "B_LeftHandController",
 "B_RightLegController",
 "B_RightLegPoleTarget",
 "B_LeftLegController",
 "B_LeftLegPoleTarget",
 "B_BodyOrigin",
 "B_Head",
 "B_UpperBody",
 "B_LowerBody",
 "B_RightHand",
 "B_RightUpperArm",
 "B_RightLowerArm",
 "B_LeftUpperArm",
 "B_LeftLowerArm",
 "B_LeftHand",
 "B_Pelvic",
 "B_LeftUpperLeg",
 "B_LeftLowerLeg",
 "B_LeftFoot",
 "B_RightUpperLeg",
 "B_RightLowerLeg",
 "B_RightFoot",
 "BoneCount",
};

global const char * NodeTag_String[] = 
{
 "N_NodeHead",
 "N_NodeTail",
 "N_RecycledNodeHead",
 "N_RecycledNodeTail",
 "DummyNodeCount",
};

global const char * RotationAxis_String[] = 
{
 "R_None",
 "R_Z",
 "R_Y",
 "R_X",
 "R_Count",
};

global const char * GameMenuType_String[] = 
{
 "GMT_Descend",
 "GMT_ToTheRight",
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
	IS_D_ShaderCode, 
	IS__ShaderInput, 
	IS__ShaderBuffer, 
	IS_String, 
	IS_TextureMetaNode, 
	IS_D_Render_State, 
	IS_D_Light_GPU_Data, 
	IS_D_Light, 
	IS_D_Vertex_Data, 
	IS__BoneState, 
	IS__KeyFrame, 
	IS__HashTableEntry, 
	IS__HashTableSlot, 
	IS__ListNode, 
	IS__List, 
	IS__HashTable, 
	IS_RotationAxisData, 
	IS__Bone, 
	IS_DrawingMenu, 
	IS_Editor_Data, 
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

global int MemberMetaCount_D_ShaderCode = 2;
global _MemberMetaData MemberMeta_D_ShaderCode[2] =
{
	{ "FragmentShaderCode" , false,true,false,0,_MT_char,"char",(int)&((D_ShaderCode *)0)->FragmentShaderCode ,_TypeMetaSize[ _MT_char],}, 
	{ "VertexShaderCode" , false,true,false,0,_MT_char,"char",(int)&((D_ShaderCode *)0)->VertexShaderCode ,_TypeMetaSize[ _MT_char],}, 
};

global int MemberMetaCount__ShaderInput = 8;
global _MemberMetaData MemberMeta__ShaderInput[8] =
{
	{ "ShaderType" , false,false,false,0,_MT_D_ShaderType,"D_ShaderType",(int)&((_ShaderInput *)0)->ShaderType ,_TypeMetaSize[ _MT_D_ShaderType],}, 
	{ "ShaderSourceFileName" , false,true,true,GS_Count,_MT_char,"char",(int)&((_ShaderInput *)0)->ShaderSourceFileName ,_TypeMetaSize[ _MT_char],}, 
	{ "Shader" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((_ShaderInput *)0)->Shader ,_TypeMetaSize[ _MT_unsigned_int],}, 
	{ "VertexArrayObject" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((_ShaderInput *)0)->VertexArrayObject ,_TypeMetaSize[ _MT_unsigned_int],}, 
	{ "VertexBufferLocationArray" , false,true,false,0,_MT_int,"int",(int)&((_ShaderInput *)0)->VertexBufferLocationArray ,_TypeMetaSize[ _MT_int],}, 
	{ "UniformLocation" , false,false,true,SU_COUNT,_MT_int,"int",(int)&((_ShaderInput *)0)->UniformLocation ,_TypeMetaSize[ _MT_int],}, 
	{ "VerticesCount" , false,false,false,0,_MT_int,"int",(int)&((_ShaderInput *)0)->VerticesCount ,_TypeMetaSize[ _MT_int],}, 
	{ "IndicesCount" , false,false,false,0,_MT_int,"int",(int)&((_ShaderInput *)0)->IndicesCount ,_TypeMetaSize[ _MT_int],}, 
};

global int MemberMetaCount__ShaderBuffer = 10;
global _MemberMetaData MemberMeta__ShaderBuffer[10] =
{
	{ "Name" , false,true,false,0,_MT_char,"char",(int)&((_ShaderBuffer *)0)->Name ,_TypeMetaSize[ _MT_char],}, 
	{ "PrimitiveDataType" , false,false,false,0,_MT_int,"int",(int)&((_ShaderBuffer *)0)->PrimitiveDataType ,_TypeMetaSize[ _MT_int],}, 
	{ "ComponentsCount" , false,false,false,0,_MT_int,"int",(int)&((_ShaderBuffer *)0)->ComponentsCount ,_TypeMetaSize[ _MT_int],}, 
	{ "MaxDataSize" , false,false,false,0,_MT_int,"int",(int)&((_ShaderBuffer *)0)->MaxDataSize ,_TypeMetaSize[ _MT_int],}, 
	{ "PrimitiveDataSize" , false,false,false,0,_MT_int,"int",(int)&((_ShaderBuffer *)0)->PrimitiveDataSize ,_TypeMetaSize[ _MT_int],}, 
	{ "IsUniformBuffer" , false,false,false,0,_MT_bool,"bool",(int)&((_ShaderBuffer *)0)->IsUniformBuffer ,_TypeMetaSize[ _MT_bool],}, 
	{ "IsIndicesBuffer" , false,false,false,0,_MT_bool,"bool",(int)&((_ShaderBuffer *)0)->IsIndicesBuffer ,_TypeMetaSize[ _MT_bool],}, 
	{ "Data" , false,true,false,0,_MT_unsigned_char,"unsigned_char",(int)&((_ShaderBuffer *)0)->Data ,_TypeMetaSize[ _MT_unsigned_char],}, 
	{ "BufferSize" , false,false,false,0,_MT_int,"int",(int)&((_ShaderBuffer *)0)->BufferSize ,_TypeMetaSize[ _MT_int],}, 
	{ "BufferHandle" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((_ShaderBuffer *)0)->BufferHandle ,_TypeMetaSize[ _MT_unsigned_int],}, 
};

global int MemberMetaCount_String = 2;
global _MemberMetaData MemberMeta_String[2] =
{
	{ "Start" , false,true,false,0,_MT_wchar_t,"wchar_t",(int)&((String *)0)->Start ,_TypeMetaSize[ _MT_wchar_t],}, 
	{ "Count" , false,false,false,0,_MT_int,"int",(int)&((String *)0)->Count ,_TypeMetaSize[ _MT_int],}, 
};

global int MemberMetaCount_TextureMetaNode = 2;
global _MemberMetaData MemberMeta_TextureMetaNode[2] =
{
	{ "Index" , false,false,false,0,_MT_int,"int",(int)&((TextureMetaNode *)0)->Index ,_TypeMetaSize[ _MT_int],}, 
	{ "Next" , false,true,false,0,_MT_TextureMetaNode,"TextureMetaNode",(int)&((TextureMetaNode *)0)->Next ,_TypeMetaSize[ _MT_TextureMetaNode],}, 
};

global int MemberMetaCount_D_Render_State = 19;
global _MemberMetaData MemberMeta_D_Render_State[19] =
{
	{ "CurrentTexture" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((D_Render_State *)0)->CurrentTexture ,_TypeMetaSize[ _MT_unsigned_int],}, 
	{ "PreviousShaderInput" , false,false,false,0,_MT_D_ShaderType,"D_ShaderType",(int)&((D_Render_State *)0)->PreviousShaderInput ,_TypeMetaSize[ _MT_D_ShaderType],}, 
	{ "CurrentShaderInput" , false,false,false,0,_MT_D_ShaderType,"D_ShaderType",(int)&((D_Render_State *)0)->CurrentShaderInput ,_TypeMetaSize[ _MT_D_ShaderType],}, 
	{ "TextureCount" , false,false,false,0,_MT_int,"int",(int)&((D_Render_State *)0)->TextureCount ,_TypeMetaSize[ _MT_int],}, 
	{ "AllTextures" , false,true,false,0,_MT_Texture,"Texture",(int)&((D_Render_State *)0)->AllTextures ,_TypeMetaSize[ _MT_Texture],}, 
	{ "AllTexturesName" , false,true,false,0,_MT_String,"String",(int)&((D_Render_State *)0)->AllTexturesName ,_TypeMetaSize[ _MT_String],}, 
	{ "ScreenFrameBuffer" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((D_Render_State *)0)->ScreenFrameBuffer ,_TypeMetaSize[ _MT_unsigned_int],}, 
	{ "GameWorldTexture" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((D_Render_State *)0)->GameWorldTexture ,_TypeMetaSize[ _MT_unsigned_int],}, 
	{ "GUITexture" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((D_Render_State *)0)->GUITexture ,_TypeMetaSize[ _MT_unsigned_int],}, 
	{ "GameWorldDepthTexture" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((D_Render_State *)0)->GameWorldDepthTexture ,_TypeMetaSize[ _MT_unsigned_int],}, 
	{ "LightingTexture" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((D_Render_State *)0)->LightingTexture ,_TypeMetaSize[ _MT_unsigned_int],}, 
	{ "BloomTexture" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((D_Render_State *)0)->BloomTexture ,_TypeMetaSize[ _MT_unsigned_int],}, 
	{ "FirstBlurTexture" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((D_Render_State *)0)->FirstBlurTexture ,_TypeMetaSize[ _MT_unsigned_int],}, 
	{ "BlurTexture" , false,false,false,0,_MT_unsigned_int,"unsigned_int",(int)&((D_Render_State *)0)->BlurTexture ,_TypeMetaSize[ _MT_unsigned_int],}, 
	{ "ShaderUniformName" , false,true,true,SU_COUNT,_MT_char,"char",(int)&((D_Render_State *)0)->ShaderUniformName ,_TypeMetaSize[ _MT_char],}, 
	{ "ShaderUniformType" , false,false,true,SU_COUNT,_MT_int,"int",(int)&((D_Render_State *)0)->ShaderUniformType ,_TypeMetaSize[ _MT_int],}, 
	{ "ShaderUniformData" , false,true,true,SU_COUNT,_MT_unsigned_char,"unsigned_char",(int)&((D_Render_State *)0)->ShaderUniformData ,_TypeMetaSize[ _MT_unsigned_char],}, 
	{ "ElapsedTime" , false,false,false,0,_MT_float,"float",(int)&((D_Render_State *)0)->ElapsedTime ,_TypeMetaSize[ _MT_float],}, 
	{ "FakeDepth" , false,false,false,0,_MT_float,"float",(int)&((D_Render_State *)0)->FakeDepth ,_TypeMetaSize[ _MT_float],}, 
};

global int MemberMetaCount_D_Light_GPU_Data = 6;
global _MemberMetaData MemberMeta_D_Light_GPU_Data[6] =
{
	{ "LightCount" , false,false,false,0,_MT_int,"int",(int)&((D_Light_GPU_Data *)0)->LightCount ,_TypeMetaSize[ _MT_int],}, 
	{ "LightPosition" , false,true,false,0,_MT_Vector3,"Vector3",(int)&((D_Light_GPU_Data *)0)->LightPosition ,_TypeMetaSize[ _MT_Vector3],}, 
	{ "LightColor" , false,true,false,0,_MT_Vector4,"Vector4",(int)&((D_Light_GPU_Data *)0)->LightColor ,_TypeMetaSize[ _MT_Vector4],}, 
	{ "LightSize" , false,true,false,0,_MT_Vector2,"Vector2",(int)&((D_Light_GPU_Data *)0)->LightSize ,_TypeMetaSize[ _MT_Vector2],}, 
	{ "LightMul" , false,true,false,0,_MT_float,"float",(int)&((D_Light_GPU_Data *)0)->LightMul ,_TypeMetaSize[ _MT_float],}, 
	{ "LightTexCoord" , false,true,true,Vertex_Count,_MT_Vector2,"Vector2",(int)&((D_Light_GPU_Data *)0)->LightTexCoord ,_TypeMetaSize[ _MT_Vector2],}, 
};

global int MemberMetaCount_D_Light = 4;
global _MemberMetaData MemberMeta_D_Light[4] =
{
	{ "LightPosition" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((D_Light *)0)->LightPosition ,_TypeMetaSize[ _MT_Vector3],}, 
	{ "LightSize" , false,false,false,0,_MT_Vector2,"Vector2",(int)&((D_Light *)0)->LightSize ,_TypeMetaSize[ _MT_Vector2],}, 
	{ "LightColor" , false,false,false,0,_MT_Vector4,"Vector4",(int)&((D_Light *)0)->LightColor ,_TypeMetaSize[ _MT_Vector4],}, 
	{ "LightStrength" , false,false,false,0,_MT_float,"float",(int)&((D_Light *)0)->LightStrength ,_TypeMetaSize[ _MT_float],}, 
};

global int MemberMetaCount_D_Vertex_Data = 13;
global _MemberMetaData MemberMeta_D_Vertex_Data[13] =
{
	{ "Indices" , false,true,false,0,_MT_unsigned_short,"unsigned_short",(int)&((D_Vertex_Data *)0)->Indices ,_TypeMetaSize[ _MT_unsigned_short],}, 
	{ "Position" , false,true,false,0,_MT_Vector3,"Vector3",(int)&((D_Vertex_Data *)0)->Position ,_TypeMetaSize[ _MT_Vector3],}, 
	{ "Rotation" , false,true,false,0,_MT_Vector4,"Vector4",(int)&((D_Vertex_Data *)0)->Rotation ,_TypeMetaSize[ _MT_Vector4],}, 
	{ "Color" , false,true,false,0,_MT_Vector4,"Vector4",(int)&((D_Vertex_Data *)0)->Color ,_TypeMetaSize[ _MT_Vector4],}, 
	{ "TexCoord" , false,true,false,0,_MT_Vector2,"Vector2",(int)&((D_Vertex_Data *)0)->TexCoord ,_TypeMetaSize[ _MT_Vector2],}, 
	{ "LineStart" , false,true,false,0,_MT_Vector3,"Vector3",(int)&((D_Vertex_Data *)0)->LineStart ,_TypeMetaSize[ _MT_Vector3],}, 
	{ "LineEnd" , false,true,false,0,_MT_Vector3,"Vector3",(int)&((D_Vertex_Data *)0)->LineEnd ,_TypeMetaSize[ _MT_Vector3],}, 
	{ "LineSize" , false,true,false,0,_MT_float,"float",(int)&((D_Vertex_Data *)0)->LineSize ,_TypeMetaSize[ _MT_float],}, 
	{ "FakeDepth" , false,true,false,0,_MT_float,"float",(int)&((D_Vertex_Data *)0)->FakeDepth ,_TypeMetaSize[ _MT_float],}, 
	{ "Size" , false,true,false,0,_MT_Vector2,"Vector2",(int)&((D_Vertex_Data *)0)->Size ,_TypeMetaSize[ _MT_Vector2],}, 
	{ "CornerRadius" , false,true,false,0,_MT_Vector4,"Vector4",(int)&((D_Vertex_Data *)0)->CornerRadius ,_TypeMetaSize[ _MT_Vector4],}, 
	{ "CornerColor" , false,true,true,Vertex_Count,_MT_Vector4,"Vector4",(int)&((D_Vertex_Data *)0)->CornerColor ,_TypeMetaSize[ _MT_Vector4],}, 
	{ "CornerTexCoord" , false,true,true,Vertex_Count,_MT_Vector2,"Vector2",(int)&((D_Vertex_Data *)0)->CornerTexCoord ,_TypeMetaSize[ _MT_Vector2],}, 
};

global int MemberMetaCount__BoneState = 3;
global _MemberMetaData MemberMeta__BoneState[3] =
{
	{ "LocalPosition" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((_BoneState *)0)->LocalPosition ,_TypeMetaSize[ _MT_Vector3],}, 
	{ "LocalRotation" , false,false,false,0,_MT_Quaternion,"Quaternion",(int)&((_BoneState *)0)->LocalRotation ,_TypeMetaSize[ _MT_Quaternion],}, 
	{ "Direction" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((_BoneState *)0)->Direction ,_TypeMetaSize[ _MT_Vector3],}, 
};

global int MemberMetaCount__KeyFrame = 6;
global _MemberMetaData MemberMeta__KeyFrame[6] =
{
	{ "BoneState" , false,false,false,0,_MT__BoneState,"_BoneState",(int)&((_KeyFrame *)0)->BoneState ,_TypeMetaSize[ _MT__BoneState],}, 
	{ "FrameIndex" , false,false,false,0,_MT_int,"int",(int)&((_KeyFrame *)0)->FrameIndex ,_TypeMetaSize[ _MT_int],}, 
	{ "PreviousBezierOffset" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((_KeyFrame *)0)->PreviousBezierOffset ,_TypeMetaSize[ _MT_Vector3],}, 
	{ "NextBezierOffset" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((_KeyFrame *)0)->NextBezierOffset ,_TypeMetaSize[ _MT_Vector3],}, 
	{ "NodeIndex" , false,false,false,0,_MT_int,"int",(int)&((_KeyFrame *)0)->NodeIndex ,_TypeMetaSize[ _MT_int],}, 
	{ "OrderListNodeIndex" , false,false,false,0,_MT_int,"int",(int)&((_KeyFrame *)0)->OrderListNodeIndex ,_TypeMetaSize[ _MT_int],}, 
};

global int MemberMetaCount__HashTableEntry = 2;
global _MemberMetaData MemberMeta__HashTableEntry[2] =
{
	{ "HeadIndex" , false,false,false,0,_MT_int,"int",(int)&((_HashTableEntry *)0)->HeadIndex ,_TypeMetaSize[ _MT_int],}, 
	{ "TailIndex" , false,false,false,0,_MT_int,"int",(int)&((_HashTableEntry *)0)->TailIndex ,_TypeMetaSize[ _MT_int],}, 
};

global int MemberMetaCount__HashTableSlot = 5;
global _MemberMetaData MemberMeta__HashTableSlot[5] =
{
	{ "Valid" , false,false,false,0,_MT_bool,"bool",(int)&((_HashTableSlot *)0)->Valid ,_TypeMetaSize[ _MT_bool],}, 
	{ "SlotValue" , false,false,false,0,_MT_int,"int",(int)&((_HashTableSlot *)0)->SlotValue ,_TypeMetaSize[ _MT_int],}, 
	{ "DataIndex" , false,false,false,0,_MT_int,"int",(int)&((_HashTableSlot *)0)->DataIndex ,_TypeMetaSize[ _MT_int],}, 
	{ "NextIndex" , false,false,false,0,_MT_int,"int",(int)&((_HashTableSlot *)0)->NextIndex ,_TypeMetaSize[ _MT_int],}, 
	{ "PreviousIndex" , false,false,false,0,_MT_int,"int",(int)&((_HashTableSlot *)0)->PreviousIndex ,_TypeMetaSize[ _MT_int],}, 
};

global int MemberMetaCount__ListNode = 3;
global _MemberMetaData MemberMeta__ListNode[3] =
{
	{ "DataIndex" , false,false,false,0,_MT_int,"int",(int)&((_ListNode *)0)->DataIndex ,_TypeMetaSize[ _MT_int],}, 
	{ "Next" , false,false,false,0,_MT_int,"int",(int)&((_ListNode *)0)->Next ,_TypeMetaSize[ _MT_int],}, 
	{ "Previous" , false,false,false,0,_MT_int,"int",(int)&((_ListNode *)0)->Previous ,_TypeMetaSize[ _MT_int],}, 
};

global int MemberMetaCount__List = 2;
global _MemberMetaData MemberMeta__List[2] =
{
	{ "NodeArray" , false,true,false,0,_MT__ListNode,"_ListNode",(int)&((_List *)0)->NodeArray ,_TypeMetaSize[ _MT__ListNode],}, 
	{ "UnusedIndex" , false,false,false,0,_MT_int,"int",(int)&((_List *)0)->UnusedIndex ,_TypeMetaSize[ _MT_int],}, 
};

global int MemberMetaCount__HashTable = 2;
global _MemberMetaData MemberMeta__HashTable[2] =
{
	{ "EntryArray" , false,true,false,0,_MT__HashTableEntry,"_HashTableEntry",(int)&((_HashTable *)0)->EntryArray ,_TypeMetaSize[ _MT__HashTableEntry],}, 
	{ "SlotArray" , false,true,false,0,_MT__HashTableSlot,"_HashTableSlot",(int)&((_HashTable *)0)->SlotArray ,_TypeMetaSize[ _MT__HashTableSlot],}, 
};

global int MemberMetaCount_RotationAxisData = 5;
global _MemberMetaData MemberMeta_RotationAxisData[5] =
{
	{ "Rect" , false,false,false,0,_MT__Rect,"_Rect",(int)&((RotationAxisData *)0)->Rect ,_TypeMetaSize[ _MT__Rect],}, 
	{ "TargetAxisColor" , false,false,false,0,_MT_Color,"Color",(int)&((RotationAxisData *)0)->TargetAxisColor ,_TypeMetaSize[ _MT_Color],}, 
	{ "TargetAxisLineSize" , false,false,false,0,_MT_float,"float",(int)&((RotationAxisData *)0)->TargetAxisLineSize ,_TypeMetaSize[ _MT_float],}, 
	{ "AxisColor" , false,false,false,0,_MT_Color,"Color",(int)&((RotationAxisData *)0)->AxisColor ,_TypeMetaSize[ _MT_Color],}, 
	{ "AxisLineSize" , false,false,false,0,_MT_float,"float",(int)&((RotationAxisData *)0)->AxisLineSize ,_TypeMetaSize[ _MT_float],}, 
};

global int MemberMetaCount__Bone = 16;
global _MemberMetaData MemberMeta__Bone[16] =
{
	{ "BoneIndex" , false,false,false,0,_MT_int,"int",(int)&((_Bone *)0)->BoneIndex ,_TypeMetaSize[ _MT_int],}, 
	{ "BoneName" , false,false,true,64,_MT_char,"char",(int)&((_Bone *)0)->BoneName ,_TypeMetaSize[ _MT_char],}, 
	{ "State" , false,false,false,0,_MT__BoneState,"_BoneState",(int)&((_Bone *)0)->State ,_TypeMetaSize[ _MT__BoneState],}, 
	{ "Position" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((_Bone *)0)->Position ,_TypeMetaSize[ _MT_Vector3],}, 
	{ "EndPosition" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((_Bone *)0)->EndPosition ,_TypeMetaSize[ _MT_Vector3],}, 
	{ "Rotation" , false,false,false,0,_MT_Quaternion,"Quaternion",(int)&((_Bone *)0)->Rotation ,_TypeMetaSize[ _MT_Quaternion],}, 
	{ "FreePosition" , false,false,false,0,_MT_bool,"bool",(int)&((_Bone *)0)->FreePosition ,_TypeMetaSize[ _MT_bool],}, 
	{ "FreeBone" , false,false,false,0,_MT_bool,"bool",(int)&((_Bone *)0)->FreeBone ,_TypeMetaSize[ _MT_bool],}, 
	{ "Hovered" , false,false,false,0,_MT_bool,"bool",(int)&((_Bone *)0)->Hovered ,_TypeMetaSize[ _MT_bool],}, 
	{ "Selected" , false,false,false,0,_MT_bool,"bool",(int)&((_Bone *)0)->Selected ,_TypeMetaSize[ _MT_bool],}, 
	{ "KeyFrameArray" , false,true,false,0,_MT__KeyFrame,"_KeyFrame",(int)&((_Bone *)0)->KeyFrameArray ,_TypeMetaSize[ _MT__KeyFrame],}, 
	{ "KeyFrameList" , false,false,false,0,_MT__List,"_List",(int)&((_Bone *)0)->KeyFrameList ,_TypeMetaSize[ _MT__List],}, 
	{ "FirstKeyFrameOnPointerLeft_OrderListIndex" , false,false,false,0,_MT_int,"int",(int)&((_Bone *)0)->FirstKeyFrameOnPointerLeft_OrderListIndex ,_TypeMetaSize[ _MT_int],}, 
	{ "KeyFrameOrderList" , false,false,false,0,_MT__List,"_List",(int)&((_Bone *)0)->KeyFrameOrderList ,_TypeMetaSize[ _MT__List],}, 
	{ "KeyFrameHashTable" , false,false,false,0,_MT__HashTable,"_HashTable",(int)&((_Bone *)0)->KeyFrameHashTable ,_TypeMetaSize[ _MT__HashTable],}, 
	{ "DraggingKeyFrameList" , false,false,false,0,_MT__List,"_List",(int)&((_Bone *)0)->DraggingKeyFrameList ,_TypeMetaSize[ _MT__List],}, 
};

global int MemberMetaCount_DrawingMenu = 7;
global _MemberMetaData MemberMeta_DrawingMenu[7] =
{
	{ "CurrentButtonPosition" , false,false,false,0,_MT_Vector2,"Vector2",(int)&((DrawingMenu *)0)->CurrentButtonPosition ,_TypeMetaSize[ _MT_Vector2],}, 
	{ "OnScreen" , false,false,false,0,_MT_bool,"bool",(int)&((DrawingMenu *)0)->OnScreen ,_TypeMetaSize[ _MT_bool],}, 
	{ "CurrentButtonLeft" , false,false,false,0,_MT_float,"float",(int)&((DrawingMenu *)0)->CurrentButtonLeft ,_TypeMetaSize[ _MT_float],}, 
	{ "ButtonHover" , false,false,false,0,_MT_bool,"bool",(int)&((DrawingMenu *)0)->ButtonHover ,_TypeMetaSize[ _MT_bool],}, 
	{ "ButtonClicked" , false,false,false,0,_MT_bool,"bool",(int)&((DrawingMenu *)0)->ButtonClicked ,_TypeMetaSize[ _MT_bool],}, 
	{ "MenuType" , false,false,false,0,_MT_int,"int",(int)&((DrawingMenu *)0)->MenuType ,_TypeMetaSize[ _MT_int],}, 
	{ "ButtonExtraOffset" , false,false,false,0,_MT_float,"float",(int)&((DrawingMenu *)0)->ButtonExtraOffset ,_TypeMetaSize[ _MT_float],}, 
};

global int MemberMetaCount_Editor_Data = 26;
global _MemberMetaData MemberMeta_Editor_Data[26] =
{
	{ "ControlRig" , false,false,false,0,_MT_bool,"bool",(int)&((Editor_Data *)0)->ControlRig ,_TypeMetaSize[ _MT_bool],}, 
	{ "PointerFrameIndex" , false,false,false,0,_MT_int,"int",(int)&((Editor_Data *)0)->PointerFrameIndex ,_TypeMetaSize[ _MT_int],}, 
	{ "StickMan" , false,true,true,BoneCount,_MT__Bone,"_Bone",(int)&((Editor_Data *)0)->StickMan ,_TypeMetaSize[ _MT__Bone],}, 
	{ "RootBoneList" , false,false,false,0,_MT__List,"_List",(int)&((Editor_Data *)0)->RootBoneList ,_TypeMetaSize[ _MT__List],}, 
	{ "BoneParentHashTable" , false,false,false,0,_MT__HashTable,"_HashTable",(int)&((Editor_Data *)0)->BoneParentHashTable ,_TypeMetaSize[ _MT__HashTable],}, 
	{ "BoneChildrenHashTable" , false,false,false,0,_MT__HashTable,"_HashTable",(int)&((Editor_Data *)0)->BoneChildrenHashTable ,_TypeMetaSize[ _MT__HashTable],}, 
	{ "SelectingBezier" , false,false,false,0,_MT_bool,"bool",(int)&((Editor_Data *)0)->SelectingBezier ,_TypeMetaSize[ _MT_bool],}, 
	{ "SelectedBoneCount" , false,false,false,0,_MT_int,"int",(int)&((Editor_Data *)0)->SelectedBoneCount ,_TypeMetaSize[ _MT_int],}, 
	{ "SelectedBoneStack" , false,false,true,BoneCount,_MT_int,"int",(int)&((Editor_Data *)0)->SelectedBoneStack ,_TypeMetaSize[ _MT_int],}, 
	{ "Playing" , false,false,false,0,_MT_bool,"bool",(int)&((Editor_Data *)0)->Playing ,_TypeMetaSize[ _MT_bool],}, 
	{ "PlayTimer" , false,false,false,0,_MT_float,"float",(int)&((Editor_Data *)0)->PlayTimer ,_TypeMetaSize[ _MT_float],}, 
	{ "TimelineSliderOffset" , false,false,false,0,_MT_float,"float",(int)&((Editor_Data *)0)->TimelineSliderOffset ,_TypeMetaSize[ _MT_float],}, 
	{ "StartFrameIndex" , false,false,false,0,_MT_int,"int",(int)&((Editor_Data *)0)->StartFrameIndex ,_TypeMetaSize[ _MT_int],}, 
	{ "EndFrameIndex" , false,false,false,0,_MT_int,"int",(int)&((Editor_Data *)0)->EndFrameIndex ,_TypeMetaSize[ _MT_int],}, 
	{ "TimelineScale" , false,false,false,0,_MT_float,"float",(int)&((Editor_Data *)0)->TimelineScale ,_TypeMetaSize[ _MT_float],}, 
	{ "TargetCameraZoom" , false,false,false,0,_MT_float,"float",(int)&((Editor_Data *)0)->TargetCameraZoom ,_TypeMetaSize[ _MT_float],}, 
	{ "CurrentCameraZoom" , false,false,false,0,_MT_float,"float",(int)&((Editor_Data *)0)->CurrentCameraZoom ,_TypeMetaSize[ _MT_float],}, 
	{ "Right" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((Editor_Data *)0)->Right ,_TypeMetaSize[ _MT_Vector3],}, 
	{ "Up" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((Editor_Data *)0)->Up ,_TypeMetaSize[ _MT_Vector3],}, 
	{ "BillboardRotation" , false,false,false,0,_MT_Quaternion,"Quaternion",(int)&((Editor_Data *)0)->BillboardRotation ,_TypeMetaSize[ _MT_Quaternion],}, 
	{ "CameraEuler" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((Editor_Data *)0)->CameraEuler ,_TypeMetaSize[ _MT_Vector3],}, 
	{ "CameraOffset" , false,false,false,0,_MT_Vector3,"Vector3",(int)&((Editor_Data *)0)->CameraOffset ,_TypeMetaSize[ _MT_Vector3],}, 
	{ "CurrentMousePosition" , false,false,false,0,_MT_Vector2,"Vector2",(int)&((Editor_Data *)0)->CurrentMousePosition ,_TypeMetaSize[ _MT_Vector2],}, 
	{ "PreviousMousePosition" , false,false,false,0,_MT_Vector2,"Vector2",(int)&((Editor_Data *)0)->PreviousMousePosition ,_TypeMetaSize[ _MT_Vector2],}, 
	{ "OperateMenuPosition" , false,false,false,0,_MT_Vector2,"Vector2",(int)&((Editor_Data *)0)->OperateMenuPosition ,_TypeMetaSize[ _MT_Vector2],}, 
	{ "FlatColor" , false,false,false,0,_MT_bool,"bool",(int)&((Editor_Data *)0)->FlatColor ,_TypeMetaSize[ _MT_bool],}, 
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
	GetStructMeta(D_ShaderCode),
	{},
	GetStructMeta(_ShaderInput),
	GetStructMeta(_ShaderBuffer),
	GetStructMeta(String),
	GetStructMeta(TextureMetaNode),
	GetStructMeta(D_Render_State),
	GetStructMeta(D_Light_GPU_Data),
	GetStructMeta(D_Light),
	GetStructMeta(D_Vertex_Data),
	{},
	GetStructMeta(_BoneState),
	GetStructMeta(_KeyFrame),
	GetStructMeta(_HashTableEntry),
	GetStructMeta(_HashTableSlot),
	GetStructMeta(_ListNode),
	{},
	GetStructMeta(_List),
	GetStructMeta(_HashTable),
	{},
	GetStructMeta(RotationAxisData),
	GetStructMeta(_Bone),
	{},
	GetStructMeta(DrawingMenu),
	GetStructMeta(Editor_Data),
	{},
	{},
	{},
	{},
	{},
};

internal void ResetWindowTheme();
internal long long CounterStamp();
internal double TimeStamp();
internal long long EndCounterStamp(long long BeginTime);
internal double EndTimeStamp(long long BeginTime);
internal long long CycleStamp();
internal Vector4 ColorToLinear(Color _Color);
internal Color LinearToColor(Vector4 _Color);
internal Vector4 LinearFade(Vector4 LinearColor ,float Mul);
internal Color ColorMultiply(Color Base , float Value);
internal Color ColorInvert(Color Base);
internal Color ColorLerp(Color A , Color B , float T);
internal Color ColorScreen(Color BaseColor ,Color LayerColor , float Blend);
internal Color ColorMultiply(Color Base , Color Layer , float Blend);
internal Vector4 Vector4Lerp(Vector4 v1, Vector4 v2, float amount);
internal void ColorQuad(Vector4* QuadColor , Vector4 TargetColor);
internal Ray GetScreenToWorldRayEx(Vector2 position, Camera camera, int width, int height);
internal RayCollision GetRayCollisionTriangle(Ray ray, Vector3 p1, Vector3 p2, Vector3 p3);
internal RayCollision GetCollisionQuad3D(_Quad Quad);
internal bool CheckCollisionQuad3D(_Quad Quad);
internal RayCollision GetCollisionRect3D(Ray _Ray, _Rect Rect);
internal RayCollision GetCollisionRect3D(_Rect Rect);
internal bool CheckCollisionRect3D(_Rect Rect);
internal Vector4 RandomLinearColor();
internal float PixelToHeight(float Y);
internal float PixelToWidth(float X);
internal Vector2 PixelToWorld(float X, float Y);
internal Vector2 PixelToWorld(Vector2 Position);
internal Vector2 PixelToWorldNoTransform(Vector2 Position);
internal Vector3 PixelToWorldNoTransform(Vector3 Position);
internal Vector2 PixelToSize(float X, float Y);
internal _Rect GetRect();
internal _Rect GetRect(int PixelSizeX, int PixelSizeY);
internal float Noise(float seed);
internal unsigned int StringToHash(const char * s);
internal unsigned char* AllocateTemp_(int Size);
internal unsigned char * AllocateFrame_(int Size);
internal void CombineFilePath(const char* FileName,char * ResultPath);
internal char * CombineString(char * StrA , char * StrB);
internal char * GetAppFilePath(const char* FileName);
internal char * GetLevelFilePath(const char * LevelName);
internal char * GetLevelFilePath();
internal void ParseToBasicType(_TypeMeta _Type,int Space,_MemberMetaData StructMemberMeta,int Count);
internal void * AllocatePersist_
(	
 int Size,
 int Count,
 
 const char * DataName ,
 
 _TypeMeta _Type,
 bool IsPointer , 
 _PersistMemory * TargetMemory
 );
internal void ResetPersistMemory(_PersistMemory * TargetMemory);
internal Vector4 QuaternionToVector4(Quaternion _Quaternion);
internal float CubicBezier(float x1 , float x2 , float x3 ,float x4, float t);
internal Vector3 Vector3CubicBezier(Vector3 P1 , Vector3 P2 , Vector3 P3 , Vector3 P4  , float t);
internal float EaseInOutSine(float x);
internal float EaseInOutCubic(float x);
internal float EaseOutQuint(float x );
internal float EaseInBack(float x , float c1);
internal float EaseInBack(float x );
internal float EaseOutBack(float x  , float c1);
internal float EaseOutBack(float x);
internal int GetIndexFromArray(void *Array , void* Element);
internal void ClearArray(void* Array);
internal void RecheckArray(void * Array);
internal bool CheckItem(void * Array , void * Item);
internal void *AddToArray(void* Array);
internal bool DeleteFromArray(void* Array,void * Element);
internal void ReassignPointer(_StructMetaData StructMetaData, unsigned char* Memory, unsigned char* Save_StartMemoryAddress);
internal _ShaderBuffer* Get_GPU_BufferInfo(void * Data);
internal void * CreateBufferForGPU(const char * BufferName , int PrimitiveType,int ComponentCount , int MaxSize);
internal void * Create_GPU_VertexBuffer(const char * BufferName , int PrimitiveType,int ComponentCount , int BufferSize );
internal void * Create_GPU_IndicsBuffer(const char * BufferName , int PrimitiveType,int ComponentCount , int BufferSize );
internal void RenderStateInit();
internal void ShaderInit();
internal void Update_GPU_VertexBufferAll(_ShaderInput * CurrentShader,int Count);
internal void D_GameDraw();
internal _Quad RectToQuad(_Rect Rect);
internal _Quad RectToRhombus(_Rect Rect);
internal void QuadDrawCallCheck(int TargetTexture);
internal void DrawTriangle(Vector3* AllVertices ,Vector4 VertexColor);
internal void DrawTriangleUp(_Quad Quad , Color TriangleColor );
internal void DrawTriangleRight(_Quad Quad, Color TriangleColor);
internal void DrawTriangleLeft(_Quad Quad, Color TriangleColor);
internal void DrawTriangleBottom(_Quad Quad, Color TriangleColor);
internal void DrawQuadWithTexture(_Quad Quad,int TargetTexture);
internal void ChangeMatrix(Matrix MVP_Matrix);
internal void _D_DrawRect(_Rect Rect, Vector4 CornerRadius, int TargetTexture , Vector4 * VertexColor , Vector2 * TextureCoord);
internal void Depercreted_D_DrawRect(_Rect Rect, Vector4 CornerRadius,int TargetTexture);
internal void DrawRect(_Rect Rect , Vector4 RectColor, int TextureIndex);
internal void DrawRect(_Rect Rect , Vector4 RectColor);
internal void DrawRectWithTexture(_Rect Rect , Vector4 Corners, Color RectColor, int TextureIndex);
internal void DrawRectWithTexture(_Rect Rect , Color RectColor, int Texture);
internal void DrawQuadAsRect(_Quad Quad,int TargetTexture);
internal void DrawQuadAsRect(_Quad *Quad, int TargetTexture);
internal void DrawQuadAsRect(_Quad Quad,int TargetTexture,Color QuadColor);
internal void DrawQuadAsRect(_Quad *Quad , int TargetTexture , Color QuadColor);
internal void DrawQuad(_Quad* Quad, int TargetTexture);
internal void DrawQuad(_Quad* Quad);
internal void DrawQuad(Vector3 Position , float Size , Quaternion Rotation, Vector4 QuadColor );
internal void DrawQuad(_Quad Quad, Color QuadColor);
internal void DrawQuad(_Quad * Quad, Color QuadColor);
internal void DrawRect(_Rect Rect, float CornerRoundness);
internal void DrawRect(_Rect Rect, float CornerRoundness, Color TargetColor);
internal void DrawCircle(_Rect Rect, Color TargetColor);
internal _Rect GetBillboardRect( Vector3 Position , float Size );
internal void DrawBillboardCircle(Vector3 Position , float Size, Color TargetColor);
internal void DrawCircle(Vector3 Position,float Size, Color TargetColor);
internal void DrawCircle(Vector2 Position,float Size, Color TargetColor);
internal void DrawCircle(Vector2 Position,float Size, Vector4 TargetColor);
internal void DrawCircle(Vector3 Position,float Size, Vector4 TargetColor);
internal void DrawLineQuad(_Quad Quad , Vector3 LineStart , Vector3 LineEnd , float LineSize);
internal void DrawLineQuad(_Quad Quad , Color QuadColor , Vector3 LineStart , Vector3 LineEnd , float LineSize);
internal void AddPointToLine(Vector3 Point, Vector4 PointColor , float PointSize);
internal void AddPointToLine(Vector3 Point, Color PointColor , float PointSize);
internal void AddPointToLine(Vector3 Point, Color PointColor);
internal void AddPointToLine2D(Vector2 Point, Vector4 PointColor,float PointSize);
internal void AddPointToLine2D(Vector2 Point, Vector4 PointColor);
internal void AddPointToLine2D(Vector2 Point, Color PointColor , float PointSize);
internal void AddPointToLine2D(Vector2 Point, Color PointColor);
internal unsigned int LoadDepthTexture(int width, int height);
internal Vector3 isect_line_plane_v3(Vector3 p0, Vector3 p1, Vector3 p_co,Vector3  p_no);
internal float GetLineIntersectWithPlaneTime(Vector3 Start , Vector3 End , Vector3 PlaneNormal , Vector3 PlaneOrigin);
internal Vector3 TransformvectorDeviceCoord(Vector3 position , Matrix MVP);
internal Vector3 TransformVector(Vector3 Position , Matrix TransformMatrix);
internal float RemovePrespective(Vector3 q);
internal _Quad GetBillboardQuad(Vector3 Point, float Width , float Height);
internal void DrawRoundLine(Vector3 Start , Vector3 End , float Size , Color LineColor);
internal void DrawRoundLine(Vector3 Start , Vector3 End , float Size , Color StartColor , Color EndColor);
internal void DrawRectLine(Vector2 StartPosition , Vector2 EndPosition , Vector4 LineColor);
internal void DrawQuadLine(_Quad Quad , Color LineColor , float LineSize);
internal void DrawRectLine(Vector2 StartPosition , Vector2 EndPosition,Color LineColor);
internal void DrawRect(Vector3 Position,float Size, Color TargetColor , Quaternion Rotation);
internal void DrawRect(Vector3 Position, float Size, Vector4 TargetColor , Quaternion Rotation);
internal void DrawRectLine(_Rect Rect ,Vector4 LineColor , float LineSize);
internal void DrawRectLine(_Rect Rect, Vector4 LineColor);
internal void DrawRectLine(_Rect Rect, Color LineColor , float LineSize );
internal void DrawRectLine(_Rect Rect , Color LineColor);
internal void DrawQuadLine(_Quad Quad, Color LineColor);
internal void DrawQuadLine(_Quad * Quad, Color LineColor);
internal void DrawRect(_Rect Rect,Color TargetColor);
internal void DrawRect(_Rect Rect);
internal void _DrawRectText(_Rect Quad ,Vector4* Color ,Vector2* TextureCoord , Vector3 Up ,Vector3 Right);
internal void DrawBackground();
internal void DrawScreenFlat(int TextureIndex , Vector4 _Color);
internal void DrawScreenFlat(int TextureIndex ,Color _Color);
internal void DrawSpring(_Quad Quad);
internal D_GlyphInfo D_GetGlyphFromCodepoint(wchar_t CurrentCodepoint,_D_Font_Info * FontInfo);
internal float GetCodepointWidth(wchar_t CodepointPointer, _D_Font_Info* FontInfo, float Scale, float SpaceSizeOffset, float SpaceScaler);
internal float GetGlyphWidth(D_GlyphInfo GlyphInfo , _D_Font_Info *FontInfo , float SpaceSizeOffset , float SpaceScaler , float Scale);
internal void GetStringWidth(int Count,wchar_t * CodepointPointer,_D_Font_Info *FontInfo, float SpaceSizeOffset,float SpaceScaler,float Scale,int * ResultCount,float * ResultWidth);
internal float GetTotalStringWidth(const wchar_t* Text,float Scale);
internal float GetTotalStringWidth(char * Text,float Scale);
internal void DrawTextWithFont(_D_Font_Info* FontInfo, _Rect Rect , Color TextColor, wchar_t* String, float Scale, float SpaceSizeOffset, float SpaceScaler, bool Centre , Vector3 Up , Vector3 Right);
internal void D_DrawTextScreen(Vector3 Position ,char * Text , float Size, Color TextColor , bool Centre);
internal void D_DrawText(_Rect Rect , const wchar_t* String, Color TextColor, float Scale, float SpaceSizeOffset, float SpaceScaler, bool Centre);
internal void D_DrawText(_Rect Rect, const wchar_t* String, Color TextColor, bool Centre);
internal void D_DrawText(Vector2 Position,const wchar_t* String, Color TextColor, float Size, bool Centre);
internal void D_DrawText(Vector3 Position, const wchar_t* String, Color TextColor, float Size, bool Centre);
internal void D_DrawText(Vector3 Position , float Size , char * String , Color TextColor , bool Centre);
internal void D_DrawText(Vector2 Position , float Size , char * String , Color TextColor , bool Centre);
internal void D_DrawText(_Rect Quad, const char * String,Color TextColor, float Scale, float SpaceSizeOffset, float SpaceScaler, bool Centre);
internal void SimpleFloatInput(wchar_t * TargetString , float *TargetFloat);
internal void GetFloatInput(float *TargetFloat);
internal void SimpleIntInput(wchar_t * TargetString , int * TargetInt);
internal void UpdateFloatInput();
internal _Rect QuadPositionLeft(_Rect Quad, float PositionOffsetX, float SizeX, float SizeOffsetY);
internal _Rect QuadPositionLeft(float PositionOffsetX, float SizeX, float SizeOffsetY);
internal _Rect OffsetFromBottomLeft(float PixelOffsetX, float PixelOffsetY, float PixelSizeX,float PixelSizeY);
internal _Rect OffsetFromQuadTopLeft(_Rect ParentQuad,_Rect ChildQuad, float PixelOffsetX,float PixelOffsetY);
internal _Rect OffsetFromTopLeft(_Rect Quad , float PixelOffsetX,float PixelOffsetY);
internal _Rect QuadToTopLeft(_Rect Quad);
internal _Rect QuadToBottomLeft(_Rect Quad);
internal _Rect QuadMoveDownHalf(_Rect Quad);
internal Vector2 GetRectTopRightCorner(_Rect Quad);
internal Vector2 GetRectBottomLeftCorner(_Rect Quad);
internal _Quad ResizeBlock(_Quad Block,float Size);
internal _Quad ResizeBlock(_Quad * Block, float Size);
internal bool CheckCollisionRect(_Rect Rect , Vector2 Point);
internal bool CheckCollisionRectMouse(_Rect Rect);
internal bool CheckCollisionRectMouse(Vector2 TopRight , Vector2 BottomLeft);
internal Vector3 GetEdgeDirection(Vector3 Start,Vector3 End , Vector3 Point);
internal bool CheckCollisionQuadPoint(_Quad Quad, Vector3 Point);
internal bool CheckCollisionQuadToRect(_Quad Quad , _Rect Rect);
internal bool CheckCollisionQuadMouse(_Quad Block);
internal Vector4 SelectionFromStartToEndToRect(Vector2 StartPosition, Vector2 End);
internal Vector2 GetMenuIndividualItemPositionEnd(Vector2 Position, String* OptionString, bool OnScreen,int Index,int OffsetY);
internal Vector2 GetMenuIndividualItemPositionEnd(Vector2 Position, String* OptionString, bool OnScreen, int Index);
internal DrawingMenu StartDrawMenu(Vector2 Position , bool OnScreen , GameMenuType MenuType);
internal DrawingMenu StartDrawDraggingMenu(Vector2 * PixelPosition , GameMenuType MenuType);
internal DrawingMenu StartDrawMenuMouse();
internal bool DrawMenuButton(DrawingMenu * Menu, wchar_t * ButtonString , Color ButtonTextColor , bool ChangeButtonTextColor);
internal bool DrawMenuButton(DrawingMenu * Menu, char * ButtonString , Color ButtonTextColor , bool ChangeButtonTextColor);
internal bool DrawMenuButton(DrawingMenu * Menu, wchar_t * ButtonString );
internal bool DrawMenuButton(DrawingMenu * Menu, char * ButtonString);
internal bool RayLineSegmentIntersection_Example(Vector3 o, Vector3 d, Vector3 a, Vector3 b);
internal Vector2 RayLineSegmentIntersection(Vector3 O, Vector3 D, Vector3 A, Vector3 B);
internal Vector2 RayLineSegmentIntersection_(Vector3 O, Vector3 D, Vector3 A, Vector3 B);
internal bool CheckCollisionRect(_Rect RectA, _Rect RectB);
internal unsigned int StringToHash(wchar_t *s);
internal int HashInt(int key);
internal void _PrintHashTable(_HashTableSlot persist * HashSlotArray , int Count);
internal void AddToHashTable(int HashValue , int DataIndex , _HashTable * HashTable , int Count);
internal void _AddToHashTable(int HashValue , int DataIndex, _HashTable * HashTable , int Count);
internal void ClearHashTable(_HashTable * HashTable , int Count);
internal bool DeleteFromHashTable(int HashValue , int DataIndex , _HashTable * HashTable , int Count);
internal bool _DeleteFromHashTable(int HashValue , int DataIndex , _HashTable * HashTable , int Count);
internal int _GetIndexFromHashTable(int HashValue , _HashTableSlot persist * HashTableSlotArray , int Count);
internal void _IterateHashTable( int HashValue , int * SlotIndex , int * DataIndex, _HashTable * HashTable , int Count);
internal void ClearList(_List * List);
internal int CreateNewNode(_List * List , int Count);
internal int GetNextNodeIndex(int NodeIndex , _List * List);
internal int GetHeadNodeIndex(_List * List);
internal int GetTailNodeIndex(_List * List);
internal int GetPreviousNodeIndex(int NodeIndex , _List * List);
internal int GetDataIndexFromNodeIndex(int NodeIndex , _List * List);
internal int GetPreviousDataIndexFromNodeIndex(int NodeIndex , _List * List);
internal int GetNextDataIndexFromNodeIndex(int NodeIndex , _List * List);
internal void InsertToList
(
 bool ConnectBeforeNode , 
 int NodeToConnectIndex , 
 int NodeIndex , 
 int DataIndex , 
 _List * List
 );
internal int CreateAndInsertToList(bool ConnectBeforeNode , int NodeToConnectIndex , int DataIndex , _List * List , int Count);
internal int AddToListHead(int DataIndex , _List * List , int Count);
internal int AddToListTail( int DataIndex ,_List * List , int Count);
internal int GetEmtyNodeFromList(_List * List , int Count);
internal bool DeleteFromList(int NodeIndexToDelete , _List * List);
internal void DrawArrowRay(Vector3 StartPosition , Vector3 Direction  ,Color LineColor );
internal void DrawArrowRay(Vector3 StartPosition , Vector3 Direction  ,Color StartColor , Color EndColor);
internal void DrawArrowLine(Vector3 StartPosition , Vector3 EndPosition , Color LineColor);
internal void DrawArrowLine(Vector3 StartPosition , Vector3 EndPosition , Color StartColor , Color EndColor);
internal int GetKeyFrameCount(_KeyFrame * StartKeyFrame , _KeyFrame * EndKeyFrame);
internal bool IsRig(int BoneIndex);
internal void IterateChildBone(_Bone * Bone);
internal void MemberDataToString(_MemberMetaData* MemberMeta, unsigned char* DataHead, wchar_t* StringBuffer);
internal void _PrintStructMemberInfo(_StructMetaData StructMetaData, unsigned char* StructData, _Rect TextQuad);
internal void CreateStructNameStringHash();
internal int GetDataSize(char * TypeName);
internal void RecordPointer(void** Pointer , int CurrentType, int PreviousType , _PersistMemory * PreviousMemory);
internal _TypeFormDifference CompareType(int CurrentType, int PreviousType);
internal _MemberDifference CompareMember(_MemberMetaData * CurrentMember , _MemberMetaData * PreviousMember);
internal void AddDecomposedType(int Type , int Offset , char * Name , bool IsPointer,int DataCount , _MemberNode * ReverseMemberList , bool Current);
internal void DecomposeType(int Type, _MemberNode * PreviousNode ,bool Current);
internal void CheckAndPatchData(_PersistMemory * TargetMemory , FILE* DataTypeAndDetailInfoFile);
internal void LoadData(char * Name , _PersistMemory * TargetMemory);
internal void SaveData(char * Name , _PersistMemory * TargetMemory);
internal _List _AllocatePersistList( char * DataName , int NodeCount , _PersistMemory * TargetMemory);
internal _HashTable _AllocatePersistHashTable( char * DataName , int SlotCount , _PersistMemory * TargetMemory);
internal void CreateAWholeNewWorld();
internal bool CheckSelectedBoneRotation(int SingleBoneIndex);
internal MOUSE_BUTTON_CHECK(SelectionCheck);
internal void BoneSelection();
internal void Editor_GUI();
internal void IterateBoneStructure(_Bone * RootBone);
internal void UpdateBoneStructure();
internal void BoneIKUpdate(_Bone * TargetBone , _Bone * PoleBone , _Bone * IKBone , int IterationCount , int BoneChainMaxLength);
internal void DrawRoundLine(int StartBoneIndex , int EndBoneIndex);
internal void DrawAllBone();
internal void BoneIKUpdate(int TargetBoneIndex , int PoleBoneIndex , int IKBoneIndex , int IterationCount , int BoneChainMaxLength);
internal void GameUpdate();
internal GAME_LOOP(GameLoop);
internal GAME_UNLOAD(GameUnload);
internal void LoadTextureFolder();
internal void AttachChildBone(int BoneIndex , int ChildBoneIndex);
internal void MakeAStickMan();
internal void GameInit();
internal MOUSE_BUTTON_CHECK(SelectionCheck)
#line 163 "../Code/d_game.cpp"
    {
#line 164 "../Code/d_game.cpp"
        if(MenuInteruped) return false;
#line 165 "../Code/d_game.cpp"
        if(Editor->SelectingBezier) return false;
#line 166 "../Code/d_game.cpp"
        
#line 167 "../Code/d_game.cpp"
        return true;
#line 168 "../Code/d_game.cpp"
    }
internal void DrawRoundLine(int StartBoneIndex , int EndBoneIndex)
#line 1562 "../Code/d_game.cpp"
	{
#line 1563 "../Code/d_game.cpp"
		DrawRoundLine(Editor->StickMan[StartBoneIndex]->Position, Editor->StickMan[EndBoneIndex]->Position , 80 , WHITE , WHITE);
#line 1564 "../Code/d_game.cpp"
	}
internal void BoneIKUpdate(int TargetBoneIndex , int PoleBoneIndex , int IKBoneIndex , int IterationCount , int BoneChainMaxLength)
#line 2059 "../Code/d_game.cpp"
    {
#line 2060 "../Code/d_game.cpp"
        _Bone * TargetBone = Editor->StickMan[TargetBoneIndex];
#line 2061 "../Code/d_game.cpp"
        _Bone * PoleBone = Editor->StickMan[PoleBoneIndex];
#line 2062 "../Code/d_game.cpp"
        _Bone * IKBone = Editor->StickMan[IKBoneIndex];
#line 2063 "../Code/d_game.cpp"
        
#line 2064 "../Code/d_game.cpp"
        BoneIKUpdate(TargetBone , PoleBone , IKBone , IterationCount , BoneChainMaxLength);
#line 2065 "../Code/d_game.cpp"
    }
internal void AttachChildBone(int BoneIndex , int ChildBoneIndex)
#line 2574 "../Code/d_game.cpp"
    {
#line 2575 "../Code/d_game.cpp"
        AddToHashTable(ChildBoneIndex , BoneIndex , &Editor->BoneParentHashTable , BoneCount);
#line 2576 "../Code/d_game.cpp"
        AddToHashTable(BoneIndex , ChildBoneIndex , &Editor->BoneChildrenHashTable , BoneCount);
#line 2577 "../Code/d_game.cpp"
    }
