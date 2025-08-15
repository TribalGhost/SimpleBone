internal void ResetWindowTheme()
{
#if 0
	SetSysColors(2, aElements, aOldColors);
#endif
}

internal long long CounterStamp()
{
	LARGE_INTEGER Time = {};
	QueryPerformanceCounter(&Time);
	return Time.QuadPart;
}

internal double TimeStamp()
{
	LARGE_INTEGER Frequency = {};
	LARGE_INTEGER EndTime = {};
	QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&EndTime);
    
	return (double)(EndTime.QuadPart) * 1e6 / (double)Frequency.QuadPart;
}

internal long long EndCounterStamp(long long BeginTime)
{
	LARGE_INTEGER Frequency = {};
	LARGE_INTEGER EndTime = {};
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&EndTime);
	return (EndTime.QuadPart - BeginTime);
}

internal double EndTimeStamp(long long BeginTime)
{
	LARGE_INTEGER Frequency = {};
	LARGE_INTEGER EndTime = {};
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&EndTime);
    
	return (double)(EndTime.QuadPart - BeginTime) * 1e6 / (double)Frequency.QuadPart;
}

internal long long CycleStamp()
{
	return __rdtsc();
}

internal Vector4 ColorToLinear(Color _Color)
{
	return {
		(float)_Color.r / 255.0f,
		(float)_Color.g / 255.0f,
		(float)_Color.b / 255.0f,
		(float)_Color.a / 255.0f };
}

internal Color LinearToColor(Vector4 _Color)
{
	return
	{
		(unsigned char)(_Color.x * 255.0f),
		(unsigned char)(_Color.y * 255.0f),
		(unsigned char)(_Color.z * 255.0f),
		(unsigned char)(_Color.w * 255.0f),
	};
}

internal Vector4 LinearFade(Vector4 LinearColor ,float Mul)
{
    LinearColor.w *= Mul;
    return LinearColor;
}

//TODO : make blend function?
internal Color ColorMultiply(Color Base , float Value)
{
	if (Value >= 1) Value = 1;
    
	Base.r *= Value;
	Base.g *= Value;
	Base.b *= Value;
    
	return Base;
}

internal Color ColorInvert(Color Base)
{
	Base.r = 255 - Base.r;
	Base.g = 255 - Base.g;
	Base.b = 255 - Base.b;
    
	return Base;
}

internal Color ColorLerp(Color A , Color B , float T)
{
	Color NewColor = {0,0,0,255};
	NewColor.r = (unsigned char)Lerp((float)A.r , (float)B.r , T);
	NewColor.g = (unsigned char)Lerp((float)A.g , (float)B.g , T);
	NewColor.b = (unsigned char)Lerp((float)A.b , (float)B.b , T);
	NewColor.a = (unsigned char)Lerp((float)A.a , (float)B.a , T);
    
	return NewColor;
}

internal Color ColorScreen(Color BaseColor ,Color LayerColor , float Blend)
{
	Vector3 Base = {((float)BaseColor.r)/255.0f , ((float)BaseColor.g)/255.0f ,((float)BaseColor.b)/255.0f };
	Vector3 Layer = {((float)LayerColor.r)/255.0f ,((float)LayerColor.g)/255.0f ,((float)LayerColor.b)/255.0f };
	Vector3 NewColor = {};
    
	NewColor.x = 1.0 - (1.0 - Base.x)*( 1.0 - Layer.x);
	NewColor.y = 1.0 - (1.0 - Base.y)*( 1.0 - Layer.y);
	NewColor.z = 1.0 - (1.0 - Base.z)*( 1.0 - Layer.z);
    
	NewColor= Vector3Lerp(Base , NewColor , Blend);
    
	if (NewColor.x > 1.0) NewColor.x = 1.0f;	
	if (NewColor.y > 1.0) NewColor.y = 1.0f;
	if (NewColor.z > 1.0) NewColor.z = 1.0f;
    
	return { (unsigned char)(NewColor.x * 255.0) , (unsigned char)(NewColor.y * 255.0) , (unsigned char)(NewColor.z * 255.0) , BaseColor.a};
}

