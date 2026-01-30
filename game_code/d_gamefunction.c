#ifdef BUILD_D_WINDOWS
internal long long function counter_stamp()
{
	LARGE_INTEGER time = {};
	QueryPerformanceCounter(&time);
	return time.QuadPart;
}

internal double function time_stamp()
{
    LARGE_INTEGER frequency = {};
	LARGE_INTEGER end_time = {};
	QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&end_time);
    
	return (double)(end_time.QuadPart) * 1e6 / (double)frequency.QuadPart;
}

internal void function draw_triangle_line(Vector3 a , Vector3 b , Vector3 c , Color color);
internal void function draw_triangle(Vector3* all_vertices ,Vector4 vertex_color);
internal void function draw_box_line(Box box , Color line_color , float line_size);
internal void function draw_arrow_ray(Vector3 start_position , Vector3 direction  , Color line_color);
internal void function draw_simplex_triangle(Vector3 a , Vector3 b , Vector3 c);
internal void function draw_quad_line(Quad quad , Color line_color , float line_size);
internal void function draw_arrow_line(Vector3 start_position , Vector3 end_position , Color line_color);
#endif

#ifdef BUILD_D_LINUX
double teteted time_stamp()
{
    struct timespec now = {};
    clock_gettime(CLOCK_MONOTONIC_RAW , &now);
    double nano_second = now.tv_nsec;
    nano_second /= 1000000000.0;
    return ((double)now.tv_sec) + nano_second;
}
#endif

internal Vector4 function color_to_linear(Color color)
{
    Vector4 result = {};
    result.x = ((float)color.r) / 255.0f;
    result.y = ((float)color.g) / 255.0f;
    result.z = ((float)color.b) / 255.0f;
    result.w = ((float)color.a) / 255.0f;
    
    return result;
}

internal Color function linear_to_color(Vector4 color)
{
	Color result = {};
    result.r = (unsigned char)(color.x * 255.0f);
    result.g = (unsigned char)(color.y * 255.0f);
    result.b = (unsigned char)(color.z * 255.0f);
    result.a = (unsigned char)(color.w * 255.0f);
    
    return result;
}

internal Vector4 function linear_fade(Vector4 linear_color ,float mul)
{
    linear_color.w *= mul;
    return linear_color;
}

//TODO : make blend function?
internal Color function color_multiply_B(Color base , float value)
{
	if (value >= 1) value = 1;
    
	base.r *= value;
	base.g *= value;
	base.b *= value;
    
	return base;
}

internal Color function color_invert(Color base)
{
	base.r = 255 - base.r;
	base.g = 255 - base.g;
	base.b = 255 - base.b;
    
	return base;
}

internal Color function color_lerp(Color a , Color b , float t)
{
	Color new_color = {0,0,0,255};
	new_color.r = (unsigned char)Lerp((float)a.r , (float)b.r , t);
	new_color.g = (unsigned char)Lerp((float)a.g , (float)b.g , t);
	new_color.b = (unsigned char)Lerp((float)a.b , (float)b.b , t);
	new_color.a = (unsigned char)Lerp((float)a.a , (float)b.a , t);
    
	return new_color;
}

internal Color function color_screen(Color base_color ,Color layer_color , float blend)
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

internal Color function color_multiply(Color base , Color layer , float blend)
{
	Color new_color = base;
    
	new_color.r *= layer.r;
	new_color.g *= layer.g;
	new_color.b *= layer.b;
    
	base = color_lerp(base , layer , blend);
    
	return base;
}

internal Vector4 function vector4_lerp(Vector4 v1, Vector4 v2, float amount)
{
	Vector4 result = { 0 };
    
	result.x = v1.x + amount * (v2.x - v1.x);
	result.y = v1.y + amount * (v2.y - v1.y);
	result.z = v1.z + amount * (v2.z - v1.z);
	result.w = v1.w + amount * (v2.w - v1.w);
    
	return result;
}

internal void function color_quad(Vector4* quad_color , Vector4 target_color)
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

internal Vector4 function random_linear_color()
{
    Vector4 result = {};
	result.x = (float)rand() / (float)(RAND_MAX / 1);
	result.y = (float)rand() / (float)(RAND_MAX / 1);
	result.z = (float)rand() / (float)(RAND_MAX / 1);
	result.w = 1;
	
    return result;
}

//TODO : don't use these
internal float function noise(float seed)
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
internal unsigned int function string_to_hash(const char * s)
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

#define malloc_and_memset(type,count) (type*)memset(malloc(sizeof(type)*count),0,sizeof(type)*count)

internal Vector4 function quaternion_to_vector4(Quaternion _Quaternion)
{
    Vector4 result = {_Quaternion.x ,_Quaternion.y , _Quaternion.z, _Quaternion.w};
	return result;
}

internal Vector3 function project_on_plane(Vector3 direction , Vector3 normal)
{
    Vector3 right_axis = Vector3CrossProduct(direction , normal);
    if(Vector3LengthSqr(right_axis) > 0.0000001f)
    {
        Vector3 forward_axis = Vector3CrossProduct(normal , right_axis);
        direction = Vector3Project(direction , forward_axis);
    }
    
    return direction;
}

internal float function cubic_bezier(float x1 , float x2 , float x3 ,float x4, float t)
{
    float xa = Lerp( x1 , x2 , t );
    float xb = Lerp( x2 , x3 , t );
    
    float xc = Lerp(x2,x3 , t);
    float xd = Lerp(x3, x4 , t);
    
    float xe = Lerp(xa , xb , t);
    float xf = Lerp(xc , xd , t);
    
    return Lerp( xe , xf , t );
}

internal Vector3 function vector3_cubic_bezier(Vector3 p1 , Vector3 p2 , Vector3 p3 , Vector3 p4  , float t)
{
    Vector3 result = {};
    
    result.x = cubic_bezier(p1.x,  p2.x,p3.x , p4.x , t);
    result.y = cubic_bezier(p1.y,  p2.y,p3.y , p4.y , t);
    result.z = cubic_bezier(p1.z,  p2.z,p3.z , p4.z , t);
    
    return result;
}

internal float function ease_in_out_sine(float x)
{
	return -(cos(PI * x) - 1.0) / 2.0;
}

internal float function ease_in_out_cubic(float x)
{
	return x < 0.5 ? 4.0 * x * x * x : 1.0 - pow(-2.0 * x + 2.0 , 3.0 ) / 2.0;
}

internal float function ease_out_quint(float x )
{
	return 1.0 - pow(1.0 - x, 5.0);
}

internal float function ease_in_back(float x , float c1)
{
	float c3 = c1 + 1;
    
	return c3 * x * x * x - c1 * x * x;
}

internal float function ease_out_back(float x  , float c1)
{
	float c3 = c1 + 1;
    
	return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
}

internal bool function compare_string_C(char * string_A, char * string_B , int count)
{
    for(int char_index = 0 ; char_index < count ; char_index++)
    {
        if(string_A[char_index] != string_B[char_index])
        {
            return false;
        }
    }
    
    return true;
}

internal bool function compare_string_W( wchar_t * string_A , wchar_t * string_B)
{
    int string_B_length = 0;
    for(int char_index = 0 ; string_B[char_index] != '\0' ; char_index++ ,string_B_length++);
    
    int name_length = 0;
    for(int char_index = 0 ; string_A[char_index] != '\0' ; char_index++ , name_length++);
    
    if(name_length != string_B_length) return false;
    
    for(int char_index = 0 ; char_index < name_length ; char_index++)
    {
        if(string_A[char_index] != string_B[char_index])
        {
            return false;
        }
    }
    
    return true;
}

static int hash_array[] = { 208,34,231,213,32,248,233,56,161,78,24,140,71,48,140,254,245,255,247,247,40,
    185,248,251,245,28,124,204,204,76,36,1,107,28,234,163,202,224,245,128,167,204,
    9,92,217,54,239,174,173,102,193,189,190,121,100,108,167,44,43,77,180,204,8,81,
    70,223,11,38,24,254,210,210,177,32,81,195,243,125,8,169,112,32,97,53,195,13,
    203,9,47,104,125,117,114,124,165,203,181,235,193,206,70,180,174,0,167,181,41,
    164,30,116,127,198,245,146,87,224,149,206,57,4,192,210,65,210,129,240,178,105,
    228,108,245,148,140,40,35,195,38,58,65,207,215,253,65,85,208,76,62,3,237,55,89,
    232,50,217,64,244,157,199,121,252,90,17,212,203,149,152,140,187,234,177,73,174,
    193,100,192,143,97,53,145,135,19,103,13,90,135,151,199,91,239,247,33,39,145,
    101,120,99,3,186,86,99,41,237,203,111,79,220,135,158,42,30,154,120,67,87,167,
    135,176,183,191,253,115,184,21,233,58,129,233,142,39,128,211,118,137,139,255,
    114,20,218,113,154,27,127,246,250,1,8,198,250,209,92,222,173,21,88,102,219 };

#define PNoise2(x, y,SEED) hash_array[(hash_array[(y + SEED) % 256] + x) % 256];

#define PLinInter(x,y,s) x + s * (y - x)

#define SmoothInter(x, y, s) PLinInter(x, y, s * s * (3 - 2 * s))

internal bool function check_and_comsumed_input(int code , InputArray * input)
{
    for (int i = 0; i < input->count; i++)
	{
		if (input->array[i] == code) 
        {
            if(input->consumed_array[i])
            {
                return false;
            }
            else
            {
                input->consumed_array[i] = true;
                return true;
            }
        }
	}
    
    return false;
}

internal bool function check_input(int code , InputArray * input)
{
    for (int i = 0; i < input->count; i++)
	{
		if (input->array[i] == code) 
        {
            return true;
        }
	}
    
    return false;
}

internal bool function key_pressing(int key) 
{
    return check_and_comsumed_input(key , input_state->input_array + IA_pressing_key);
}

internal bool function key_pressed(int key)
{
    return check_and_comsumed_input(key , input_state->input_array + IA_pressed_key);
}

internal bool function key_released(int key)
{
    return check_and_comsumed_input(key , input_state->input_array + IA_released_key);
}

internal bool function mouse_pressing(int button)
{
    return check_and_comsumed_input(button , input_state->input_array + IA_pressing_mouse);
}

internal bool function mouse_pressed(int button)
{
    return check_and_comsumed_input(button , input_state->input_array + IA_pressed_mouse);
}

internal bool function mouse_released(int button)
{
    return check_and_comsumed_input(button , input_state->input_array + IA_released_mouse);
}

internal bool function mouse_pressing_no_check(int button)
{
    return check_input(button , input_state->input_array + IA_pressing_mouse);
}

internal bool function mouse_pressed_no_check(int button)
{
    return check_input(button , input_state->input_array + IA_pressed_mouse);
}

internal bool function mouse_released_no_check(int button)
{
    return check_input(button , input_state->input_array + IA_released_mouse);
}

internal bool function key_pressing_no_check(int button)
{
    return check_input(button , input_state->input_array + IA_pressing_key);
}

internal bool function key_pressed_no_check(int button)
{
    return check_input(button , input_state->input_array + IA_pressed_key);
}

internal bool function key_released_no_check(int button)
{
    return check_input(button , input_state->input_array + IA_released_key);
}

internal Quad function resize_block(Quad quad,float size)
{
    Vector3 centre = {};
    
    int vertex_order[quad_vertex_count] = {};
    vertex_order[0] = vertex_top_left;
    vertex_order[1] = vertex_top_right;
    vertex_order[2] = vertex_bottom_right;
    vertex_order[3] = vertex_bottom_left;
    
    for (int order_index = 0; order_index < 4; order_index++)
    {
        
        int start_vertex_index = vertex_order[order_index];
        int end_vertex_index = vertex_order[0];
        
        if ((order_index+1) < 3)
        {
            end_vertex_index = vertex_order[order_index + 1];
        }
        
        Vector3 start = quad.vertex_position[start_vertex_index];
        Vector3 end = quad.vertex_position[start_vertex_index];
        
        Vector3 edge_direction = Vector3Subtract(start, end);
        Vector3 edge_normal = { -edge_direction.y,edge_direction.x,0 };
    }
    
    for (int vertex_index = 0; vertex_index < quad_vertex_count; vertex_index++)
    {
        centre.x += quad.vertex_position[vertex_index].x;
        centre.y += quad.vertex_position[vertex_index].y;
        centre.z += quad.vertex_position[vertex_index].z;
    }
    
    centre.x /= quad_vertex_count;
    centre.y /= quad_vertex_count;
    centre.z /= quad_vertex_count;
    
    for (int vertex_index = 0; vertex_index < quad_vertex_count; vertex_index++)
    {
        Vector3 offset = Vector3Subtract(quad.vertex_position[vertex_index], centre);
        offset = Vector3Scale(offset, size );
        
        offset = Vector3Add(offset, centre);
        
        quad.vertex_position[vertex_index] = offset;
    }
    
    return quad;
}

internal Quad function resize_block_B(Quad * quad, float size)
{
    return resize_block(*quad, size);
}

//beware the normal isn't normalized
internal RayCollision function get_ray_collision_triangle(Ray ray, Vector3 p1, Vector3 p2, Vector3 p3)
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

internal Vector3 function get_edge_direction(Vector3 start,Vector3 end , Vector3 point)
{
    Vector3 edge = Vector3Subtract(end, start);
    
    Vector3 edge_point = Vector3Subtract(point, start);
    
    float inside = Vector3CrossProduct(edge, edge_point).z;
    
    float dx = start.x - end.x;
    float dy = start.y - end.y;
    
    Vector3 edge_normal_inside = { dy,-dx,inside  };
    
    return edge_normal_inside;
}

internal bool function check_collision_quad_point(Quad quad, Vector3 point)
{
    enum
    {
        edge_top,
        edge_right,
        edge_bottom,
        edge_left,
        
        edge_count
    };
    
    Vector3 edge_normal_inside_distance[edge_count] = {};
    
    edge_normal_inside_distance[edge_top] = get_edge_direction(quad.vertex_position[vertex_top_left], quad.vertex_position[vertex_top_right], point);
    edge_normal_inside_distance[edge_right] = get_edge_direction(quad.vertex_position[vertex_top_right], quad.vertex_position[vertex_bottom_right], point);
    edge_normal_inside_distance[edge_bottom] = get_edge_direction(quad.vertex_position[vertex_bottom_right], quad.vertex_position[vertex_bottom_left], point);
    edge_normal_inside_distance[edge_left] = get_edge_direction(quad.vertex_position[vertex_bottom_left], quad.vertex_position[vertex_top_left], point);
    
    for (int edge_index = 0; edge_index < edge_count; edge_index++)
    {
        Vector3 current_edge_normal_inside = edge_normal_inside_distance[edge_index];
        
        if (current_edge_normal_inside.z > 0)
        {
            return false;
        }
    }
    
    return true;
}

//TODO : this is bad
internal bool function check_collision_quad_to_rect(Quad quad , Rect rect)
{
    Vector3 rect_vertices[quad_vertex_count] = {};
    
    rect_vertices[vertex_top_left].x -= rect.size.x * 0.5f;	
    rect_vertices[vertex_top_left].y += rect.size.y * 0.5f;
    
    rect_vertices[vertex_top_right].x += rect.size.x * 0.5f;	
    rect_vertices[vertex_top_right].y += rect.size.y * 0.5f;
    
    rect_vertices[vertex_bottom_left].x -= rect.size.x * 0.5f;	
    rect_vertices[vertex_bottom_left].y -= rect.size.y * 0.5f;
    
    rect_vertices[vertex_bottom_right].x += rect.size.x * 0.5f;	
    rect_vertices[vertex_bottom_right].y -= rect.size.y * 0.5f;
    
    for (int vertex_index = 0 ; vertex_index < quad_vertex_count ; vertex_index++)
    {
        rect_vertices[vertex_index] = Vector3RotateByQuaternion(rect_vertices[vertex_index] , rect.rotation);
        
        rect_vertices[vertex_index].x += rect.position.x;
        rect_vertices[vertex_index].y += rect.position.y;
        
        if (check_collision_quad_point(quad, rect_vertices[vertex_index]))
        {
            return true;
        }
    }
    
    return false;
}

internal Vector4 function selection_from_start_to_end_to_rect(Vector2 start_position, Vector2 end)
{
    Vector2 drag_centre = {};
    drag_centre.x = (start_position.x + end.x) / 2;
    drag_centre.y = (start_position.y + end.y) / 2;
    
    Vector2 drag_size = {};
    
    drag_size.x = start_position.x - end.x;
    if (drag_size.x < 0)
        drag_size.x *= -1;
    
    drag_size.y = start_position.y - end.y;
    if (drag_size.y < 0)
        drag_size.y *= -1;
    
    return (Vector4){ drag_centre.x,drag_centre.y,drag_size.x,drag_size.y };
}

//why are you broken??
internal bool function ray_line_segment_intersection_example(Vector3 o, Vector3 d, Vector3 a, Vector3 b)
{
    Vector3 ortho = { -d.y, d.x };
    Vector3 aToO = Vector3Subtract(o , a);
    Vector3 aToB = Vector3Subtract(b , a);
    
    float denom = Vector3DotProduct(aToB, ortho);
    
    // Here would be a good time to see if denom is zero in which case the line segment and
    // the ray are parallel.
    
    // The length of this cross product can also be written as abs( aToB.x * aToO.y - aToO.x * aToB.y ).
    float t1 = Vector3Length(Vector3CrossProduct(aToB, aToO)) / denom;
    float t2 = Vector3DotProduct(aToO, ortho) / denom;
    
    return t2 >= 0 && t2 <= 1 && t1 >= 0;
}

//this is how normal people solve it
internal Vector2 function ray_line_segment_intersection(Vector3 o, Vector3 d, Vector3 a, Vector3 b)
{
    float t1 = (o.x - a.x) * (b.y - a.y) - (o.y - a.y) * (b.x - a.x);
    t1 /= (d.y * (b.x - a.x) - d.x * (b.y - a.y));
    
    float t2 = (a.x - o.x) * d.y - (a.y - o.y) * d.x;
    t2 /= (b.y - a.y) * d.x - (b.x - a.x) * d.y;
    
    return (Vector2){ t1 , t2 };
}

//this is how 200+ iq people solve it :)
internal Vector2 function ray_line_segment_intersection_(Vector3 o, Vector3 d, Vector3 a, Vector3 b)
{
    Vector3 n1 = Vector3Subtract(b, a);
    n1 = (Vector3){ -n1.y , n1.x };
    
    float t1 = Vector3DotProduct(Vector3Subtract(a, o), n1);
    
    float t1Bottom = Vector3DotProduct(d, n1);
    if (t1Bottom == 0)
    {
        t1 = -1;
    }
    else
    {
        t1 /= t1Bottom;
    }
    
    Vector3 n2 = { -d.y , d.x };
    
    float t2 = Vector3DotProduct(Vector3Subtract(o, a), n2);
    float t2Bottom = Vector3DotProduct(Vector3Subtract(b, a), n2);
    
    if (t2Bottom == 0)
    {
        t2 = -1;
    }
    else
    {
        t2 /= t2Bottom;
    }
    
    //this is pretty old
#if 0
    wchar_t DebugString[128] = {};
    swprintf(DebugString, L"t1:%.2f | t2:%.2f | rate: %.2f", t1 , t2 , 1.0 / (float)D_UPDATE_RATE);
    D_draw_text_C(Vector3Lerp(a, b, 0.5f), 0.06, DebugString, Fade(WHITE, 0.5), true);
#endif
    
    return (Vector2){ t1 , t2 };
}

//every allocation in pool require calling this first
//too much?
internal void function set_pool(ChunkPool * pool , int * chunk_index)
{
    current_pool_to_allocate = pool;
    current_chunk_index = chunk_index;
}

internal void * allocate_memory(int size , int allocate_type);

#define allocate_memory_type(type , size , allocate_type) (type*)allocate_memory(sizeof(type)*(size) , allocate_type)

#define buffer_full(buffer) ((buffer).count == (buffer).capacity)

#define allocate_buffer_EX( buffer_pointer , buffer_data_size , new_capacity , allocate_type) \
allocate_buffer_raw( (void**)&((buffer_pointer)->data) , &((buffer_pointer)->data_size) , &((buffer_pointer)->count) , &((buffer_pointer)->capacity) , buffer_data_size , new_capacity , allocate_type);

#define allocate_buffer( buffer_pointer , data_type , new_capacity , allocate_type) \
{data_type * type_check = (buffer_pointer)->data;} allocate_buffer_EX(buffer_pointer , sizeof(data_type) , new_capacity , allocate_type);

internal void function allocate_buffer_raw( void ** buffer_data, int * buffer_data_size , int * buffer_count , int * buffer_capacity ,  int  data_size , int capacity , int  allocate_type)
{
    (*buffer_data_size) = data_size;
    (*buffer_count) = 0;
    (*buffer_capacity) = capacity;
    (*buffer_data) = allocate_memory(data_size * capacity , allocate_type);
}

#define reallocate_buffer_EX(buffer_pointer , new_capacity , allocate_type) _reallocate_buffer_EX( (void **)&((buffer_pointer)->data) , new_capacity , &((buffer_pointer)->capacity) , (buffer_pointer)->data_size , allocate_type);

internal void function _reallocate_buffer_EX(void ** data , int new_capacity , int * buffer_capacity , int buffer_data_size , int allocate_type)
{
    unsigned char * new_buffer = allocate_memory( buffer_data_size * new_capacity , allocate_type);
    memcpy( new_buffer, (*data) , (*buffer_capacity) * buffer_data_size );
    (*buffer_capacity) = new_capacity;
    (*data)= new_buffer;
}

#define reallocate_buffer(buffer_pointer , allocate_type) _reallocate_buffer( (void **)&((buffer_pointer)->data) , &((buffer_pointer)->capacity) , (buffer_pointer)->data_size , allocate_type);

internal void function _reallocate_buffer( void ** data , int * buffer_capacity , int buffer_data_size , int allocate_type)
{
    int new_capacity = (*buffer_capacity) * 2;
    if(new_capacity == 0) CATCH;
    
    _reallocate_buffer_EX(data , new_capacity , buffer_capacity , buffer_data_size , allocate_type);
}

internal List function allocate_list(int capacity , int allocate_type)
{
    ListNode * all_node = (ListNode *)allocate_memory(sizeof(ListNode) * (capacity + DUMMY_NODE_COUNT) , allocate_type);
    ListNode * node_array = all_node + DUMMY_NODE_COUNT;
    
    List list = {};
    
    node_array[N_node_head].next = N_node_tail;
    node_array[N_node_head].previous = invalid_node;
    node_array[N_node_head].data_index = -1;
    
    node_array[N_node_tail].previous = N_node_head;
    node_array[N_node_tail].next = invalid_node;
    node_array[N_node_tail].data_index = -1;
    
    node_array[N_recycled_node_head].next = N_recycled_node_tail;
    node_array[N_recycled_node_head].previous = invalid_node;
    node_array[N_recycled_node_head].data_index = -1;
    
    node_array[N_recycled_node_tail].previous = N_recycled_node_head;
    node_array[N_recycled_node_tail].next = invalid_node;
    node_array[N_recycled_node_tail].data_index = -1;
    
    list.all_node = all_node;
    list.node_array = node_array;
    list.unuse_index = 0;
    list.capacity = capacity;
    
    return list;
}

internal void function reallocate_list(List * list , int allocate_type)
{
    int new_capacity = list->capacity * 2;
    if(new_capacity == 0) CATCH;
    
    List new_list = allocate_list(new_capacity , allocate_type);
    
    for(int node_index = 0 ; node_index < list->capacity + DUMMY_NODE_COUNT ; node_index++)
    {
        new_list.all_node[node_index] = list->all_node[node_index];
    }
    
    new_list.unuse_index = list->unuse_index;
    new_list.capacity = new_capacity;
    
    (*list) = new_list;
}

internal Array function allocate_array(int capacity , int allocate_type)
{
    Array array = {};
    array.valid_array = allocate_memory(capacity , allocate_type);
    array.capacity = capacity;
    
    return array;
}

internal void function reallocate_array_EX(Array * array , int new_capacity , int allocate_type)
{
    Array new_array = allocate_array(new_capacity , allocate_type);
    for(int index = 0 ; index < array->capacity ; index++)
    {
        new_array.valid_array[index] = array->valid_array[index];
    }
    new_array.capacity = new_capacity;
    new_array.upper_bound = array->upper_bound;
    new_array.lowest_index = array->lowest_index;
    new_array.count = array->count;
    
    (*array) = new_array;
}

internal void function reallocate_array(Array * array , int allocate_type)
{
    int new_capacity = array->capacity * 2;
    if(new_capacity == 0) CATCH;
    
    reallocate_array_EX(array , new_capacity , allocate_type);
}

//steal from somewhere
internal unsigned int function string_to_hash_W(wchar_t *s)
{
    unsigned int hash_value = 0;
    
    for( ; *s; ++s)
    {
        hash_value += *s;
        hash_value += (hash_value << 10);
        hash_value ^= (hash_value >> 6);
    }
    
    hash_value += (hash_value << 3);
    hash_value ^= (hash_value >> 11);
    hash_value += (hash_value << 15);
    
    return hash_value;
}

//steal from somewhere
internal int function hash_int(int key)
{
    key = ~key + (key << 15); // key = (key << 15) - key - 1;
    key = key ^ (key >> 12);
    key = key + (key << 2);
    key = key ^ (key >> 4);
    key = key * 2057; // key = (key + (key << 3)) + (key << 11);
    key = key ^ (key >> 16);
    return key;
}

#define SlotToData(Slot , HashTable) (HashTable.DataArray + (Slot - HashTable.HashSlotArray))
#define DataToSlot(Data , HashTable) ( HashTable.HashSlotArray (Data - HashTable.DataArray))

internal HashTable function allocate_hash_table(int capacity , int allocate_type)
{
    HashTable hash_table = {};
    hash_table.entry_array = (HashTableEntry *)allocate_memory(sizeof(HashTableEntry) * capacity , allocate_type);
    hash_table.slot_array = (HashTableSlot *)allocate_memory(sizeof(HashTableSlot) * capacity , allocate_type);
    hash_table.capacity = capacity;
    
    for (int entry_index = 0; entry_index < capacity; entry_index++)
    {
        hash_table.entry_array[entry_index].head_index = -1;
        hash_table.entry_array[entry_index].tail_index = -1;
    }
    
    return hash_table;
    
}

