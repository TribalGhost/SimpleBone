
internal long long counter_stamp()
{
	LARGE_INTEGER time = {};
	QueryPerformanceCounter(&time);
	return time.QuadPart;
}

internal double time_stamp()
{
	LARGE_INTEGER frequency = {};
	LARGE_INTEGER end_time = {};
	QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&end_time);
    
	return (double)(end_time.QuadPart) * 1e6 / (double)frequency.QuadPart;
}

internal long long end_counter_stamp(long long begin_time)
{
	LARGE_INTEGER frequency = {};
	LARGE_INTEGER end_time = {};
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&end_time);
	return (end_time.QuadPart - begin_time);
}

internal double end_time_stamp(long long begin_time)
{
    
	LARGE_INTEGER frequency = {};
	LARGE_INTEGER end_time = {};
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&end_time);
    
	return (double)(end_time.QuadPart - begin_time) * 1e6 / (double)frequency.QuadPart;
}

internal long long cycle_stamp()
{
	return __rdtsc();
}

internal Vector4 color_to_linear(Color color)
{
    Vector4 result = {};
    result.x = ((float)color.r) / 255.0f;
    result.y = ((float)color.g) / 255.0f;
    result.z = ((float)color.b) / 255.0f;
    result.w = ((float)color.a) / 255.0f;
    
    return result;
}

internal Color linear_to_color(Vector4 color)
{
	Color result = {};
    result.r = (unsigned char)(color.x * 255.0f);
    result.g = (unsigned char)(color.y * 255.0f);
    result.b = (unsigned char)(color.z * 255.0f);
    result.a = (unsigned char)(color.w * 255.0f);
    
    return result;
}

internal Vector4 linear_fade(Vector4 linear_color ,float mul)
{
    linear_color.w *= mul;
    return linear_color;
}

//TODO : make blend function?
internal Color color_multiply_B(Color base , float value)
{
	if (value >= 1) value = 1;
    
	base.r *= value;
	base.g *= value;
	base.b *= value;
    
	return base;
}

internal Color color_invert(Color base)
{
	base.r = 255 - base.r;
	base.g = 255 - base.g;
	base.b = 255 - base.b;
    
	return base;
}

internal Color color_lerp(Color a , Color b , float t)
{
	Color new_color = {0,0,0,255};
	new_color.r = (unsigned char)Lerp((float)a.r , (float)b.r , t);
	new_color.g = (unsigned char)Lerp((float)a.g , (float)b.g , t);
	new_color.b = (unsigned char)Lerp((float)a.b , (float)b.b , t);
	new_color.a = (unsigned char)Lerp((float)a.a , (float)b.a , t);
    
	return new_color;
}

internal Color color_screen(Color base_color ,Color layer_color , float blend)
{
	Vector3 base = {((float)base_color.r)/255.0f , ((float)base_color.g)/255.0f ,((float)base_color.b)/255.0f };
	Vector3 layer = {((float)layer_color.r)/255.0f ,((float)layer_color.g)/255.0f ,((float)layer_color.b)/255.0f };
	Vector3 new_color = {};
    
	new_color.x = 1.0 - (1.0 - base.x)*( 1.0 - layer.x);
	new_color.y = 1.0 - (1.0 - base.y)*( 1.0 - layer.y);
	new_color.z = 1.0 - (1.0 - base.z)*( 1.0 - layer.z);
    
	new_color= Vector3Lerp(base , new_color , blend);
    
	if (new_color.x > 1.0) new_color.x = 1.0f;	
	if (new_color.y > 1.0) new_color.y = 1.0f;
	if (new_color.z > 1.0) new_color.z = 1.0f;
    
    Color result = {};
    result.r = (unsigned char)(new_color.x * 255.0);
    result.g = (unsigned char)(new_color.y * 255.0);
    result.b = (unsigned char)(new_color.z * 255.0);
    result.a = base_color.a;
    
    return result;
}

internal Color color_multiply(Color base , Color layer , float blend)
{
    
	Color new_color = base;
    
	new_color.r *= layer.r;
	new_color.g *= layer.g;
	new_color.b *= layer.b;
    
	base = color_lerp(base , layer , blend);
    
	return base;
}

internal Vector4 vector4_lerp(Vector4 v1, Vector4 v2, float amount)
{
	Vector4 result = { 0 };
    
	result.x = v1.x + amount * (v2.x - v1.x);
	result.y = v1.y + amount * (v2.y - v1.y);
	result.z = v1.z + amount * (v2.z - v1.z);
	result.w = v1.w + amount * (v2.w - v1.w);
    
	return result;
}