internal Color ColorMultiply(Color Base , Color Layer , float Blend)
{
    
	Color NewColor = Base;
    
	NewColor.r *= Layer.r;
	NewColor.g *= Layer.g;
	NewColor.b *= Layer.b;
    
	Base = ColorLerp(Base , Layer , Blend);
    
	return Base;
}

internal Vector4 Vector4Lerp(Vector4 v1, Vector4 v2, float amount)
{
	Vector4 result = { 0 };
    
	result.x = v1.x + amount * (v2.x - v1.x);
	result.y = v1.y + amount * (v2.y - v1.y);
	result.z = v1.z + amount * (v2.z - v1.z);
	result.w = v1.w + amount * (v2.w - v1.w);
    
	return result;
}

internal void ColorQuad(Vector4* QuadColor , Vector4 TargetColor)
{   
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			float* ColorComponent = (float*)(QuadColor + i);
			float* TargetColorCompeont = (float*)&TargetColor;
			ColorComponent[j] = TargetColorCompeont[j];
		}
	}
}

//create ray
internal Ray GetScreenToWorldRayEx(Vector2 position, Camera camera, int width, int height)
{
	Ray ray = { 0 };
    
	// Calculate normalized device coordinates
	// NOTE: y value is negative
	float x = (2.0f * position.x) / (float)width - 1.0f;
	float y = 1.0f - (2.0f * position.y) / (float)height;
	float z = 1.0f;
    
	// Store values in a vector
	Vector3 deviceCoords = { x, y, z };
    
	// Calculate view matrix from camera look at
	Matrix matView = MatrixLookAt(camera.position, camera.target, camera.up);
    
	Matrix matProj = MatrixIdentity();
	matProj = MatrixPerspective(camera.fovy * DEG2RAD, ((double)width / (double)height), rlGetCullDistanceNear(), rlGetCullDistanceFar());
    
	// Unproject far/near points
	Vector3 nearVector = { deviceCoords.x, deviceCoords.y, 0.0f };
	Vector3 farVector = { deviceCoords.x, deviceCoords.y, 1.0f };
    
	Vector3 nearPoint = Vector3Unproject(nearVector, matProj, matView);
	Vector3 farPoint = Vector3Unproject(farVector, matProj, matView);
    
	// Unproject the mouse cursor in the near plane
	// We need this as the source position because orthographic projects,
	// compared to perspective doesn't have a convergence point,
	// meaning that the "eye" of the camera is more like a plane than a point
	Vector3 cameraPlanePointerVector = { deviceCoords.x, deviceCoords.y, -1.0f };
	Vector3 cameraPlanePointerPos = Vector3Unproject(cameraPlanePointerVector, matProj, matView);
    
	// Calculate normalized direction vector
	Vector3 direction = Vector3Normalize(Vector3Subtract(farPoint, nearPoint));
    
	ray.position = camera.position;
    
	// Apply calculated vectors to ray
	ray.direction = direction;
    
	return ray;
}

//why i take you out?
internal RayCollision GetRayCollisionTriangle(Ray ray, Vector3 p1, Vector3 p2, Vector3 p3)
{
#define EPSILON 0.000001f        // A small number
    
	RayCollision collision = { 0 };
	Vector3 edge1 = { 0 };
	Vector3 edge2 = { 0 };
	Vector3 p, q, tv;
	float det, invDet, u, v, t;
    
	// Find vectors for two edges sharing V1
	edge1 = Vector3Subtract(p2, p1);
	edge2 = Vector3Subtract(p3, p1);
    
	// Begin calculating determinant - also used to calculate u parameter
	p = Vector3CrossProduct(ray.direction, edge2);
    
	// If determinant is near zero, ray lies in plane of triangle or ray is parallel to plane of triangle
	det = Vector3DotProduct(edge1, p);
    
	// Avoid culling!
	if ((det > -EPSILON) && (det < EPSILON)) return collision;
    
	invDet = 1.0f / det;
    
	// Calculate distance from V1 to ray origin
	tv = Vector3Subtract(ray.position, p1);
    
	// Calculate u parameter and test bound
	u = Vector3DotProduct(tv, p) * invDet;
    
	// The intersection lies outside the triangle
	if ((u < 0.0f) || (u > 1.0f)) return collision;
    
	// Prepare to test v parameter
	q = Vector3CrossProduct(tv, edge1);
    
	// Calculate V parameter and test bound
	v = Vector3DotProduct(ray.direction, q) * invDet;
    
	// The intersection lies outside the triangle
	if ((v < 0.0f) || ((u + v) > 1.0f)) return collision;
    
	t = Vector3DotProduct(edge2, q) * invDet;
    
	if (t > EPSILON)
	{
		// Ray hit, get hit point and normal
		collision.hit = true;
		collision.distance = t;
		collision.normal = Vector3Normalize(Vector3CrossProduct(edge1, edge2));
		collision.point = Vector3Add(ray.position, Vector3Scale(ray.direction, t));
	}
    
	return collision;
}