internal bool function compare_string( char * string_A, char * string_B)
{
    int string_B_length = 0;
    for(int char_index = 0 ; string_B[char_index] != '\0' ; char_index++ ,string_B_length++);
    
    int name_length = 0;
    for(int char_index = 0 ; string_A[char_index] != '\0' ; char_index++ , name_length++);
    
    if(name_length != string_B_length) return false;
    
    for(int char_index = 0 ; char_index < name_length ; char_index++)
    {
        if(string_A[char_index] != string_B[char_index])
        {
            return false;
        }
    }
    
    return true;
}

internal wchar_t * function combine_string_W(wchar_t * string_a , wchar_t * string_b)
{
    int string_size= wcslen(string_a) + wcslen(string_b) + 2;//adding 2 just to be save
    wchar_t * temp_string = allocate_memory_type(wchar_t , string_size , AT_frame);
    
	wcscpy(temp_string , string_a);
	wcscat(temp_string , string_b);
    
	return temp_string;
}

#define READ_BUFFER(data_to_assign , name , type , index) \
{\
local_persist bool initialized = false;\
local_persist type * buffer = 0;\
if(!initialized) {initialized = true; buffer = (type *)get_data_buffer_by_name(name);}\
if(buffer) (data_to_assign) = buffer[index];\
}

#define READ_DATA(data , name , type) { type * data_pointer = (type *)get_data_buffer_by_name(name) ; if(data_pointer) data = (*data_pointer); }

internal unsigned char * function get_data_buffer_by_name(char * name)
{
    for(int header_index = 0 ; header_index < save_header_count ; header_index++)
    {
        DataHeader * current_header = data_header_array + header_index;
        
        if(compare_string(current_header->name.string , name))
        {
            return save_memory + current_header->data_offset;
        }
        
    }
    
    return 0;
}

//this is kinda dumb
internal void function combine_file_path(const char* file_name,char * result_path)
{
	strcat(result_path, app_data->application_path);
	strcat(result_path, file_name);
}

internal char * function combine_string(char * string_a , char * string_b)
{
	int string_size= strlen(string_a) + strlen(string_b) + 2;//adding 2 just to be save
	char * temp_string = allocate_memory_type(char , string_size , AT_frame);
    
	strcpy(temp_string , string_a);
	strcat(temp_string , string_b);
    
	return temp_string;
}

internal char * function get_app_file_path(const char* file_name)
{
	return combine_string(app_data->application_path , (char*) file_name);
}

internal char * function get_level_file_path(const char * level_name)
{
	return combine_string(combine_string(app_data->application_path , "GameLevel\\") , (char*)level_name);
}

internal char * function get_level_file_path_B()
{
	return combine_string(app_data->application_path , "GameLevel\\");
}

//this look bad
//this is really really bad

internal int function get_emty_slot_index_from_hash_table(int hash_value , HashTable * hash_table)
{
    
    HashTableSlot * slot_array = hash_table->slot_array;
    HashTableEntry * entry_array = hash_table->entry_array;
    
    HashTableEntry * entry = entry_array + (hash_value % hash_table->capacity);
    
    HashTableSlot * emty_slot = 0;
    
    bool started_from_chain = false;
    
    if (entry->head_index == -1)
    {
        emty_slot = slot_array + (hash_value % hash_table->capacity);
    }
    else
    {
        emty_slot = slot_array + entry->tail_index;
    }
    
    if(emty_slot != (slot_array + entry->head_index) ) started_from_chain = true;
    
    for ( ; emty_slot->valid ; )
    {
        
        if (emty_slot == (slot_array + entry->head_index))
        {
            if (started_from_chain)
            {
                CATCH;
            }
            else
            {
                started_from_chain = true;
            }
            
        }
        
        emty_slot++;
        if (emty_slot >= (slot_array + hash_table->capacity)) emty_slot = slot_array;
    }
    
    int emty_slot_index = (emty_slot - slot_array);
    
    return emty_slot_index;
}

internal int function get_hash_table_head_slot_index(int hash_value ,  HashTable * hash_table)
{
    HashTableEntry * entry_array = hash_table->entry_array;
    
    HashTableEntry * entry = entry_array + (hash_value % hash_table->capacity);
    return entry->head_index;
}

internal int function get_data_index_from_slot_index(int slot_index , HashTable * hash_table)
{
    return hash_table->slot_array[slot_index].data_index;
}

internal int function insert_to_hash_table(int slot_index_to_insert_after , int hash_value , int data_index , HashTable * hash_table)
{
    
    HashTableSlot * slot_array = ( HashTableSlot *)hash_table->slot_array;
    HashTableEntry * entry_array = ( HashTableEntry *)hash_table->entry_array;
    
    HashTableEntry * entry = entry_array + (hash_value % hash_table->capacity);
    
    int emty_slot_index = get_emty_slot_index_from_hash_table( hash_value , hash_table);
    HashTableSlot * emty_slot = slot_array + emty_slot_index;
    
    if (entry->head_index == -1)
    {
        entry->head_index = emty_slot_index;
        entry->tail_index = emty_slot_index;
    }
    
    emty_slot->valid = true;
    emty_slot->slot_value = (hash_value % hash_table->capacity);
    emty_slot->data_index = data_index;
    emty_slot->next_index = -1;
    emty_slot->previous_index = -1;
    
    if(slot_index_to_insert_after != -1)
    {
        
        HashTableSlot * slot_to_insert_after = slot_array + slot_index_to_insert_after;
        
        if(slot_to_insert_after->next_index != -1)
        {
            HashTableSlot * next_slot = slot_array + slot_to_insert_after->next_index;
            next_slot->previous_index = emty_slot_index;
        }
        
        emty_slot->next_index = slot_to_insert_after->next_index;
        slot_to_insert_after->next_index = emty_slot_index;
        emty_slot->previous_index = slot_index_to_insert_after;
        
        if(slot_index_to_insert_after == entry->tail_index)
        {
            entry->tail_index = emty_slot_index;
        }
    }
    
    return emty_slot_index;
}

internal int function add_to_hash_table(int hash_value , int data_index , HashTable * hash_table)
{
    
    HashTableSlot * slot_array = hash_table->slot_array;
    HashTableEntry * entry_array = hash_table->entry_array;
    
    int entry_index = (hash_value % hash_table->capacity);
    if(entry_index < 0) CATCH;
    HashTableEntry * entry = entry_array + entry_index;
    
    int emty_slot_index = get_emty_slot_index_from_hash_table( hash_value , hash_table);
    HashTableSlot * emty_slot = slot_array + emty_slot_index;
    
    if (entry->head_index == -1)
    {
        entry->head_index = emty_slot_index;
    }
    
    if(entry->tail_index != -1)
    {
        slot_array[entry->tail_index].next_index = emty_slot_index;
    }
    
    emty_slot->previous_index = entry->tail_index;
    entry->tail_index = emty_slot_index;
    
    emty_slot->valid = true;
    emty_slot->slot_value = (hash_value % hash_table->capacity);
    emty_slot->data_index = data_index;
    emty_slot->next_index = -1;
    
    return emty_slot_index;
}

internal void function clear_hash_table(HashTable * hash_table)
{
    HashTableEntry * entry_array = hash_table->entry_array;
    HashTableSlot * slot_array = hash_table->slot_array;
    
    for(int slot_index = 0 ; slot_index < hash_table->capacity ; slot_index++)
    {
        
        entry_array[slot_index].head_index = -1;
        entry_array[slot_index].tail_index = -1;
        
        slot_array[slot_index].valid = false;
        slot_array[slot_index].slot_value = -1;
        slot_array[slot_index].data_index = -1;
        slot_array[slot_index].next_index = -1;
        slot_array[slot_index].previous_index = -1;
        
    }
    
}

internal bool function delete_from_hash_table_by_slot_index(int slot_index , int hash_value , HashTable * hash_table)
{
    int slot_value = (hash_value % hash_table->capacity);
    
    HashTableSlot * slot_array = hash_table->slot_array;
    HashTableSlot * slot_chain = slot_array + slot_value;
    
    HashTableEntry * entry_array = hash_table->entry_array;
    HashTableEntry * entry = entry_array + slot_value;
    
    HashTableSlot * current_slot = slot_array + slot_index;
    
    if(current_slot->slot_value != slot_value ) return false;
    
    if (current_slot->previous_index != -1)
    {
        slot_array[current_slot->previous_index].next_index = current_slot->next_index;
    }
    
    if (current_slot->next_index != -1)
    {
        slot_array[current_slot->next_index].previous_index = current_slot->previous_index;
    }
    
    if (entry->head_index == slot_index)
    {
        entry->head_index = current_slot->next_index;
    }
    
    if (entry->tail_index == slot_index)
    {
        entry->tail_index = current_slot->previous_index;
    }
    
    current_slot->valid = false;
    current_slot->slot_value = -1;
    current_slot->data_index = -1;
    current_slot->next_index = -1;
    current_slot->previous_index = -1;
    
    return true;
    
}

//insane shit
internal bool function delete_from_hash_table(int hash_value , int data_index , HashTable * hash_table)
{
    HashTableSlot * slot_array = hash_table->slot_array;
    HashTableSlot * slot_chain = slot_array + (hash_value % hash_table->capacity);
    
    HashTableEntry * entry_array = hash_table->entry_array;
    HashTableEntry * entry = entry_array + ( hash_value % hash_table->capacity );
    
    if (!entry->head_index == -1) return false;
    
    int slot_index = entry->head_index;
    for (; slot_index != -1 ; slot_index = slot_array[slot_index].next_index)
    {
        HashTableSlot * current_slot = slot_array + slot_index;
        
        if (current_slot->data_index == data_index)
        {
            return delete_from_hash_table_by_slot_index(slot_index , hash_value , hash_table);
        }
    }
    
    return false;
}

internal int function get_hash_table_tail_data_index(int hash_value , HashTable * hash_table)
{
    int tail_slot_index = hash_table->entry_array[hash_value % hash_table->capacity].tail_index;
    
    if(tail_slot_index == -1) return -1;
    return hash_table->slot_array[tail_slot_index].data_index;
}

#define hash_table_iterate_ex(DataIndex , SlotIndex , StartSlotIndex , HashValue , hash_tableVar) \
for \
(\
int SlotIndex = StartSlotIndex , DataIndex = -1; \
_iterate_hash_table((HashValue),  &SlotIndex , &DataIndex , hash_tableVar) ,SlotIndex != -1 ;\
)

#define hash_table_iterate(DataIndex , HashValue , hash_tableVar) hash_table_iterate_ex(DataIndex , SlotIndex , -1 , HashValue , hash_tableVar)

#define HASH_DEBUG 0

internal void function _iterate_hash_table( int hash_value , int * slot_index , int * data_index, HashTable * hash_table)
{
    
#if HASH_DEBUG
    
    printf("(Iterating : %d)(CurrentSlot : %d)\n" , (hash_value%count) , (*slot_index));
    _print_hash_table(hash_table->SlotArray , count);
    
#endif
    
    HashTableSlot * slot_array = hash_table->slot_array;
    HashTableSlot * slot_chain_head = (slot_array + ( hash_value % hash_table->capacity ));
    HashTableEntry * entry_array = hash_table->entry_array;
    HashTableEntry * entry = entry_array + ( hash_value % hash_table->capacity );
    
    if ( (*slot_index) == -1)
    {
        (*slot_index) = entry->head_index;
        (*data_index) = slot_array[entry->head_index].data_index;
        return;
    }
    
    HashTableSlot * slot = slot_array + (*slot_index);
    
    (*slot_index) = slot->next_index;
    (*data_index) = slot_array[slot->next_index].data_index;
}

#define hash_table_iterate_reverse_ex(DataIndex , SlotIndex , StartSlotIndex , HashValue , hash_tableVar)\
for \
(\
int SlotIndex = StartSlotIndex , DataIndex = -1; \
_iterate_hash_table_reverse((HashValue),  &SlotIndex , &DataIndex , hash_tableVar) ,SlotIndex != -1 ;\
)

#define hash_table_iterate_reverse(DataIndex , HashValue , hash_tableVar) hash_table_iterate_reverse_ex(DataIndex , SlotIndex , -1 , HashValue , hash_tableVar)

internal void function _iterate_hash_table_reverse( int hash_value , int * slot_index , int * data_index, HashTable * hash_table)
{
    
#if HASH_DEBUG
    
    printf("(Iterating : %d)(CurrentSlot : %d)\n" , (hash_value%count) , (*slot_index));
    _print_hash_table(hash_table->SlotArray , count);
    
#endif
    
    int slot_value = ( hash_value % hash_table->capacity );
    
    HashTableSlot * slot_array = hash_table->slot_array;
    
    HashTableEntry * entry_array = hash_table->entry_array;
    HashTableEntry * entry = entry_array + slot_value;
    
    if ( (*slot_index) == -1)
    {
        (*slot_index) = entry->tail_index;
        (*data_index) = slot_array[entry->tail_index].data_index;
        return;
    }
    
    HashTableSlot * slot = slot_array + (*slot_index);
    
    if(slot->slot_value != slot_value) CATCH;
    
    (*slot_index) = slot->previous_index;
    (*data_index) = slot_array[slot->previous_index].data_index;
    
}

#define list_foreach_reverse_EX(DataIndexVar , start_node_index , node_index , List) \
for\
( \
int node_index = (List)->node_array[start_node_index].previous , DataIndexVar = (List)->node_array[node_index].data_index ;\
DataIndexVar != -1 ; \
node_index = (List)->node_array[node_index].previous , DataIndexVar = (List)->node_array[node_index].data_index\
)

#define list_foreach_reverse(DataIndex , List) list_foreach_reverse_EX(DataIndex , N_NodeTail , node_index , List)

#define list_foreach_EX(DataIndexVar , start_node_index , node_index , List) \
for\
( \
int node_index = (List)->node_array[start_node_index].next , DataIndexVar = (List)->node_array[node_index].data_index ;\
DataIndexVar != -1 ; \
node_index = (List)->node_array[node_index].next , DataIndexVar = (List)->node_array[node_index].data_index\
)

#define list_foreach(DataIndex , List) list_foreach_EX(DataIndex ,N_node_head , node_index , List)

internal void function clear_list(List * list)
{
    
    list->unuse_index = 0;
    list->node_array[N_node_head].next = N_node_tail;
    list->node_array[N_node_tail].previous = N_node_head;
    list->node_array[N_recycled_node_head].next = N_recycled_node_head;
    list->node_array[N_recycled_node_tail].previous = N_recycled_node_tail;
    
}

//TODO: this doesn't feel good to use
//i can try a worse version, turn all these function into macro and see how it goes
//there is one i found online with 1000 lines of code, seem bad as well
internal bool function list_full(List * list)
{
    ListNode * node_array = list->node_array;
    int recycled_node_index = node_array[N_recycled_node_head].next;
    
    if (node_array[recycled_node_index].data_index == -1)
    {
        if (list->unuse_index > list->capacity) CATCH;
        if (list->unuse_index == list->capacity) return true;
    }
    
    return false;
}

internal int function create_new_node( List * list)
{
    int new_node_index = invalid_node;
    ListNode * node_array = list->node_array;
    
    int recycled_node_index = node_array[N_recycled_node_head].next;
    ListNode * recycled_node = node_array + recycled_node_index;
    
    if (recycled_node->data_index != -1)
    {
        new_node_index = recycled_node_index;
        
        node_array[N_recycled_node_head].next = recycled_node->next;
        node_array[recycled_node->next].previous = recycled_node->previous;
        
    }
    else
    {
        if (list->unuse_index >= list->capacity) CATCH;
        
        new_node_index = list->unuse_index++;
        
    }
    
    return new_node_index;
}

internal int function get_next_node_index(int node_index , List * list)
{
    int result =list->node_array[node_index].next;
    
    if(list->node_array[result].data_index == -1) return invalid_node;
    
    return result;
}

internal int function get_head_node_index(List * list)
{
    return list->node_array[N_node_head].next;
}

internal int function get_tail_node_index(List * list)
{
    return list->node_array[N_node_tail].next;
}

internal int function get_previous_node_index(int node_index , List * list)
{
    int result = list->node_array[node_index].previous;
    
    if(list->node_array[result].data_index == -1) return invalid_node;
    
    return result;
}

internal int function get_data_index_from_node_index(int node_index , List * list)
{
    if(node_index == invalid_node ) return -1;
    return list->node_array[node_index].data_index;
}

internal int function get_previous_data_index_from_node_index(int node_index , List * list)
{
    return get_data_index_from_node_index(get_previous_node_index(node_index, list) , list);
}

internal int function get_next_data_index_from_node_index(int node_index , List * list)
{
    return get_data_index_from_node_index(get_next_node_index(node_index, list) , list);
}

internal void function insert_to_list(bool connect_before_node , int node_to_connect_index , int node_index , int data_index , List * list)
{
    if(node_index == invalid_node) return;
    
    if(!list->node_array) CATCH;
    
    ListNode * node_array = list->node_array;
    
    ListNode * node = node_array + node_index;
    node->data_index = data_index;
    
    ListNode * node_to_connect = node_array + node_to_connect_index;
    
    if(connect_before_node)
    {
        ListNode * previous_node = node_array +node_to_connect->previous;
        previous_node->next = node_index;
        
        node->previous = node_to_connect->previous;
        node->next = node_to_connect_index;
        
        node_to_connect->previous = node_index;
    }
    else
    {
        
        ListNode * next_node = node_array + node_to_connect->next;
        next_node->previous = node_index;
        
        node->next = node_to_connect->next;
        node->previous = node_to_connect_index;
        
        node_to_connect->next = node_index;
    }
}

internal int function get_emty_node_from_list( List * list)
{
    int new_node_index = invalid_node;
    ListNode * node_array = list->node_array;
    
    int recycled_node_index = node_array[N_recycled_node_head].next;
    ListNode * recycled_node = node_array + recycled_node_index;
    
    if (recycled_node->data_index != -1)
    {
        new_node_index = recycled_node_index;
    }
    else
    {
        if (list->unuse_index >= list->capacity) return invalid_node;
        new_node_index = list->unuse_index;
    }
    
    return new_node_index;
}

internal int function create_and_insert_to_list(bool connect_before_node , int node_to_connect_index , int data_index , List * list)
{
    int new_node_index = create_new_node(list);
    if(new_node_index == -1) CATCH;
    insert_to_list( connect_before_node ,  node_to_connect_index , new_node_index , data_index , list );
    
    return new_node_index;
}

internal int function add_to_list_head(int data_index , List * list)
{
    return create_and_insert_to_list(false , N_node_head , data_index , list);
}

internal int function add_to_list_tail( int data_index , List * list)
{
    return create_and_insert_to_list(true , N_node_tail , data_index , list);
}

//feel really weird
internal int function add_to_list_tail_B(List * list)
{
    return add_to_list_tail(get_emty_node_from_list(list) , list);
}

internal bool function delete_from_list(int node_index_to_delete , List * list)
{
    if(node_index_to_delete == invalid_node) return false;
    
    ListNode * node_array = list->node_array;
    
    ListNode * node_to_delete =  node_array + node_index_to_delete;
    if (node_to_delete->data_index == -1) return false;
    
    node_array[node_to_delete->previous].next = node_to_delete->next;
    node_array[node_to_delete->next].previous = node_to_delete->previous;
    
    insert_to_list(true , N_recycled_node_tail , node_index_to_delete , node_to_delete->data_index , list);
    
    return true;
}

internal void function clear_array(Array * array)
{
	for (int array_index = 0; array_index < array->upper_bound; array_index++)
	{
		array->valid_array[array_index] = false;
	}
    
    array->count = 0;
	array->upper_bound = 0;
	array->lowest_index = 0;
}

internal void function recheck_array(Array * array)
{
	array->lowest_index = 0;
	array->upper_bound = 0;
	array->count = 0;
    
    for( ; array->valid_array[array->lowest_index] ;array->lowest_index++);
    
	for (int i = 0; i < array->capacity; i++)
	{
		if (array->valid_array[i])
		{
			array->count++;
            array->upper_bound = i + 1;
		}
	}
}

internal int function add_to_array(Array * array)
{
    if(array->lowest_index >= array->capacity)
    {
        array->lowest_index = array->capacity;
        return -1;
    }
    
	array->valid_array[array->lowest_index] = true;
    int data_index = array->lowest_index;
    array->count++;
    
	for (;;)
    {
        if(array->lowest_index >= array->capacity)
        {
            array->lowest_index = array->capacity;
            break;
        }
        
        if(!array->valid_array[array->lowest_index])
        {
            break;
        }
        
        array->lowest_index++;
    }
    
	if (array->upper_bound < array->lowest_index)
		array->upper_bound = array->lowest_index;
    
	if (array->upper_bound > array->capacity) CATCH;
    
	return data_index;
}

internal bool function delete_from_array(Array * array , int data_index)
{
	if (data_index == -1)
		return false;
    
    if(data_index >= array->upper_bound) CATCH;
    
	array->valid_array[data_index] = false;
    
	if (array->lowest_index > data_index)
		array->lowest_index = data_index;
    
	int test_index = 0;
	for( ; array->valid_array[test_index] ;test_index++);
	if (test_index != array->lowest_index) CATCH;
	
	for ( ; (!array->valid_array[array->upper_bound - 1] ) && (array->upper_bound  > 0 );array->upper_bound--);
    
    array->count--;
	return true;
}

internal bool function array_full(Array * array)
{
    return (array->lowest_index == array->capacity);
}

internal bool function iterate_array(int * data_index , Array * array)
{
    if((*data_index) >= array->upper_bound) return false;
    
    for(;;)
    {
        if(array->valid_array[(*data_index)]) return true;
        
        (*data_index)++;
        if((*data_index) >= array->upper_bound) return false;
    }
}

//this is much better
#define array_foreach(data_index , array) for(int data_index = 0 ; iterate_array(&data_index , (array)) ; data_index++)

#define array_foreach_B(data_index , iterate_index , array) for(int data_index = 0 , iterate_index = 0; iterate_array(&data_index , (array)) ; data_index++ , iterate_index++)

internal void function allocate_chunk_pool(ChunkPool * pool , int pool_size , int chunk_size , int allocate_type)
{
    //you can detele this line if you implmented reallocation of pool
    //but you shouldn't allocate pool from pool at the begining
    if(allocate_type == AT_pool) CATCH;
    pool->chunk_size = chunk_size;
    pool->pool_size = pool_size;
    allocate_buffer(&pool->buffer , Chunk , pool_size , allocate_type);
    pool->array = allocate_array(pool_size , allocate_type);
    pool->memory_start = allocate_memory(pool_size * chunk_size , allocate_type);
    pool->allocate_type = allocate_type;
}

internal void * function allocate_memory(int size , int allocate_type)
{
    unsigned char * memory = 0;
    
    if(allocate_type == AT_pool)
    {
        if(!current_pool_to_allocate) CATCH;
        Chunk * chunk = current_pool_to_allocate->buffer.data + (*current_chunk_index);
        ChunkPool * pool = current_pool_to_allocate;
        
        bool need_new_chunk = false;
        if(chunk->allocated_size + size > chunk->chunk_count * pool->chunk_size) need_new_chunk = true;
        if((*current_chunk_index) == -1) need_new_chunk = true;
        
        if(need_new_chunk)
        {
            int previous_chunk_index = (*current_chunk_index);
            if(previous_chunk_index >= pool->buffer.capacity) CATCH;
            if(previous_chunk_index < -1) CATCH;
            
            int require_chunk_count = 1;
            for(; require_chunk_count * pool->chunk_size < size; require_chunk_count++);
            
            bool space_found = false;
            bool head_found = false;
            int new_chunk_index = 0;
            
            for(;;)
            {
                bool * valid_array = pool->array.valid_array;
                for(int array_index = pool->array.lowest_index; array_index < pool->array.capacity; array_index++)
                {
                    if(valid_array[array_index])
                    {
                        head_found = false;
                    }
                    else
                    {
                        if(!head_found)
                        {
                            new_chunk_index = array_index;
                            head_found = true;
                        }
                        
                        if((new_chunk_index - array_index + 1) == require_chunk_count)
                        {
                            space_found = true;
                            break;
                        }
                    }
                }
                
                if(space_found)
                {
                    for(int array_index = new_chunk_index; array_index < new_chunk_index + require_chunk_count; array_index++)
                    {
                        if(valid_array[array_index]) CATCH;
                        valid_array[array_index] = true;
                    }
                    
                    recheck_array(&pool->array);
                    break;
                }
                else
                {
                    if(pool->allocate_type == AT_pool) CATCH;
                    pool->pool_size *= 2;
                    reallocate_buffer_EX(&pool->buffer , pool->pool_size , pool->allocate_type);
                    reallocate_array_EX(&pool->array , pool->pool_size , pool->allocate_type);
                }
            }
            
            Chunk * new_chunk = pool->buffer.data + new_chunk_index;
            new_chunk->allocated_size = 0;
            new_chunk->chunk_count = require_chunk_count;
            new_chunk->previous = previous_chunk_index;
            (*current_chunk_index) = new_chunk_index;
            
            chunk = current_pool_to_allocate->buffer.data + new_chunk_index;
        }
        
        int memory_offset = chunk->allocated_size;
        chunk->allocated_size += size;
        current_pool_to_allocate = 0;
        return (pool->memory_start + memory_offset + (*current_chunk_index) * pool->chunk_size);
    }
    else
    {
        D_Memory * game_memory = 0;
        
        switch(allocate_type)
        {
            case AT_temp: game_memory = &run_time_memory; break;
            case AT_frame: game_memory = &frame_time_memory; break;
            
            default: CATCH; break;
        }
        
        if(size < 0) CATCH;
        
        if (size == 0)
        {
            return 0;
        }
        
        unsigned char* start = game_memory->current_memory;
        game_memory->current_memory += size;
        if (game_memory->current_memory >= game_memory->start_memory + game_memory->size)
        {
            CATCH;
        }
        
        memset(start, 0, size);
        
        return start;
    }
}

internal Box function get_box()
{
    Box box = {};
    box.rotation = QuaternionIdentity();
    return box;
}