internal void color_quad(Vector4* quad_color , Vector4 target_color)
{   
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			float* color_component = (float*)(quad_color + i);
			float* target_color_compeont = (float*)&target_color;
			color_component[j] = target_color_compeont[j];
		}
	}
}

//create ray
internal Ray get_screen_to_world_ray_EX(Vector2 position, Camera camera, int viewport_x , int viewport_y, int width, int height)
{
	Ray ray = { 0 };
    
	// Calculate normalized device coordinates
	// NOTE: y value is negative
	float x = (2.0f * (position.x - viewport_x)) / (float)width - 1.0f;
	float y = 1.0f - (2.0f * ( position.y - (app_data->window_size.y - viewport_y - height) )) / (float)(height);
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

//beware the normal isn't normalized
internal RayCollision get_ray_collision_triangle(Ray ray, Vector3 p1, Vector3 p2, Vector3 p3)
{
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
		collision.normal = Vector3CrossProduct(edge1, edge2);
		collision.point = Vector3Add(ray.position, Vector3Scale(ray.direction, t));
	}
    
	return collision;
}

internal RayCollision get_collision_rect_3D(Ray ray, Rect rect)
{
	Vector3 world_space_vertex[4] = {};
    
	Vector3 position = rect.position;
	Vector2 size = rect.size;
	Quaternion rotation = rect.rotation;
    
	Vector3 vertex_left_top = { size.x / 2, size.y / 2, 0 };
	Vector3 vertex_right_top = { -size.x / 2, size.y / 2, 0 };
	Vector3 vertex_left_bottom = { size.x / 2, -size.y / 2, 0 };
	Vector3 vertex_right_bottom = { -size.x / 2, -size.y / 2, 0 };
    
	vertex_left_top = Vector3RotateByQuaternion(vertex_left_top, rotation);
	vertex_right_top = Vector3RotateByQuaternion(vertex_right_top, rotation);
	vertex_left_bottom = Vector3RotateByQuaternion(vertex_left_bottom, rotation);
	vertex_right_bottom = Vector3RotateByQuaternion(vertex_right_bottom, rotation);
    
	vertex_left_top.x += position.x;
	vertex_left_top.y += position.y;
	vertex_left_top.z += position.z;
    
	vertex_right_top.x += position.x;
	vertex_right_top.y += position.y;
	vertex_right_top.z += position.z;
    
	vertex_left_bottom.x += position.x;
	vertex_left_bottom.y += position.y;
	vertex_left_bottom.z += position.z;
    
	vertex_right_bottom.x += position.x;
	vertex_right_bottom.y += position.y;
	vertex_right_bottom.z += position.z;
    
	world_space_vertex[0] = vertex_left_top;
    
	world_space_vertex[1] = vertex_right_top;
    
	world_space_vertex[2] = vertex_left_bottom;
    
	world_space_vertex[3] = vertex_right_bottom;
    
	RayCollision ray_result = get_ray_collision_triangle(ray, world_space_vertex[0], world_space_vertex[1], world_space_vertex[2]);
    
	if (!ray_result.hit)
		ray_result = get_ray_collision_triangle(ray, world_space_vertex[1], world_space_vertex[3], world_space_vertex[2]);
    
	return ray_result;
}

internal Vector4 random_linear_color()
{
    Vector4 result = {};
	result.x = (float)rand() / (float)(RAND_MAX / 1);
	result.y = (float)rand() / (float)(RAND_MAX / 1);
	result.z = (float)rand() / (float)(RAND_MAX / 1);
	result.w = 1;
	
    
    return result;
}

internal Rect get_rect()
{
	Rect result = {};
	result.rotation = QuaternionIdentity();
	return result;
}

internal Rect get_rect_B(int pixel_size_x, int pixel_size_y)
{
	Rect result = get_rect();
	result.size.x = pixel_to_width(pixel_size_x);
	result.size.y = pixel_to_width(pixel_size_y);
    
	return result;
}