internal RayCollision GetCollisionQuad3D(_Quad Quad)
{
	Vector3 WorldSpaceVertex[Vertex_Count] = {};
	WorldSpaceVertex[0] = Quad.VertexPosition[Vertex_TopLeft];
    
	WorldSpaceVertex[1] = Quad.VertexPosition[Vertex_TopRight];
    
	WorldSpaceVertex[2] = Quad.VertexPosition[Vertex_BottomLeft];
    
	WorldSpaceVertex[3] = Quad.VertexPosition[Vertex_BottomRight];
    
	RayCollision RayResult = GetRayCollisionTriangle(AppData->MouseRay3D, WorldSpaceVertex[0], WorldSpaceVertex[1], WorldSpaceVertex[2]);
    
	if (!RayResult.hit)
		RayResult = GetRayCollisionTriangle(AppData->MouseRay3D, WorldSpaceVertex[1], WorldSpaceVertex[3], WorldSpaceVertex[2]);
    
	return RayResult;
    
}

internal bool CheckCollisionQuad3D(_Quad Quad)
{
	return GetCollisionQuad3D(Quad).hit;
}

internal RayCollision GetCollisionRect3D(Ray _Ray, _Rect Rect)
{
	Vector3 WorldSpaceVertex[4] = {};
    
	Vector3 Position = Rect.Position;
	Vector2 Size = Rect.Size;
	Quaternion Rotation = Rect.Rotation;
    
	Vector3 VertexLeftTop = { Size.x / 2, Size.y / 2, 0 };
	Vector3 VertexRightTop = { -Size.x / 2, Size.y / 2, 0 };
	Vector3 VertexLeftBottom = { Size.x / 2, -Size.y / 2, 0 };
	Vector3 VertexRightBottom = { -Size.x / 2, -Size.y / 2, 0 };
    
	VertexLeftTop = Vector3RotateByQuaternion(VertexLeftTop, Rotation);
	VertexRightTop = Vector3RotateByQuaternion(VertexRightTop, Rotation);
	VertexLeftBottom = Vector3RotateByQuaternion(VertexLeftBottom, Rotation);
	VertexRightBottom = Vector3RotateByQuaternion(VertexRightBottom, Rotation);
    
	VertexLeftTop.x += Position.x;
	VertexLeftTop.y += Position.y;
	VertexLeftTop.z += Position.z;
    
	VertexRightTop.x += Position.x;
	VertexRightTop.y += Position.y;
	VertexRightTop.z += Position.z;
    
	VertexLeftBottom.x += Position.x;
	VertexLeftBottom.y += Position.y;
	VertexLeftBottom.z += Position.z;
    
	VertexRightBottom.x += Position.x;
	VertexRightBottom.y += Position.y;
	VertexRightBottom.z += Position.z;
    
	WorldSpaceVertex[0] = VertexLeftTop;
    
	WorldSpaceVertex[1] = VertexRightTop;
    
	WorldSpaceVertex[2] = VertexLeftBottom;
    
	WorldSpaceVertex[3] = VertexRightBottom;
    
	RayCollision RayResult = GetRayCollisionTriangle(_Ray, WorldSpaceVertex[0], WorldSpaceVertex[1], WorldSpaceVertex[2]);
    
	if (!RayResult.hit)
		RayResult = GetRayCollisionTriangle(_Ray, WorldSpaceVertex[1], WorldSpaceVertex[3], WorldSpaceVertex[2]);
    
	return RayResult;
}

internal RayCollision GetCollisionRect3D(_Rect Rect)
{
	return GetCollisionRect3D(AppData->MouseRay3D, Rect);
}