internal void function create_a_whole_new_world()
{
    reference_frame_list = allocate_list(16 , AT_temp);
    allocate_buffer(&reference_frame_buffer , Vector3 , 16 , AT_temp);
    
    allocate_buffer(&quad_in_map_buffer , Quad , 16 , AT_temp);
    quad_in_map_array = allocate_array(16 , AT_temp);
    
    allocate_buffer(&box_in_map_buffer , Box , 16 , AT_temp);
    box_in_map_array = allocate_array(16 , AT_temp);
    
    allocate_buffer(&camera_buffer , CameraTrigger , 16 , AT_temp);
    allocate_buffer(&camera_zone_buffer , Box , 16 , AT_temp);
    camera_array = allocate_array(16 , AT_temp);
    camera_within_list = allocate_list(16 , AT_temp);
    
    player_chunk_size = (sizeof(Player));
    allocate_chunk_pool(&player_pool , 16 , player_chunk_size , AT_temp);
    
    allocate_buffer(&entity_layout_buffer , Entity , 16 , AT_temp);
    entity_layout_array = allocate_array(16 , AT_temp);
    
    allocate_buffer(&entity_active_buffer , Entity , 16 , AT_temp);
    entity_active_array = allocate_array(16 , AT_temp);
    
    allocate_buffer(&client_player_buffer , Player , 16 , AT_temp);
    allocate_buffer(&client_entity_buffer , Entity , 16 , AT_temp);
}

internal Vector3 * function box_to_point(Box box)
{
    Vector3 * points = allocate_memory_type(Vector3 , box_vertex_count , AT_frame);
    
    points[bv_A] = (Vector3){ -box.size.x * 0.5f , box.size.y * 0.5f , box.size.z * 0.5f};
    points[bv_B] = (Vector3){ box.size.x * 0.5f , box.size.y * 0.5f , box.size.z * 0.5f};
    points[bv_C] = (Vector3){ -box.size.x * 0.5f , box.size.y * 0.5f , -box.size.z * 0.5f};
    points[bv_D] = (Vector3){ box.size.x * 0.5f , box.size.y * 0.5f , -box.size.z * 0.5f};
    points[bv_E] = (Vector3){ -box.size.x * 0.5f , -box.size.y * 0.5f , box.size.z * 0.5f};
    points[bv_F] = (Vector3){ box.size.x * 0.5f , -box.size.y * 0.5f , box.size.z * 0.5f};
    points[bv_G] = (Vector3){ -box.size.x * 0.5f , -box.size.y * 0.5f , -box.size.z * 0.5f};
    points[bv_H] = (Vector3){ box.size.x * 0.5f , -box.size.y * 0.5f , -box.size.z * 0.5f};
    
    Vector3 right_tilt = {0 , box.right_top , box.right_front};
    Vector3 top_tilt = {box.top_right , 0 , box.top_front};
    Vector3 front_tilt = {box.front_right , box.front_top , 0};
    
    points[bv_B] = Vector3Add(points[bv_B] , right_tilt);
    points[bv_D] = Vector3Add(points[bv_D] , right_tilt);
    points[bv_F] = Vector3Add(points[bv_F] , right_tilt);
    points[bv_H] = Vector3Add(points[bv_H] , right_tilt);
    
    points[bv_A] = Vector3Subtract(points[bv_A] , right_tilt);
    points[bv_C] = Vector3Subtract(points[bv_C] , right_tilt);
    points[bv_E] = Vector3Subtract(points[bv_E] , right_tilt);
    points[bv_G] = Vector3Subtract(points[bv_G] , right_tilt);
    
    points[bv_A] = Vector3Add(points[bv_A] , top_tilt);
    points[bv_B] = Vector3Add(points[bv_B] , top_tilt);
    points[bv_C] = Vector3Add(points[bv_C] , top_tilt);
    points[bv_D] = Vector3Add(points[bv_D] , top_tilt);
    
    points[bv_E] = Vector3Subtract(points[bv_E] , top_tilt);
    points[bv_F] = Vector3Subtract(points[bv_F] , top_tilt);
    points[bv_G] = Vector3Subtract(points[bv_G] , top_tilt);
    points[bv_H] = Vector3Subtract(points[bv_H] , top_tilt);
    
    points[bv_A] = Vector3Add(points[bv_A] , front_tilt);
    points[bv_B] = Vector3Add(points[bv_B] , front_tilt);
    points[bv_E] = Vector3Add(points[bv_E] , front_tilt);
    points[bv_F] = Vector3Add(points[bv_F] , front_tilt);
    
    points[bv_C] = Vector3Subtract(points[bv_C] , front_tilt);
    points[bv_D] = Vector3Subtract(points[bv_D] , front_tilt);
    points[bv_G] = Vector3Subtract(points[bv_G] , front_tilt);
    points[bv_H] = Vector3Subtract(points[bv_H] , front_tilt);
    
    for(int point_index = 0 ; point_index < box_vertex_count ; point_index++)
    {
        points[point_index] = Vector3RotateByQuaternion(points[point_index] , box.rotation);
        points[point_index] = Vector3Add(points[point_index] , box.position);
    }
    
    return points;
}

internal float function get_line_intersect_with_plane_time(Vector3 start , Vector3 end , Vector3 plane_normal , Vector3 plane_origin)
{
	float result = plane_normal.x * (plane_origin.x - start.x) + plane_normal.y * (plane_origin.y - start.y) + plane_normal.z * (plane_origin.z - start.z);
	result /= plane_normal.x * (end.x - start.x) + plane_normal.y * (end.y - start.y) + plane_normal.z * (end.z - start.z);
    
	return result;
}

internal void function get_box_face(Quad * temp_box_quad , Box box)
{
    Vector3 * box_points = box_to_point(box);
    
    Quad * top = temp_box_quad + face_top;
    Quad * bottom = temp_box_quad + face_bottom;
    Quad * right = temp_box_quad + face_right;
    Quad * left = temp_box_quad + face_left;
    Quad * front = temp_box_quad + face_front;
    Quad * back = temp_box_quad + face_back;
    
    top->vertex_position[vertex_top_right] = box_points[bv_B];
    top->vertex_position[vertex_top_left] = box_points[bv_A];
    top->vertex_position[vertex_bottom_right] = box_points[bv_D];
    top->vertex_position[vertex_bottom_left] = box_points[bv_C];
    
    bottom->vertex_position[vertex_top_right] = box_points[bv_H];
    bottom->vertex_position[vertex_top_left] = box_points[bv_G];
    bottom->vertex_position[vertex_bottom_right] = box_points[bv_F];
    bottom->vertex_position[vertex_bottom_left] = box_points[bv_E];
    
    right->vertex_position[vertex_top_right] = box_points[bv_B];
    right->vertex_position[vertex_top_left] = box_points[bv_D];
    right->vertex_position[vertex_bottom_right] = box_points[bv_F];
    right->vertex_position[vertex_bottom_left] = box_points[bv_H];
    
    left->vertex_position[vertex_top_right] = box_points[bv_C];
    left->vertex_position[vertex_top_left] = box_points[bv_A];
    left->vertex_position[vertex_bottom_right] = box_points[bv_G];
    left->vertex_position[vertex_bottom_left] = box_points[bv_E];
    
    front->vertex_position[vertex_top_right] = box_points[bv_A];
    front->vertex_position[vertex_top_left] = box_points[bv_B];
    front->vertex_position[vertex_bottom_right] = box_points[bv_E];
    front->vertex_position[vertex_bottom_left] = box_points[bv_F];
    
    back->vertex_position[vertex_top_right] = box_points[bv_G];
    back->vertex_position[vertex_top_left] = box_points[bv_H];
    back->vertex_position[vertex_bottom_right] = box_points[bv_C];
    back->vertex_position[vertex_bottom_left] = box_points[bv_D];
}

internal bool function box_collision_ray(Vector3 origin , Vector3 ray_direction , Box box , int * hit_face , float * hit_time , Vector3 * hit_normal)
{
    Quad temp_box_quad[face_count] = {};
    get_box_face(temp_box_quad , box);
    
    float closest_hit_time = FLT_MAX;
    int closest_hit_face;
    Vector3 closest_hit_normal = {};
    bool hited = false;
    
    for(int face_index = 0; face_index < face_count; face_index++)
    {
        Quad quad = temp_box_quad[face_index];
        Vector3 a = quad.vertex_position[vertex_top_left];
        Vector3 b = quad.vertex_position[vertex_top_right];
        Vector3 c = quad.vertex_position[vertex_bottom_left];
        Vector3 d = quad.vertex_position[vertex_bottom_right];
        
        Vector3 a_to_b = Vector3Subtract(b , a);
        Vector3 a_to_c = Vector3Subtract(c , a);
        Vector3 d_to_b = Vector3Subtract(b , d);
        Vector3 d_to_c = Vector3Subtract(c , d);
        
        Vector3 face_normal = Vector3CrossProduct(a_to_b , a_to_c);
        float current_hit_time = get_line_intersect_with_plane_time(origin , Vector3Add(origin , ray_direction) , face_normal , a);
        if(current_hit_time > 0)
        {
            Vector3 hit_point = Vector3Add(origin , Vector3Scale(ray_direction , current_hit_time));
            
            Vector3 a_to_hit_point = Vector3Subtract(hit_point , a);
            Vector3 d_to_hit_point = Vector3Subtract(hit_point , d);
            
            if(Vector3DotProduct(Vector3CrossProduct(a_to_c , a_to_hit_point) , ray_direction) > 0)
            {
                if(Vector3DotProduct(Vector3CrossProduct(a_to_hit_point , a_to_b) , ray_direction) > 0)
                {
                    if(Vector3DotProduct(Vector3CrossProduct(d_to_hit_point , d_to_c) , ray_direction) > 0)
                    {
                        if(Vector3DotProduct(Vector3CrossProduct(d_to_b , d_to_hit_point) , ray_direction) > 0)
                        {
                            if(closest_hit_time > current_hit_time)
                            {
                                hited = true;
                                closest_hit_time = current_hit_time;
                                closest_hit_face = face_index;
                                closest_hit_normal = face_normal;
                            }
                        }
                    }
                }
            }
        }
    }
    
    if(hited)
    {
        if(hit_face) (*hit_face) = closest_hit_face;
        if(hit_time) (*hit_time) = closest_hit_time;
        if(hit_normal) (*hit_normal) = closest_hit_normal;
    }
    
    return hited;
}

internal bool function _box_collision_ray(Vector3 origin , Vector3 direction, Box box , int * hit_face , float * hit_time)
{
    Vector3 * points = box_to_point(box);
    
    Vector3 A_to_B = Vector3Subtract(points[bv_B] , points[bv_A]);
    Vector3 A_to_C = Vector3Subtract(points[bv_C] , points[bv_A]);
    Vector3 A_to_E = Vector3Subtract(points[bv_E] , points[bv_A]);
    
    Vector3 H_to_G = Vector3Subtract(points[bv_G] , points[bv_H]);
    Vector3 H_to_F = Vector3Subtract(points[bv_F] , points[bv_H]);
    Vector3 H_to_D = Vector3Subtract(points[bv_D] , points[bv_H]);
    
    Vector3 bottom_face_direction = Vector3CrossProduct(A_to_B , A_to_C);
    Vector3 backward_face_direction = Vector3CrossProduct(A_to_B , A_to_E);
    Vector3 right_face_direction = Vector3CrossProduct(A_to_C , A_to_E);
    
    Vector3 top_face_direction = Vector3Negate(bottom_face_direction);
    Vector3 forward_face_direction = Vector3Negate(backward_face_direction);
    Vector3 left_face_direction = Vector3Negate(right_face_direction);
    
    int face_to_check[3] = {};
    int face_to_check_count = 0;
    
    //TODO: why some of the vector inverted?
    //not gonna fix this now, probably just me messing up the cross product
    if(Vector3DotProduct(top_face_direction , direction) < 0) face_to_check[face_to_check_count++] = face_top;
    if(Vector3DotProduct(bottom_face_direction , direction) < 0) face_to_check[face_to_check_count++] = face_bottom;
    if(Vector3DotProduct(right_face_direction , direction) < 0) face_to_check[face_to_check_count++] = face_right;
    if(Vector3DotProduct(left_face_direction , direction) < 0) face_to_check[face_to_check_count++] = face_left;
    if(Vector3DotProduct(forward_face_direction , direction) > 0) face_to_check[face_to_check_count++] = face_front;
    if(Vector3DotProduct(backward_face_direction , direction) > 0) face_to_check[face_to_check_count++] = face_back;
    
    bool result = false;
    float closest_hit_time = FLT_MAX;
    
    for(int face_index = 0 ; face_index < 3 ; face_index++)
    {
        Vector3 face_normal = {};
        Vector3 face_edges[2] = {};
        Vector3 top_left_corner = {};
        int count = 0;
        
        switch(face_to_check[face_index])
        {
            case face_top: face_normal = top_face_direction; face_edges[count++] = A_to_B; face_edges[count++] = A_to_C; top_left_corner = points[bv_A]; break;
            case face_bottom: face_normal = bottom_face_direction; face_edges[count++] = H_to_G; face_edges[count++] = H_to_F; top_left_corner = points[bv_H]; break;
            case face_right: face_normal = right_face_direction; face_edges[count++] = H_to_F; face_edges[count++] = H_to_D; top_left_corner = points[bv_H]; break;
            case face_left: face_normal = left_face_direction; face_edges[count++] = A_to_C; face_edges[count++] = A_to_E; top_left_corner = points[bv_A]; break;
            case face_front: face_normal = forward_face_direction; face_edges[count++] = A_to_B; face_edges[count++] = A_to_E; top_left_corner = points[bv_A]; break;
            case face_back: face_normal = backward_face_direction; face_edges[count++] = H_to_G; face_edges[count++] = H_to_D; top_left_corner = points[bv_H]; break;
        }
        
        Vector3 bottom_right_corner = Vector3Add(top_left_corner , face_edges[0]);
        bottom_right_corner = Vector3Add(bottom_right_corner , face_edges[1]);
        
        Vector3 end_point = Vector3Add(origin , direction);
        
        //i suspect there is a better way than getting point on plane
        //but too lazy to found out
        
        //it call project you dummy
        float intersect_point_time = get_line_intersect_with_plane_time(origin , end_point  , face_normal , top_left_corner);
        Vector3 intersect_point = Vector3Lerp(origin , end_point , intersect_point_time);
        
        Vector3 top_left_to_intersect = Vector3Subtract(intersect_point , top_left_corner);
        Vector3 bottom_right_to_intersect = Vector3Subtract(intersect_point , bottom_right_corner);
        
        if(Vector3DotProduct(top_left_to_intersect , face_edges[0]) > 0) 
        { 
            if(Vector3DotProduct(top_left_to_intersect , face_edges[1]) > 0) 
            { 
                if(Vector3DotProduct(bottom_right_to_intersect , face_edges[0]) < 0) 
                { 
                    if(Vector3DotProduct(bottom_right_to_intersect , face_edges[1]) < 0) 
                    { 
                        if(closest_hit_time > intersect_point_time)
                        {
                            closest_hit_time = intersect_point_time;
                            if(hit_face)
                            {
                                (*hit_face) = face_to_check[face_index];
                            }
                            if(hit_time)
                            {
                                (*hit_time) = closest_hit_time;
                            }
                        }
                        
                        result = true;
                    }
                }
            }
        }
    }
    
    return result;
}

internal int function float_to_grid(float x, float size)
{
    x /= size;
    x = ceil(x);
    return x;
}

internal Vector3 function position_to_grid(Vector3 position , float size)
{
    position = Vector3Scale(position , 1.0 / size);
    position.x = ceil(position.x);
    position.y = ceil(position.y);
    position.z = ceil(position.z);
    position = Vector3Scale(position , size);
    
    return position;
}

internal Vector3 function position_to_cell(Vector3 position , float size)
{
    position = Vector3Scale(position , 1.0 / size);
    position.x = round(position.x);
    position.y = round(position.y);
    position.z = round(position.z);
    position = Vector3Scale(position , size);
    
    return position;
}

internal Vector3 function get_farest_point_by_direction( Vector3 direction , Vector3 * points , int point_count)
{
    Vector3 furthest_point = {};
    float furthest_distance = -FLT_MAX;
    
    for(int point_index = 0 ; point_index < point_count ; point_index++)
    {
        float distance = Vector3DotProduct(direction , points[point_index]);
        if(distance > furthest_distance)
        {
            furthest_distance = distance;
            furthest_point = points[point_index];
        }
    }
    
    return furthest_point;
}

internal Vector3 function get_support_point(Vector3 direction)
{
    Vector3 a = get_farest_point_by_direction( Vector3Negate(direction) , convex_shape_a_vertices , convex_shape_a_vertices_count);
    Vector3 b = get_farest_point_by_direction(direction , convex_shape_b_vertices , convex_shape_b_vertices_count);
    
    return Vector3Subtract(b , a);
}

internal bool function same_direction_b(Vector3 start , Vector3 end_a , Vector3 end_b)
{
    Vector3 direction_a = Vector3Subtract(end_a , start);
    Vector3 direction_b = Vector3Subtract(end_b , start);
    return Vector3DotProduct( direction_a , direction_b ) > 0;
}

//produce vertical direction to a and same direction to b
internal Vector3 function triple_cross_product(Vector3 a , Vector3 b)
{
    Vector3 third_vector = Vector3CrossProduct(b , a);
    return Vector3CrossProduct(a , third_vector);
}

#define SAME_DIRECTION(direction_a , direction_b) (Vector3DotProduct(direction_a , direction_b) > 0)

internal void function search_triangle(GJK_State * state)
{
    Vector3 a = state->simplex[0];
    Vector3 b = state->simplex[1];
    Vector3 c = state->simplex[2];
    
    Vector3 a_to_b = Vector3Subtract(b , a);
    Vector3 a_to_c = Vector3Subtract(c , a);
    Vector3 a_to_origin = Vector3Subtract(state->origin , a);
    
    Vector3 a_c_inward_direction = triple_cross_product(a_to_c , a_to_b);
    
    if(SAME_DIRECTION(a_c_inward_direction , a_to_origin))
    {
        Vector3 b_to_c = Vector3Subtract(c , b);
        Vector3 b_to_a = Vector3Subtract(a , b);
        Vector3 b_to_origin = Vector3Subtract(state->origin , b);
        
        Vector3 b_c_inward_direction = triple_cross_product(b_to_c , b_to_a);
        
        if(SAME_DIRECTION(b_c_inward_direction , b_to_origin))
        {
            Vector3 triangle_normal = Vector3CrossProduct(b_to_c , b_to_a);
            if(SAME_DIRECTION(triangle_normal , b_to_origin))
            {
                state->search_direction = triangle_normal;
            }
            else
            {
                state->simplex[0] = c;
                state->simplex[1] = b;
                state->simplex[2] = a;
                state->search_direction = Vector3Negate(triangle_normal);
            }
        }
        else
        {
            Vector3 c_to_b = Vector3Negate(b_to_c);
            Vector3 c_to_origin = Vector3Subtract(state->origin , c);
            
            if(SAME_DIRECTION(c_to_b , c_to_origin))
            {
                state->simplex[0] = b;
                state->simplex[1] = c;
                state->simplex_count = 2;
                
                state->search_direction = triple_cross_product(c_to_b , c_to_origin);
            }
            else
            {
                //would this even happen?
                //because there shouldn't any further point than support point
                
                //it do happen but at that time the code was wrong
                
                //CATCH;
                
                state->simplex[0] = c;
                state->simplex_count = 1;
                
                state->search_direction = c_to_origin;
            }
        }
    }
    else
    {
        Vector3 c_to_a = Vector3Negate(a_to_c);
        Vector3 c_to_origin = Vector3Subtract(state->origin , c);
        
        if(SAME_DIRECTION(c_to_a , c_to_origin))
        {
            state->simplex[0] = a;
            state->simplex[1] = c;
            state->simplex_count = 2;
            state->search_direction = triple_cross_product(c_to_a , c_to_origin);
        }
        else
        {
            state->simplex[0] = c;
            state->simplex_count = 1;
            state->search_direction = c_to_origin;
        }
    }
}

internal bool function iterate_simplex( GJK_State * state)
{
    float small_number = 0.000001f;
    
    if(state->simplex_count == 2)
    {
        Vector3 a = state->simplex[0];
        Vector3 b = state->simplex[1];
        
        Vector3 b_to_a = Vector3Subtract(a , b);
        Vector3 b_to_origin = Vector3Subtract(state->origin , b);
        
        if(SAME_DIRECTION(b_to_a , b_to_origin))
        {
            state->search_direction = triple_cross_product(b_to_a , b_to_origin);
            if(Vector3LengthSqr(state->search_direction) < small_number)
            {
                return true;
            }
        }
        else
        {
            state->search_direction = b_to_origin;
            state->simplex[0] = b;
            state->simplex_count = 1;
        }
    }
    else if(state->simplex_count == 3)
    {
        search_triangle(state);
    }
    else if(state->simplex_count == 4)
    {
        Vector3 a = state->simplex[0];
        Vector3 b = state->simplex[1];
        Vector3 c = state->simplex[2];
        Vector3 d = state->simplex[3];
        
        Vector3 a_to_b = Vector3Subtract(b , a);
        Vector3 a_to_d = Vector3Subtract(d , a);
        Vector3 a_b_d_face_outward_direction = Vector3CrossProduct(a_to_b , a_to_d );
        Vector3 a_to_origin = Vector3Subtract(state->origin , a);
        
        Vector3 b_to_c = Vector3Subtract(c , b);
        Vector3 b_to_d = Vector3Subtract(d , b);
        Vector3 b_c_d_face_outward_direction = Vector3CrossProduct(b_to_c , b_to_d);
        Vector3 b_to_origin = Vector3Subtract(state->origin , b);
        
        Vector3 c_to_a = Vector3Subtract(a , c);
        Vector3 c_to_d = Vector3Subtract(d , c);
        Vector3 c_a_d_face_outward_direction = Vector3CrossProduct(c_to_a , c_to_d);
        Vector3 c_to_origin = Vector3Subtract(state->origin , c);
        
        Vector3 a_to_c = Vector3Subtract(c , a);
        Vector3 bottom_triangle_normal = Vector3CrossProduct(a_to_c , a_to_b);
        if( fabs(Vector3DotProduct(bottom_triangle_normal , a_to_origin)) < small_number)
        {
            return true;
        }
        
        if(!SAME_DIRECTION(a_b_d_face_outward_direction , a_to_origin))
        {
            if(!SAME_DIRECTION(b_c_d_face_outward_direction , b_to_origin))
            {
                if(!SAME_DIRECTION(c_a_d_face_outward_direction , c_to_origin))
                {
                    return true;
                }
                else
                {
                    float dot_product = fabs(Vector3DotProduct(c_a_d_face_outward_direction , c_to_origin));
                    if( dot_product < small_number)
                    {
                        return true;
                    }
                    else
                    {
                        state->simplex[0] = c;
                        state->simplex[1] = a;
                        state->simplex[2] = d;
                        state->simplex_count = 3;
                        search_triangle(state);
                    }
                }
            }
            else
            {
                if(!SAME_DIRECTION(c_a_d_face_outward_direction , c_to_origin))
                {
                    float dot_product = fabs(Vector3DotProduct(b_c_d_face_outward_direction , b_to_origin));
                    if( dot_product < small_number)
                    {
                        return true;
                    }
                    else
                    {
                        state->simplex[0] = b;
                        state->simplex[1] = c;
                        state->simplex[2] = d;
                        state->simplex_count = 3;
                        search_triangle(state);
                    }
                }
                else
                {
                    state->simplex[0] = c;
                    state->simplex[1] = d;
                    state->simplex_count = 2;
                    state->search_direction = triple_cross_product(c_to_d , c_to_origin);
                }
            }
        }
        else
        {
            if(!SAME_DIRECTION(b_c_d_face_outward_direction , b_to_origin))
            {
                if(!SAME_DIRECTION(c_a_d_face_outward_direction , c_to_origin))
                {
                    float dot_product = fabs(Vector3DotProduct(a_b_d_face_outward_direction , a_to_origin));
                    if(dot_product < small_number)
                    {
                        return true;
                    }
                    else
                    {
                        state->simplex[0] = a;
                        state->simplex[1] = b;
                        state->simplex[2] = d;
                        state->simplex_count = 3;
                        search_triangle(state);
                    }
                }
                else
                {
                    state->simplex[0] = a;
                    state->simplex[1] = d;
                    state->simplex_count = 2;
                    state->search_direction = triple_cross_product(a_to_d , a_to_origin);
                }
            }
            else
            {
                if(!SAME_DIRECTION(c_a_d_face_outward_direction , c_to_origin))
                {
                    state->simplex[0] = b;
                    state->simplex[1] = d;
                    state->simplex_count = 2;
                    state->search_direction = triple_cross_product(b_to_d , b_to_origin);
                }
                else
                {
                    state->simplex[0] = d;
                    state->simplex_count = 1;
                    state->search_direction = Vector3Subtract(state->origin , d);
                }
            }
        }
    }
    
    return false;
}

internal bool function check_shape(Vector3 origin , Vector3 * vertices_a , int vertices_a_count , Vector3 * vertices_b , int vertices_b_count , GJK_State * state_result)
{
    convex_shape_a_vertices = vertices_a;
    convex_shape_a_vertices_count = vertices_a_count;
    convex_shape_b_vertices = vertices_b;
    convex_shape_b_vertices_count = vertices_b_count;
    
    bool result = false;
    float small_number = 0.000001f;
    
    GJK_State state = {};
    state.search_direction = (Vector3){1,0,0};
    state.origin = origin;
    
    Vector3 start_point = get_support_point(state.search_direction);
    
    state.search_direction = Vector3Negate(start_point);
    state.simplex[state.simplex_count++] = start_point;
    
    int iterate_count = 0;
    
    for(;;)
    {
        if(iterate_count > 50)
        {
            printf("too much %d %lld\n" , iterate_count , game_update_count);
            //CATCH;
            break;
        }
        
        iterate_count++;
        
        Vector3 new_support_point = get_support_point(state.search_direction);
        
        if(Vector3DotProduct(state.search_direction , new_support_point) < 0 ) 
        {
            result = false;
            break;
        }
        
        state.simplex[state.simplex_count++] = new_support_point;
        if(state.simplex_count > 4) CATCH;
        
        if(iterate_simplex(&state))
        {
            result = true;
            break;
        }
    }
    
    //draw_simplex(&state);
    if(state_result) (*state_result) = state;
    return result;
}