//TODO : don't use these
internal float noise(float seed)
{
	int n;
    
	n = seed * 57;
	n = (n << 13) ^ n;
    
	return 1.0 - ((n * ((n * n * 15731) + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0;
}

#define RandomFloat(Min,Max,Seed) Min+((noise(Seed)+1)/2)*(Max-Min)

#define RandomInt(Seed) ((Seed * 1103515245 + 12345) & RAND_MAX)

#define RandomIntRange(Min,Max,Seed) Min+RandomInt(Seed)%(Max-Min)

//stolen
internal unsigned int string_to_hash(const char * s)
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

#define allocate_temp( type , count) (type*)allocate_temp_(sizeof(type)*(count))

internal unsigned char* allocate_temp_(int size)
{
	if (size == 0)
	{
		return 0;
	}
    
	unsigned char* start = app_data->run_time_memory.current_memory;
	app_data->run_time_memory.current_memory += size;
	if (app_data->run_time_memory.current_memory >= app_data->run_time_memory.start_memory + app_data->run_time_memory.size)
	{
		CATCH;
	}
    
	memset(start, 0, size);
    
	return start;
}

#define allocate_frame(Type,Size) (Type*)allocate_frame_(sizeof(Type)*(Size))

internal unsigned char * allocate_frame_(int size)
{
    if (size == 0)
	{
		return 0;
	}
    
	unsigned char* start = app_data->frame_time_memory.current_memory;
	app_data->frame_time_memory.current_memory += size;
	if (app_data->frame_time_memory.current_memory >= app_data->frame_time_memory.start_memory + app_data->frame_time_memory.size)
	{
		CATCH;
	}
    
	memset(start, 0, size);
    
	return start;
}

internal DataHeader * add_data_header()
{
	(*current_data_header) = (DataHeader){};
    DataHeader * data_header = current_data_header;
    current_data_header++;
    
    return data_header;
}

#define allocate_save(type,size) (type*)allocate_save_(sizeof(type)*(size))

internal unsigned char* allocate_save_(int size)
{
    
	if (size == 0)
	{
		return 0;
	}
    
	unsigned char* start = current_save_memory_location;
	current_save_memory_location += size;
	if (current_save_memory_location >= save_memory + MAX_SAVE_SIZE)
	{
		CATCH;
	}
    
	memset(start, 0, size);
    
	return start;
    
}

//this is kinda dumb
internal void combine_file_path(const char* file_name,char * result_path)
{
	strcat(result_path, app_data->aplication_path);
	strcat(result_path, file_name);
}

internal wchar_t * combine_string_W(wchar_t * string_a , wchar_t * string_b)
{
    int string_size= wcslen(string_a) + wcslen(string_b) + 2;//adding 2 just to be save
    wchar_t * temp_string = allocate_frame(wchar_t , string_size);
    
	wcscpy(temp_string , string_a);
	wcscat(temp_string , string_b);
    
	return temp_string;
}

internal char * combine_string(char * string_a , char * string_b)
{
	int string_size= strlen(string_a) + strlen(string_b) + 2;//adding 2 just to be save
	char * temp_string = allocate_frame(char , string_size);
    
	strcpy(temp_string , string_a);
	strcat(temp_string , string_b);
    
	return temp_string;
}

internal char * get_app_file_path(const char* file_name)
{
	return combine_string(app_data->aplication_path , (char*) file_name);
}

internal char * get_level_file_path(const char * level_name)
{
	return combine_string(combine_string(app_data->aplication_path , "GameLevel\\") , (char*)level_name);
}

internal char * get_level_file_path_B()
{
	return combine_string(app_data->aplication_path , "GameLevel\\");
}

internal Vector4 quaternion_to_vector4(Quaternion _Quaternion)
{
    Vector4 result = {_Quaternion.x ,_Quaternion.y , _Quaternion.z, _Quaternion.w};
	return result;
}

internal float cubic_bezier(float x1 , float x2 , float x3 ,float x4, float t)
{
    float xa = Lerp( x1 , x2 , t );
    float xb = Lerp( x2 , x3 , t );
    
    float xc = Lerp(x2,x3 , t);
    float xd = Lerp(x3, x4 , t);
    
    float xe = Lerp(xa , xb , t);
    float xf = Lerp(xc , xd , t);
    
    return Lerp( xe , xf , t );
}

internal Vector3 vector3_cubic_bezier(Vector3 p1 , Vector3 p2 , Vector3 p3 , Vector3 p4  , float t)
{
    Vector3 result = {};
    
    result.x = cubic_bezier(p1.x,  p2.x,p3.x , p4.x , t);
    result.y = cubic_bezier(p1.y,  p2.y,p3.y , p4.y , t);
    result.z = cubic_bezier(p1.z,  p2.z,p3.z , p4.z , t);
    
    return result;
}

internal float ease_in_out_sine(float x)
{
	return -(cos(PI * x) - 1.0) / 2.0;
}

internal float ease_in_out_cubic(float x)
{
	return x < 0.5 ? 4.0 * x * x * x : 1.0 - pow(-2.0 * x + 2.0 , 3.0 ) / 2.0;
}

internal float ease_out_quint(float x )
{
	return 1.0 - pow(1.0 - x, 5.0);
}

internal float ease_in_back(float x , float c1)
{
	float c3 = c1 + 1;
    
	return c3 * x * x * x - c1 * x * x;
    
}

internal float ease_out_back(float x  , float c1)
{
	float c3 = c1 + 1;
    
	return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
}