internal bool CheckCollisionRect3D(_Rect Rect)
{
	return GetCollisionRect3D(AppData->MouseRay3D,Rect).hit;
}

internal Vector4 RandomLinearColor()
{
	float R = (float)rand() / (float)(RAND_MAX / 1);
	float G = (float)rand() / (float)(RAND_MAX / 1);
	float B = (float)rand() / (float)(RAND_MAX / 1);
	float A = 1;
	return { R,G,B,A };
}

//why multiply top and right???
internal float PixelToHeight(float Y)
{
	return (Y / AppData->WindowSize.y) * AppData->Top * 2;
}

internal float PixelToWidth(float X)
{
	return (X / AppData->WindowSize.x) * AppData->Right * 2;
}

internal Vector2 PixelToWorld(float X, float Y)
{
	X = (X / AppData->WindowSize.x) * 2 - 1;
	Y = (-Y / AppData->WindowSize.y) * 2 + 1;
    
	Vector2 Result = Vector2Transform({ X,Y }, MatrixInvert(AppData->MVP_Matrix));
	return Result;
}

internal Vector2 PixelToWorld(Vector2 Position)
{
	return PixelToWorld(Position.x, Position.y);
}

internal Vector2 PixelToWorldNoTransform(Vector2 Position)
{
	float X = Position.x;
	float Y = Position.y;
    
	X = (X / AppData->WindowSize.x) * 2 - 1;
	Y = (-Y / AppData->WindowSize.y) * 2 + 1;
    
	return {X, Y };
}

internal Vector3 PixelToWorldNoTransform(Vector3 Position)
{
	float X = Position.x;
	float Y = Position.y;
    
	X = (X / AppData->WindowSize.x) * 2 - 1;
	Y = (-Y / AppData->WindowSize.y) * 2 + 1;
    
	return {X, Y , Position.z};
}

internal Vector2 PixelToSize(float X, float Y)
{
	return { PixelToWidth(X), PixelToHeight(Y) };
}


internal _Rect GetRect()
{
	_Rect Result = {};
	Result.Rotation = QuaternionIdentity();
	return Result;
}

internal _Rect GetRect(int PixelSizeX, int PixelSizeY)
{
	_Rect Quad = GetRect();
	Quad.Size.x = PixelToWidth(PixelSizeX);
	Quad.Size.y = PixelToWidth(PixelSizeY);
    
	return Quad;
}