internal Vector3 function closest_point_on_line(Vector3 a , Vector3 b , Vector3 point)
{
    Vector3 a_to_point = Vector3Subtract(point , a);
    Vector3 a_to_b = Vector3Subtract(b , a);
    
    Vector3 b_to_point = Vector3Subtract(point , b);
    Vector3 b_to_a = Vector3Subtract(a , b);
    
    if(SAME_DIRECTION(a_to_point , a_to_b))
    {
        if(SAME_DIRECTION( b_to_point , b_to_a ))
        {
            Vector3 project_point = Vector3Project(a_to_point , a_to_b);
            return Vector3Add(a , project_point);
        }
        else
        {
            return b;
        }
    }
    else
    {
        return a;
    }
}

internal Vector3 function closest_point_on_triangle(Vector3 a , Vector3 b , Vector3 c , Vector3 point)
{
    float small_number = 0.000001f;
    
    Vector3 a_to_b = Vector3Subtract(b , a);
    Vector3 b_to_c = Vector3Subtract(c , b);
    Vector3 c_to_a = Vector3Subtract(a , c);
    Vector3 simplex_normal = Vector3CrossProduct(a_to_b , c_to_a);
    
    Vector3 a_to_point = Vector3Subtract(point , a);
    Vector3 a_to_b_vertical_inward = triple_cross_product(a_to_b , b_to_c);
    
    Vector3 b_to_point = Vector3Subtract(point , b);
    Vector3 b_to_c_vertical_inward = triple_cross_product(b_to_c , c_to_a);
    
    Vector3 c_to_point = Vector3Subtract(point , c);
    Vector3 c_to_a_vertical_inward = triple_cross_product(c_to_a , a_to_b);
    
    Vector3 closest_point = {};
    
    if(SAME_DIRECTION(a_to_point , a_to_b_vertical_inward))
    {
        if(SAME_DIRECTION(b_to_point , b_to_c_vertical_inward))
        {
            if(SAME_DIRECTION(c_to_point , c_to_a_vertical_inward))
            {
                Vector3 end_point = Vector3Add(simplex_normal , point);
                float hit_point_time = get_line_intersect_with_plane_time(point , end_point , simplex_normal , a);
                Vector3 hit_point = Vector3Lerp(point , end_point , hit_point_time);
                closest_point = hit_point;
            }
            else
            {
                closest_point = closest_point_on_line(c , a , point);
            }
        }
        else
        {
            if(SAME_DIRECTION(c_to_point , c_to_a_vertical_inward))
            {
                closest_point = closest_point_on_line(b , c , point);
            }
            else
            {
                closest_point = c;
            }
        }
    }
    else
    {
        if(SAME_DIRECTION(b_to_point , b_to_c_vertical_inward))
        {
            if(SAME_DIRECTION(c_to_point , c_to_a_vertical_inward))
            {
                closest_point = closest_point_on_line(a , b , point);
            }
            else
            {
                closest_point = a;
            }
        }
        else
        {
            if(SAME_DIRECTION(c_to_point , c_to_a_vertical_inward))
            {
                closest_point = b;
            }
            else
            {
                //it become a line or point
                if(Vector3DistanceSqr(a , b) < small_number)
                {
                    if(Vector3DistanceSqr(b , c) < small_number)
                    {
                        closest_point = c;
                    }
                    else
                    {
                        closest_point = closest_point_on_line(b , c , point);
                    }
                }
                else
                {
                    closest_point = closest_point_on_line(a , b , point);
                }
                //draw_simplex_triangle(a, b, c);
            }
        }
    }
    
    //draw_arrow_ray( a, a_to_b_vertical_inward , RED);
    //draw_arrow_ray( b, b_to_c_vertical_inward , GREEN);
    //draw_arrow_ray( c, c_to_a_vertical_inward , BLUE);
    
    //draw_simplex_triangle(a , b, c);
    //draw_arrow_line( closest_point , point , Fade(PINK , 0.5f));
    
    return closest_point;
}

internal bool function check_shape_impact(ShapeImpactData * data)
{
    convex_shape_a_vertices = data->shape_a_vertices;
    convex_shape_a_vertices_count = data->shape_a_vertices_count;
    convex_shape_b_vertices = data->shape_b_vertices;
    convex_shape_b_vertices_count = data->shape_b_vertices_count;
    
    float small_number = 0.000001f;
    
    bool result = false;
    data->time_of_impact = -FLT_MAX;
    
    float ray_time = 0;
    Vector3 ray_end = {};
    
    int ray_iterate_count = 0;
    int total_simplex_iterate_count = 0;
    
    Vector3 simplex[3] = {};
    Vector3 search_direction = Vector3Negate(data->ray_direction);
    
    simplex[0] = get_support_point(search_direction);
    simplex[1] = get_support_point(Vector3Negate(search_direction));
    simplex[2] = get_support_point(Vector3Add(search_direction , (Vector3){5,-5,2} ));
    
    for(;;)
    {
        search_direction = Vector3Negate(data->ray_direction);
        
        float closest_distance = FLT_MAX;
        Vector3 closest_point = {};
        
        int simplex_iterate_count = 0;
        int retry_count = 0;
        
        for(;;)
        {
            Vector3 a = simplex[0];
            Vector3 b = simplex[1];
            Vector3 c = simplex[2];
            
            simplex_iterate_count++;
            total_simplex_iterate_count++;
            if(total_simplex_iterate_count > 30)
            {
                printf("simplex iterate too many %lld\n" , game_update_count);
                break;
            }
            
            Vector3 new_support_point = get_support_point(Vector3Subtract(ray_end , closest_point));
            
            int vertex_to_replace = -1;
            Vector3 current_closest_point = {};
            float closest_test_distance = FLT_MAX;
            
            for(int simplex_index = 0 ; simplex_index < 3 ; simplex_index++)
            {
                Vector3 test_simplex[3] ={};
                test_simplex[0] = simplex[0];
                test_simplex[1] = simplex[1];
                test_simplex[2] = simplex[2];
                test_simplex[simplex_index] = new_support_point;
                
                Vector3 test_closest_point = {};
                test_closest_point = closest_point_on_triangle(test_simplex[0] , test_simplex[1] , test_simplex[2] , ray_end);
                
                float current_distance = Vector3DistanceSqr(ray_end , test_closest_point);
                
                if(closest_test_distance > current_distance)
                {
                    current_closest_point = test_closest_point;
                    closest_test_distance = current_distance;
                    vertex_to_replace = simplex_index;
                }
            }
            
            if(vertex_to_replace == -1) CATCH;
            
            
            float current_distance = Vector3DistanceSqr(current_closest_point , ray_end);
            
            if(closest_distance > current_distance)
            {
                closest_distance = current_distance;
                closest_point = current_closest_point;
                
                //retry_count = 0;
            }
            else if(retry_count < 1)
            {
                //i think it added existing point to the simplex because it wasn't a triangle but a line
                //it result the same distance
                //check one more time to see is this really the closest simplex
                //TODO: check if the simplex a line or a point
                
                //ok this 2 lines was all luck
                //it just fix the bug with no idea however
                closest_distance = current_distance;
                closest_point = current_closest_point;
                
                retry_count++;
            }
            else
            {
                a = simplex[0];
                b = simplex[1];
                c = simplex[2];
                
                Vector3 a_to_b = Vector3Subtract(b , a);
                Vector3 b_to_c = Vector3Subtract(c , b);
                Vector3 c_to_a = Vector3Subtract(a , c);
                Vector3 simplex_normal = Vector3CrossProduct(a_to_b , c_to_a);
                
                closest_point = current_closest_point;
                
                float hit_point_time = get_line_intersect_with_plane_time((Vector3){} , data->ray_direction , simplex_normal , a);
                Vector3 hit_point = Vector3Lerp((Vector3){} , data->ray_direction , hit_point_time);
                
                Vector3 a_to_hit_point = Vector3Subtract(hit_point , a);
                Vector3 a_to_b_vertical_inward = triple_cross_product(a_to_b , b_to_c);
                
                Vector3 b_to_hit_point = Vector3Subtract(hit_point , b);
                Vector3 b_to_c_vertical_inward = triple_cross_product(b_to_c , c_to_a);
                
                Vector3 c_to_hit_point = Vector3Subtract(hit_point , c);
                Vector3 c_to_a_vertical_inward = triple_cross_product(c_to_a , a_to_b);
                
                if(capture_collision)
                {
                    if(buffer_full(collision_visual_buffer))
                    {
                        reallocate_buffer(&collision_visual_buffer , AT_temp);
                    }
                    
                    CollisionVisual * collision_visual = collision_visual_buffer.data + collision_visual_buffer.count++;
                    (*collision_visual) = (CollisionVisual){}; 
                    
                    collision_visual->a = Vector3Add(a , collision_visual_offset);
                    collision_visual->b = Vector3Add(b , collision_visual_offset);
                    collision_visual->c = Vector3Add(c , collision_visual_offset);
                    collision_visual->closest_point = Vector3Add(closest_point , collision_visual_offset);
                    collision_visual->ray_end = Vector3Add(ray_end , collision_visual_offset);
                    collision_visual->offset = collision_visual_offset;
                    collision_visual->direction = data->ray_direction;
                    
                    collision_visual->shape_a = shape_a_union;
                    collision_visual->shape_b = shape_b_union;
                }
                
                if(SAME_DIRECTION(a_to_hit_point , a_to_b_vertical_inward))
                {
                    if(SAME_DIRECTION(b_to_hit_point , b_to_c_vertical_inward))
                    {
                        if(SAME_DIRECTION(c_to_hit_point , c_to_a_vertical_inward))
                        {
                            float hit_point_distance = Vector3DistanceSqr(hit_point , ray_end);
                            
                            //this check seem pointless
                            if(hit_point_distance < 0.00001)
                            {
                                if(data->stop_if_too_far)
                                {
                                    if(hit_point_time > 1.0f)
                                    {
                                        break;
                                    }
                                }
                                
                                data->impact_normal = simplex_normal;
                                
                                data->impact_point = hit_point;
                                data->time_of_impact = hit_point_time;
                                result = true;
                            }
                        }
                    }
                }
                
                break;
            }
            
            simplex[vertex_to_replace] = new_support_point;
        }
        
        if(total_simplex_iterate_count > 50) 
        {
            printf("too many iteration %lld\n" , game_update_count);
            break;
        }
        
        if(result) break;
        
        Vector3 surface_normal = Vector3Subtract(ray_end , closest_point);
        float dot_product_length = Vector3DotProduct(surface_normal , data->ray_direction);
        if( dot_product_length >= 0) 
        {
            if(fabs(dot_product_length) < small_number)
            {
                if(Vector3DistanceSqr(ray_end , closest_point) < small_number * small_number)
                {
                    Vector3 a = simplex[0];
                    Vector3 b = simplex[1];
                    Vector3 c = simplex[2];
                    
                    Vector3 a_to_b = Vector3Subtract(b , a);
                    Vector3 b_to_c = Vector3Subtract(c , b);
                    Vector3 c_to_a = Vector3Subtract(a , c);
                    Vector3 simplex_normal = Vector3CrossProduct(a_to_b , c_to_a);
                    
                    data->impact_point = ray_end;
                    data->time_of_impact = ray_time;
                    data->impact_normal = simplex_normal;
                    result = true;
                }
            }
            
            if(!result)
            {
                //printf("missed iterated:%d %lld\n" , ray_iterate_count , game_update_count);
            }
            
            break;
        }
        
        if(Vector3LengthSqr(surface_normal) > 0.0000001f)
        {
        }
        ray_time = ray_time - (Vector3DotProduct(surface_normal , surface_normal) / Vector3DotProduct(surface_normal , data->ray_direction));
        
        if(data->stop_if_too_far)
        {
            if(ray_time >= 1.0)
            {
                result = false;
                break;
            }
            else if(ray_time < 0)
            {
                result = false;
                break;
            }
        }
        
        ray_end = Vector3Scale(data->ray_direction , ray_time);
        ray_end = Vector3Add( (Vector3){} , ray_end);
    }
    
    //printf("total iterate count : %d\n", total_simplex_iterate_count);
    return result;
}

internal void function get_bound(Vector3 * vertices , int vertices_count , Vector3 * right_top_forward , Vector3 * left_bottom_backward )
{
    for(int vertex_index = 0 ; vertex_index < vertices_count ; vertex_index++)
    {
        Vector3 vertex = vertices[vertex_index];
        if(right_top_forward->x < vertex.x) right_top_forward->x = vertex.x;
        if(right_top_forward->y < vertex.y) right_top_forward->y = vertex.y;
        if(right_top_forward->z < vertex.z) right_top_forward->z = vertex.z;
        if(left_bottom_backward->x > vertex.x) left_bottom_backward->x = vertex.x;
        if(left_bottom_backward->y > vertex.y) left_bottom_backward->y = vertex.y;
        if(left_bottom_backward->z > vertex.z) left_bottom_backward->z = vertex.z;
    }
}

internal BoundingBoxNode function box_to_bound(Box box)
{
    BoundingBoxNode node = {};
    node.right_top_forward = (Vector3){-FLT_MAX , -FLT_MAX , -FLT_MAX};
    node.left_bottom_backward = (Vector3){FLT_MAX , FLT_MAX , FLT_MAX};
    
    get_bound(box_to_point(box) , box_vertex_count , &node.right_top_forward , &node.left_bottom_backward);
    return node;
}

internal bool function bounding_box_collided(BoundingBoxNode box_a , BoundingBoxNode box_b)
{
    float a_right = box_a.right_top_forward.x;
    float a_top = box_a.right_top_forward.y;
    float a_forward = box_a.right_top_forward.z;
    float a_left = box_a.left_bottom_backward.x;
    float a_bottom = box_a.left_bottom_backward.y;
    float a_backward = box_a.left_bottom_backward.z;
    
    float b_right = box_b.right_top_forward.x;
    float b_top = box_b.right_top_forward.y;
    float b_forward = box_b.right_top_forward.z;
    float b_left = box_b.left_bottom_backward.x;
    float b_bottom = box_b.left_bottom_backward.y;
    float b_backward = box_b.left_bottom_backward.z;
    
    bool x_collided = false;
    
    if(a_right > b_right)
    {
        if(a_left < b_right)
        {
            x_collided = true;
        }
    }
    else
    {
        if(b_left < a_right)
        {
            x_collided = true;
        }
    }
    
    if(!x_collided) return false;
    
    bool y_collided = false;
    
    if(a_top > b_top)
    {
        if(a_bottom < b_top)
        {
            y_collided = true;
        }
    }
    else
    {
        if(b_bottom < a_top)
        {
            y_collided = true;
        }
    }
    
    if(!y_collided) return false;
    
    if(a_forward > b_forward)
    {
        if(a_backward < b_forward)
        {
            return true;
        }
    }
    else
    {
        if(b_backward < a_forward)
        {
            return true;
        }
    }
    
    return false;
}

internal void function shape_union_to_vertices(ShapeUnion shape_union , Vector3 ** vertices , int * vertices_count)
{
    if(shape_union.type == ST_box)
    {
        (*vertices) = box_to_point(shape_union.box);
        (*vertices_count) = box_vertex_count;
    }
    else if(shape_union.type == ST_quad)
    {
        (*vertices) = shape_union.quad.vertex_position;
        (*vertices_count) = quad_vertex_count;
    }
    else 
    {
        CATCH;
    }
}

internal void function collision_push_root(BoundingBoxNode * root)
{
    if(buffer_full(bounding_box_root_stack)) reallocate_buffer(&bounding_box_root_stack , AT_temp);
    bounding_box_root_stack.data[bounding_box_root_stack.count++] = root;
}

internal ShapeUnionBuffer function get_collided_bounding_box(ConvexShape convex_shape)
{
    double tree_walk_time = time_stamp();
    
    shape_union_to_vertices(convex_shape.shape , &convex_shape.shape_vertices , &convex_shape.shape_vertices_count);
    
    if(convex_shape.shape.type == ST_quad)
    {
        convex_shape.shape_vertices = convex_shape.shape.quad.vertex_position;
        convex_shape.shape_vertices_count = quad_vertex_count;
    }
    else if(convex_shape.shape.type == ST_box)
    {
        convex_shape.shape_vertices = box_to_point(convex_shape.shape.box);
        convex_shape.shape_vertices_count = box_vertex_count;
    }
    else
    {
        CATCH;
    }
    
    Vector3 * all_vertices = allocate_memory_type(Vector3 , convex_shape.shape_vertices_count * 2 , AT_frame);
    for(int vertex_index = 0 ; vertex_index < convex_shape.shape_vertices_count ; vertex_index++)
    {
        all_vertices[vertex_index] = convex_shape.shape_vertices[vertex_index];
    }
    
    for(int vertex_index = 0 ; vertex_index < convex_shape.shape_vertices_count ; vertex_index++)
    {
        all_vertices[vertex_index + convex_shape.shape_vertices_count] = Vector3Add(convex_shape.shape_vertices[vertex_index] , convex_shape.velocity);
    }
    
    BoundingBoxNode convex_shape_bounding_box = {};
    convex_shape_bounding_box.right_top_forward = (Vector3){-FLT_MAX , -FLT_MAX , -FLT_MAX};
    convex_shape_bounding_box.left_bottom_backward = (Vector3){FLT_MAX , FLT_MAX , FLT_MAX};
    get_bound(all_vertices , convex_shape.shape_vertices_count * 2 , &convex_shape_bounding_box.right_top_forward , &convex_shape_bounding_box.left_bottom_backward );
    
    convex_shape_bounding_box.right_top_forward.x += UNIT_SIZE * 0.5f;
    convex_shape_bounding_box.right_top_forward.y += UNIT_SIZE * 0.5f;
    convex_shape_bounding_box.right_top_forward.z += UNIT_SIZE * 0.5f;
    
    convex_shape_bounding_box.left_bottom_backward.x -= UNIT_SIZE * 0.5f;
    convex_shape_bounding_box.left_bottom_backward.y -= UNIT_SIZE * 0.5f;
    convex_shape_bounding_box.left_bottom_backward.z -= UNIT_SIZE * 0.5f;
    
    ShapeUnionBuffer shape_union_buffer = {};
    allocate_buffer(&shape_union_buffer , ShapeUnion , 16 , AT_frame);
    
    if(bounding_box_root_stack.count == 0) CATCH;
    
    for(int root_stack_index = 0; root_stack_index < bounding_box_root_stack.count; root_stack_index++)
    {
        BoundingBoxNode * node_stack[128] = {};
        int node_stack_count = 0;
        
        if(bounding_box_root_stack.data[root_stack_index]) node_stack[node_stack_count++] = bounding_box_root_stack.data[root_stack_index];
        
        for(;;)
        {
            if(node_stack_count <= 0) break;
            
            node_stack_count--;
            BoundingBoxNode * node = node_stack[node_stack_count];
            
            if(node->left) node_stack[node_stack_count++] = node->left;
            if(node->right) node_stack[node_stack_count++] = node->right;
            
            //TODO: why there is invalid node?
            if(node->shape.type == ST_invalid) continue;
            
            if(bounding_box_collided((*node) , convex_shape_bounding_box))
            {
                if(buffer_full(shape_union_buffer))
                {
                    reallocate_buffer(&shape_union_buffer , AT_frame);
                }
                
                ShapeUnion * new_shape = shape_union_buffer.data + shape_union_buffer.count++;
                (*new_shape) = node->shape;
            }
        }
    }
    
    bounding_box_root_stack.count = 0;
    
    tree_walk_time = (time_stamp() - tree_walk_time) / (1000.0);
    
    return shape_union_buffer;
}

internal void function start_record_collision(int type , ConvexShape convex_shape)
{
    if(capture_collision) 
    {
        collision_visual_offset = convex_shape.position;
    }
    
    if(store_multiple_frame_collision)
    {
        if(buffer_full(frame_collision_buffer))
        {
            reallocate_buffer(&frame_collision_buffer , AT_temp);
        }
        
        current_frame_collision = frame_collision_buffer.data + frame_collision_buffer.count++;
        current_frame_collision->update_index = game_update_count;
        current_frame_collision->collision_type = type;
        current_frame_collision->collision_visual_offset = convex_shape.position;
        current_frame_collision->slice_start = collision_visual_buffer.count;
        current_frame_collision->start = convex_shape.position;
        current_frame_collision->velocity = convex_shape.velocity;
    }
}

internal void function end_record_collision()
{
    if(store_multiple_frame_collision)
    {
        current_frame_collision->slice_end = collision_visual_buffer.count;
    }
}

//this is slow but fast enough for my game
internal CollisionResult function update_collision(ConvexShape convex_shape)
{
    
    bool skip_this = capture_collision;
    if(!convex_shape.capture_collision) capture_collision = false;
    start_record_collision(CT_collision , convex_shape);
    
    collision_push_root(bounding_box_in_map_root);
    ShapeUnionBuffer shape_buffer = get_collided_bounding_box(convex_shape);
    
    shape_a_union = convex_shape.shape;
    
    double shape_impact_check_time = time_stamp();
    
    int check_count = 0;
    
    shape_union_to_vertices(convex_shape.shape , &convex_shape.shape_vertices , &convex_shape.shape_vertices_count);
    Vector3 shape_offset = {};
    Vector3 shape_velocity = convex_shape.velocity;
    int surface_normal_count = 0;
    Vector3 total_surface_normal = {};
    bool impacted = false;
    bool collided_something = false;
    
    //how about start from triangle from previous GJK iteration so no need to iterate all of them again
    //i did that but there is no noticable difference?
    for(;;)
    {
        //break;
        check_count++;
        if(check_count > 8)
        {
            shape_velocity = (Vector3){};
            //printf("too many contact %lld\n" , game_update_count);
            break;
        }
        
        impacted = false;
        float closest_impact_time = FLT_MAX;
        Vector3 surface_normal = {};
        Vector3 collided_shape_velocity = {};
        Vector3 collided_shape_origin = {};
        
        for(int shape_index = 0 ; shape_index < shape_buffer.count ; shape_index++)
        {
            ShapeUnion collided_shape = shape_buffer.data[shape_index];
            
            if(convex_shape.shape.owner.is_entity)
            {
                if(collided_shape.owner.is_entity == convex_shape.shape.owner.is_entity)
                {
                    if(collided_shape.owner.entity_handle.entity_index == convex_shape.shape.owner.entity_handle.entity_index)
                    {
                        if(collided_shape.owner.entity_handle.generation_index == convex_shape.shape.owner.entity_handle.generation_index)
                        {
                            continue;
                        }
                    }
                }
            }
            
            if(capture_collision)
            {
                shape_b_union = collided_shape;
            }
            
            Vector3 * collided_shape_vertices = 0;
            int collided_shape_vertices_count = 0;
            
            shape_union_to_vertices(collided_shape , &collided_shape_vertices , &collided_shape_vertices_count);
            
            //the origin can land within the minkowski difference
            //because the collided shape have velocity and the position can be either before or after apply velocity
            //in both case the origin can land within minkowski difference because the ray cast was the subtract velocity from shape a by velocity from shape b
            //and still can consider as not collided
            //the algorithm can't handle ray start within minkowski difference, i haven't revisit the paper yet but i don't think it know this
            //in here i used the position before applying velocity
            //to avoid this you have to check is origin within minkowski difference
            //if it is it can still mean the collided shape doesn't collided after it apply its velocity
            //so you can apply velocity to position, and because it already applied there no need to subtract velocity from the ray
            bool shape_overlapped = check_shape((Vector3){} , convex_shape.shape_vertices , convex_shape.shape_vertices_count , collided_shape_vertices , collided_shape_vertices_count , 0);
            if(shape_overlapped)
            {
                if(collided_shape.type != ST_box) CATCH;
                for(int vertex_index = 0; vertex_index < collided_shape_vertices_count; vertex_index++)
                {
                    collided_shape_vertices[vertex_index] = Vector3Add(collided_shape_vertices[vertex_index] , collided_shape.velocity);
                }
            }
            
            ShapeImpactData impact_data = {};
            impact_data.shape_b_vertices = collided_shape_vertices;
            impact_data.shape_b_vertices_count = collided_shape_vertices_count;
            impact_data.shape_a_vertices = convex_shape.shape_vertices;
            impact_data.shape_a_vertices_count = convex_shape.shape_vertices_count;
            impact_data.stop_if_too_far = true;
            if(shape_overlapped)
            {
                impact_data.ray_direction = convex_shape.velocity;
            }
            else
            {
                impact_data.ray_direction = Vector3Subtract(convex_shape.velocity , collided_shape.velocity);
            }
            
            if(check_shape_impact(&impact_data))
            {
                if(impact_data.time_of_impact > 0)
                {
                    impacted = true;
                    
                    if(closest_impact_time > impact_data.time_of_impact)
                    {
                        closest_impact_time = impact_data.time_of_impact;
                        surface_normal = impact_data.impact_normal;
                        collided_shape_velocity = collided_shape.velocity;
                        if(collided_shape.type != ST_box) CATCH;
                        collided_shape_origin = collided_shape.box.position;
                    }
                }
            }
        }
        
        bool collided_in_this_iteration = false;
        
        if(impacted)
        {
            if(closest_impact_time > 0)
            {
                if(closest_impact_time < 1.0)
                {
                    collided_in_this_iteration = true;
                }
            }
        }
        
        Vector3 start_velocity = shape_velocity;
        if(Vector3DotProduct(Vector3Subtract(convex_shape.position , collided_shape_origin) , surface_normal) < 0) surface_normal = Vector3Negate(surface_normal);
        Vector3 impact_point = Vector3Add(Vector3Add(convex_shape.position , collided_shape_velocity) , Vector3Scale(start_velocity, closest_impact_time));
        
        if(collided_in_this_iteration)
        {
            collided_something = true;
            surface_normal = Vector3Normalize(surface_normal);
            
            if(fabs(Vector3DotProduct(surface_normal , start_velocity)) > 0.001f)
            {
                surface_normal_count++;
                total_surface_normal = Vector3Add(total_surface_normal , surface_normal);
            }
            
            Vector3 average_surface_normal = Vector3Scale(total_surface_normal , 1.0 / surface_normal_count);
            
            if(capture_collision)
            {
                CollisionVisual * visual = collision_visual_buffer.data + (collision_visual_buffer.count - 1);
                visual->collided =true;
                visual->collision_point = impact_point;
                visual->collision_normal = surface_normal;
            }
            
            Vector3 offset = {};
            
            if(Vector3DotProduct(start_velocity , surface_normal) < 0.0f)
            {
                offset = Vector3Scale(start_velocity , 1.0 - closest_impact_time);
                offset = Vector3Project(offset , surface_normal);
                offset = Vector3Negate(offset);
            }
            
            //sometime player can clipping ceiling when jumping against ceiling and being push by block on side
            //weir
            
            float gap = 0.004f;
            
            //sometime check_shape_impact give weir normal
            //this check prevent that but this worry me
            //if(fabs(Vector3DotProduct(surface_normal , start_velocity)) > 0.001f)
            {
                //if not adding this offset from surface give more smooth collision on concave corner
                offset = Vector3Add(offset , Vector3Scale(surface_normal , gap));
                //a bit worry if the average is zero
                //but it make fewer iterate
                if(surface_normal_count) offset = Vector3Add(offset , Vector3Scale(average_surface_normal , gap));
            }
            
            shape_velocity = Vector3Add(shape_velocity , offset);
            
            if(Vector3LengthSqr(surface_normal) > 0.0001f)
            {
                if(Vector3DotProduct(collided_shape_velocity , surface_normal) > 0.01f)
                {
                    Vector3 push_force = Vector3Scale(collided_shape_velocity , 1.0 - closest_impact_time);
                    push_force = Vector3Project(push_force , surface_normal);
                    shape_offset = Vector3Add(shape_offset , push_force);
                    shape_offset = push_force;
                }
            }
            
            //draw_arrow_ray(convex_shape.position , Vector3Scale(start_velocity , 100) , WHITE);
            //draw_arrow_ray(convex_shape.position , Vector3Scale(shape_velocity , 100) , BLUE);
            //draw_arrow_ray(convex_shape.position , Vector3Scale(offset , 100) , YELLOW);
            //draw_arrow_ray(convex_shape.position , Vector3Scale(shape_offset , 100) , GREEN);
            
            if(shape_velocity.x != shape_velocity.x) CATCH;
            if(shape_velocity.y != shape_velocity.y) CATCH;
            if(shape_velocity.z != shape_velocity.z) CATCH;
            
            convex_shape.velocity = Vector3Add(shape_velocity , shape_offset);
        }
        else
        {
            //printf("couldn't hit anything %lld\n" , game_update_count);
            break;
        }
    }
    
    //it was so nice before
    //only checking ray and that is all of it
    //and it start clipping through to the back of a moving block
    //i checked the math and if the math and the code is correct that shouldn't happen at all
    //but it happened so now i gotta push it out ):
    
    //good new i work it out in one night
    
    //the push is wrong tho
    collision_push_root(bounding_box_in_map_root);
    shape_buffer = get_collided_bounding_box(convex_shape);
    
    bool stucked = false;
    
    int push_attemp = 0;
    Vector3 offset_within_shape = {};
    for(;;)
    {
        break;
        push_attemp++;
        if(push_attemp > 10)
        {
            stucked = true;
            break;
        }
        
        shape_union_to_vertices(convex_shape.shape , &convex_shape.shape_vertices , &convex_shape.shape_vertices_count);
        
        for(int vertex_index = 0; vertex_index < convex_shape.shape_vertices_count; vertex_index++)
        {
            convex_shape.shape_vertices[vertex_index] = Vector3Add(convex_shape.shape_vertices[vertex_index] , offset_within_shape);
        }
        
        bool within_other_shape = false;
        
        for(int buffer_index = 0; buffer_index < shape_buffer.count; buffer_index++)
        {
            ShapeUnion collided_shape = shape_buffer.data[buffer_index];
            
            if(convex_shape.shape.owner.is_entity)
            {
                if(collided_shape.owner.is_entity == convex_shape.shape.owner.is_entity)
                {
                    if(collided_shape.owner.entity_handle.entity_index == convex_shape.shape.owner.entity_handle.entity_index)
                    {
                        if(collided_shape.owner.entity_handle.generation_index == convex_shape.shape.owner.entity_handle.generation_index)
                        {
                            continue;
                        }
                    }
                }
            }
            
            Vector3 * collided_shape_vertices = 0;
            int collided_shape_vertices_count = 0;
            
            shape_union_to_vertices(collided_shape , &collided_shape_vertices , &collided_shape_vertices_count);
            
            for(int vertex_index = 0; vertex_index < collided_shape_vertices_count; vertex_index++)
            {
                //collided_shape_vertices[vertex_index] = Vector3Add(collided_shape_vertices[vertex_index] , collided_shape.velocity);
            }
            
            GJK_State end_state = {};
            if(check_shape((Vector3){} , convex_shape.shape_vertices , convex_shape.shape_vertices_count , collided_shape_vertices , collided_shape_vertices_count , &end_state))
            {
                within_other_shape = true;
                if(end_state.simplex_count != 4) continue;
                
                TriangleIndexBuffer triangle_index_buffer = {};
                Array triangle_index_array = {};
                Vector3Buffer polytope = {};
                
                allocate_buffer(&polytope , Vector3 , 16 , AT_frame);
                allocate_buffer(&triangle_index_buffer , TriangleIndex , 16 , AT_frame);
                triangle_index_array = allocate_array(16 , AT_frame);
                
                polytope.data[polytope.count++] = end_state.simplex[0];
                polytope.data[polytope.count++] = end_state.simplex[1];
                polytope.data[polytope.count++] = end_state.simplex[2];
                polytope.data[polytope.count++] = end_state.simplex[3];
                
                triangle_index_buffer.data[add_to_array(&triangle_index_array)] = (TriangleIndex){0,1,2};
                triangle_index_buffer.data[add_to_array(&triangle_index_array)] = (TriangleIndex){3,1,2};
                triangle_index_buffer.data[add_to_array(&triangle_index_array)] = (TriangleIndex){0,3,2};
                triangle_index_buffer.data[add_to_array(&triangle_index_array)] = (TriangleIndex){0,1,3};
                
                int try_attemp = 0;
                
                //Vector3 previous_closest_normal = {FLT_MAX , FLT_MAX , FLT_MAX};
                for(;;)
                {
                    try_attemp++;
                    bool stop = false;
                    if(try_attemp > 6) stop = true;
                    
                    Vector3 closest_normal = {FLT_MAX , FLT_MAX , FLT_MAX};
                    
                    array_foreach(triangle_index , &triangle_index_array)
                    {
                        Vector3 a = polytope.data[triangle_index_buffer.data[triangle_index].a_index];
                        Vector3 b = polytope.data[triangle_index_buffer.data[triangle_index].b_index];
                        Vector3 c = polytope.data[triangle_index_buffer.data[triangle_index].c_index];
                        Vector3 a_to_b = Vector3Subtract(b , a);
                        Vector3 a_to_c = Vector3Subtract(c , a);
                        
                        Vector3 normal = Vector3CrossProduct(a_to_b , a_to_c);
                        if(Vector3DotProduct(normal , a) < 0) normal = Vector3Negate(normal);
                        normal = Vector3Project(a , normal);
                        
                        if(Vector3LengthSqr(closest_normal) > Vector3LengthSqr(normal))
                        {
                            closest_normal = normal;
                        }
                        
                        //draw_arrow_ray(convex_shape.position , normal , BLACK);
                    }
                    
                    bool found = false;
                    
                    Vector3 new_support_point = get_support_point(closest_normal);
                    Vector3 new_support_point_on_normal = Vector3Project(new_support_point , closest_normal);
                    
                    if(Vector3DistanceSqr(new_support_point_on_normal , closest_normal) < 0.001f)
                    {
                        float gap = 0.002;
                        
                        found = true;
                        offset_within_shape = Vector3Add(offset_within_shape , new_support_point_on_normal);
                        offset_within_shape = Vector3Add(offset_within_shape , Vector3Scale(Vector3Normalize(new_support_point_on_normal) , gap));
                        
                        if(Vector3DotProduct(offset_within_shape , shape_velocity) > 0)
                        {
                            //shape_offset = (Vector3){};
                        }
                    }
                    
                    //if(found) draw_arrow_ray(convex_shape.position , closest_normal , SKYBLUE);
                    
                    if(found)
                    {
                        shape_velocity = project_on_plane(shape_velocity , new_support_point_on_normal);
                        shape_velocity = (Vector3){};
                        
#if 1
                        if(convex_shape.capture_collision)
                        {
                            array_foreach(triangle_index , &triangle_index_array)
                            {
                                Vector3 a = polytope.data[triangle_index_buffer.data[triangle_index].a_index];
                                Vector3 b = polytope.data[triangle_index_buffer.data[triangle_index].b_index];
                                Vector3 c = polytope.data[triangle_index_buffer.data[triangle_index].c_index];
                                
                                Vector3 a_to_b = Vector3Subtract(b , a);
                                Vector3 a_to_c = Vector3Subtract(c , a);
                                
                                Vector3 normal = Vector3CrossProduct(a_to_b , a_to_c);
                                if(Vector3DotProduct(normal , a) < 0) normal = Vector3Negate(normal);
                                normal = Vector3Project(a , normal);
                                
                                a = Vector3Add(a , convex_shape.position);
                                b = Vector3Add(b , convex_shape.position);
                                c = Vector3Add(c , convex_shape.position);
                                
                                draw_simplex_triangle(a , b , c);
                                
                                draw_arrow_ray(convex_shape.position , normal , Fade(BLACK , 0.3f));
                            }
                        }
                        draw_arrow_ray(convex_shape.position , offset_within_shape , RED);
                        draw_arrow_ray(convex_shape.position , new_support_point_on_normal , BLUE);
                        draw_arrow_ray(convex_shape.position , new_support_point , YELLOW);
#endif
                        
                        break;
                    }
                    
                    if(stop)
                    {
                        break;
                    }
                    
                    if(buffer_full(polytope)) reallocate_buffer(&polytope , AT_frame);
                    
                    int new_support_point_index = polytope.count++;
                    polytope.data[new_support_point_index] = new_support_point;
                    
                    local_persist bool edge_buffer_initialized = false;
                    local_persist NewEdgeBuffer new_edge_buffer = {};
                    local_persist Array new_edge_array = {};
                    
                    if(!edge_buffer_initialized)
                    {
                        edge_buffer_initialized = true;
                        allocate_buffer(&new_edge_buffer , NewEdge , 16 , AT_temp);
                        new_edge_array = allocate_array(16 , AT_temp);
                    }
                    
                    clear_array(&new_edge_array);
                    
                    array_foreach(triangle_index , &triangle_index_array)
                    {
                        int a_index = triangle_index_buffer.data[triangle_index].a_index;
                        int b_index = triangle_index_buffer.data[triangle_index].b_index;
                        int c_index = triangle_index_buffer.data[triangle_index].c_index;
                        
                        Vector3 a = polytope.data[a_index];
                        Vector3 b = polytope.data[b_index];
                        Vector3 c = polytope.data[c_index];
                        Vector3 a_to_b = Vector3Subtract(b , a);
                        Vector3 a_to_c = Vector3Subtract(c , a);
                        
                        Vector3 normal = Vector3CrossProduct(a_to_b , a_to_c);
                        if(Vector3DotProduct(normal , a) < 0) normal = Vector3Negate(normal);
                        
                        if(Vector3DotProduct(new_support_point , normal) > 2)
                        {
                            delete_from_array(&triangle_index_array , triangle_index);
                            NewEdge all_new_edge[3] = {{a_index , b_index} , {b_index , c_index} , {c_index , a_index}};
                            bool valid_edge[3] = {};
                            
                            for(int new_edge_index = 0; new_edge_index < 3; new_edge_index++)
                            {
                                bool no_duplication = true;
                                NewEdge new_edge = all_new_edge[new_edge_index];
                                
                                array_foreach(previous_edge_index , &new_edge_array)
                                {
                                    NewEdge previous_edge = new_edge_buffer.data[previous_edge_index];
                                    if(new_edge.a_index == previous_edge.a_index)
                                    {
                                        if(new_edge.b_index == previous_edge.b_index)
                                        {
                                            no_duplication = false;
                                        }
                                    }
                                    else if(new_edge.a_index == previous_edge.b_index)
                                    {
                                        if(new_edge.b_index == previous_edge.a_index)
                                        {
                                            no_duplication = false;
                                        }
                                    }
                                    
                                    if(!no_duplication)
                                    {
                                        delete_from_array(&new_edge_array , previous_edge_index);
                                        break;
                                    }
                                }
                                
                                valid_edge[new_edge_index] = no_duplication;
                            }
                            
                            for(int new_edge_index = 0; new_edge_index < 3; new_edge_index++)
                            {
                                NewEdge new_edge = all_new_edge[new_edge_index];
                                
                                if(valid_edge[new_edge_index])
                                {
                                    if(array_full(&new_edge_array))
                                    {
                                        reallocate_array(&new_edge_array , AT_temp);
                                        reallocate_buffer(&new_edge_buffer , AT_temp);
                                    }
                                    new_edge_buffer.data[add_to_array(&new_edge_array)] = new_edge;
                                }
                            }
                        }
                    }
                    
                    array_foreach(edge_index , &new_edge_array)
                    {
                        
                        NewEdge edge = new_edge_buffer.data[edge_index];
                        TriangleIndex new_triangle = {edge.a_index , edge.b_index , new_support_point_index};
                        
                        if(array_full(&triangle_index_array))
                        {
                            reallocate_array(&triangle_index_array , AT_frame);
                            reallocate_buffer(&triangle_index_buffer , AT_frame);
                        }
                        
                        triangle_index_buffer.data[add_to_array(&triangle_index_array)] = new_triangle;
                    }
                }
            }
        }
        
        if(!within_other_shape) 
        {
            break;
        }
    }
    
    end_record_collision();
    
    capture_collision = skip_this;
    
    //printf( "check : %f\n", (time_stamp() - shape_impact_check_time) / (1000 ));
    //printf( "making tree : %f , walk in tree : %f , check : %f count : %d\n", shape_tree_time , tree_walk_time , (time_stamp() - shape_impact_check_time) / 1000 , shape_buffer_count);
    CollisionResult result = {};
    result.stucked = stucked;
    result.collided = collided_something;
    result.velocity = shape_velocity;
    result.offset = Vector3Add(shape_offset , offset_within_shape);
    return result;
}

internal RayCastResultBuffer function convex_shape_ray_cast(ConvexShape convex_shape)
{
    bool skip_this = capture_collision;
    if(!convex_shape.capture_collision) capture_collision = false;
    start_record_collision(CT_raycast , convex_shape);
    
    double check_time = time_stamp();
    
    ShapeUnionBuffer shape_buffer = get_collided_bounding_box(convex_shape);
    
    shape_a_union = convex_shape.shape;
    shape_union_to_vertices(convex_shape.shape , &convex_shape.shape_vertices , &convex_shape.shape_vertices_count);
    
    RayCastResultBuffer result_buffer = {};
    allocate_buffer(&result_buffer , RayCastResult , 4 , AT_frame);
    
    for(int shape_index = 0; shape_index < shape_buffer.count ; shape_index++)
    {
        ShapeUnion shape = shape_buffer.data[shape_index];
        
        if(Vector3DotProduct(shape.velocity , convex_shape.velocity) > 0)
        {
            if(shape.type != ST_box) CATCH;
            shape.box.position = Vector3Add(shape.box.position , shape.velocity);
            shape.velocity = (Vector3){};
        }
        
        if(capture_collision)
        {
            shape_b_union = shape;
        }
        
        Vector3 * shape_vertices = 0;
        int shape_vertices_count = 0;
        
        shape_union_to_vertices(shape , &shape_vertices , &shape_vertices_count);
        
        ShapeImpactData impact_data = {};
        impact_data.shape_b_vertices = shape_vertices;
        impact_data.shape_b_vertices_count = shape_vertices_count;
        impact_data.shape_a_vertices = convex_shape.shape_vertices;
        impact_data.shape_a_vertices_count = convex_shape.shape_vertices_count;
        impact_data.ray_direction = Vector3Subtract(convex_shape.velocity , shape.velocity);
        impact_data.stop_if_too_far = !convex_shape.collect_all_collision;
        
        if(check_shape_impact(&impact_data))
        {
            if(impact_data.time_of_impact >= 0)
            {
                if(buffer_full(result_buffer))
                {
                    reallocate_buffer(&result_buffer , AT_frame);
                }
                
                if(Vector3DotProduct(convex_shape.velocity , impact_data.impact_normal) > 0) impact_data.impact_normal = Vector3Negate(impact_data.impact_normal);
                
                RayCastResult * result = result_buffer.data + result_buffer.count++;
                result->hit_time = impact_data.time_of_impact;
                result->surface_normal = impact_data.impact_normal;
                result->shape_owner = shape.owner;
            }
            
            //draw_quad_D(quad , MAROON);
        }
        else
        {
            //draw_quad_D(quad , PURPLE);
        }
        
    }
    
    capture_collision = skip_this;
    //printf( "ray cast time %f\n" , (time_stamp() - check_time) / (1000.0 ));
    end_record_collision();
    
    return result_buffer;
}

internal ShapeUnionBuffer function convex_shape_overlap(ConvexShape convex_shape)
{
    ShapeUnionBuffer buffer = get_collided_bounding_box(convex_shape);
    
    shape_union_to_vertices(convex_shape.shape , &convex_shape.shape_vertices , &convex_shape.shape_vertices_count);
    
    ShapeUnionBuffer collided_buffer = {};
    allocate_buffer(&collided_buffer , ShapeUnion , 8 , AT_frame);
    
    for(int shape_index = 0 ; shape_index < buffer.count ; shape_index++)
    {
        ShapeUnion shape = buffer.data[shape_index];
        Vector3 * shape_vertices = 0;
        int shape_vertices_count = 0;
        
        shape_union_to_vertices(shape , &shape_vertices , &shape_vertices_count);
        
        if(check_shape((Vector3){} , convex_shape.shape_vertices , convex_shape.shape_vertices_count , shape_vertices , shape_vertices_count , 0))
        {
            if(buffer_full(collided_buffer))
            {
                reallocate_buffer(&collided_buffer , AT_frame);
            }
            
            ShapeUnion * collided_shape = collided_buffer.data + collided_buffer.count++;
            (*collided_shape) = shape;
        }
    }
    
    return collided_buffer;
}

internal int function shape_cell_hash(int x , int y , int z)
{
    int hash_size = 16;
    return hash_int( hash_size * hash_size * z + hash_size * y + x );
}

//remove this
internal bool function iterate_cell_by_bound(CellIterator * iterator, Vector3 * vertices , int vertex_count , float cell_size)
{
    if(!iterator->initialized)
    {
        float right = vertices[0].x;
        float left = vertices[0].x;
        float top = vertices[0].y;
        float bottom = vertices[0].y;
        float forward = vertices[0].z;
        float backward = vertices[0].z;
        
        for(int vertex_index = 1; vertex_index < vertex_count ; vertex_index++)
        {
            Vector3 vertex = vertices[vertex_index];
            
            if(right < vertex.x) right = vertex.x;
            if(left > vertex.x) left = vertex.x;
            if(top < vertex.y) top = vertex.y;
            if(bottom > vertex.y) bottom = vertex.y;
            if(forward < vertex.z) forward = vertex.z;
            if(backward > vertex.z) backward = vertex.z;
        }
        
        iterator->initialized = true;
        iterator->cell_x = float_to_grid(left , cell_size);
        iterator->cell_left = float_to_grid(left , cell_size);
        iterator->cell_right = float_to_grid(right , cell_size);
        iterator->cell_y = float_to_grid(bottom , cell_size);
        iterator->cell_bottom = float_to_grid(bottom , cell_size);
        iterator->cell_top = float_to_grid(top , cell_size);
        iterator->cell_z = float_to_grid(backward , cell_size);
        iterator->cell_backward = float_to_grid(backward , cell_size);
        iterator->cell_forward = float_to_grid(forward , cell_size);
    }
    else
    {
        iterator->cell_x++;
        if(iterator->cell_x > iterator->cell_right)
        {
            iterator->cell_x = iterator->cell_left;
            iterator->cell_y++;
        }
        if(iterator->cell_y > iterator->cell_top)
        {
            iterator->cell_y = iterator->cell_bottom;
            iterator->cell_z++;
        }
        if(iterator->cell_z > iterator->cell_forward)
        {
            return false;
        }
    }
    
    return true;
}

//TODO: i don't know how to split to a binary tree
internal BoundingBoxNode * function split_bounding_box(BoundingBoxNode * buffer , int buffer_count , int split_type , int split_fail_attemp)
{
    if(buffer_count == 0) return 0;
    if(buffer_count == 1) return buffer;
    
    BoundingBoxNode * root_node = allocate_memory_type(BoundingBoxNode , 1 , AT_frame);
    root_node->left = 0;
    root_node->right = 0;
    
    if(buffer_count == 2)
    {
        root_node->left = buffer;
        root_node->right = buffer + 1;
        return root_node;
    }
    
    for(int bounding_box_index = 0; bounding_box_index < buffer_count ; bounding_box_index++)
    {
        BoundingBoxNode * bounding_box = buffer + bounding_box_index;
        Vector3 right_top_forward = bounding_box->right_top_forward;
        Vector3 left_bottom_backward = bounding_box->left_bottom_backward;
        
        if(root_node->right_top_forward.x < right_top_forward.x) root_node->right_top_forward.x = right_top_forward.x;
        if(root_node->right_top_forward.y < right_top_forward.y) root_node->right_top_forward.y = right_top_forward.y;
        if(root_node->right_top_forward.z < right_top_forward.z) root_node->right_top_forward.z = right_top_forward.z;
        if(root_node->left_bottom_backward.x > left_bottom_backward.x) root_node->left_bottom_backward.x = left_bottom_backward.x;
        if(root_node->left_bottom_backward.y > left_bottom_backward.y) root_node->left_bottom_backward.y = left_bottom_backward.y;
        if(root_node->left_bottom_backward.z > left_bottom_backward.z) root_node->left_bottom_backward.z = left_bottom_backward.z;
    }
    
    BoundingBoxNode right_box = (*root_node);
    BoundingBoxNode left_box = (*root_node);
    
    //TODO: how to split better?
    if(split_type == split_yz)
    {
        right_box.right_top_forward.x = Lerp( right_box.right_top_forward.x , right_box.left_bottom_backward.x , 0.5f);
        left_box.left_bottom_backward.x = right_box.right_top_forward.x;
    }
    else if(split_type == split_xz)
    {
        right_box.right_top_forward.y = Lerp( right_box.right_top_forward.y , right_box.left_bottom_backward.y , 0.5f);
        left_box.left_bottom_backward.y = right_box.right_top_forward.y;
    }
    else if(split_type == split_xy)
    {
        right_box.right_top_forward.z = Lerp( right_box.right_top_forward.z , right_box.left_bottom_backward.z , 0.5f);
        left_box.left_bottom_backward.z = right_box.right_top_forward.z;
    }
    
    BoundingBoxNode * left_buffer = 0;
    int left_buffer_count = 0;
    left_buffer = allocate_memory_type(BoundingBoxNode , buffer_count , AT_frame);
    
    BoundingBoxNode * right_buffer = 0;
    right_buffer = allocate_memory_type(BoundingBoxNode , buffer_count , AT_frame);
    int right_buffer_count = 0;
    
    for(int bounding_box_index = 0 ; bounding_box_index < buffer_count ; bounding_box_index++)
    {
        BoundingBoxNode node = buffer[bounding_box_index];
        BoundingBoxNode * new_node = 0;
        
        if(bounding_box_collided(node , right_box))
        {
            new_node = right_buffer + right_buffer_count++;
        }
        else
        {
            new_node = left_buffer + left_buffer_count++;
        }
        
        (*new_node) = node;
    }
    
    split_type++;
    if(split_type >= split_count) split_type = split_yz;
    
    bool split_failed = false;
    if(right_buffer_count == 0) split_failed = true;
    if(left_buffer_count == 0) split_failed = true;
    
    if(split_failed) 
    {
        split_fail_attemp++;
    }
    else
    {
        split_fail_attemp = 0;
    }
    
    if(split_fail_attemp < 3)
    {
        root_node->right = split_bounding_box(right_buffer , right_buffer_count , split_type , split_fail_attemp);
        root_node->left = split_bounding_box(left_buffer , left_buffer_count , split_type , split_fail_attemp);
    }
    else
    {
        if(right_buffer_count > 0)
        {
            root_node->right = split_bounding_box(right_buffer , right_buffer_count - 1 , split_type , 0);
            root_node->left = split_bounding_box(right_buffer + right_buffer_count - 1 , 1 , split_type , 0);
        }
        else
        {
            root_node->right = split_bounding_box(left_buffer , left_buffer_count - 1 , split_type , 0);
            root_node->left = split_bounding_box(left_buffer + left_buffer_count - 1 , 1 , split_type , 0);
        }
    }
    
    return root_node;
}

internal int function cell_to_index(Int3 cell)
{
    if(cell.x < 0) CATCH;
    if(cell.y < 0) CATCH;
    if(cell.z < 0) CATCH;
    if(cell.x > nav_mesh_size.x) CATCH;
    if(cell.y > nav_mesh_size.y) CATCH;
    if(cell.z > nav_mesh_size.z) CATCH;
    
    return cell.z * nav_mesh_size.x * nav_mesh_size.y + cell.y * nav_mesh_size.x + cell.x;
}