internal float Noise(float seed)
{
	int n;
    
	n = seed * 57;
	n = (n << 13) ^ n;
    
	return 1.0 - ((n * ((n * n * 15731) + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0;
}

#define RandomFloat(Min,Max,Seed) Min+((Noise(Seed)+1)/2)*(Max-Min)

#define RandomInt(Seed) ((Seed * 1103515245 + 12345) & RAND_MAX)

#define RandomIntRange(Min,Max,Seed) Min+RandomInt(Seed)%(Max-Min)

//stolen
//or i will say "shared"
internal unsigned int StringToHash(const char * s)
{
    unsigned int hash = 0;
    
    for(; *s; ++s)
    {
        hash += *s;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    
    return hash;
}

#define AllocateMemory(Type,Count) (Type*)memset(malloc(sizeof(Type)*Count),0,sizeof(Type)*Count)

internal unsigned char* AllocateTemp_(int Size)
{
	if (Size == 0)
	{
		return 0;
	}
    
	unsigned char* Start = AppData->RunTimeMemory.CurrentMemory;
	AppData->RunTimeMemory.CurrentMemory += Size;
	if (AppData->RunTimeMemory.CurrentMemory >= AppData->RunTimeMemory.StartMemory + AppData->RunTimeMemory.Size)
	{
		ASSERT;
	}
    
	memset(Start, 0, Size);
    
	return Start;
}

#define AllocateTemp(Type,Size) (Type*)AllocateTemp_(sizeof(Type)*(Size))

internal unsigned char * AllocateFrame_(int Size)
{
    if (Size == 0)
	{
		return 0;
	}
    
	unsigned char* Start = AppData->FrameTimeMemory.CurrentMemory;
	AppData->FrameTimeMemory.CurrentMemory += Size;
	if (AppData->FrameTimeMemory.CurrentMemory >= AppData->FrameTimeMemory.StartMemory + AppData->FrameTimeMemory.Size)
	{
		ASSERT;
	}
    
	memset(Start, 0, Size);
    
	return Start;
}

#define AllocateFrame(Type,Size) (Type*)AllocateFrame_(sizeof(Type)*(Size))

#define PARSE_SHOW_NESTED_DATA 0
#define PARSE_SHOW_RESULT_OF_BASIC_TYPE_CHECK 0

//this is kinda dumb
internal void CombineFilePath(const char* FileName,char * ResultPath)
{
	strcat(ResultPath, AppData->ApplicationPath);
	strcat(ResultPath, FileName);
}

internal char * CombineString(char * StrA , char * StrB)
{
	int StringSize= strlen(StrA) + strlen(StrB) + 2;//adding 2 just to be save
	char * TempString = AllocateFrame(char , StringSize);
    
	strcpy(TempString , StrA);
	strcat(TempString , StrB);
    
	return TempString;
}

internal char * GetAppFilePath(const char* FileName)
{
	return CombineString(AppData->ApplicationPath , (char*) FileName);
}

internal char * GetLevelFilePath(const char * LevelName)
{
	return CombineString(CombineString(AppData->ApplicationPath , "GameLevel\\") , (char*)LevelName);
}

internal char * GetLevelFilePath()
{
	return CombineString(AppData->ApplicationPath , "GameLevel\\");
}

internal void ParseToBasicType(_TypeMeta _Type,int Space,_MemberMetaData StructMemberMeta,int Count)
{
    _StructMetaData StructMeta = _TypeStructMeta_[_Type];
    
#if PARSE_SHOW_NESTED_DATA
    printf("\n");
    for(int SpaceIndex = 0 ;SpaceIndex < Space ; SpaceIndex++ ) printf(" ");
    
    if(StructMemberMeta.IsPointer) 
        printf("(POINTER) ");;
    
    printf(_TypeMetaName[_Type]);
    printf(" ");
    
    printf(StructMemberMeta.Name);
    
    if(Count == 0) ASSERT;
    
    if(Count>1)
        printf(" (Count : %d)",Count);
    
    if(StructMemberMeta.IsArray)
        printf(" (Array size : %d)",StructMemberMeta.ArraySize);
#endif
    
#if PARSE_SHOW_RESULT_OF_BASIC_TYPE_CHECK
    
    bool Probably_Is_Basic_Type = false;
    
    if(StructMeta.MemberCount == 0)
    {
        Probably_Is_Basic_Type = true;
    }
    
    if(StructMemberMeta.IsPointer)
    {
        Probably_Is_Basic_Type = true;
    }
    
    if(Probably_Is_Basic_Type)
    {
        printf("\n");
        for(int SpaceIndex = 0 ;SpaceIndex < Space ; SpaceIndex++ ) printf(" ");
        printf("is this basic type?");
        
        if(_Type > _MT_void)
        {
            if(!StructMemberMeta.IsPointer)
            {
                if(!_IsTypeEnum[_Type])
                {
                    //TODO :no sure is it catching unexpecting type
                    ASSERT;
                }
            }
        }
        
#if !PARSE_SHOW_NESTED_DATA
        printf(" ---> ");
        
        if(StructMemberMeta.IsPointer) 
        {
            printf("(POINTER) ");;
        }
        
        printf(_TypeMetaName[_Type]);
        printf(" ");
        printf(StructMemberMeta.Name);
#endif
        printf("\n");
        
    }
    
#endif
    
    if(StructMemberMeta.MultiplePointer) return;
    if(StructMemberMeta.IsPointer) return;
    
    if(StructMeta.MemberCount == 0) return;
    
#if PARSE_SHOW_NESTED_DATA
    printf("\n");
    for(int SpaceIndex = 0 ;SpaceIndex < Space ; SpaceIndex++ ) printf(" ");
    printf("{\n");
#endif
    
    for(int MemberIndex = 0 ; MemberIndex < StructMeta.MemberCount ; MemberIndex++)
    {
        _MemberMetaData MemberMeta = StructMeta.MembersArray[MemberIndex];
        
        int MemberType = MemberMeta.MemberType;
        ParseToBasicType((_TypeMeta)MemberType,Space + 4 , MemberMeta , 1);
        
    }
    
#if PARSE_SHOW_NESTED_DATA
    printf("\n");
    for(int SpaceIndex = 0 ;SpaceIndex < Space ; SpaceIndex++ ) printf(" ");
    
    printf("} (scope : ");
    printf(_TypeMetaName[_Type]);
    
    printf(")\n");
#endif
}

#define TO_STRING(Type) #Type
#define TYPE_TO_ENUM(Type) _MT_##Type

#define AllocatePersist( Data , Type , Count , TargetMemory) AllocatePersist_(sizeof(Type) , Count , #Data  , TYPE_TO_ENUM(Type),false , TargetMemory)

#define AllocatePersistCustomName( Name , Type , Count , TargetMemory) AllocatePersist_(sizeof(Type) , Count , Name , TYPE_TO_ENUM(Type),false , TargetMemory)

#define AllocatePersistPointer( Data , Type, Count , TargetMemory) AllocatePersist_( sizeof(void*) , Count , #Data ,_MT_void,true , TargetMemory)

internal void * AllocatePersist_
(	
 int Size,
 int Count,
 
 const char * DataName ,
 
 _TypeMeta _Type,
 bool IsPointer , 
 _PersistMemory * TargetMemory
 )
{
    
#if PARSE_SHOW_NESTED_DATA || PARSE_SHOW_RESULT_OF_BASIC_TYPE_CHECK 
    printf("\n");
    printf("\n");
    printf(DataName);
    printf("\n");
    ParseToBasicType(_Type,0 ,{} , Count);
    printf("\n");
#endif
    
    _StructMetaData StructMeta = _TypeStructMeta_[_Type];
    
	_MemoryDetail * NewNode = AllocateTemp(_MemoryDetail, 1);
    
	TargetMemory->MemoryDetailCount++;
    
	if (!TargetMemory->MemoryDetailHead)
	{
		TargetMemory->MemoryDetailHead = NewNode;
		TargetMemory->MemoryDetailTail = TargetMemory->MemoryDetailHead;
	}
	else
	{
		TargetMemory->MemoryDetailTail->NextNode = NewNode;
		TargetMemory->MemoryDetailTail = TargetMemory->MemoryDetailTail->NextNode;
	}
    
	unsigned char* Start = (unsigned char* )_StartMemory_ + TargetMemory->CurrentOffset;
	TargetMemory->CurrentOffset+= Size * Count;
	if (TargetMemory->CurrentOffset >= TargetMemory->StartOffset + TargetMemory->Size)
	{
		ASSERT;
	}
    
	NewNode->Start = Start - _StartMemory_;
	NewNode->End = TargetMemory->CurrentOffset;
	NewNode->DataCount = Count;
	NewNode->IsPointer = IsPointer;
    
	if (DataName)
	{
		strcpy(NewNode->DataName, DataName);
	}
    
	NewNode->Type = (int)_Type;
    
	memset(Start, 0, Size * Count);
	return Start;
}

internal void ResetPersistMemory(_PersistMemory * TargetMemory)
{
	TargetMemory->CurrentOffset= TargetMemory->StartOffset;
    TargetMemory->MemoryDetailHead = 0;
    TargetMemory->MemoryDetailTail = 0;
    TargetMemory->MemoryDetailCount = 0;
}

internal Vector4 QuaternionToVector4(Quaternion _Quaternion)
{
	return {_Quaternion.x ,_Quaternion.y , _Quaternion.z, _Quaternion.w};
}

internal float CubicBezier(float x1 , float x2 , float x3 ,float x4, float t)
{
    float xa = Lerp( x1 , x2 , t );
    float xb = Lerp( x2 , x3 , t );
    
    float xc = Lerp(x2,x3 , t);
    float xd = Lerp(x3, x4 , t);
    
    float xe = Lerp(xa , xb , t);
    float xf = Lerp(xc , xd , t);
    
    return Lerp( xe , xf , t );
}

internal Vector3 Vector3CubicBezier(Vector3 P1 , Vector3 P2 , Vector3 P3 , Vector3 P4  , float t)
{
    Vector3 Result = {};
    
    Result.x = CubicBezier(P1.x,  P2.x,P3.x , P4.x , t);
    Result.y = CubicBezier(P1.y,  P2.y,P3.y , P4.y , t);
    Result.z = CubicBezier(P1.z,  P2.z,P3.z , P4.z , t);
    
    return Result;
}

internal float EaseInOutSine(float x)
{
	return -(cos(PI * x) - 1.0) / 2.0;
}

internal float EaseInOutCubic(float x)
{
	return x < 0.5 ? 4.0 * x * x * x : 1.0 - pow(-2.0 * x + 2.0 , 3.0 ) / 2.0;
}

internal float EaseOutQuint(float x )
{
	return 1.0 - pow(1.0 - x, 5.0);
}

internal float EaseInBack(float x , float c1)
{
	float c3 = c1 + 1;
    
	return c3 * x * x * x - c1 * x * x;
    
}

internal float EaseInBack(float x )
{
	float c1 = 1.70158;
    
	return EaseInBack(x , c1);
}

internal float EaseOutBack(float x  , float c1)
{
	float c3 = c1 + 1;
    
	return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
}

internal float EaseOutBack(float x)
{
	float c1 = 1.70158;
	return EaseOutBack(x , c1);
}

#if 0
internal int GetIndexFromArray(void *Array , void* Element)
{
	D_Array* CurrentArray = GetArray(Array);
    
	unsigned char* ArrayStart = (unsigned char*)CurrentArray->DataArray;
	int ElementIndex = ((unsigned char*)Element )- ((unsigned char*)ArrayStart);
    
    unsigned char* ArrayEnd = (unsigned char*)CurrentArray->IsValidArray;
    int TypeSize = ArrayEnd - ArrayStart;
    TypeSize /= CurrentArray->ArrayMaxSize;
    
	int Extra = ElementIndex % TypeSize;
	if (Extra != 0)
	{
		return -1;
	}
    
	ElementIndex /= TypeSize;
    
	return ElementIndex;
}

internal void ClearArray(void* Array)
{
	D_Array* CurrentArray = GetArray(Array);
    
	for (int ArrayIndex = 0; ArrayIndex < CurrentArray->ArrayCount; ArrayIndex++)
	{
		CurrentArray->IsValidArray[ArrayIndex] = false;
	}
    
	CurrentArray->ArrayCount = 0;
	CurrentArray->LowestArraySlot = 0;
}

internal void RecheckArray(void * Array)
{
	D_Array* CurrentArray = GetArray(Array);    
    
	CurrentArray->LowestArraySlot = 0;
	CurrentArray->ArrayCount = 0;
	for( ; CurrentArray->IsValidArray[CurrentArray->LowestArraySlot] ;CurrentArray->LowestArraySlot++);
    
	for (int ArrayIndex = 0; ArrayIndex < CurrentArray->ArrayMaxSize; ArrayIndex++)
	{
		if (CurrentArray->IsValidArray[ArrayIndex])
		{
			CurrentArray->ArrayCount = ArrayIndex + 1;
		}
	}
}

internal bool CheckItem(void * Array , void * Item)
{
	D_Array * ArrayHeader= GetArray(Array);
	int Index = GetIndexFromArray(Array , Item);
	return ArrayHeader->IsValidArray[Index];
}

internal void *AddToArray(void* Array)
{
	D_Array* CurrentArray = GetArray(Array);
    
	CurrentArray->IsValidArray[CurrentArray->LowestArraySlot] = true;
    
	unsigned char* Data = (unsigned char*)CurrentArray->DataArray;
	
    unsigned char* ArrayEnd = (unsigned char*)CurrentArray->IsValidArray;
    int TypeSize = ArrayEnd - Data;
    TypeSize /= CurrentArray->ArrayMaxSize;
    
    Data += TypeSize * CurrentArray->LowestArraySlot;
    
	for (; CurrentArray->IsValidArray[CurrentArray->LowestArraySlot]; CurrentArray->LowestArraySlot++);
    
	if (CurrentArray->ArrayCount < CurrentArray->LowestArraySlot)
		CurrentArray->ArrayCount = CurrentArray->LowestArraySlot;
    
	if (CurrentArray->ArrayCount >= CurrentArray->ArrayMaxSize)
		ASSERT;
    
	return Data;
}

internal bool DeleteFromArray(void* Array,void * Element)
{
	D_Array* CurrentArray = GetArray(Array);
	
	unsigned char* ArrayStart = (unsigned char *)CurrentArray->DataArray;
	int ElementIndex = GetIndexFromArray(Array, Element);
    
	if (ElementIndex == -1)
		return false;
    
	CurrentArray->IsValidArray[ElementIndex] = false;
	
    
    unsigned char* ArrayEnd = (unsigned char*)CurrentArray->IsValidArray;
    int TypeSize = ArrayEnd - ArrayStart;
    TypeSize /= CurrentArray->ArrayMaxSize;
    
    
    memset(Element, 0, TypeSize);
    
	if (CurrentArray->LowestArraySlot > ElementIndex)
		CurrentArray->LowestArraySlot = ElementIndex;
    
	int TestIndex = 0;
	for( ; CurrentArray->IsValidArray[TestIndex] ;TestIndex++);
	if (TestIndex != CurrentArray->LowestArraySlot) ASSERT;
	
	for ( ; (!CurrentArray->IsValidArray[CurrentArray->ArrayCount - 1] ) && (CurrentArray->ArrayCount  > 0 );CurrentArray->ArrayCount--);
    
	return true;
}

//you shouldn't use this
//and i shouldn't make this
//this is sad and depressed ):
struct _ArrayForeachData
{
	D_Array* CurrentArray;
	int ArrayIndex;
};

//too wacky man
//why every time i try thing like this just go like this...

//there is so many typing

//it has to be good right?...

//it can't break from the loop
#define D_Foreach(Type ,Variable,Array) \
for (_ArrayForeachData ForeachData ={GetArray(Array),0}; ForeachData.ArrayIndex < ForeachData.CurrentArray->ArrayCount; ForeachData.ArrayIndex++) \
for (Type* Variable= ((Type*)ForeachData.CurrentArray->DataArray) + ForeachData.ArrayIndex; ForeachData.CurrentArray->IsValidArray[ForeachData.ArrayIndex]; ForeachData.ArrayIndex++, Variable= ((Type*)ForeachData.CurrentArray->DataArray) + ForeachData.ArrayIndex)

#define D_ForLoop(Index,Array) \
for (int Index = 0, Break = 0; !Break; ) \
for (D_Array* CurrentArray = GetArray(GameWorld->EntityArray); !(Break = Index >= CurrentArray->ArrayCount); Index++) \
for (; Break = CurrentArray->IsValidArray[Index]; Index++) \

#endif

#if 0
//very limited saving
internal void ReassignPointer(_StructMetaData StructMetaData, unsigned char* Memory, unsigned char* Save_StartMemoryAddress)
{
	for (int MemberIndex = 0; MemberIndex < StructMetaData.MemberCount; MemberIndex++)
	{
		_MemberMetaData* MemberMetaData = StructMetaData.MembersArray + MemberIndex;
		if (MemberMetaData->IsIntropsected)
		{
			_StructMetaData MemberStructMeta = _TypeStructMeta_[MemberMetaData->MemberValueType];
            
			if (MemberStructMeta.MemberCount == 0)
			{
				ASSERT;
			}
            
			if (MemberMetaData->IsPointer)
			{
				unsigned char** MemberMemory = (unsigned char**)(Memory + MemberMetaData->MemberOffset);
				unsigned char* PointerAddress = *MemberMemory;
                
				if ((*MemberMemory))
				{
					int PointerOffset = PointerAddress - Save_StartMemoryAddress;
					unsigned char* ReassignedAddress = AppData->AllocatedMemory.StartMemory + PointerOffset;
                    
					(*MemberMemory) = ReassignedAddress;
                    
					if (!MemberMetaData->MultiplePointer)
					{
						ReassignPointer(MemberStructMeta, ReassignedAddress, Save_StartMemoryAddress);
					}
				}
			}
			else if (MemberMetaData->IsArray)
			{
			}
			else
			{
				unsigned char* MemberMemory = Memory + MemberMetaData->MemberOffset;
				ReassignPointer(MemberStructMeta, MemberMemory, Save_StartMemoryAddress);
			}
		}
	}
}
#endif