//TODO: big to-do here
//make it better? less cell and build faster
//my best chance is delaunay triangulation
internal void function generate_nav_mesh()
{
    if(box_in_map_array.count == 0) return;
    
    Vector3 whole_mesh_max = { -FLT_MAX , -FLT_MAX , -FLT_MAX };
    Vector3 whole_mesh_min = { FLT_MAX , FLT_MAX , FLT_MAX };
    
    array_foreach(box_index , &box_in_map_array)
    {
        Box box = box_in_map_buffer.data[box_index];
        get_bound(box_to_point(box) , box_vertex_count , &whole_mesh_max , &whole_mesh_min );
    }
    
    Vector3 mesh_max = position_to_grid(Vector3Scale(whole_mesh_max , 1.0) , nav_mesh_cell_size);
    Vector3 mesh_min = position_to_grid(Vector3Scale(whole_mesh_min , 1.0) , nav_mesh_cell_size);
    
    mesh_max = Vector3Scale(mesh_max , 1.0 / nav_mesh_cell_size);
    mesh_min = Vector3Scale(mesh_min , 1.0 / nav_mesh_cell_size);
    
    Int3 nav_mesh_start = {};
    nav_mesh_start.x = mesh_min.x;
    nav_mesh_start.y = mesh_min.y;
    nav_mesh_start.z = mesh_min.z;
    
    //why plus one?
    Vector3 mesh_cell_size_float = Vector3Subtract(mesh_max , mesh_min);
    nav_mesh_size.x = fabs(mesh_cell_size_float.x) + 1;
    nav_mesh_size.y = fabs(mesh_cell_size_float.y) + 1;
    nav_mesh_size.z = fabs(mesh_cell_size_float.z) + 1;
    
    Vector3 cell_start = Vector3Scale(mesh_min , nav_mesh_cell_size);
    
    Vector3 cell_end = Vector3Scale(mesh_max , nav_mesh_cell_size);
    cell_end.x -= nav_mesh_cell_size * 0.5f;
    cell_end.y -= nav_mesh_cell_size * 0.5f;
    cell_end.z -= nav_mesh_cell_size * 0.5f;
    
    nav_mesh_start_box = get_box();
    nav_mesh_start_box.size = (Vector3){nav_mesh_cell_size , nav_mesh_cell_size , nav_mesh_cell_size};
    nav_mesh_start_box.position = Vector3Scale(mesh_min , nav_mesh_cell_size);
    nav_mesh_start_box.position.x -= nav_mesh_cell_size * 0.5;
    nav_mesh_start_box.position.y -= nav_mesh_cell_size * 0.5;
    nav_mesh_start_box.position.z -= nav_mesh_cell_size * 0.5;
    
    nav_mesh_whole_box = get_box();
    nav_mesh_whole_box.position = Vector3Lerp(cell_start , cell_end , 0.5f);
    nav_mesh_whole_box.size.x = nav_mesh_size.x * nav_mesh_cell_size;
    nav_mesh_whole_box.size.y = nav_mesh_size.y * nav_mesh_cell_size;
    nav_mesh_whole_box.size.z = nav_mesh_size.z * nav_mesh_cell_size;
    
    int new_nav_mesh_count = nav_mesh_size.x * nav_mesh_size.y * nav_mesh_size.z;
    int new_nav_mesh_capacity = 1;
    for(; new_nav_mesh_count > new_nav_mesh_capacity; new_nav_mesh_capacity*=2);
    if(new_nav_mesh_capacity > nav_mesh_cell_capacity)
    {
        nav_mesh_cell_capacity = new_nav_mesh_capacity;
        nav_mesh_cell = allocate_memory_type(CellData , nav_mesh_cell_capacity , AT_temp);
    }
    
    for(int cell_index = 0; cell_index < new_nav_mesh_count ; cell_index++)
    {
        CellData cell_data = {};
        cell_data.blocked = false;
        cell_data.search_index = 0;
        cell_data.cost = 0;
        
        nav_mesh_cell[cell_index] = cell_data;
    }
    
    array_foreach(box_index , &box_in_map_array)
    {
        Box box = box_in_map_buffer.data[box_index];
        Vector3 * box_vertices = box_to_point(box);
        
        Vector3 box_max = {-FLT_MAX , -FLT_MAX , -FLT_MAX};
        Vector3 box_min = {FLT_MAX , FLT_MAX , FLT_MAX};
        get_bound(box_to_point(box) , box_vertex_count , &box_max , &box_min);
        
        box_max = position_to_grid(box_max , nav_mesh_cell_size);
        box_min = position_to_grid(box_min , nav_mesh_cell_size);
        
        box_max = Vector3Scale(box_max , 1.0 / nav_mesh_cell_size);
        box_min = Vector3Scale(box_min , 1.0 / nav_mesh_cell_size);
        
        Int3 box_cell_max = {};
        box_cell_max.x = box_max.x;
        box_cell_max.y = box_max.y;
        box_cell_max.z = box_max.z;
        
        Int3 box_cell_min = {};
        box_cell_min.x = box_min.x;
        box_cell_min.y = box_min.y;
        box_cell_min.z = box_min.z;
        
        for(int x = box_cell_min.x, y = box_cell_min.y, z = box_cell_min.z;;)
        {
            Vector3 cell_position = {x , y ,z};
            cell_position = Vector3Scale(cell_position , nav_mesh_cell_size);
            cell_position.x -= nav_mesh_cell_size * 0.5f;
            cell_position.y -= nav_mesh_cell_size * 0.5f;
            cell_position.z -= nav_mesh_cell_size * 0.5f;
            
            Box cell_box = get_box();
            cell_box.position = cell_position;
            cell_box.size = (Vector3){nav_mesh_cell_size , nav_mesh_cell_size , nav_mesh_cell_size};
            //draw_box_line(cell_box , Fade(WHITE , 0.1) , 5);
            
            if(check_shape((Vector3){} , box_to_point(cell_box) , box_vertex_count , box_vertices , box_vertex_count , 0))
            {
                Int3 cell = {};
                cell.x = x - nav_mesh_start.x;
                cell.y = y - nav_mesh_start.y;
                cell.z = z - nav_mesh_start.z;
                
                nav_mesh_cell[cell_to_index(cell)].blocked = true;
            }
            
            x++;
            if(x > box_cell_max.x)
            {
                x = box_cell_min.x;
                y++;
            }
            if(y > box_cell_max.y)
            {
                y = box_cell_min.y;
                z++;
            }
            if(z > box_cell_max.z)
            {
                break;
            }
        }
    }
}

//this is extremely slow?
internal PathResult function path_finding(Vector3 start , Vector3 end)
{
    PathResult result = {};
    result.path_found = false;
    
    search_index++;
    
    start = Vector3Subtract(start , nav_mesh_start_box.position);
    start = position_to_grid(start , nav_mesh_cell_size);
    start = Vector3Scale(start , 1.0 / nav_mesh_cell_size); 
    Int3 start_cell = {start.x , start.y , start.z};
    
    end = Vector3Subtract(end , nav_mesh_start_box.position);
    end = position_to_grid(end , nav_mesh_cell_size);
    end = Vector3Scale(end , 1.0 / nav_mesh_cell_size); 
    Int3 end_cell = {end.x , end.y , end.z};
    
    if(start_cell.x >= nav_mesh_size.x) return result;
    if(start_cell.y >= nav_mesh_size.y) return result;
    if(start_cell.y >= nav_mesh_size.z) return result;
    if(start_cell.x < 0) return result;
    if(start_cell.y < 0) return result;
    if(start_cell.y < 0) return result;
    
    if(end_cell.x >= nav_mesh_size.x) return result;
    if(end_cell.y >= nav_mesh_size.y) return result;
    if(end_cell.y >= nav_mesh_size.z) return result;
    if(end_cell.x < 0) return result;
    if(end_cell.y < 0) return result;
    if(end_cell.y < 0) return result;
    
    if(start_cell.x == end_cell.x)
    {
        if(start_cell.y == end_cell.y)
        {
            if(start_cell.z == end_cell.z)
            {
                return result;
            }
        }
    }
    
    int start_cell_index = cell_to_index(start_cell);
    CellData * start_cell_data = nav_mesh_cell + start_cell_index;
    start_cell_data->cost = 0;
    
    int search_queue_head = 0;
    int search_queue_tail = 0;
    search_queue[search_queue_tail++] = start_cell;
    bool path_found = false;
    
    for(;;)
    {
        if(path_found) break;
        if(search_queue_head == search_queue_tail) break;
        Int3 search_cell = search_queue[search_queue_head++];
        if(search_queue_head >= search_queue_capacity) search_queue_head = 0;
        
        int search_cell_index = cell_to_index(search_cell);
        CellData * search_cell_data = nav_mesh_cell + search_cell_index;
        search_cell_data->search_index = search_index;
        
        for(int x = 0 , y = 0 , z = 0 ;;)
        {
            Int3 cell = {x , y  , z };
            if(cell.x > 1) cell.x = -1;
            if(cell.y > 1) cell.y = -1;
            if(cell.z > 1) cell.z = -1;
            
            cell.x += search_cell.x;
            cell.y += search_cell.y;
            cell.z += search_cell.z;
            
            bool skip = false;
            if(cell.x >= nav_mesh_size.x) skip = true;
            if(cell.x < 0) skip = true;
            if(cell.y >= nav_mesh_size.y) skip = true;
            if(cell.y < 0) skip = true;
            if(cell.z >= nav_mesh_size.z) skip = true;
            if(cell.z < 0) skip = true;
            
            if(skip)
            {
                goto ITERATE_CELL;
            }
            
            int cell_index = cell_to_index(cell);
            CellData * cell_data = nav_mesh_cell + cell_index;
            
            if(cell.x == end_cell.x)
            {
                if(cell.y == end_cell.y)
                {
                    if(cell.z == end_cell.z)
                    {
                        cell_data->previous_cell = search_cell;
                        path_found = true;
                        break;
                    }
                }
            }
            
            bool add_to_stack = false;
            
            float x_cost = x - 1;
            x_cost *= x_cost;
            
            float y_cost = y - 1;
            y_cost *= y_cost;
            
            float z_cost = z - 1;
            z_cost *= z_cost;
            
            float cell_cost = ( x_cost + y_cost + z_cost) + search_cell_data->cost;
            cell_cost += (end_cell.x - cell.x) * (end_cell.x - cell.x) + (end_cell.y - cell.y) * (end_cell.y - cell.y) + (end_cell.z - cell.z) * (end_cell.z - cell.z);
            
            if(cell_data->search_index != search_index)
            {
                add_to_stack = true;
            }
            else
            {
                if(cell_data->cost > cell_cost)
                {
                    add_to_stack = true;
                }
            }
            
            if(add_to_stack)
            {
                cell_data->cost = cell_cost;
                cell_data->previous_cell = search_cell;
                cell_data->search_index = search_index;
                
                int new_cell_index = search_queue_tail++;
                search_queue[new_cell_index] = cell;
                
                for(int queue_index = new_cell_index;;)
                {
                    if(queue_index == search_queue_head) break;
                    int previous_queue_index = queue_index-1;
                    
                    CellData * current_cell_data = nav_mesh_cell + cell_to_index(search_queue[queue_index]);
                    CellData * previous_cell_data = nav_mesh_cell + cell_to_index(search_queue[previous_queue_index]);
                    
                    if(current_cell_data->cost < previous_cell_data->cost)
                    {
                        Int3 temp_cell = search_queue[previous_queue_index];
                        search_queue[previous_queue_index] = search_queue[queue_index];
                        search_queue[queue_index] = temp_cell;
                    }
                    else
                    {
                        break;
                    }
                    
                    queue_index--;
                    if(queue_index < 0) queue_index = search_queue_capacity - 1;
                }
                
                if(search_queue_tail >= search_queue_capacity) search_queue_tail = 0;
                if(search_queue_tail == search_queue_head)
                {
                    int old_queue_capacity = search_queue_capacity;
                    search_queue_capacity *= 2;
                    if(search_queue_capacity > 10000) CATCH;
                    
                    Int3 * new_queue = allocate_memory_type(Int3 , search_queue_capacity , AT_temp);
                    
                    int new_queue_index = 0;
                    for(int queue_index = search_queue_head; queue_index < old_queue_capacity; queue_index++ , new_queue_index++)
                    {
                        new_queue[new_queue_index] = search_queue[queue_index];
                    }
                    
                    for(int queue_index = 0; queue_index < search_queue_head ; queue_index++ , new_queue_index++)
                    {
                        new_queue[new_queue_index] = search_queue[queue_index];
                    }
                    
                    search_queue = new_queue;
                    search_queue_head = 0;
                    search_queue_tail = old_queue_capacity - 1;
                }
            }
            
            ITERATE_CELL:
            
            x++;
            if(x >= 3)
            {
                x = 0;
                y++;
            }
            if(y >= 3)
            {
                y = 0;
                z++;
            }
            if(z >= 3)
            {
                break;
            }
        }
    }
    
    if(path_found)
    {
        result.path_found = true;
        allocate_buffer( &result.buffer , Int3 , 64 , AT_frame);
        
        Int3 cell = end_cell;
        
        for(;;)
        {
            CellData * cell_data = nav_mesh_cell + cell_to_index(cell);
            if(result.buffer.count == result.buffer.capacity)
            {
                reallocate_buffer(&result.buffer , AT_frame);
            }
            Int3 * new_path_cell = result.buffer.data + result.buffer.count++;
            (*new_path_cell) = cell;
            
            if(cell.x == start_cell.x)
            {
                if(cell.y == start_cell.y)
                {
                    if(cell.z == start_cell.z)
                    {
                        break;
                    }
                }
            }
            
            cell = cell_data->previous_cell;
        }
    }
    
    return result;
}

internal bool function load_data_from_file(char * path)
{
    FILE * game_save_file = fopen(path , "rb");
    
    if(!game_save_file) return false;
    
    int save_header_size = 0;
    fread(&save_header_size , sizeof(int), 1, game_save_file);
    
    save_header_count = save_header_size / sizeof(DataHeader);
    
    int save_size = 0;
    fread(&save_size , sizeof(int), 1, game_save_file);
    
    scratch_buffer_for_read = malloc(save_header_size + save_size);
    
    fread(scratch_buffer_for_read , save_header_size + save_size , 1 , game_save_file);
    
    fclose(game_save_file);
    
    data_header_array = (DataHeader*)scratch_buffer_for_read;
    current_data_header = data_header_array;
    
    save_memory = scratch_buffer_for_read + save_header_size;
    current_save_memory_location = save_memory;
    
    return true;
}

internal bool function load_map()
{
    if(!load_data_from_file(get_app_file_path(map_save_name))) return false;
    
    int quad_count = 0;
    int quad_capacity = 1;
    READ_DATA(quad_count , "map_quad_count" , int);
    for( ;quad_capacity < quad_count; quad_capacity *= 2 );
    
    quad_in_map_array = allocate_array(quad_capacity , AT_temp);
    allocate_buffer( &quad_in_map_buffer , Quad , quad_capacity , AT_temp);
    
    for(int quad_index = 0 ; quad_index < quad_count ; quad_index++)
    {
        Quad * quad = quad_in_map_buffer.data + add_to_array(&quad_in_map_array);
        
        READ_BUFFER(quad->vertex_position[vertex_top_left] , "map_quad_top_left_vertex" , Vector3 , quad_index);
        READ_BUFFER(quad->vertex_position[vertex_top_right] , "map_quad_top_right_vertex" , Vector3 , quad_index);
        READ_BUFFER(quad->vertex_position[vertex_bottom_left] , "map_quad_bottom_left_vertex" , Vector3 , quad_index);
        READ_BUFFER(quad->vertex_position[vertex_bottom_right] , "map_quad_bottom_right_vertex" , Vector3 , quad_index);
    }
    
    int box_count = 0;
    int box_capacity = 1;
    READ_DATA(box_count , "map_box_count" , int);
    for(;box_capacity < box_count; box_capacity *= 2);
    box_in_map_array = allocate_array(box_capacity , AT_temp);
    allocate_buffer(&box_in_map_buffer , Box , box_capacity , AT_temp);
    
    for(int box_index = 0 ; box_index < box_count ; box_index++)
    {
        Box * box = box_in_map_buffer.data + add_to_array(&box_in_map_array);
        
        READ_BUFFER(box->position , "map_box_position" , Vector3 , box_index);
        READ_BUFFER(box->size , "map_box_size" , Vector3 , box_index);
        READ_BUFFER(box->rotation , "map_box_rotation" , Quaternion , box_index);
        
        READ_BUFFER(box->top_front , "map_box_top_front" , float , box_index);
        READ_BUFFER(box->top_right , "map_box_top_right" , float , box_index);
        READ_BUFFER(box->right_top , "map_box_right_top" , float , box_index);
        READ_BUFFER(box->right_front , "map_box_right_front" , float , box_index);
        READ_BUFFER(box->front_top , "map_box_front_top" , float , box_index);
        READ_BUFFER(box->front_right , "map_box_front_right" , float , box_index);
    }
    
    int entity_count = 0;
    int entity_capacity = 1;
    READ_DATA(entity_count , "entity_count" , int);
    for(;entity_capacity < entity_count; entity_capacity *= 2);
    entity_layout_array = allocate_array(entity_capacity , AT_temp);
    allocate_buffer(&entity_layout_buffer , Entity , entity_capacity , AT_temp);
    
    for(int entity_index = 0; entity_index < entity_count ; entity_index++)
    {
        int new_entity_index = add_to_array(&entity_layout_array);
        Entity * entity = entity_layout_buffer.data + new_entity_index;
        
        READ_BUFFER(entity->type , "entity_type" , int , entity_index);
        READ_BUFFER(entity->position , "entity_position" , Vector3 , entity_index);
        READ_BUFFER(entity->box.rotation , "entity_rotation" , Quaternion , entity_index);
        READ_BUFFER(entity->box.size , "entity_size" , Vector3 , entity_index);
        READ_BUFFER(entity->generation_index , "entity_generation_index" , int ,entity_index);
        
        entity->box.position = entity->position;
        //entity->player_index = -1;
        entity->entity_index = entity_index;
    }
    
#ifdef BUILD_D_WINDOWS
    
    int trigger_box_count = 0;
    int trigger_box_capacity = 1;
    READ_DATA(trigger_box_count , "camera_trigger_count" , int);
    for(;trigger_box_capacity < trigger_box_count ; trigger_box_capacity*=2);
    camera_array = allocate_array(trigger_box_capacity , AT_temp);
    allocate_buffer(&camera_buffer , CameraTrigger , trigger_box_capacity , AT_temp);
    allocate_buffer(&camera_zone_buffer , Box , trigger_box_capacity , AT_temp);
    
    for(int trigger_box_index = 0 ; trigger_box_index < trigger_box_count ; trigger_box_index++)
    {
        int new_camera_index = add_to_array(&camera_array);
        Box * zone = camera_zone_buffer.data + new_camera_index;
        CameraTrigger * trigger = camera_buffer.data + new_camera_index;
        
        READ_BUFFER(zone->position , "camera_zone_position" , Vector3 , trigger_box_index);
        READ_BUFFER(zone->size , "camera_zone_size" , Vector3 , trigger_box_index);
        READ_BUFFER(zone->rotation , "camera_zone_rotation" , Quaternion , trigger_box_index);
        READ_BUFFER(trigger->camera_target_offset , "camera_zone_offset" , Vector3 , trigger_box_index);
    }
    
    READ_DATA(selected_reference_frame_index , "selected reference frame" , int);
    
    int reference_frame_count = 0;
    READ_DATA(reference_frame_count , "reference frame count" , int);
    
    for(int reference_frame_index = 0 ; reference_frame_index < reference_frame_count ; reference_frame_index++)
    {
        Vector3 refernce_frame = {};
        READ_BUFFER(refernce_frame , "reference frame" , Vector3 , reference_frame_index);
        
        if(list_full(&reference_frame_list))
        {
            reallocate_list(&reference_frame_list , AT_temp);
            reallocate_buffer( &reference_frame_buffer , AT_temp);
        }
        
        int new_reference_frame_index= add_to_list_tail_B(&reference_frame_list);
        reference_frame_buffer.data[new_reference_frame_index] = refernce_frame;
    }
#endif
    
    free(scratch_buffer_for_read);
    
    return true;
}

internal void function add_box_to_bounding_box_buffer(Box box , Vector3 velocity , ShapeOwner owner , BoundingBoxNodeBuffer * bounding_box_buffer)
{
    Vector3 box_vertices[box_vertex_count * 2] = {};
    
    Box box_after_velocity = box;
    box_after_velocity.position = Vector3Add(box_after_velocity.position , velocity);
    
    Vector3 * box_before_velocity_vertices = box_to_point(box);
    Vector3 * box_after_velocity_vertices = box_to_point(box_after_velocity);
    
    for(int box_index = 0; box_index < box_vertex_count; box_index++) box_vertices[box_index] = box_before_velocity_vertices[box_index];
    for(int box_index = 0; box_index < box_vertex_count; box_index++) box_vertices[box_index + box_vertex_count] = box_after_velocity_vertices[box_index];
    
    Vector3 right_top_forward = {-FLT_MAX , -FLT_MAX , -FLT_MAX};
    Vector3 left_bottom_backward = {FLT_MAX , FLT_MAX , FLT_MAX};
    
    get_bound(box_vertices , box_vertex_count * 2 , &right_top_forward , &left_bottom_backward);
    
    if(buffer_full(*bounding_box_buffer))
    {
        reallocate_buffer(bounding_box_buffer , AT_frame);
    }
    
    BoundingBoxNode * new_bounding_box = bounding_box_buffer->data + bounding_box_buffer->count++;
    new_bounding_box->right_top_forward = right_top_forward;
    new_bounding_box->left_bottom_backward = left_bottom_backward;
    new_bounding_box->shape.type = ST_box;
    new_bounding_box->shape.box = box;
    new_bounding_box->shape.velocity = velocity;
    new_bounding_box->shape.owner = owner;
    new_bounding_box->left = 0;
    new_bounding_box->right = 0;
}

internal float function snap_to_fixed_angle(float x)
{
    float a[5] = {};
    
    a[0] = -1.0; 
    a[1] = -0.70710678118; 
    a[2] = 0.0; 
    a[3] = 0.70710678118; 
    a[4] = 1.0;
    
    int closest_index = 0;
    float closest = FLT_MAX;
    for(int i = 0 ; i < 5 ; i++)
    {
        float distance = fabs(x - a[i]);
        
        if(closest > distance) 
        {
            closest = distance;
            closest_index = i;
        }
    }
    
    return a[closest_index];
}

internal EntityHandle function entity_to_handle(Entity * entity)
{
    EntityHandle handle = {};
    handle.entity_index = entity->entity_index;
    handle.generation_index = entity->generation_index;
    return handle;
}

internal Entity * function handle_to_entity(EntityHandle handle)
{
    if(handle.entity_index < 0) CATCH;
    if(handle.entity_index >= entity_active_array.upper_bound) return 0;
    Entity * entity = entity_active_buffer.data + handle.entity_index;
    if(entity->respawning) return 0;
    if(!entity_active_array.valid_array[handle.entity_index]) return 0;
    if(handle.generation_index != entity->generation_index) return 0;
    if(handle.entity_index != entity->entity_index) CATCH;
    
    return entity;
}

internal bool function ground_spring(Box box, Vector3 * velocity , Vector3 ray, float spring_percent , float max_slop_angle, Vector3 * ground_normal , float * ground_hit_time , ShapeOwner * ground_owner)
{
    bool grounded = false;
    
    //ray = Vector3Add(ray , Vector3Project(Vector3Scale(box.size , -0.5f) , ray ));
    
    ConvexShape ray_cast_shape = {};
    ray_cast_shape.shape.box = box;
    ray_cast_shape.shape.type = ST_box;
    ray_cast_shape.velocity = ray;
    ray_cast_shape.position = box.position;
    //ray_cast_shape.capture_collision = true;
    
    collision_push_root(bounding_box_in_map_root);
    RayCastResultBuffer ray_cast_result_buffer = convex_shape_ray_cast(ray_cast_shape);
    
    float closest_ground_hit_time = FLT_MAX;
    Vector3 closest_ground_normal = {};
    ShapeOwner closest_ground_owner = {};
    
    for(int buffer_index = 0 ; buffer_index < ray_cast_result_buffer.count ; buffer_index++)
    {
        RayCastResult result = ray_cast_result_buffer.data[buffer_index];
        
        Vector3 surface_normal = Vector3Normalize(result.surface_normal);
        float ground_angle = Vector3Angle((Vector3){0,1,0} , surface_normal) * RAD2DEG;
        
        if(closest_ground_hit_time > result.hit_time)
        {
            if(ground_angle < max_slop_angle)
            {
                grounded = true;
                
                closest_ground_hit_time = result.hit_time;
                closest_ground_normal = surface_normal;
                closest_ground_owner = result.shape_owner;
            }
        }
    }
    
    if(ground_normal) (*ground_normal) = closest_ground_normal;
    if(ground_hit_time) (*ground_hit_time) = closest_ground_hit_time;
    
    if(grounded)
    {
        //i don't think i'm gonna stand on a wall but i make it possible anyway
        Vector3 velocity_on_ray = Vector3Project((*velocity) , ray);
        Vector3 spring = Vector3Scale(ray , (closest_ground_hit_time - spring_percent) * 2.0f);
        spring = Vector3Add(spring , velocity_on_ray);
        spring = Vector3Scale(spring , 0.2f);
        (*velocity) = Vector3Subtract((*velocity) , velocity_on_ray);
        (*velocity) = Vector3Add((*velocity) , spring);
        if(ground_owner) (*ground_owner) = closest_ground_owner;
        
        //float spring = ray.y * (closest_ground_hit_time - spring_percent) + player->velocity.y;
        //spring *= 0.5f;
        //player->velocity.y = spring;
    }
    
    return grounded;
}

internal Vector3 function vector3_abs(Vector3 vector)
{
    vector.x = fabs(vector.x);
    vector.y = fabs(vector.y);
    vector.z = fabs(vector.z);
    
    return vector;
}

internal Quaternion function quaternion_from_vector3_to_vector3(Vector3 from , Vector3 to)
{
    Quaternion rotation = QuaternionIdentity();
    float dot_product = Vector3DotProduct(from , to);
    if(dot_product < -0.999999f)
    {
        rotation = QuaternionFromEuler(0 , DEG2RAD * 90 , 0);
    }
    else if(dot_product < 0.999999f)
    {
        Quaternion result = { 0 };
        
        float cos2Theta = (from.x*to.x + from.y*to.y + from.z*to.z);    // Vector3DotProduct(from, to)
        Vector3 cross = { from.y*to.z - from.z*to.y, from.z*to.x - from.x*to.z, from.x*to.y - from.y*to.x }; // Vector3CrossProduct(from, to)
        
        result.x = cross.x;
        result.y = cross.y;
        result.z = cross.z;
        result.w = 1.0f + cos2Theta;
        
        // QuaternionNormalize(q);
        // NOTE: Normalize to essentially nlerp the original and identity to 0.5
        Quaternion q = result;
        float length = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
        if (length == 0.0f) length = 1.0f;
        float ilength = 1.0f/length;
        
        result.x = q.x*ilength;
        result.y = q.y*ilength;
        result.z = q.z*ilength;
        result.w = q.w*ilength;
        
        rotation = result;
    }
    
    return rotation;
}

internal void function update_player_box(Player * player)
{
    player->box = get_box();
    player->box.position = player->position;
    player->box.size = player_box_size;
    player->collide_box = player->box;
    
    player->collide_box.position = Vector3Subtract( player->collide_box.position , Vector3Scale(player_spring_ray , 0.5f * player_spring_percent));
    player->collide_box.size = Vector3Subtract(player->collide_box.size , Vector3Scale(vector3_abs(player_spring_ray) , player_spring_percent));
}

internal void function copy_from_layout(Entity * entity , int layout_entity_index)
{
    if(entity_layout_array.valid_array[layout_entity_index])
    {
        Entity * layout_entity = entity_layout_buffer.data + layout_entity_index;
        
        (*entity) = (*layout_entity);
        entity->box.size.x -= 0.05 * UNIT_SIZE;
        entity->box.size.y -= 0.05 * UNIT_SIZE;
        entity->box.size.z -= 0.05 * UNIT_SIZE;
        entity->respawnable = true;
        entity->previous_position = entity->position;
        entity->velocity = (Vector3){};
        
        entity->respawning = false;
        entity->clearable = false;
        
        bool fixed_size = false;
        if(entity->type == E_hook) fixed_size = true;
        if(entity->type == E_small_block) fixed_size = true;
        if(entity->type == E_bubble) fixed_size = true;
        
        if(fixed_size)
        {
            entity->box.size.x = GRID_SIZE * 0.5f;
            entity->box.size.y = GRID_SIZE * 0.5f;
            entity->box.size.z = GRID_SIZE * 0.5f;
        }
    }
}

internal Player * function index_to_player(int player_index)
{
    if(!player_pool.array.valid_array[player_index]) return 0;
    if(player_pool.buffer.data[player_index].previous != -1) return 0;
    Player * player = (Player*)(player_pool.memory_start + (player_index * player_pool.chunk_size));
    
    return player;
}

internal void function world_update()
{
    if(!entity_initialized)
    {
        entity_initialized = true;
        clear_array(&entity_active_array);
        
        int entity_capacity = entity_layout_array.capacity;
        
        if(entity_active_array.capacity < entity_capacity)
        {
            entity_active_array = allocate_array(entity_capacity , AT_temp);
            allocate_buffer(&entity_active_buffer , Entity , entity_capacity , AT_temp);
        }
        
        entity_active_array.count = entity_layout_array.count;
        entity_active_array.upper_bound = entity_layout_array.upper_bound;
        entity_active_array.lowest_index = entity_layout_array.lowest_index;
        
        for(int entity_index = 0 ; entity_index < entity_capacity ; entity_index++)
        {
            entity_active_array.valid_array[entity_index] = entity_layout_array.valid_array[entity_index];
            
            copy_from_layout(entity_active_buffer.data + entity_index , entity_index);
        }
    }
    
#ifdef BUILD_D_WINDOWS
    glClearDepth(1);
    glClear(GL_DEPTH_BUFFER_BIT);
#endif
    
#if 0
    double nav_mesh_time = time_stamp();
    generate_nav_mesh();
    nav_mesh_time = (time_stamp() - nav_mesh_time) / (1000.0f);
    printf( "nav mesh time : %f\n" , nav_mesh_time );
#endif
    
    double shape_tree_time = time_stamp();
    
    BoundingBoxNodeBuffer bounding_box_buffer = {};
    allocate_buffer( &bounding_box_buffer , BoundingBoxNode , 128 , AT_frame);
    
    array_foreach(box_index , &box_in_map_array)
    {
        Box box = box_in_map_buffer.data[box_index];
        ShapeOwner owner = {};
        owner.is_entity = false;
        
        add_box_to_bounding_box_buffer(box , (Vector3){} , owner , &bounding_box_buffer );
    }
    
    //TODO: quads are missing???
    //something wrong with editor
    //quad was spawn on the other side of origin
    array_foreach(quad_index , &quad_in_map_array)
    {
        Quad quad = quad_in_map_buffer.data[quad_index];
        
        Vector3 right_top_forward = {-FLT_MAX , -FLT_MAX , -FLT_MAX};
        Vector3 left_bottom_backward = {FLT_MAX , FLT_MAX , FLT_MAX};
        
        get_bound(quad.vertex_position , quad_vertex_count , &right_top_forward , &left_bottom_backward);
        
        if(buffer_full(bounding_box_buffer))
        {
            reallocate_buffer(&bounding_box_buffer , AT_frame);
        }
        
        BoundingBoxNode * new_bounding_box = bounding_box_buffer.data + bounding_box_buffer.count++;
        new_bounding_box->right_top_forward = right_top_forward;
        new_bounding_box->left_bottom_backward = left_bottom_backward;
        new_bounding_box->shape.type = ST_quad;
        new_bounding_box->shape.quad = quad;
        new_bounding_box->left = 0;
        new_bounding_box->right = 0;
    }
    
    array_foreach(entity_index , &entity_active_array)
    {
        Entity * entity = entity_active_buffer.data + entity_index;
        if(entity->respawning) continue;
        
        bool collidable_entity = false;
        if(entity->type == E_moving_wall) collidable_entity = true;
        if(entity->solid) collidable_entity = true;
        
        if(!collidable_entity) continue;
        
        ShapeOwner owner = {};
        owner.is_entity = true;
        owner.entity_handle = entity_to_handle(entity);
        
        Box entity_box = entity->box;
        entity_box.position = entity->previous_position;
        add_box_to_bounding_box_buffer(entity_box , Vector3Subtract(entity->position , entity->previous_position) , owner , &bounding_box_buffer);
    }
    
    bounding_box_in_map_root = split_bounding_box(bounding_box_buffer.data , bounding_box_buffer.count , split_yz , 0);
    
    shape_tree_time = (time_stamp() - shape_tree_time) / (1000.0);
    
    BoundingBoxNodeBuffer player_bounding_box_buffer = {};
    int player_count = 0;
    array_foreach(player_index , &player_pool.array)
    {
        if(!index_to_player(player_index)) continue;
        player_count++;
    }
    allocate_buffer(&player_bounding_box_buffer , BoundingBoxNode , player_count , AT_frame);
    
    array_foreach(player_index , &player_pool.array)
    {
        Player * player = index_to_player(player_index);
        if(!player) continue;
        
        ShapeOwner owner = {};
        owner.is_player = true;
        owner.player_index = player_index;
        add_box_to_bounding_box_buffer(player->box , player->velocity , owner , &player_bounding_box_buffer);
    }
    
    BoundingBoxNode * player_bounding_box_root = split_bounding_box(player_bounding_box_buffer.data , player_bounding_box_buffer.count , split_yz , 0);
    
    array_foreach(entity_index , &entity_active_array)
    {
        Entity * entity = entity_active_buffer.data + entity_index;
        
        if(entity->clearable)
        {
            entity->clear_timer += DELTA_TIME;
            
            float clear_time = default_entity_clear_time;
            if(entity->override_clear_time) clear_time = entity->clear_time;
            if(entity->clear_timer >= clear_time)
            {
                entity->cleared = true;
            }
        }
        
        if(entity->cleared)
        {
            if(entity->respawnable)
            {
                if(!entity->respawning) entity->respawn_timer = 0;
                entity->respawning = true;
            }
            else
            {
                delete_from_array(&entity_active_array , entity_index);
                continue;
            }
        }
        
        if(entity->respawning) 
        {
            entity->respawn_timer += DELTA_TIME;
            
            float respawn_time = default_entity_respawn_time;
            if(entity->override_respawn_time) respawn_time = entity->respawn_time;
            
            if(entity->respawn_timer < respawn_time)
            {
                continue;
            }
            else
            {
                copy_from_layout(entity , entity_index);
            }
        }
        
        if(!entity->stop_expanding)
        {
            if(entity->expanding)
            {
                float expand_speed = UNIT_SIZE * 0.8f;
                Vector3 direction = Vector3Normalize(entity->charge_direction);
                Vector3 expand_size = Vector3Scale(direction , expand_speed);
                
                entity->box.size = Vector3Add(entity->box.size , vector3_abs(expand_size));
                entity->position = Vector3Add(entity->position , Vector3Scale(expand_size , 0.5f));
                
                entity->position = Vector3Add(entity->position , Vector3Scale(direction , UNIT_SIZE * 0.02f));
                
                ConvexShape trigger = {};
                trigger.shape.type = ST_box;
                trigger.shape.box = entity->box;
                trigger.velocity = (Vector3){};
                trigger.position = entity->box.position;
                
                collision_push_root(bounding_box_in_map_root);
                ShapeUnionBuffer shape_buffer = convex_shape_overlap(trigger);
                
                if(shape_buffer.count > 0) entity->stop_expanding = true;
            }
        }
        
        entity->box.position = entity->position;
        
        //Player * player = index_to_player(entity->player_index);
        //Player * player = 0;
        
        if(entity->charging)
        {
            Vector3 charge_force = Vector3Normalize(entity->charge_direction);
            
            entity->charge_timer += DELTA_TIME;
            if(entity->charge_timer > 1.42) entity->charging = false;
            
            if(entity->type == E_moving_wall || entity->solid)
            {
                float target_force = 0.008f * UNIT_SIZE;
                float max_force = 0.4f * UNIT_SIZE;
                float apply_force = max_force - Vector3Length(entity->velocity);
                if(apply_force < 0) apply_force = 0;
                if(apply_force > target_force) apply_force = target_force;
                
                charge_force = Vector3Scale(charge_force , target_force);
                entity->velocity = Vector3Add(entity->velocity , charge_force);
            }
            
            if(entity->type == E_hook) 
            {
                entity->velocity = Vector3Scale(charge_force , 1.0f * UNIT_SIZE);
            }
            
            Box box_to_stop = entity->box;
            box_to_stop.size.x -= 0.01 * UNIT_SIZE;
            box_to_stop.size.y -= 0.01 * UNIT_SIZE;
            box_to_stop.size.z -= 0.01 * UNIT_SIZE;
            
            Vector3 box_offset = Vector3Normalize(entity->charge_direction);
            box_offset = Vector3Scale(box_offset , UNIT_SIZE * 0.02f);
            box_to_stop.position = Vector3Add(box_to_stop.position , box_offset);
            box_to_stop.size = Vector3Add(box_to_stop.size , vector3_abs(box_offset));
            
            ConvexShape trigger = {};
            trigger.shape.type = ST_box;
            trigger.shape.box = box_to_stop;
            trigger.velocity = (Vector3){};
            trigger.position = box_to_stop.position;
            
            collision_push_root(bounding_box_in_map_root);
            ShapeUnionBuffer shape_buffer = convex_shape_overlap(trigger);
            
            //draw_box_line(box_to_stop , Fade(YELLOW , 0.5f) , 5);
            
            for(int buffer_index = 0; buffer_index < shape_buffer.count; buffer_index++)
            {
                ShapeOwner owner = shape_buffer.data[buffer_index].owner;
                if(owner.is_entity)
                {
                    if(handle_to_entity(owner.entity_handle) == entity)
                    {
                        continue;
                    }
                }
                
                if(entity->type == E_moving_wall)
                {
                    entity->velocity = (Vector3){};
                    entity->charging = false;
                    entity->clearable = true;
                }
                else if(entity->type == E_hook)
                {
                    entity->charging = false;
                    //entity->unpickable = false;
                    entity->velocity = (Vector3){};
                    entity->hook_on_surface = true;
                }
                break;
            }
        }
        
        bool detoned = false;
        
        if(key_pressed(KEY_U))
        {
            if(entity->detonable) detoned = true;
        }
        
        if(detoned)
        {
            entity->cleared = true;
            
            Box bomb_trigger_box = entity->box;
            
            float bomb_radius = GRID_SIZE * 4.0;
            bomb_trigger_box.size = (Vector3){bomb_radius , bomb_radius , bomb_radius};
            
            ConvexShape bomb_trigger = {};
            bomb_trigger.shape.type = ST_box;
            bomb_trigger.shape.box = bomb_trigger_box;
            bomb_trigger.velocity = (Vector3){0 , 0 , 0};
            bomb_trigger.position = bomb_trigger_box.position;
            bomb_trigger.capture_collision = false;
            
            collision_push_root(player_bounding_box_root);
            collision_push_root(bounding_box_in_map_root);
            ShapeUnionBuffer hit_buffer = convex_shape_overlap(bomb_trigger);
            
            for(int hit_buffer_index = 0; hit_buffer_index < hit_buffer.count; hit_buffer_index++)
            {
                ShapeUnion hit_trigger =  hit_buffer.data[hit_buffer_index];
                ShapeOwner hit_owner = hit_trigger.owner;
                
                if(hit_owner.is_entity)
                {
                    Entity * hit_entity = handle_to_entity(hit_owner.entity_handle);
                }
                else if(hit_owner.is_player)
                {
                    Player * hit_player = index_to_player(hit_owner.player_index);
                    Vector3 hit_direction = Vector3Subtract(hit_player->position , entity->position);
                    hit_direction = Vector3Normalize(hit_direction);
                    Vector3 explosion_force = Vector3Scale(hit_direction , UNIT_SIZE * 1.0f);
                    //printf("%f %f %f : %f\n" , explosion_force.x , explosion_force.y , explosion_force.z , Vector3Length(explosion_force));
                    hit_player->velocity = Vector3Add(hit_player->velocity , explosion_force);
                }
            }
        }
        
        if(entity->position.y < -5 * GRID_SIZE)
        {
            entity->clearable = true;
        }
        
        bool has_physics = false;
        if(entity->type == E_moving_wall) has_physics = true;
        if(entity->solid) has_physics = true;
        
        if(has_physics)
        {
            if(!entity->charging)
            {
                entity->velocity_multipler = Lerp(entity->velocity_multipler , 0.9 , 0.2f);
                entity->gravity_force = Lerp(entity->gravity_force , UNIT_SIZE * 0.3f , 0.02f);
            }
            else
            {
                entity->velocity_multipler = Lerp(entity->velocity_multipler , 1.0 , 0.2f);
                entity->gravity_force = Lerp(entity->gravity_force , UNIT_SIZE * 0.01f , 0.3f);
            }
            
            entity->velocity.y -= entity->gravity_force;
            entity->velocity = Vector3Scale(entity->velocity , entity->velocity_multipler);
            
            ShapeOwner shape_owner = {};
            shape_owner.is_entity = true;
            shape_owner.entity_handle = entity_to_handle(entity);
            
            ConvexShape entity_shape = {};
            entity_shape.shape.owner = shape_owner;
            entity_shape.shape.type = ST_box;
            entity_shape.shape.box = entity->box;
            entity_shape.velocity = entity->velocity;
            entity_shape.position = entity->position;
            entity_shape.capture_collision = true;
            
            CollisionResult collision_result = update_collision(entity_shape);
            
            entity->position = Vector3Add(entity->position , collision_result.offset);
            entity->velocity = collision_result.velocity;
        }
        
        entity->previous_position = entity->position;
        entity->position = Vector3Add(entity->position , entity->velocity);
    }
    
    BoundingBoxNodeBuffer entity_bounding_box_buffer = {};
    allocate_buffer(&entity_bounding_box_buffer , BoundingBoxNode , entity_active_array.count , AT_frame);
    
    array_foreach(entity_index , &entity_active_array)
    {
        Entity * entity = entity_active_buffer.data + entity_index;
        if(entity->respawning) continue;
        
        bool pickable = false;
        if(entity->type == E_hook) pickable = true;
        if(entity->type == E_small_block) pickable = true;
        if(entity->type == E_bubble) pickable = true;
        if(!pickable) continue;
        
        ShapeOwner owner = {};
        owner.is_entity = true;
        owner.entity_handle = entity_to_handle(entity_active_buffer.data + entity_index);
        add_box_to_bounding_box_buffer(entity_active_buffer.data[entity_index].box , entity->velocity , owner , &entity_bounding_box_buffer);
    }
    
    BoundingBoxNode * entity_bounding_box_root = split_bounding_box(entity_bounding_box_buffer.data , entity_bounding_box_buffer.count , split_yz , 0);
    
    array_foreach(player_index , &player_pool.array)
    {
        Player * player = index_to_player(player_index);
        if(!player) continue;
        PlayerConnection * player_connection = player_connection_buffer.data + player->connection_index;
        input_state = &player_connection->input_state;
        
        update_player_box(player);
        
        float player_forward = 0;
        float player_right = 0;
        
        //this feel too clear
        //hope i won't regret it
        if(key_pressing(KEY_W)) player_forward += 1;
        if(key_pressing(KEY_S)) player_forward -= 1;
        if(key_pressing(KEY_D)) player_right += 1;
        if(key_pressing(KEY_A)) player_right -= 1;
        
        Vector3 player_forward_direction = Vector3Subtract(player->camera_target , player->camera_position);
        Vector3 player_right_direction = Vector3CrossProduct(player_forward_direction , (Vector3){0,1,0});
        player_forward_direction = Vector3CrossProduct((Vector3){0,1,0} , player_right_direction);
        
        player_forward_direction = Vector3Normalize(player_forward_direction);
        player_right_direction = Vector3Normalize(player_right_direction);
        
        Vector3 ground_normal = {0,1,0};
        
        if(player->velocity.y < 0)
        {
            player->moving_upward = false;
        }
        
        player->floating_in_bubble = false;
        
        ConvexShape player_trigger = {};
        player_trigger.shape.type = ST_box;
        player_trigger.shape.box = player->box;
        player_trigger.velocity = (Vector3){0 , 0 , 0};
        player_trigger.position = player->box.position;
        player_trigger.capture_collision = false;
        
        collision_push_root(entity_bounding_box_root);
        ShapeUnionBuffer trigger_bufffer = convex_shape_overlap(player_trigger);
        
        for(int buffer_index = 0; buffer_index < trigger_bufffer.count; buffer_index++)
        {
            ShapeUnion trigger = trigger_bufffer.data[buffer_index];
            if(trigger.type == ST_box)
            {
                ShapeOwner owner = trigger.owner;
                if(owner.is_entity)
                {
                    Entity * entity = handle_to_entity(owner.entity_handle);
                    
                    if(entity->expanding) player->floating_in_bubble = true;
                    
                    if(!entity->unpickable)
                    {
                        if(!player->is_holding_entity)
                        {
                            switch(entity->type)
                            {
                                //a little wacky
                                case E_hook:
                                case E_small_block:
                                case E_bubble:
                                
                                entity->clearable = false;
                                entity->clear_timer = 0;
                                entity->detonable = false;
                                player->holding_entity_handle = owner.entity_handle;
                                player->is_holding_entity = true;
                            }
                        }
                    }
                }
            }
            else
            {
                CATCH;
            }
        }
        
        bool stand_still_on_ground = false;
        bool stand_still = false;
        
        if((fabs(player_forward) + fabs(player_right)) == 0)
        {
            stand_still = true;
            if(player->grounded)
            {
                stand_still_on_ground = true;
            }
        }
        
        bool sliding = false;
        Vector3 sliding_surface_normal = {};
        
        Vector3 slide_direction = {};
        slide_direction = Vector3Add(slide_direction , Vector3Scale(player_forward_direction , player_forward));
        slide_direction = Vector3Add(slide_direction , Vector3Scale(player_right_direction , player_right));
        slide_direction = Vector3Scale(slide_direction , UNIT_SIZE * 2.0f);
        
        Box silde_box = player->box;
        silde_box.size.y -= 1 * UNIT_SIZE;
        
        ConvexShape player_slide_box = {};
        
        player_slide_box.shape.type = ST_box;
        player_slide_box.shape.box = silde_box;
        player_slide_box.velocity = slide_direction;
        player_slide_box.position = player->position;
        player_slide_box.capture_collision = false;
        
        collision_push_root(bounding_box_in_map_root);
        RayCastResultBuffer shape_slide_against_buffer = convex_shape_ray_cast(player_slide_box);
        
        if(!stand_still)
        {
            for(int buffer_index = 0; buffer_index < shape_slide_against_buffer.count; buffer_index++)
            {
                if(!player->grounded)
                {
                    if(player->velocity.y < 0.0001f)
                    {
                        //sliding = true;
                    }
                }
                
                RayCastResult shape_slide_against = shape_slide_against_buffer.data[buffer_index];
                sliding_surface_normal = shape_slide_against.surface_normal;
                
                if(shape_slide_against.shape_owner.is_entity)
                {
                    Entity * entity_slide_against = handle_to_entity(shape_slide_against.shape_owner.entity_handle);
                    
                    if(key_pressing(KEY_K))
                    {
                        entity_slide_against->charging = true;
                        entity_slide_against->clearable = true;
                        entity_slide_against->charge_direction = sliding_surface_normal;
                    }
                    
#if 0
                    if(sliding)
                    {
                        Vector3 offset = Vector3Subtract(entity_slide_against->position , entity_slide_against->previous_position);
                        
                        update_player_box(player);
                        ConvexShape player_shape = {};
                        player_shape.shape.box = player->collide_box;
                        player_shape.shape.type = ST_box;
                        player_shape.velocity = offset;
                        player_shape.position = player->collide_box.position;
                        player_shape.capture_collision = false;
                        
                        bounding_box_root_to_check = bounding_box_in_map_root;
                        CollisionResult collision_result = update_collision(player_shape);
                        Vector3 total_offset = Vector3Add(collision_result.velocity , collision_result.offset);
                        
                        player->position = Vector3Add(player->position , total_offset);
                        update_player_box(player);
                    }
#endif
                }
            }
        }
        
        player->grounded = false;
        bool check_ground = true;
        if(player->moving_upward) check_ground = false;
        //if(player->floating_in_bubble) check_ground = false;
        
        if(check_ground)
        {
            float closest_ground_hit_time = 0;
            ShapeOwner ground_owner = {};
            player->grounded = ground_spring(player->collide_box , &player->velocity, player_spring_ray , player_spring_percent , 45 , &ground_normal, &closest_ground_hit_time , &ground_owner);
            
            if(player->grounded)
            {
                player->float_timer = 1.5f;
                
                if(ground_owner.is_entity)
                {
                    Entity * ground_entity = handle_to_entity(ground_owner.entity_handle);
                    Vector3 offset = Vector3Subtract(ground_entity->position , ground_entity->previous_position);
                    
                    ConvexShape player_shape = {};
                    player_shape.shape.box = player->collide_box;
                    player_shape.shape.type = ST_box;
                    player_shape.velocity = offset;
                    player_shape.position = player->position;
                    player_shape.capture_collision = false;
                    
                    CollisionResult collision_result = update_collision(player_shape);
                    player->position = Vector3Add(player->position , Vector3Add(collision_result.velocity , collision_result.offset));
                    
                    update_player_box(player);
                }
            }
        }
        
        bool decl jumpable = false;
        if(player->grounded) jumpable = true;
        if(player->ground_time < 0.1f) jumpable = true;
        
        player->floating = false;
        
        if(!player->grounded)
        {
            player->ground_time += DELTA_TIME;
            
            if(sliding)
            {
                //player->velocity = Vector3Add(player->velocity , (Vector3){0,-UNIT_SIZE * 0.02f,0});
            }
            
            if(!jumpable)
            {
                if(player->floatable)
                {
                    if(key_pressing(KEY_SPACE))
                    {
                        if(player->float_timer > 0)
                        {
                            player->float_timer -= DELTA_TIME;
                            player->floating = true;
                        }
                    }
                }
            }
            
            if(player->floating_in_bubble)
            {
                //player->velocity = Vector3Add(player->velocity , (Vector3){0,-UNIT_SIZE * 0.06f,0});
            }
            if(player->floating)
            {
                player->velocity.y = Lerp(player->velocity.y , 0 , 0.15f);
            }
            else
            {
                player->velocity = Vector3Add(player->velocity , (Vector3){0,-UNIT_SIZE * 0.05f,0});
            }
        }
        else
        {
            player->ground_time = 0;
        }
        
        Vector3 decl walk_velocity = {};
        
        walk_velocity = Vector3Add(walk_velocity , Vector3Scale(player_forward_direction , player_forward));
        walk_velocity = Vector3Add(walk_velocity , Vector3Scale(player_right_direction , player_right));
        
        walk_velocity = project_on_plane(walk_velocity , ground_normal);
        walk_velocity = Vector3Normalize(walk_velocity);
        
        float decl walk_accel = 0.04 * UNIT_SIZE;
        
        player->velocity = Vector3Add(player->velocity , Vector3Scale(walk_velocity , walk_accel));
        
        bool decl throw_entity = false;
        bool decl throw_down_entity = false;
        Entity * decl holding_entity = handle_to_entity(player->holding_entity_handle);
        if(!holding_entity) player->is_holding_entity = false;
        
        player->interact = false;
        
        if(key_pressing(KEY_I)) player->interact = true;
        if(key_pressing(KEY_K)) player->interact = true;
        if(key_pressing(KEY_J)) player->interact = true;
        if(key_pressing(KEY_L)) player->interact = true;
        
        if(player->interact)
        {
            if(key_pressed(KEY_I)) player->interact_input = (Vector2){0,1};
            if(key_pressed(KEY_K)) player->interact_input = (Vector2){0,-1};
            if(key_pressed(KEY_L)) player->interact_input = (Vector2){1,0};
            if(key_pressed(KEY_J)) player->interact_input = (Vector2){-1,0};
            
            player->interact_direction = Vector3Add(Vector3Scale(player_forward_direction , player->interact_input.y) , Vector3Scale(player_right_direction , player->interact_input.x));
            player->interact_direction = Vector3Normalize(player->interact_direction);
            
            if(player->is_holding_entity)
            {
                throw_entity = true;
            }
            else
            {
                ConvexShape surface_detect = {};
                Box detect_box = get_box();
                detect_box.position = player->position;
                detect_box.size = (Vector3){GRID_SIZE * 0.5 , GRID_SIZE * 0.5 , GRID_SIZE * 0.5};
                surface_detect.shape.type = ST_box;
                surface_detect.shape.box = detect_box;
                surface_detect.velocity = Vector3Scale(player->interact_direction , GRID_SIZE * 0.4);
                surface_detect.position = detect_box.position;
                surface_detect.capture_collision = false;
                
                collision_push_root(bounding_box_in_map_root);
                RayCastResultBuffer surface_buffer = convex_shape_ray_cast(surface_detect);
                
                for(int buffer_index = 0; buffer_index < surface_buffer.count; buffer_index++)
                {
                    RayCastResult result = surface_buffer.data[buffer_index];
                    
                    if(result.shape_owner.is_entity)
                    {
                        Entity * entity = handle_to_entity(result.shape_owner.entity_handle);
                        
                        if(entity->type == E_moving_wall)
                        {
                            entity->charging = true;
                            entity->clearable = true;
                            entity->clear_timer = 0;
                            entity->charge_direction = Vector3Negate(result.surface_normal);
                            entity->charge_timer = 0;
                        }
                    }
                }
            }
        }
        
        if(player->is_holding_entity)
        {
            holding_entity->position = Vector3Add(player->position , (Vector3){0 , GRID_SIZE * 1.0f , 0});
        }
        
        if(player->floating_in_bubble)
        {
            if(key_pressing(KEY_SPACE))
            {
                //player->velocity.y += UNIT_SIZE * 0.01f;
            }
        }
        else
        {
            
        }
        
        player->spring_time += DELTA_TIME;
        
        if(key_pressed(KEY_SPACE))
        {
            if(player->spring_time < 0.2f)
            {
                player->moving_upward = true;
                player->is_holding_entity = false;
                player->velocity.y += UNIT_SIZE * 0.8f;
                player->floatable = false;
            }
            
            if(jumpable)
            {
                player->velocity.y = UNIT_SIZE * 0.8f;
                player->grounded = false;
                player->moving_upward = true;
                player->floatable = false;
            }
            else
            {
                if(player->hook_array.count > 0)
                {
                    clear_array(&player->hook_array);
                    player->moving_upward = true;
                    player->velocity.y += UNIT_SIZE * 0.8f;
                    player->floatable = false;
                }
                else if(player->is_holding_entity)
                {
                    if(!player->grounded)
                    {
                        player->floatable = false;
                        player->velocity.y = UNIT_SIZE * 0.6f;
                        player->moving_upward = true;
                        throw_entity = true;
                        throw_down_entity = true;
                    }
                }
            }
        }
        
        if(throw_entity)
        {
            player->is_holding_entity = false;
            holding_entity->clearable = true;
            holding_entity->clear_timer = 0;
            holding_entity->unpickable = true;
            
            if(throw_down_entity)
            {
                holding_entity->position = Vector3Add(player->position , (Vector3){0 , -GRID_SIZE * 0.2f , 0});
                holding_entity->charge_direction = (Vector3){0 , -1 , 0};
            }
            else
            {
                holding_entity->charge_direction = player->interact_direction;
                holding_entity->position = Vector3Add(player->position , Vector3Scale(player->interact_direction , GRID_SIZE * 1.0f));
                holding_entity->position = Vector3Add(holding_entity->position , (Vector3){0 , 0.1f * GRID_SIZE , 0});
            }
            
            if(holding_entity->type == E_hook)
            {
                player->hook_buffer.data[add_to_array(&player->hook_array)] = entity_to_handle(holding_entity);
                holding_entity->hook_on_surface = false;
                holding_entity->detonable = true;
                holding_entity->charging = true;
            }
            else if(holding_entity->type == E_small_block)
            {
                holding_entity->solid = true;
                holding_entity->box.size = (Vector3){GRID_SIZE * 0.7 , GRID_SIZE * 0.7 , GRID_SIZE * 0.7};
                holding_entity->velocity = (Vector3){};
                holding_entity->previous_position = holding_entity->position;
            }
            else if(holding_entity->type == E_bubble)
            {
                holding_entity->expanding = true;
                holding_entity->box.size = (Vector3){GRID_SIZE , GRID_SIZE , GRID_SIZE};
            }
        }
        
        if(key_released(KEY_SPACE))
        {
            player->floatable = true;
        }
        
        float drag = 0.94f;
        
        if(player->floating_in_bubble)
        {
            //drag = 0.65;
        }
        
        if(player->floating)
        {
            drag = 0.8f;
        }
        else if(stand_still_on_ground)
        {
            drag = 0.85f;
        }
        else if(player->grounded)
        {
            drag = 0.96f;
        }
        
        player->velocity.x *= drag;
        player->velocity.z *= drag;
        
        if(sliding)
        {
            //player->velocity.y *= 0.6;
        }
        else
        {
            //player->velocity.y *= 0.95;
        }
        
        array_foreach(decl handle_index , &player->hook_array)
        {
            Entity * decl entity = handle_to_entity(player->hook_buffer.data[handle_index]);
            if(!entity) 
            {
                delete_from_array(&player->hook_array , handle_index);
                continue;
            }
            
            bool decl pull_player = false;
            if(entity->hook_on_surface) pull_player = true;
            if(Vector3Distance(player->position , entity->position) > GRID_SIZE * 2.5f) pull_player = true; 
            
            if(pull_player)
            {
                Vector3 decl pull_force = Vector3Subtract(entity->position , player->position);
                float decl distance = Vector3Length(pull_force);
                
                float decl force = 0.003f;
                
                if(distance > 1.5f * GRID_SIZE)
                {
                    force = 0.003f;
                }
                
                pull_force = Vector3Normalize(pull_force);
                pull_force = Vector3Scale(pull_force , force * distance);
                
                if(distance > 0.5f * GRID_SIZE)
                {
                    
                }
                
                //float decl force_distance = Vector3DotProduct(player->velocity , pull_force);
                //if(force_distance < 0) pull_force = Vector3Scale(pull_force , 1.0f - force_distance);
                
                player->velocity = Vector3Add(player->velocity , pull_force);
                player->moving_upward = true;
                
                if(distance < UNIT_SIZE * 2)
                {
                    player->spring_time = 0;
                    delete_from_array(&player->hook_array , handle_index);
                    continue;
                }
                
                if(!player->floating)
                {
                    player->floatable = false;
                }
            }
            
            if(pull_player)
            {
                draw_arrow_line(player->position , entity->position , Fade(ORANGE , 0.5f));
            }
            else
            {
                draw_arrow_line(player->position , entity->position , Fade(WHITE , 0.5f));
            }
        }
        
        ConvexShape player_shape = {};
        player_shape.shape.box = player->collide_box;
        player_shape.shape.type = ST_box;
        player_shape.velocity = player->velocity;
        player_shape.position = player->position;
        player_shape.capture_collision = false;
        
        CollisionResult collision_result = update_collision(player_shape);
        Vector3 player_offset = Vector3Add(collision_result.offset , collision_result.velocity);
        player->velocity = collision_result.velocity;
        player->position = Vector3Add(player->position , player_offset);
        update_player_box(player);
        
        if(collision_result.stucked)
        {
            player->position = player_spawn_point;
        }
        
        player->character_position = player->box.position;
        player->character_position.y += (player->box.size.y - player_box_size.y) * 0.5f;
    }
}

#define send_all(target_socket , buffer , type , count) send_all_EX( target_socket , buffer , sizeof(type) * count)
internal void function send_all_EX(int target_socket, void * source, int buffer_size)
{
    unsigned char * buffer = source;
    
    int total = 0;
    int bytes_left = buffer_size;
    int sent_bytes = 0;
    
    while(total < buffer_size) 
    {
        int send_flag = 0;
        
#ifdef BUILD_D_LINUX
        send_flag = MSG_NOSIGNAL;
#endif
        
        sent_bytes = send(target_socket, buffer + total , bytes_left, send_flag);
        if (sent_bytes == -1) 
        {
            int error_code = get_socket_error();
            
            if(0)
            {
            }
#ifdef BUILD_D_WINDOWS
            else if(error_code == WSAECONNRESET)
            {
                //TODO: what next?
                CATCH;
            }
#endif
            else
            {
                CATCH;
            }
        }
        total += sent_bytes;
        bytes_left -= sent_bytes;
    }
}

internal void function send_pack(int target_socket , NetDataHeaderBuffer * header_buffer , ByteBuffer * byte_buffer)
{
    send_all(target_socket , &header_buffer->count , int , 1);
    send_all(target_socket , header_buffer->data , NetDataHeader , header_buffer->count);
    send_all(target_socket , &byte_buffer->count , int , 1);
    send_all(target_socket , byte_buffer->data , unsigned char , byte_buffer->count);
}

#define data_pack(flag , data , count , type ) data_pack_EX(flag , data , sizeof(type) * count)
internal void function data_pack_EX(int flag , void * data , int data_size)
{
    if(data_size == 0) return;
    if(net_state.header_buffer.count >= net_state.header_buffer.capacity) CATCH;
    if(net_state.send_buffer.count >= net_state.send_buffer.capacity) CATCH;
    
    int data_offset = net_state.send_buffer.count;
    unsigned char * buffer = net_state.send_buffer.data + data_offset;
    net_state.send_buffer.count += data_size;
    memcpy(buffer , data , data_size);
    
    NetDataHeader * new_header = net_state.header_buffer.data + net_state.header_buffer.count++;
    new_header->flag = flag;
    new_header->offset = data_offset;
}

global int initialize_pack_buffer_index = 0;

#define buffer_pack( flag , data , index , count , type ) \
{ \
local_persist int _last_initialize_index = -1;\
local_persist type * _hidden_buffer = 0;\
if(_last_initialize_index != initialize_pack_buffer_index) \
{\
_last_initialize_index = initialize_pack_buffer_index;\
_hidden_buffer = buffer_pack_EX(flag , sizeof(type) * count);\
}\
_hidden_buffer[index] = data;\
}

internal void * function buffer_pack_EX(int flag , int data_size)
{
    if(data_size == 0) return 0;
    if(net_state.header_buffer.count >= net_state.header_buffer.capacity) CATCH;
    if(net_state.send_buffer.count >= net_state.send_buffer.capacity) CATCH;
    
    int data_offset = net_state.send_buffer.count;
    net_state.send_buffer.count += data_size;
    
    NetDataHeader * new_header = net_state.header_buffer.data + net_state.header_buffer.count++;
    new_header->flag = flag;
    new_header->offset = data_offset;
    
    return net_state.send_buffer.data + data_offset;
}

internal void function update_receive_state(ReceiveState * receive_state)
{
    for(;;)
    {
        int receive_buffer_index = receive_state->receiving_buffer_index;
        unsigned char * byte_buffer = receive_state->buffer[receive_buffer_index];
        NetDataHeader * header_buffer = receive_state->header_buffer[receive_buffer_index];
        
        int receive_size = -1;
        unsigned char * receive_buffer = 0;
        
        switch(receive_state->state)
        {
            case RO_header_count: receive_buffer = (void *)&receive_state->header_count; receive_size = sizeof(int); break;
            case RO_header: receive_buffer = (void *)header_buffer; receive_size = sizeof(NetDataHeader) * receive_state->header_count ; break;
            case RO_data_size: receive_buffer = (void *)&receive_state->data_size; receive_size = sizeof(int); break;
            case RO_data: receive_buffer = (void *)byte_buffer; receive_size = receive_state->data_size ; break;
            
            default: CATCH; break;
        }
        
        int received_byte = recv(receive_state->receiving_socket , receive_buffer + receive_state->collected_byte  , receive_size - receive_state->collected_byte , 0);
        
        if(received_byte == -1)
        {
            int error_code = get_socket_error();
            
            if(error_code == EAGAIN)
            {
                break;
            }
            else if(error_code == EWOULDBLOCK)
            {
                break;
            }
            else if(error_code == ECONNRESET)
            {
                receive_state->connection_reseted = true;
                break;
            }
#ifdef BUILD_D_WINDOWS
            else if(error_code == WSAEWOULDBLOCK)
            {
                break;
            }
#endif
            else
            {
                CATCH;
            }
        }
        else if(received_byte < 0)
        {
            CATCH;
        }
        else if(received_byte == 0)
        {
            break;
        }
        
        receive_state->collected_byte += received_byte;
        if(receive_state->collected_byte == receive_size) 
        {
            if(receive_state->data_size > MAX_RECEIVE_BUFFER) CATCH;
            if(receive_state->header_count > MAX_RECEIVE_HEADER) CATCH;
            
            receive_state->collected_byte = 0;
            
            bool received_full_pack = false;
            
            switch(receive_state->state)
            {
                case RO_header_count:
                {
                    if(receive_state->header_count > 0)
                    {
                        receive_state->state = RO_header;
                    }
                    else
                    {
                        receive_state->state = RO_data_size;
                    }
                }
                break;
                
                case RO_header: receive_state->state = RO_data_size; break;
                
                case RO_data_size: 
                {
                    if(receive_state->data_size > 0)
                    {
                        receive_state->state = RO_data;
                    }
                    else
                    {
                        received_full_pack = true;
                        receive_state->state = RO_header_count;
                    }
                }
                break;
                
                case RO_data: receive_state->state = RO_header_count; received_full_pack = true; break;
            }
            
            if(received_full_pack)
            {
                receive_state->last_update_index = game_update_count;
                
                if(receive_state->receiving_buffer_index == 0) 
                {
                    receive_state->receiving_buffer_index = 1;
                    receive_state->available_buffer_index = 0;
                }
                else
                {
                    receive_state->receiving_buffer_index = 0;
                    receive_state->available_buffer_index = 1;
                }
                
                int buffer_to_clear_index = receive_state->receiving_buffer_index;
            }
        }
    }
}

global ReceiveState * state_to_unpack = 0;

internal void function get_data_from_pack(int flag ,  NetDataHeader * header_result , void ** buffer_result)
{
    if(buffer_result) (*buffer_result) = 0;
    if(header_result) (*header_result) = (NetDataHeader){};
    
    int buffer_index = state_to_unpack->available_buffer_index;
    if( buffer_index == -1) return;
    
    NetDataHeader * header_buffer = state_to_unpack->header_buffer[buffer_index];
    unsigned char * byte_buffer = state_to_unpack->buffer[buffer_index];
    
    for(int header_index = 0 ; header_index < state_to_unpack->header_count ; header_index++)
    {
        NetDataHeader * header = header_buffer + header_index;
        if(header->flag == flag)
        {
            unsigned char * buffer_data = byte_buffer + header->offset;
            if(buffer_result) (*buffer_result) = buffer_data;
            if(header_result) (*header_result) = (*header);
            return;
        }
    }
}

#define data_unpack(flag , source , count , type) data_unpack_EX(flag , source , sizeof(type) * count) 
internal void function data_unpack_EX(int flag , void * source , int size)
{
    NetDataHeader header = {};
    void * buffer = 0;
    get_data_from_pack(flag , &header , &buffer);
    
    if(buffer) memcpy(source , buffer , size);
}

global int initialize_unpack_buffer_index = 0;

#define buffer_unpack(flag , data_pointer , index , type ) \
{\
local_persist int _last_initialize_unpack_index = -1;\
local_persist type * _hidden_buffer = 0;\
if(_last_initialize_unpack_index != initialize_unpack_buffer_index)\
{\
_last_initialize_unpack_index = initialize_unpack_buffer_index;\
get_data_from_pack(flag , 0 , (void **)&_hidden_buffer);\
}\
if(_hidden_buffer) (*(data_pointer)) = _hidden_buffer[index];\
}

internal void function set_socket_to_unblock(int target_socket)
{
    
#ifdef BUILD_D_WINDOWS
    int non_blocking = 1;
    ioctlsocket(target_socket, FIONBIO, &non_blocking);
#endif
    
#ifdef BUILD_D_LINUX
    fcntl(target_socket, F_SETFL, O_NONBLOCK);
#endif
    
}

internal void function server_update()
{
    double server_update_time = time_stamp();
    
    initialize_pack_buffer_index++;
    initialize_unpack_buffer_index++;
    
    struct sockaddr_storage connection_address = {};
    int conection_address_size = sizeof(struct sockaddr_storage);
    int connection_socket = accept(net_state.listening_socket , (struct sockaddr * )&connection_address , &conection_address_size);
    
    if(connection_socket == -1) 
    {
        int error_code = get_socket_error();
        
        if(error_code == EAGAIN)
        {
            
        }
        else if(error_code == EWOULDBLOCK)
        {
            
        }
        else if(error_code == WSAEWOULDBLOCK)
        {
            
        }
        else 
        {
            CATCH;
        }
    }
    else
    {
        set_socket_to_unblock(connection_socket);
        
        //it can't handle more than 16 player
        //you need to reallocate pool manually
        int chunk_index = -1;
        set_pool(&player_pool , &chunk_index);
        Player * new_player = allocate_memory_type(Player , 1 , AT_pool);
        (*new_player) = (Player){};
        new_player->position = player_spawn_point;
        new_player->box = get_box();
        new_player->box.position = new_player->position;
        new_player->box.size = (Vector3){0.6, 0.6 , 0.6};
        set_pool(&player_pool , &chunk_index);
        allocate_buffer(&new_player->hook_buffer , EntityHandle , 16 , AT_pool);
        set_pool(&player_pool , &chunk_index);
        new_player->hook_array = allocate_array(16 , AT_pool);
        
        if(array_full(&player_connection_array)) reallocate_array(&player_connection_array , AT_temp);
        
        int new_connection_index = add_to_array(&player_connection_array);
        PlayerConnection * new_connection = player_connection_buffer.data + new_connection_index;
        
        new_player->connection_index = new_connection_index;
        
        new_connection->connection_socket = connection_socket;
        ReceiveState * new_receive_state = &new_connection->receive_state;
        
        new_receive_state->receiving_socket = connection_socket;
        new_receive_state->connection_reseted = false;
        new_receive_state->available_buffer_index = -1;
        new_receive_state->receiving_buffer_index = 0;
        new_receive_state->data_size = 0;
        new_receive_state->header_count = 0;
    }
    
    array_foreach(player_index , &player_pool.array)
    {
        Player decl * player = index_to_player(player_index);
        if(!player) continue;
        PlayerConnection decl * player_connection = player_connection_buffer.data + player->connection_index;
        
        input_state = &player_connection->input_state;
        
        update_receive_state(&player_connection->receive_state);
        
        if(player_connection->receive_state.connection_reseted)
        {
            printf("%s say goodbye!\n", player_connection->message);
            //if(!delete_from_array(&player_array , player_index)) CATCH;
            if(!delete_from_array(0 , 0)) CATCH;
            CATCH;
            //continue;
        }
        
        state_to_unpack = &player_connection->receive_state;
        
        data_unpack(DF_input_state , &player_connection->input_state , 1 , InputState);
        data_unpack(DF_camera_target , &player->camera_target , 1 , Vector3);
        data_unpack(DF_camera_position , &player->camera_position , 1 , Vector3);
    }
    
    double decl world_update_time = time_stamp();
    if(!game_paused) world_update();
    world_update_time = time_stamp() - world_update_time;
    
    net_state.send_buffer.count = 0;
    net_state.header_buffer.count = 0;
    
    int decl entity_count = 0;
    array_foreach(decl array_index , &entity_active_array)
    {
        if(entity_active_buffer.data[array_index].respawning) continue;
        entity_count++;
    }
    
    data_pack(DF_entity_count , &entity_count , 1 , int);
    
    int decl pack_index = 0;
    array_foreach(decl array_index , &entity_active_array)
    {
        Entity decl * entity = entity_active_buffer.data + array_index;
        
        if(entity->respawning) continue;
        
        buffer_pack(DF_whole_entity , (*entity) , pack_index , entity_count , Entity);
        pack_index++;
    }
    
    int decl player_count = 0;
    array_foreach(decl player_index , &player_pool.array)
    {
        if(!index_to_player(player_index)) continue;
        player_count++;
    }
    
    data_pack(DF_player_count , &player_count , 1 , int );
    
    int player_index = 0;
    array_foreach(array_index , &player_pool.array)
    {
        Player * player = index_to_player(array_index);
        if(!player) continue;
        buffer_pack(DF_whole_player , (*player) , player_index , player_count , Player);
        
        player_index++;
    }
    
    bool * player_owned_array = buffer_pack_EX(DF_player_owned , player_count * sizeof(bool));
    
    player_index = 0;
    array_foreach(array_index , &player_pool.array)
    {
        Player * player = index_to_player(array_index);
        if(!player) continue;
        
        for(int owner_index = 0; owner_index < player_count ; owner_index++)
        {
            player_owned_array[owner_index] = false;
            if(player_index == owner_index) player_owned_array[owner_index] = true;
        }
        
        PlayerConnection * player_connection = player_connection_buffer.data + player->connection_index;
        send_pack(player_connection->connection_socket , &net_state.header_buffer , &net_state.send_buffer);
        
        player_index++;
    }
    
    server_update_time = time_stamp() - server_update_time;
    int used_memory = run_time_memory.current_memory - run_time_memory.start_memory;
    int memory_per_frame = frame_time_memory.current_memory - frame_time_memory.start_memory;
    //printf("\nserver: %f\nworld only: %f\nused memory: %d\nmemory per frame:%d\n" , server_update_time / 1000.0 , world_update_time / 1000.0 , used_memory , memory_per_frame);
}

#ifdef BUILD_D_WINDOWS
internal void function client_network_update()
{
    if(!net_state.connected_to_server)
    {
        if(net_state.client_to_server_socket == -1)
        {
            net_state.client_to_server_socket = socket(AF_INET, SOCK_STREAM, 0);
            if(net_state.client_to_server_socket == -1) CATCH;
        }
        
        struct hostent * host = gethostbyname(app_data->host_name);
        struct sockaddr_in test = {};
        test.sin_port = htons(CONNECTION_PORT);
        test.sin_family = AF_INET;
        test.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;
        
        //inet_ntop(AF_INET, &test.sin_addr.s_addr , ip_string, INET6_ADDRSTRLEN);
        
        int non_blocking = 1;
        ioctlsocket(net_state.client_to_server_socket, FIONBIO, &non_blocking);
        
        bool connected = false;
        
        if(connect(net_state.client_to_server_socket , (struct sockaddr *)&test , sizeof(struct sockaddr_in)) == -1) 
        {
            int error_code = -1;
            error_code = get_socket_error();
            
            //it return WSAEWOULDBLOCK even it connected or i just seeing things?
            if(error_code == WSAEWOULDBLOCK)
            {
            }
            else if(error_code == EAGAIN)
            {
            }
            else if(error_code == WSAEISCONN)
            {
                connected = true;
            }
            else if(error_code == WSAEALREADY)
            {
            }
            else if(error_code == WSAECONNREFUSED)
            {
            }
            else
            {
                switch(error_code)
                {
                    default: CATCH; break;
                }
            }
            
        }
        else
        {
            connected = true;
        }
        
        if(connected)
        {
            net_state.connected_to_server = true;
            net_state.client_receive_state.receiving_socket = net_state.client_to_server_socket;
        }
    }
    
    if(net_state.connected_to_server)
    {
        net_state.send_buffer.count = 0;
        net_state.header_buffer.count = 0;
        initialize_pack_buffer_index++;
        initialize_unpack_buffer_index++;
        
        input_state = &client_input_state;
        data_pack(DF_input_state , input_state , 1 , InputState);
        data_pack(DF_camera_target , &world_camera.target , 1 , Vector3);
        data_pack(DF_camera_position , &world_camera.position , 1 , Vector3);
        
        send_pack(net_state.client_to_server_socket , &net_state.header_buffer , &net_state.send_buffer);
        
        update_receive_state(&net_state.client_receive_state);
        state_to_unpack = &net_state.client_receive_state;
        
        int player_count = 0;
        data_unpack(DF_player_count , &player_count , 1 , int);
        if(player_count > client_player_buffer.capacity)
        {
            int new_capacity = client_player_buffer.capacity;
            for(;new_capacity < player_count; new_capacity*=2);
            reallocate_buffer_EX(&client_player_buffer , new_capacity , AT_temp);
        }
        
        client_player_buffer.count = player_count;
        
        for(int player_index = 0 ; player_index < player_count ; player_index++)
        {
            Player * player = client_player_buffer.data + player_index;
            
            buffer_unpack(DF_whole_player , player , player_index , Player);
            buffer_unpack(DF_player_owned , &player->it_is_me , player_index , bool);
        }
        
        int entity_count = 0;
        data_unpack(DF_entity_count , &entity_count , 1 , int);
        if(entity_count > client_entity_buffer.capacity)
        {
            int new_capacity = client_player_buffer.capacity;
            for(;new_capacity < player_count; new_capacity*=2);
            reallocate_buffer_EX(&client_entity_buffer , new_capacity , AT_temp);
        }
        
        client_entity_buffer.count = entity_count;
        for(int entity_index = 0; entity_index < entity_count; entity_index++)
        {
            Entity * entity = client_entity_buffer.data + entity_index;
            buffer_unpack(DF_whole_entity , entity , entity_index , Entity);
        }
    }
}
#endif

internal void function start_connection()
{
    allocate_buffer(&net_state.header_buffer , NetDataHeader , 256 , AT_temp);
    allocate_buffer(&net_state.send_buffer , unsigned char , 1024 * 64 , AT_temp);
    
    allocate_buffer(&player_connection_buffer , PlayerConnection , 16 , AT_temp);
    player_connection_array = allocate_array(16 , AT_temp);
    
    default_receive_state = (ReceiveState){};
    default_receive_state.available_buffer_index = -1;
    default_receive_state.receiving_buffer_index = 0;
    default_receive_state.state = RO_header_count;
    default_receive_state.receiving_socket = -1;
    default_receive_state.last_update_index = -1;
    
    net_state.client_receive_state = default_receive_state;
    net_state.connected_to_server = false;
    
    if(net_state.is_server)
    {
        for(int player_buffer_index = 0; player_buffer_index < player_connection_buffer.capacity ; player_buffer_index++ )
        {
            PlayerConnection * player_connection = player_connection_buffer.data + player_buffer_index;
            player_connection->receive_state = default_receive_state;
            
            player_connection->connection_socket = -1;
            player_connection->input_state = (InputState){};
        }
    }
    
#ifdef BUILD_D_WINDOWS
    WSADATA data = {};
    WSAStartup( MAKEWORD(2,2) , &data);
#endif
    
    char ip_string[INET6_ADDRSTRLEN] = {};
    
    if(net_state.is_server)
    {
        struct hostent * host = gethostbyname(app_data->host_name);
        inet_ntop(AF_INET, &((struct in_addr *)(host->h_addr))->s_addr , ip_string, INET6_ADDRSTRLEN);
        
        struct sockaddr_in test = {};
        test.sin_port = htons(CONNECTION_PORT);
        test.sin_family = AF_INET;
        test.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;
        
        net_state.listening_socket = socket(AF_INET, SOCK_STREAM, 0);
        
        int yes = true;
        if(setsockopt(net_state.listening_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(int)) == -1) CATCH;
        
        set_socket_to_unblock(net_state.listening_socket);
        
        if(bind(net_state.listening_socket, (struct sockaddr * )&test, sizeof(struct sockaddr_in)) == -1) 
        {
            int error_code = get_socket_error();
            CATCH;
        }
        
        if(listen(net_state.listening_socket , 10) == -1) CATCH;
    }
}

internal void function end_connection()
{
#ifdef BUILD_D_LINUX
    close(net_state.listening_socket);
#endif
    
#ifdef BUILD_D_WINDOWS
    
    if(net_state.is_server) closesocket(net_state.listening_socket);
    if(net_state.is_client) closesocket(net_state.client_to_server_socket);
    WSACleanup();
#endif
}