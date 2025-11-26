static int hash[] = { 208,34,231,213,32,248,233,56,161,78,24,140,71,48,140,254,245,255,247,247,40,
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

#define PNoise2(x, y,SEED) hash[(hash[(y + SEED) % 256] + x) % 256];

#define PLinInter(x,y,s) x + s * (y - x)

#define SmoothInter(x, y, s) PLinInter(x, y, s * s * (3 - 2 * s))

internal Rect quad_position_left(Rect rect, float position_offset_x, float size_x, float size_offset_y)
{
	rect.position.x = ((position_offset_x + size_x / 2) / app_data->window_size.x)* app_data->right *2 - app_data->right;
	rect.position.y = 0;
    
	rect.size.x = (size_x / app_data->window_size.x) * app_data->right*2;
	rect.size.y = app_data->top * 2 - ((size_offset_y / app_data->window_size.y) * app_data->top *2);
    
	return rect;
}

internal Rect quad_position_left_B(float position_offset_x, float size_x, float size_offset_y)
{
	return quad_position_left(get_rect(), position_offset_x, size_x, size_offset_y);
}

internal Rect offset_from_bottom_left(float pixel_offset_x, float pixel_offset_y, float pixel_size_x,float pixel_size_y)
{
	Rect rect = get_rect();
    
	rect.position.x= ((pixel_offset_x + pixel_size_x / 2) / app_data->window_size.x) * app_data->right * 2 - app_data->right;
	rect.position.y= ((pixel_offset_y + pixel_size_y / 2) / app_data->window_size.y) * app_data->top * 2 - app_data->top ;
    
	rect.size.x = (pixel_size_x / app_data->window_size.x) * app_data->right * 2;
	rect.size.y = (pixel_size_y / app_data->window_size.y) * app_data->top * 2;
    
	return rect;
}

internal Rect offset_from_quad_top_left(Rect parent_rect, Rect child_rect, float pixel_offset_x,float pixel_offset_y)
{
	child_rect.position = parent_rect.position;
	child_rect.position.x -= parent_rect.size.x/2;
	child_rect.position.y += parent_rect.size.y/2;
    
	child_rect.position.x += child_rect.size.x / 2;
	child_rect.position.y -= child_rect.size.y / 2;
    
	Vector2 child_rect_offset = {pixel_offset_x, pixel_offset_y};
    
    child_rect.position.x += child_rect_offset.x;
    child_rect.position.y -= child_rect_offset.y;
    
    return child_rect;
}

internal Rect offset_from_top_left(Rect rect , float pixel_offset_x,float pixel_offset_y)
{
    
    Vector2 world_position = {0,0};
    rect.position.x = world_position.x + rect.size.x * 0.5f + pixel_to_width(pixel_offset_x);
    rect.position.y = world_position.y - rect.size.y * 0.5f + pixel_to_height(pixel_offset_y);
    
    return rect;
}

internal Rect quad_to_top_left(Rect rect)
{
    rect.position.x -= rect.size.x * 0.5f;
    rect.position.y += rect.size.y * 0.5f;
    
    return rect;
}

internal Rect quad_to_bottom_left(Rect rect)
{
    rect.position.x -= rect.size.x * 0.5f;
    rect.position.y -= rect.size.y * 0.5f;
    
    return rect;
}

internal Rect quad_move_down_half(Rect rect)
{
    rect.position.y -= rect.size.y * 0.5f;
    return rect;
}

internal Vector2 get_rect_top_right_corner(Rect rect)
{
    return (Vector2){ rect.position.x + rect.size.x * 0.5f, rect.position.y + rect.size.y * 0.5f };
}

internal Vector2 get_rect_bottom_left_corner(Rect rect)
{
    return (Vector2){ rect.position.x - rect.size.x * 0.5f, rect.position.y - rect.size.y * 0.5f };
}

internal Quad resize_block(Quad quad,float size)
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

internal Quad resize_block_B(Quad * quad, float size)
{
    return resize_block(*quad, size);
}

internal bool check_collision_rect(Rect rect , Vector2 point)
{
    Vector2 top_right = get_rect_top_right_corner(rect);
    Vector2 bottom_left = get_rect_bottom_left_corner(rect);
    
    if (point.x < top_right.x && point.y < top_right.y)
    {
        if (point.x > bottom_left.x && point.y > bottom_left.y)
        {
            return true;
        }
    }
    
    return false;
}

internal bool check_collision_rect_mouse(Rect rect)
{
    return check_collision_rect( rect , (Vector2){app_data->mouse_position.x, app_data->mouse_position.y});
}

internal bool check_collision_rect_mouse_B(Vector2 top_right , Vector2 bottom_left)
{
    
    Vector2 current_mouse_position = {app_data->mouse_position.x , app_data->mouse_position.y};
    
    if (current_mouse_position.x < top_right.x && current_mouse_position.y < top_right.y)
    {
        if (current_mouse_position.x > bottom_left.x && current_mouse_position.y > bottom_left.y)
        {
            return true;
        }
    }
    
    return false;
}

internal Vector3 get_edge_direction(Vector3 start,Vector3 end , Vector3 point)
{
    Vector3 edge = Vector3Subtract(end, start);
    
    Vector3 edge_point = Vector3Subtract(point, start);
    
    float inside = Vector3CrossProduct(edge, edge_point).z;
    
    float dx = start.x - end.x;
    float dy = start.y - end.y;
    
    Vector3 edge_normal_inside = { dy,-dx,inside  };
    
    return edge_normal_inside;
}

internal bool check_collision_quad_point(Quad quad, Vector3 point)
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
internal bool check_collision_quad_to_rect(Quad quad , Rect rect)
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

internal bool check_collision_quad_mouse(Quad quad)
{
    
    Vector2 mouse_world_position = {app_data->mouse_position.x, app_data->mouse_position.y};
    return check_collision_quad_point(quad, (Vector3){ mouse_world_position.x,mouse_world_position.y,0 });
    
}

internal Vector4 selection_from_start_to_end_to_rect(Vector2 start_position, Vector2 end)
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

//this is dumb
internal Vector2 get_menu_individual_item_position_end(Vector2 position, String* option_string, bool on_screen,int index,int offset_y)
{
    Rect starting_point_quad = get_rect();
    
    float pixel_size = 20;
    
    float font_size = 0;
    
    if (on_screen)
        font_size = pixel_to_height(pixel_size);
    else
        font_size = pixel_to_height(pixel_size * (1 / camera_current_zoom));
    
    starting_point_quad.position.x = position.x;
    starting_point_quad.position.y = position.y;
    
    Rect menu_option_quad = starting_point_quad;
    menu_option_quad.position.y -= index * font_size;
    menu_option_quad.position.y -= font_size * 0.5f;
    
    if (on_screen)
        menu_option_quad.position.x += pixel_to_width(pixel_size / 2);
    else 
        menu_option_quad.position.x += pixel_to_width(pixel_size / 2) * (1/ camera_current_zoom);
    
    const wchar_t* string = option_string[index].start;
    
    Vector2 option_end_position = {};
    
    if (string)
    {
        float string_width = get_total_string_width(string, font_size);
        
        menu_option_quad.size.x = string_width;
        menu_option_quad.size.y = font_size;
        
        Rect operation_menu_text_rect = menu_option_quad;
        
        menu_option_quad.position.x += menu_option_quad.size.x * 0.5f;
        
        if (on_screen)
        {
            menu_option_quad.size.x += pixel_to_width(pixel_size);
        }
        else
        {
            menu_option_quad.size.x += pixel_to_width(pixel_size) * (1 / camera_current_zoom );
        }
        
        option_end_position.x += menu_option_quad.position.x;
        option_end_position.y += menu_option_quad.position.y;
        
        option_end_position.x += menu_option_quad.size.x * 0.5f;
        option_end_position.y += menu_option_quad.size.y * 0.5f;
        
        option_end_position.y -= font_size * offset_y;
    }
    
    return option_end_position;
}

internal Vector2 get_menu_individual_item_position_end_B(Vector2 position, String* option_string, bool on_screen, int index)
{
    return get_menu_individual_item_position_end(position, option_string, on_screen, index,0);
}

global float font_pixel_size = 20;

internal DrawingMenu start_draw_menu(Vector2 position , bool on_screen , GameMenuType menu_type)
{
    
    DrawingMenu menu = {};
    menu.current_button_position = position;
    menu.on_screen = on_screen;
    menu.menu_type = menu_type;
    
    return menu;
    
}

internal DrawingMenu start_draw_dragging_menu(Vector2 * pixel_position , GameMenuType menu_type)
{
    
    Rect drag_rect = get_rect(90, 20);
    drag_rect.position.x = pixel_position->x;
    drag_rect.position.y = pixel_position->y;
    
    drag_rect.position.y += drag_rect.size.y * 0.8;
    drag_rect.position.x += drag_rect.size.x * 0.6;
    
    if (check_collision_rect_mouse(drag_rect))
    {
        
        draw_rect_D(drag_rect, drag_rect.size.y * 0.3f, Fade(WHITE, 0.6));
        
        if (mouse_button_pressed(MOUSE_LEFT_BUTTON))
        {
            modifying_menu_position = pixel_position;
        }
    }
    else
    {
        draw_rect_D(drag_rect, drag_rect.size.y * 0.3f, Fade(WHITE, 0.4));
    }
    
    if (modifying_menu_position == pixel_position)
    {
        stop_mouse_input= true;
        
        Vector2 mouse_delta = app_data->mouse_position;
        mouse_delta.x -= editor->previous_mouse_position.x;
        mouse_delta.y -= editor->previous_mouse_position.y;
        
        (*pixel_position).x += mouse_delta.x;
        (*pixel_position).y += mouse_delta.y;
        
        draw_rect_D(drag_rect, 10, Fade(WHITE, 0.8));
        
        if (mouse_button_released(MOUSE_LEFT_BUTTON))
        {
            modifying_menu_position = 0;
        }
    }
    
    Rect text_rect = drag_rect;
    text_rect.position.x -= text_rect.size.x * 0.5f;
    text_rect.position.x += pixel_to_width(5);
    D_draw_text_B(text_rect, L"按住拖动", DARKBLUE, false);
    
    Vector2 dragging_position ={drag_rect.position.x, drag_rect.position.y}; 
    dragging_position.y -= drag_rect.size.y * 0.5f;
    dragging_position.x -= drag_rect.size.y * 0.5f;
    
    DrawingMenu menu = {};
    menu.current_button_position = dragging_position;
    menu.on_screen = true;
    menu.menu_type = menu_type;
    
    return menu;
}

internal DrawingMenu start_draw_menu_mouse()
{
    DrawingMenu menu = {};
    menu.current_button_position = editor->operate_menu_position;
    menu.on_screen = false;
    menu.menu_type = GMT_descend;
    
    return menu;
}

internal bool draw_menu_button_W_EX(DrawingMenu * menu, wchar_t * button_string , Color button_text_color , bool change_button_text_color)
{
    
    menu->button_hover = false;
    menu->button_clicked = false;
    
    Rect button_rect = get_rect();
    button_rect.position.x = menu->current_button_position.x;
    button_rect.position.y = menu->current_button_position.y;
    
    float font_size = 0;
    
    //this "on_screen" is absurd
    if(menu->on_screen)
        font_size = font_pixel_size;
    else
        font_size = font_pixel_size * (1 / camera_current_zoom);
    
    float button_offset_y = font_size;
    
    button_rect.position.x += font_pixel_size / 2;
    button_rect.position.y -= button_offset_y;
    
    if (button_string)
    {
        
        float string_width = get_total_string_width(button_string, font_size);
        
        button_rect.size.x = string_width / 2;
        button_rect.size.y = font_size;
        
        button_rect.position.x += button_rect.size.x * 0.5f;
        button_rect.size.x += font_size * 0.4f;
        
        Rect button_text_rect = button_rect;
        
        if (menu->on_screen)
        {
            button_text_rect.size.x += font_pixel_size / 2;
        }
        else
        {
            button_text_rect.size.x += (font_pixel_size / 2) * (1 / camera_current_zoom);
        }
        
        Color text_color = Fade(WHITE, 0.8);
        
        if (check_collision_rect_mouse(button_rect))
        {
            if (mouse_button_pressed(MOUSE_BUTTON_LEFT))
            {
                menu->button_clicked = true;
                stop_mouse_input = true;
            }
            
            //StopMouseInput = true;
            menu->button_hover = true;
            text_color = Fade(YELLOW,0.8);
        }
        
        if(change_button_text_color)
        {
            text_color = button_text_color;
        }
        
        menu->current_button_left = button_rect.position.x;
        menu->current_button_left += button_rect.size.x * 0.5;
        
        draw_rect_D(button_rect, button_rect.size.y*0.3, Fade(BLACK, 0.7f));
        button_text_rect.size.y *= 0.5f;
        D_draw_text_B(button_text_rect, button_string, text_color, true);
        
    }
    
    if (menu->menu_type == GMT_descend)
    {
        menu->current_button_position.y -= button_offset_y;		
        menu->current_button_position.y -= menu->button_extra_offset;
    }
    else if (menu->menu_type == GMT_to_the_right)
    {
        menu->current_button_position.x += button_rect.size.x;
        menu->current_button_position.x += menu->button_extra_offset;
    }
    
    return menu->button_clicked;
    
}

internal bool draw_menu_button_EX(DrawingMenu * menu, char * button_string , Color button_text_color , bool change_button_text_color)
{
    int string_length = strlen(button_string);
    wchar_t * new_string = allocate_frame(wchar_t , string_length + 1);
    for(int i = 0 ; i < string_length ; i++) new_string[i] = button_string[i];
    
    return draw_menu_button_W_EX(menu , new_string ,button_text_color , change_button_text_color);
}

internal bool draw_menu_button_W(DrawingMenu * menu, wchar_t * button_string )
{
    return draw_menu_button_W_EX(menu,button_string , (Color){} , false);
}

internal bool draw_menu_button(DrawingMenu * menu, char * button_string)
{
    int string_length = strlen(button_string);
    wchar_t * new_string = allocate_frame(wchar_t , string_length + 1);
    for(int i = 0 ; i < string_length ; i++) new_string[i] = button_string[i];
    
    return draw_menu_button_W(menu , new_string);
}

//why are you broken??
internal bool ray_line_segment_intersection_example(Vector3 o, Vector3 d, Vector3 a, Vector3 b)
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
internal Vector2 ray_line_segment_intersection(Vector3 o, Vector3 d, Vector3 a, Vector3 b)
{
    float t1 = (o.x - a.x) * (b.y - a.y) - (o.y - a.y) * (b.x - a.x);
    t1 /= (d.y * (b.x - a.x) - d.x * (b.y - a.y));
    
    float t2 = (a.x - o.x) * d.y - (a.y - o.y) * d.x;
    t2 /= (b.y - a.y) * d.x - (b.x - a.x) * d.y;
    
    return (Vector2){ t1 , t2 };
}

//this is how 200+ iq people solve it :)
internal Vector2 ray_line_segment_intersection_(Vector3 o, Vector3 d, Vector3 a, Vector3 b)
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
    
#if 0
    wchar_t DebugString[128] = {};
    swprintf(DebugString, L"t1:%.2f | t2:%.2f | rate: %.2f", t1 , t2 , 1.0 / (float)D_UPDATE_RATE);
    D_draw_text_C(Vector3Lerp(a, b, 0.5f), 0.06, DebugString, Fade(WHITE, 0.5), true);
#endif
    
    return (Vector2){ t1 , t2 };
}

//TODO : rename all of these
internal bool check_collision_rect_with_rect(Rect rect_A, Rect rect_B)
{
    Vector2 quad_B_top_right = get_rect_top_right_corner(rect_B);
    Vector2 quad_B_bottom_left = get_rect_bottom_left_corner(rect_B);
    Vector2 quad_A_top_right = get_rect_top_right_corner(rect_A);
    Vector2 quad_A_bottom_left = get_rect_bottom_left_corner(rect_A);
    
    bool x_intersect = false;
    
    if (quad_B_bottom_left.x < quad_A_bottom_left.x)
    {
        if (quad_B_top_right.x > quad_A_bottom_left.x)
        {
            x_intersect = true;
        }
    }
    else
    {
        if (quad_A_top_right.x > quad_B_bottom_left.x)
        {
            x_intersect = true;
        }
    }
    
    if (x_intersect)
    {
        if (quad_B_bottom_left.y < quad_A_bottom_left.y)
        {
            if (quad_B_top_right.y > quad_A_bottom_left.y )
            {
                return true;
            }
        }
        else
        {
            if (quad_A_top_right.y > quad_B_bottom_left.y)
            {
                return true;
            }
        }
    }
    
    return false;
}

internal RayCollision get_collision_quad_3D(Quad quad_to_check)
{
	Vector3 world_space_vertex[quad_vertex_count] = {};
	world_space_vertex[0] = quad_to_check.vertex_position[vertex_top_left];
    
	world_space_vertex[1] = quad_to_check.vertex_position[vertex_top_right];
    
	world_space_vertex[2] = quad_to_check.vertex_position[vertex_bottom_left];
    
	world_space_vertex[3] = quad_to_check.vertex_position[vertex_bottom_right];
    
	RayCollision ray_result = get_ray_collision_triangle(mouse_ray_3D, world_space_vertex[0], world_space_vertex[1], world_space_vertex[2]);
    
	if (!ray_result.hit)
		ray_result = get_ray_collision_triangle(mouse_ray_3D, world_space_vertex[1], world_space_vertex[3], world_space_vertex[2]);
    
	return ray_result;
    
}

internal bool check_collision_quad_3D_B(Quad quad_to_check)
{
	return get_collision_quad_3D(quad_to_check).hit;
}

internal RayCollision get_collision_rect_3D_B(Rect rect)
{
	return get_collision_rect_3D(mouse_ray_3D, rect);
}

internal bool check_collision_rect_3D(Rect rect)
{
	return get_collision_rect_3D(mouse_ray_3D,rect).hit;
}

//stolen
internal unsigned int string_to_hash_W(wchar_t *s)
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

//stolen
internal int hash_int(int key)
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

#if 0
#define Printhash_table(HashTable) _print_hash_table(HashTable.HashSlotArray , HashTable.HashSlotTail)

internal void _print_hash_table(HashTableSlot * hash_slot_array , int count)
{
    
    printf("\n");
    for (HashTableSlot * current_slot = hash_slot_array; current_slot < hash_slot_array + count; current_slot++)
    {
        printf( "%lld " , current_slot - hash_slot_array );
        
        if (!current_slot->valid)
        {
            printf( "Emty\n" );
        }
        else
        {
            printf( "(Slot : %d)\n" , current_slot->slot_value);
        }
    }
    
    printf("\n");
}
#endif

internal HashTable allocate_hash_table(int count)
{
    
    HashTable hash_table = {};
    hash_table.entry_array = allocate_temp(HashTableEntry , count);
    hash_table.slot_array = allocate_temp(HashTableSlot ,count);
    hash_table.count = count;
    
    for (int entry_index = 0; entry_index < count; entry_index++)
    {
        hash_table.entry_array[entry_index].head_index = -1;
        hash_table.entry_array[entry_index].tail_index = -1;
    }
    
    return hash_table;
    
}

//this look bad

//this is really really bad

internal int get_emty_slot_index_from_hash_table(int hash_value , HashTable * hash_table)
{
    
    HashTableSlot * slot_array = hash_table->slot_array;
    HashTableEntry * entry_array = hash_table->entry_array;
    
    HashTableEntry * entry = entry_array + (hash_value % hash_table->count);
    
    HashTableSlot * emty_slot = 0;
    
    bool started_from_chain = false;
    
    if (entry->head_index == -1)
    {
        emty_slot = slot_array + (hash_value % hash_table->count);
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
        if (emty_slot >= (slot_array + hash_table->count)) emty_slot = slot_array;
    }
    
    int emty_slot_index = (emty_slot - slot_array);
    
    return emty_slot_index;
}

internal int get_hash_table_head_slot_index(int hash_value ,  HashTable * hash_table)
{
    HashTableEntry * entry_array = hash_table->entry_array;
    
    HashTableEntry * entry = entry_array + (hash_value % hash_table->count);
    return entry->head_index;
}

internal int get_data_index_from_slot_index(int slot_index , HashTable * hash_table)
{
    return hash_table->slot_array[slot_index].data_index;
}

internal int insert_to_hash_table(int slot_index_to_insert_after , int hash_value , int data_index , HashTable * hash_table)
{
    
    HashTableSlot * slot_array = ( HashTableSlot *)hash_table->slot_array;
    HashTableEntry * entry_array = ( HashTableEntry *)hash_table->entry_array;
    
    HashTableEntry * entry = entry_array + (hash_value % hash_table->count);
    
    int emty_slot_index = get_emty_slot_index_from_hash_table( hash_value , hash_table);
    HashTableSlot * emty_slot = slot_array + emty_slot_index;
    
    if (entry->head_index == -1)
    {
        entry->head_index = emty_slot_index;
        entry->tail_index = emty_slot_index;
    }
    
    emty_slot->valid = true;
    emty_slot->slot_value = (hash_value % hash_table->count);
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

internal int add_to_hash_table(int hash_value , int data_index , HashTable * hash_table)
{
    
    HashTableSlot * slot_array = hash_table->slot_array;
    HashTableEntry * entry_array = hash_table->entry_array;
    
    HashTableEntry * entry = entry_array + (hash_value % hash_table->count);
    
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
    emty_slot->slot_value = (hash_value % hash_table->count);
    emty_slot->data_index = data_index;
    emty_slot->next_index = -1;
    
    return emty_slot_index;
}

internal void clear_hash_table(HashTable * hash_table)
{
    HashTableEntry * entry_array = hash_table->entry_array;
    HashTableSlot * slot_array = hash_table->slot_array;
    
    for(int slot_index = 0 ; slot_index < hash_table->count ; slot_index++)
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

internal bool delete_from_hash_table_by_slot_index(int slot_index , int hash_value , HashTable * hash_table)
{
    int slot_value = (hash_value % hash_table->count);
    
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
internal bool delete_from_hash_table(int hash_value , int data_index , HashTable * hash_table)
{
    HashTableSlot * slot_array = hash_table->slot_array;
    HashTableSlot * slot_chain = slot_array + (hash_value % hash_table->count);
    
    HashTableEntry * entry_array = hash_table->entry_array;
    HashTableEntry * entry = entry_array + ( hash_value % hash_table->count );
    
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

internal int get_hash_table_tail_data_index(int hash_value , HashTable * hash_table)
{
    int tail_slot_index = hash_table->entry_array[hash_value % hash_table->count].tail_index;
    
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

internal void _iterate_hash_table( int hash_value , int * slot_index , int * data_index, HashTable * hash_table)
{
    
#if HASH_DEBUG
    
    printf("(Iterating : %d)(CurrentSlot : %d)\n" , (hash_value%count) , (*slot_index));
    _print_hash_table(hash_table->SlotArray , count);
    
#endif
    
    HashTableSlot * slot_array = hash_table->slot_array;
    HashTableSlot * slot_chain_head = (slot_array + ( hash_value % hash_table->count ));
    HashTableEntry * entry_array = hash_table->entry_array;
    HashTableEntry * entry = entry_array + ( hash_value % hash_table->count );
    
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

internal void _iterate_hash_table_reverse( int hash_value , int * slot_index , int * data_index, HashTable * hash_table)
{
    
#if HASH_DEBUG
    
    printf("(Iterating : %d)(CurrentSlot : %d)\n" , (hash_value%count) , (*slot_index));
    _print_hash_table(hash_table->SlotArray , count);
    
#endif
    
    int slot_value = ( hash_value % hash_table->count );
    
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

internal List allocate_list(int count)
{
    ListNode * all_node = allocate_temp(ListNode , count + DUMMY_NODE_COUNT);
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
    list.count = count;
    
    return list;
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

internal void clear_list(List * list)
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
internal bool list_full(List * list)
{
    ListNode * node_array = list->node_array;
    int recycled_node_index = node_array[N_recycled_node_head].next;
    
    if (node_array[recycled_node_index].data_index == -1)
    {
        if (list->unuse_index > list->count) CATCH;
        if (list->unuse_index == list->count) return true;
    }
    
    return false;
}

internal int create_new_node( List * list)
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
        if (list->unuse_index >= list->count) CATCH;
        
        new_node_index = list->unuse_index++;
        
    }
    
    return new_node_index;
}

internal int get_next_node_index(int node_index , List * list)
{
    int result =list->node_array[node_index].next;
    
    if(list->node_array[result].data_index == -1) return invalid_node;
    
    return result;
}

internal int get_head_node_index(List * list)
{
    return list->node_array[N_node_head].next;
}

internal int get_tail_node_index(List * list)
{
    return list->node_array[N_node_tail].next;
}

internal int get_previous_node_index(int node_index , List * list)
{
    int result = list->node_array[node_index].previous;
    
    if(list->node_array[result].data_index == -1) return invalid_node;
    
    return result;
}

internal int get_data_index_from_node_index(int node_index , List * list)
{
    if(node_index == invalid_node ) return -1;
    return list->node_array[node_index].data_index;
}

internal int get_previous_data_index_from_node_index(int node_index , List * list)
{
    return get_data_index_from_node_index(get_previous_node_index(node_index, list) , list);
}

internal int get_next_data_index_from_node_index(int node_index , List * list)
{
    return get_data_index_from_node_index(get_next_node_index(node_index, list) , list);
}

internal void insert_to_list
(
 bool connect_before_node , 
 int node_to_connect_index , 
 int node_index , 
 int data_index , 
 List * list
 )
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

internal int get_emty_node_from_list( List * list)
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
        if (list->unuse_index >= list->count) return invalid_node;
        new_node_index = list->unuse_index;
    }
    
    return new_node_index;
}

internal int create_and_insert_to_list(bool connect_before_node , int node_to_connect_index , int data_index , List * list)
{
    int new_node_index = create_new_node(list);
    if(new_node_index == -1) CATCH;
    insert_to_list( connect_before_node ,  node_to_connect_index , new_node_index , data_index , list );
    
    return new_node_index;
}

internal int add_to_list_head(int data_index , List * list)
{
    return create_and_insert_to_list(false , N_node_head , data_index , list);
}

internal int add_to_list_tail( int data_index , List * list)
{
    return create_and_insert_to_list(true , N_node_tail , data_index , list);
}

//feel really weird
internal int add_to_list_tail_B(List * list)
{
    return add_to_list_tail(get_emty_node_from_list(list) , list);
}

internal bool delete_from_list(int node_index_to_delete , List * list)
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

internal Array allocate_array(int capacity)
{
    Array array = {};
    array.valid_array = allocate_temp(bool , capacity);
    array.capacity = capacity;
    
    return array;
}

internal void clear_array(Array * array)
{
	for (int array_index = 0; array_index < array->count; array_index++)
	{
		array->valid_array[array_index] = false;
	}
    
	array->count = 0;
	array->lowest_index = 0;
}

internal void recheck_array(Array * array)
{
	array->lowest_index = 0;
	array->count = 0;
	for( ; array->valid_array[array->lowest_index] ;array->lowest_index++);
    
	for (int i = 0; i < array->capacity; i++)
	{
		if (array->valid_array[i])
		{
			array->count = i + 1;
		}
	}
}

internal int add_to_array(Array * array)
{
    if(array->lowest_index >= array->capacity)
    {
        array->lowest_index = array->capacity;
        return -1;
    }
    
	array->valid_array[array->lowest_index] = true;
    int data_index = array->lowest_index;
    
	for (; ; )
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
    
	if (array->count < array->lowest_index)
		array->count = array->lowest_index;
    
	if (array->count > array->capacity) CATCH;
    
	return data_index;
}

internal bool delete_from_array(Array * array , int data_index)
{
	if (data_index == -1)
		return false;
    
    if(data_index >= array->count) CATCH;
    
	array->valid_array[data_index] = false;
    
	if (array->lowest_index > data_index)
		array->lowest_index = data_index;
    
	int test_index = 0;
	for( ; array->valid_array[test_index] ;test_index++);
	if (test_index != array->lowest_index) CATCH;
	
	for ( ; (!array->valid_array[array->count - 1] ) && (array->count  > 0 );array->count--);
    
	return true;
}

internal bool is_array_full(Array * array)
{
    return (array->lowest_index == array->capacity);
}

internal bool iterate_array(int * data_index , Array * array)
{
    for(;;)
    {
        if(array->valid_array[(*data_index)]) return true;
        
        (*data_index)++;
        if((*data_index) > array->count) return false;
    }
}

//this is much better
#define array_foreach(data_index , array) for (int data_index = 0 ; iterate_array(&data_index , (array)) ; data_index++)

internal void draw_arrow_line(Vector3 start_position , Vector3 end_position , Color start_color , Color end_color)
{
    float arrow_line_width = 10;
    
    draw_round_line(start_position , end_position, arrow_line_width, start_color , end_color);
    
    Vector3 line_direction = Vector3Normalize(Vector3Subtract(end_position , start_position));
    Vector3 line_vertical_direction = Vector3CrossProduct(line_direction , Vector3Subtract(game_camera.target , game_camera.position));
    line_vertical_direction = Vector3Normalize(line_vertical_direction);
    
    Vector3 arrow_offset_y = Vector3Scale(line_direction , -0.05f);
    
    Vector3 arrow_left_line = Vector3Scale(line_vertical_direction , 0.05);
    Vector3 arrow_right_line = Vector3Scale(line_vertical_direction  , -0.05);
    
    arrow_left_line = Vector3Add(arrow_left_line , end_position);
    arrow_right_line = Vector3Add(arrow_right_line , end_position);
    
    arrow_left_line = Vector3Add(arrow_left_line , arrow_offset_y );
    arrow_right_line = Vector3Add(arrow_right_line , arrow_offset_y );
    
    draw_round_line(end_position , arrow_left_line , arrow_line_width , end_color , end_color);
    draw_round_line(end_position , arrow_right_line , arrow_line_width , end_color , end_color);
}

internal void draw_arrow_line_B(Vector3 start_position , Vector3 end_position , Color line_color)
{
    draw_arrow_line( start_position , end_position , line_color , line_color);
}

internal void draw_arrow_ray_C(Vector3 start_position , Vector3 direction  ,Color start_color , Color end_color)
{
    Vector3 end_position = Vector3Add(start_position , direction );
    draw_arrow_line(start_position , end_position , start_color , end_color);
}

internal void draw_arrow_ray_D(Vector3 start_position , Vector3 direction  , Color line_color)
{
    draw_arrow_ray_C(start_position , direction , line_color , line_color);
}

//TODO : i'm a bit lost
internal void lerp_bone_state(Bone * base_bone , Bone * blend_bone , int bone_index , float weight)
{
    if(weight < 0) weight = 0;
    if(weight > 1) weight = 1;
    
    base_bone[bone_index].state.local_position = Vector3Lerp(base_bone[bone_index].state.local_position , blend_bone[bone_index].state.local_position , weight);
    base_bone[bone_index].state.local_rotation= QuaternionLerp(base_bone[bone_index].state.local_rotation , blend_bone[bone_index].state.local_rotation , weight);
    
    //this one should be rotate not lerp
    base_bone[bone_index].state.end_point_offset = Vector3Lerp(base_bone[bone_index].state.end_point_offset ,  blend_bone[bone_index].state.end_point_offset , weight);
}

internal void lerp_multiple_bone_state( Bone * base_bone , Bone * blend_bone , int base_bone_count , float weight)
{
    for(int bone_index = 0; bone_index < base_bone_count ; bone_index++)
    {
        lerp_bone_state(base_bone , blend_bone , bone_index , weight);
    }
}

internal int get_key_frame_count( KeyFrame * start_key_frame , KeyFrame * end_key_frame , int target_start_frame , int target_frame_count)
{
    int end_key_frame_index = target_start_frame + target_frame_count - 1;
    int frame_count = end_key_frame->frame_index - start_key_frame->frame_index;
    
    if(end_key_frame->frame_index < start_key_frame->frame_index)
    {
        frame_count = end_key_frame_index - start_key_frame->frame_index + 1;
        frame_count += end_key_frame->frame_index - target_start_frame;
    }
    
    return frame_count;
}

internal bool is_rig(int bone_index)
{
    switch(bone_index)
    {
        //case B_right_hand_controller: 
        //case B_right_arm_pole_target: 
        //case B_left_arm_pole_target: 
        //case B_left_hand_controller: 
        
        //case B_right_leg_controller: 
        //case B_right_leg_pole_target: 
        //case B_left_leg_pole_target: 
        //case B_left_leg_controller: 
        
        //case B_origin: 
        //case B_slapee_origin: 
        
        return true;
    }
    
    return false;
}

internal void get_bone(Bone * bone_array , int bone_array_count)
{
    for(int bone_index = 0 ; bone_index < bone_array_count ; bone_index++)
    {
        bone_array[bone_index] = (Bone){};
        bone_array[bone_index].rotation = QuaternionIdentity();
        bone_array[bone_index].state.local_rotation = QuaternionIdentity();
    }
}

internal Box get_box()
{
    Box box = {};
    box.rotation = QuaternionIdentity();
    return box;
}

internal Color get_random_color()
{
    
    Color random_color ={};
    
    start_color_seed = ((start_color_seed * 1103515245 + 12345) & RAND_MAX);
    random_color.r = (start_color_seed%255);
    start_color_seed = ((start_color_seed * 1103515245 + 12345) & RAND_MAX);
    random_color.g = (start_color_seed%255);
    start_color_seed = ((start_color_seed * 1103515245 + 12345) & RAND_MAX);
    random_color.b = (start_color_seed%255);
    random_color.a = 255;
    
    return random_color;
}

internal void iterate_and_draw_bone_arrow( Bone * bone_array , Bone * bone , int stack_index)
{
    
    //if(stack_index > 3) return;
    
    //Vector3 bone_up = Vector3RotateByQuaternion((Vector3){0, 0 ,0.1} ,bone->rotation);
    //Vector3 bone_right = Vector3RotateByQuaternion((Vector3){0.1, 0 ,0},bone->rotation);
    
    //DrawArrowRay(Startposition , BoneUp  ,Fade(GREEN,0.5) );
    //DrawArrowRay(Startposition , BoneRight  ,Fade(BLUE,0.5) );
    
    Vector3 end_position = Vector3Add(bone->position , Vector3RotateByQuaternion(bone->state.end_point_offset , bone->rotation));
    draw_arrow_line(bone->position , end_position , Fade(YELLOW , 0.4) ,RED);
    
    int bone_index = bone->bone_index;
    
    hash_table_iterate(child_bone_index , bone_index, &selected_model->bone_children_hash_table)
    {
        
        Bone * child_bone = bone_array + child_bone_index;
        iterate_and_draw_bone_arrow(bone_array , child_bone , stack_index + 1);
        
    }
    
}

internal void member_data_to_string(MemberMetaData* member_meta, unsigned char* data_head, wchar_t* string_buffer)
{
    const char* member_name = member_meta->name;
    int member_offset = member_meta->member_offset;
    unsigned char* member_data = data_head + member_offset;
    
#define TypeToTextBuffer(Type,StringFormat) case GetType(Type): _swprintf(string_buffer, L"%S :" StringFormat, member_name ,*(Type*)member_data); break; 
    
    switch (member_meta->member_type)
    {
        TypeToTextBuffer(float, L"%f");
        TypeToTextBuffer(double, L"%f");
        TypeToTextBuffer(int, L"%d");
        case GetType(Vector2): _swprintf(string_buffer, L"%S :" "%f,%f", member_name, (*(Vector2*)member_data).x, (*(Vector2*)member_data).y); break;
        case GetType(Vector3): _swprintf(string_buffer, L"%S :" "%f,%f,%f", member_name, (*(Vector3*)member_data).x, (*(Vector3*)member_data).y, (*(Vector3*)member_data).z); break;
        
        default: _swprintf(string_buffer, L"%S :" L"__", member_name); break;
    }
}

#define PrintStructMemberInfo(Type,StructData,Quad) _print_struct_member_info(GetStructMeta(Type),(unsigned char*)StructData,Quad)

internal void _print_struct_member_info( StructMetaData struct_meta_data, unsigned char* struct_data, Rect text_quad)
{
    wchar_t string_buffer[256] = {};
    
    for (int member_index = 0; member_index < struct_meta_data.member_count ; member_index++)
    {
        member_data_to_string(struct_meta_data.member_array + member_index, struct_data, string_buffer);
        
        D_draw_text_B(text_quad, string_buffer, Fade(WHITE, 0.6f), false);
        text_quad.position.y -= text_quad.size.y;
    }
}

//just leave it be
internal void create_struct_name_string_hash()
{
    
    _struct_meta_hash = allocate_temp(StructMetaDataNode* , STRUCT_META_HASH_MAX);
    for(int type_index = 0 ; type_index < _MT_type_count ; type_index++)
    {
        
        const char * type_name = _type_meta_name[type_index];
        unsigned int type_hash = string_to_hash(type_name);
        StructMetaDataNode ** node_pointer = _struct_meta_hash + (type_hash %STRUCT_META_HASH_MAX);
        
        for(;(*node_pointer); node_pointer = &(*node_pointer)->next);
        
        StructMetaDataNode * new_node =allocate_temp(StructMetaDataNode,1);
        (*node_pointer) = new_node;
        
        (*new_node) = (StructMetaDataNode){};
        new_node->type_index = type_index;
        new_node->name = type_name;
        new_node->size = _type_meta_size[type_index];
        new_node->struct_meta = _type_struct_meta_[type_index];
    }
    
}

internal int get_data_size(char * type_name)
{
    unsigned int type_hash = string_to_hash(type_name);
    StructMetaDataNode * node = _struct_meta_hash[type_hash%STRUCT_META_HASH_MAX];
    
    for (;node; node = node->next)
    {
        if (!strcmp(type_name , node->name))
        {
            return node->size;
        }
    }
    return 0;
}

internal int get_type_from_name(char * type_name)
{
    unsigned int type_hash = string_to_hash(type_name);
    StructMetaDataNode * node = _struct_meta_hash[ type_hash%STRUCT_META_HASH_MAX ];
    
    for (;node; node = node->next)
    {
        if (!strcmp(type_name , node->name))
        {
            return node->type_index;
        }
    }
    
    return -1;
}


internal void create_a_whole_new_world()
{
    reference_frame_list = allocate_list(16);
    all_reference_frame = allocate_temp(Vector3, 16);
    
    all_key_frame = allocate_temp(KeyFrame , all_key_frame_count);
    editor = allocate_temp(EditorData ,1);
    
    quads_in_map = allocate_temp(Quad , 16);
    quads_in_map_array = allocate_array(16);
    
    editor->timeline_scale = 1;
    editor->selected_clip_index = -1;
    editor->IK_iteration_count = 20;
    editor->assigning_parent_bone = false;
    
    editor->selected_bone_stack = allocate_temp(BoneSelection , 256);
}

internal Vector3 mouse_on_plane(Vector3 plane_origin)
{
    
    Vector3 position = mouse_ray_3D.position;
    Vector3 end_point = Vector3Add(position , mouse_ray_3D.direction);
    
    Vector3 plane_normal = Vector3Subtract(game_camera.target , game_camera.position);
    
    float camera_intersect = get_line_intersect_with_plane_time( position , end_point , plane_normal , plane_origin);
    return Vector3Lerp(position , end_point , camera_intersect);
    
}

internal BoneSelectionResult bone_selection(Vector2 size , Color unactive_color , Color active_color)
{
    int data_count = 0;
    int data_capacity = 16;
    BoneSelectionResultData * data = allocate_frame(BoneSelectionResultData , data_capacity);
    
    for(int clip_bone_stack_index = 0 ; clip_bone_stack_index  < clip_bone_stack_count ; clip_bone_stack_index++)
    {
        
        ClipBone * clip_bone = clip_bone_stack + clip_bone_stack_index;
        
        for(int bone_index = 0 ; bone_index < selected_model->bone_count ; bone_index++)
        {
            
            Bone * final_bone = clip_bone->final_bone_pose + bone_index;
            
            Vector3 bone_position_end = Vector3RotateByQuaternion(final_bone->state.end_point_offset , final_bone->rotation);
            bone_position_end = Vector3Add(bone_position_end , final_bone->position);
            
            Vector3 bone_position = bone_position_end;
            Vector3 bone_screen_point = transform_vector(bone_position , world_3D_to_screen_matrix);
            
            Rect bone_screen_rect = get_rect();
            bone_screen_rect.position = bone_screen_point;
            bone_screen_rect.size = size;
            
            Vector3 hit_point = mouse_on_plane(bone_position);
            
            if(check_collision_rect_mouse(bone_screen_rect))
            {
                REALLOCATE_BUFFER_IF_TOO_SMALL(BoneSelectionResultData , data , data_capacity , data_count , allocate_frame_);
                BoneSelectionResultData * new_data = data + data_count++;
                
                new_data->hit_point = hit_point;
                new_data->bone_index = bone_index;
                draw_rect_line_E(bone_screen_rect , active_color , 4);
                
            }
            else
            {
                draw_rect_line_E(bone_screen_rect , unactive_color , 4);
            }
            
        }
    }
    
    BoneSelectionResult result = {};
    result.data_count = data_count;
    result.data = data;
    
    return result;
}

//i just gonna sort all key frame instead of caching thing
//i "could" make another hash table for slot for hash table
internal void sort_bone_hash_table(int bone_index , HashTable * hash_table_by_bone)
{
    
    HashTableSlot * slot_array = hash_table_by_bone->slot_array;
    
    hash_table_iterate_ex(key_frame_index , key_frame_slot_index , -1 , bone_index , hash_table_by_bone)
    {
        HashTableSlot * slot = slot_array + key_frame_slot_index;
        KeyFrame * key_frame = all_key_frame + key_frame_index;
        
        if(slot->next_index == -1) break;
        
        HashTableSlot * next_slot = slot_array + slot->next_index;
        KeyFrame * next_key_frame = all_key_frame + next_slot->data_index;
        
        if(key_frame->frame_index > next_key_frame->frame_index)
        {
            int temp_index = slot->data_index;
            slot->data_index = next_slot->data_index;
            next_slot->data_index = temp_index;
        }
        
        if(key_frame->frame_index == next_key_frame->frame_index) CATCH;
    }
    
}

internal Quad direction_to_quad(Vector3 direction , float width)
{
    Quad quad = {};
    
    Vector3 up = Vector3Scale(editor->up , width);
    Vector3 right = Vector3Scale(editor->right , width);
    
    Vector3 top_left = Vector3Subtract(direction , right);
    top_left = Vector3Add(top_left , up);
    
    Vector3 top_right = Vector3Add(direction , right);
    top_right = Vector3Add(top_right , up);
    
    Vector3 bottom_left = Vector3Subtract(Vector3Negate(up) , right);
    Vector3 bottom_right = Vector3Add(Vector3Negate(up) , right);
    
#if 1
    if(Vector3DotProduct(direction , up) > 0)
    {
        Vector3 temp_1 = top_left;
        Vector3 temp_2 = top_right;
        
        top_left = bottom_left;
        top_right = bottom_right;
        
        bottom_left = temp_1;
        bottom_right = temp_2;
    }
    
    if(Vector3DotProduct(direction , right) > 0)
    {
        Vector3 temp_1 = top_left;
        Vector3 temp_2 = bottom_left;
        
        top_left = top_right;
        bottom_left = bottom_right;
        
        top_right = temp_1;
        bottom_right = temp_2;
    }
#endif
    
    quad.vertex_position[vertex_top_left] = top_left;
    quad.vertex_position[vertex_top_right] = top_right;
    quad.vertex_position[vertex_bottom_left] = bottom_left;
    quad.vertex_position[vertex_bottom_right] = bottom_right;
    
    return quad;
}

internal Vector3 * box_to_point(Box box)
{
    Vector3 * points = allocate_frame(Vector3 , box_vertex_count);
    
    points[bv_A] = (Vector3){ -box.size.x * 0.5f , box.size.y * 0.5f , box.size.z * 0.5f};
    points[bv_B] = (Vector3){ box.size.x * 0.5f , box.size.y * 0.5f , box.size.z * 0.5f};
    points[bv_C] = (Vector3){ -box.size.x * 0.5f , box.size.y * 0.5f , -box.size.z * 0.5f};
    points[bv_D] = (Vector3){ box.size.x * 0.5f , box.size.y * 0.5f , -box.size.z * 0.5f};
    points[bv_E] = (Vector3){ -box.size.x * 0.5f , -box.size.y * 0.5f , box.size.z * 0.5f};
    points[bv_F] = (Vector3){ box.size.x * 0.5f , -box.size.y * 0.5f , box.size.z * 0.5f};
    points[bv_G] = (Vector3){ -box.size.x * 0.5f , -box.size.y * 0.5f , -box.size.z * 0.5f};
    points[bv_H] = (Vector3){ box.size.x * 0.5f , -box.size.y * 0.5f , -box.size.z * 0.5f};
    
    for(int point_index = 0 ; point_index < box_vertex_count ; point_index++)
    {
        points[point_index] = Vector3RotateByQuaternion(points[point_index] , box.rotation);
        points[point_index] = Vector3Add(points[point_index] , box.position);
    }
    
    return points;
}

internal bool box_collision_ray( Vector3 origin , Vector3 direction, Box box)
{
    Vector3 * points = box_to_point(box);
    
    Vector3 A_to_B = Vector3Subtract(points[bv_B] , points[bv_A]);
    Vector3 A_to_C = Vector3Subtract(points[bv_C] , points[bv_A]);
    Vector3 A_to_E = Vector3Subtract(points[bv_E] , points[bv_A]);
    
    Vector3 H_to_G = Vector3Subtract(points[bv_G] , points[bv_H]);
    Vector3 H_to_F = Vector3Subtract(points[bv_F] , points[bv_H]);
    Vector3 H_to_D = Vector3Subtract(points[bv_D] , points[bv_H]);
    
    Vector3 top_face_direction = Vector3CrossProduct(A_to_B , A_to_C);
    Vector3 forward_face_direction = Vector3CrossProduct(A_to_B , A_to_E);
    Vector3 left_face_direction = Vector3CrossProduct(A_to_C , A_to_E);
    
    Vector3 bottom_face_direction = Vector3Negate(top_face_direction);
    Vector3 backward_face_direction = Vector3Negate(forward_face_direction);
    Vector3 right_face_direction = Vector3Negate(left_face_direction);
    
    int face_to_check[3] = {};
    int face_to_check_count = 0;
    
    //TODO: why some of the vector inverted?
    
    //not gonna fix this ,probably me assuming the axis of cross product wrong
    if(Vector3DotProduct(top_face_direction , direction) > 0) face_to_check[face_to_check_count++] = face_top;
    if(Vector3DotProduct(bottom_face_direction , direction) > 0) face_to_check[face_to_check_count++] = face_bottom;
    if(Vector3DotProduct(right_face_direction , direction) > 0) face_to_check[face_to_check_count++] = face_right;
    if(Vector3DotProduct(left_face_direction , direction) > 0) face_to_check[face_to_check_count++] = face_left;
    if(Vector3DotProduct(forward_face_direction , direction) < 0) face_to_check[face_to_check_count++] = face_front;
    if(Vector3DotProduct(backward_face_direction , direction) < 0) face_to_check[face_to_check_count++] = face_back;
    
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
        
        //TODO: i suspect there is a better way than getting point on plane
        //but too lazy to found out
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
                        return true;
                    }
                }
            }
        }
        
    }
    
    return false;
}

internal Vector3 project_direction_on_plane(Vector3 normal , Vector3 direction)
{
    Vector3 side_axis = Vector3CrossProduct(direction , normal);
    Vector3 direction_axis = Vector3CrossProduct( side_axis , normal);
    direction = Vector3Project(direction , direction_axis);
    return direction;
}

internal Vector3 position_to_grid(Vector3 position , float size)
{
    position = Vector3Scale(position , 1.0 / size);
    position.x = ceil(position.x);
    position.y = ceil(position.y);
    position.z = ceil(position.z);
    position = Vector3Scale(position , size);
    
    return position;
}

internal Vector3 get_furthest_point_by_direction( Vector3 direction , Vector3 * points , int point_count)
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

internal Vector3 get_support_point(Vector3 direction)
{
    Vector3 farest_direction_a = get_furthest_point_by_direction( Vector3Negate(direction) , vertices_a , vertices_a_count);
    Vector3 farest_direction_b = get_furthest_point_by_direction(direction , vertices_b , vertices_b_count);
    
    return Vector3Subtract(farest_direction_b , farest_direction_a);
}

internal bool same_direction_b(Vector3 start , Vector3 end_a , Vector3 end_b)
{
    Vector3 direction_a = Vector3Subtract(end_a , start);
    Vector3 direction_b = Vector3Subtract(end_b , start);
    return Vector3DotProduct( direction_a , direction_b ) > 0;
}

//produce vertical direction to a and same direction to b
internal Vector3 triple_cross_product(Vector3 a , Vector3 b)
{
    Vector3 third_vector = Vector3CrossProduct(b , a);
    return Vector3CrossProduct(a , third_vector);
}

#define SAME_DIRECTION(direction_a , direction_b) (Vector3DotProduct(direction_a , direction_b) > 0)

internal void search_triangle(GJK_State * state)
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

internal bool iterate_simplex( GJK_State * state)
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

internal void draw_simplex_triangle(Vector3 a , Vector3 b , Vector3 c)
{
    draw_round_line(a , b , 5 , Fade(RED , 0.5) , Fade(GREEN, 0.5));
    draw_round_line(b , c , 5 , Fade(GREEN , 0.5) , Fade(BLUE , 0.5));
    draw_round_line(c , a , 5 , Fade(BLUE , 0.5) , Fade(RED , 0.5));
}

internal void draw_simplex(GJK_State * state)
{
    Vector3 a = state->simplex[0];
    Vector3 b = state->simplex[1];
    Vector3 c = state->simplex[2];
    Vector3 d = state->simplex[3];
    
    if(state->simplex_count == 1)
    {
        draw_round_line((Vector3){} , a , 5 , RED , RED);
    }
    else if(state->simplex_count == 2)
    {
        draw_round_line(a , b , 5 , RED , GREEN);
    }
    else if(state->simplex_count == 3)
    {
        draw_round_line(a , b , 5 , RED , GREEN);
        draw_round_line(b , c , 5 , GREEN , BLUE);
        draw_round_line(c , a , 5 , BLUE , RED);
    }
    else if(state->simplex_count == 4)
    {
        draw_round_line(a , b , 5 , RED , GREEN);
        draw_round_line(b , c , 5 , GREEN , BLUE);
        draw_round_line(c , a , 5 , BLUE , RED);
        draw_round_line(d , a , 5 , BLACK ,  RED);
        draw_round_line(d , b , 5 , BLACK  ,GREEN);
        draw_round_line(d , c , 5 , BLACK  ,BLUE);
        
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
        
        draw_arrow_ray_D(a , a_b_d_face_outward_direction  , RED);
        draw_arrow_ray_D(a , a_to_origin  , RED);
        
        draw_arrow_ray_D(b , b_c_d_face_outward_direction  , GREEN);
        draw_arrow_ray_D(b , b_to_origin  , GREEN);
        
        draw_arrow_ray_D(c , c_a_d_face_outward_direction  , BLUE);
        draw_arrow_ray_D(c , c_to_origin  , BLUE);
    }
}

internal bool check_shape(Vector3 origin)
{
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
        if(iterate_count > 20)
        {
            printf("too much %d %lld\n" , iterate_count , game_update_count);
            CATCH;
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
    
#if 1
    draw_simplex(&state);
#endif
    
    return result;
}

internal Vector3 closest_point_on_line(Vector3 a , Vector3 b , Vector3 point)
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

internal Vector3 closest_point_on_triangle(Vector3 a , Vector3 b , Vector3 c , Vector3 point)
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
    
    //draw_arrow_ray_D( a, a_to_b_vertical_inward , RED);
    //draw_arrow_ray_D( b, b_to_c_vertical_inward , GREEN);
    //draw_arrow_ray_D( c, c_to_a_vertical_inward , BLUE);
    
    //draw_simplex_triangle(a , b, c);
    //draw_arrow_line_B( closest_point , point , Fade(PINK , 0.5f));
    
    return closest_point;
}

//Vector3 ray_direction , float * time_of_impact , Vector3 * impact_point
internal bool check_shape_impact(ShapeImpactData * data)
{
    float small_number = 0.000001f;
    
    bool result = false;
    data->time_of_impact = -FLT_MAX;
    
    float ray_time = 0;
    Vector3 ray_end = {};
    
    int ray_iterate_count = 0;
    
    for(;;)
    {
        ray_iterate_count++;
        if(ray_iterate_count > 10)
        {
            //printf();
            break;
        }
        
        Vector3 simplex[3] = {};
        Vector3 search_direction = Vector3Negate(data->ray_direction);
        
        simplex[0] = get_support_point(search_direction);
        simplex[1] = get_support_point(Vector3Negate(search_direction));
        simplex[2] = get_support_point(Vector3Add(search_direction , (Vector3){5,-5,2} ));
        
        float closest_distance = FLT_MAX;
        Vector3 closest_point = {};
        
        int simplex_iterate_count = 0;
        
        for(;;)
        {
            Vector3 a = simplex[0];
            Vector3 b = simplex[1];
            Vector3 c = simplex[2];
            
            simplex_iterate_count++;
            if(simplex_iterate_count > 50)
            {
                printf("iterate too much %d %lld" , simplex_iterate_count , game_update_count);
                break;
            }
            
            Vector3 new_support_point = get_support_point(Vector3Subtract( ray_end , closest_point));
            
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
                
                if(ray_iterate_count == test_ray_count)
                {
                    if(simplex_iterate_count == test_simplex_count)
                    {
                        Box new_point_box = get_box();
                        new_point_box.position = new_support_point;
                        new_point_box.size = (Vector3){UNIT_SIZE * 0.5 , UNIT_SIZE  * 0.5, UNIT_SIZE * 0.5};
                        draw_box( new_point_box , GOLD );
                        draw_simplex_triangle(test_simplex[0] , test_simplex[1] , test_simplex[2]);
                    }
                }
                
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
            
            
            if(ray_iterate_count == test_ray_count)
            {
                if(simplex_iterate_count == test_simplex_count)
                {
                    draw_arrow_line_B( ray_end , current_closest_point , RED);
                }
            }
            
            float current_distance = Vector3DistanceSqr(current_closest_point , ray_end);
            
            if(closest_distance > current_distance)
            {
                closest_distance = current_distance;
                closest_point = current_closest_point;
            }
            else
            {
                closest_point = current_closest_point;
                
                //simplex[vertex_to_replace] = new_support_point;
                
                a = simplex[0];
                b = simplex[1];
                c = simplex[2];
                
                Vector3 a_to_b = Vector3Subtract(b , a);
                Vector3 b_to_c = Vector3Subtract(c , b);
                Vector3 c_to_a = Vector3Subtract(a , c);
                Vector3 simplex_normal = Vector3CrossProduct(a_to_b , c_to_a);
                
                float hit_point_time = get_line_intersect_with_plane_time((Vector3){} , data->ray_direction , simplex_normal , a);
                Vector3 hit_point = Vector3Lerp((Vector3){} , data->ray_direction , hit_point_time);
                
                Vector3 a_to_hit_point = Vector3Subtract(hit_point , a);
                Vector3 a_to_b_vertical_inward = triple_cross_product(a_to_b , b_to_c);
                
                Vector3 b_to_hit_point = Vector3Subtract(hit_point , b);
                Vector3 b_to_c_vertical_inward = triple_cross_product(b_to_c , c_to_a);
                
                Vector3 c_to_hit_point = Vector3Subtract(hit_point , c);
                Vector3 c_to_a_vertical_inward = triple_cross_product(c_to_a , a_to_b);
                
                if(SAME_DIRECTION(a_to_hit_point , a_to_b_vertical_inward))
                {
                    if(SAME_DIRECTION(b_to_hit_point , b_to_c_vertical_inward))
                    {
                        if(SAME_DIRECTION(c_to_hit_point , c_to_a_vertical_inward))
                        {
                            float hit_point_distance = Vector3DistanceSqr(hit_point , ray_end);
                            if(hit_point_distance < 0.1)
                            {
                                if(data->stop_if_too_far)
                                {
                                    if(hit_point_time > 1)
                                    {
                                        return false;
                                    }
                                }
                                
                                data->impact_normal = simplex_normal;
                                data->impact_point = hit_point;
                                data->time_of_impact = hit_point_time;
                                //ray_end = hit_point;
                                result = true;
                            }
                            
                        }
                    }
                }
                
                draw_simplex_triangle(a , b, c);
                draw_arrow_line_B( closest_point , ray_end  , Fade(PINK , 0.5f));
                break;
            }
            
            if(vertex_to_replace == -1) break;
            
            simplex[vertex_to_replace] = new_support_point;
        }
        
        if(result)
        {
            draw_arrow_line_B((Vector3){} , ray_end , GOLD);
        }
        else
        {
            draw_arrow_line_B((Vector3){} , ray_end , Fade(ORANGE , 0.3f));
        }
        
        if(result) break;
        
        Vector3 surface_normal = Vector3Subtract(ray_end , closest_point);
        float dot_product_length = Vector3DotProduct(surface_normal , data->ray_direction);
        if( dot_product_length >= 0) 
        {
            if(fabs(dot_product_length) < small_number)
            {
                data->impact_point = ray_end;
                data->time_of_impact = ray_time;
                data->impact_normal = surface_normal;
                result = true;
            }
            
            //draw_arrow_line_B((Vector3){} , ray_end , YELLOW);
            if(!result)
            {
                //printf("missed iterated:%d %lld\n" , ray_iterate_count , game_update_count);
            }
            break;
        }
        
        ray_time = ray_time - (Vector3DotProduct(surface_normal , surface_normal) / Vector3DotProduct(surface_normal , data->ray_direction));
        
        if(data->stop_if_too_far)
        {
            if(ray_time >= 1.0)
            {
                return false;
            }
            else if(ray_time < 0)
            {
                return false;
            }
        }
        
        ray_end = Vector3Scale(data->ray_direction , ray_time);
        ray_end = Vector3Add( (Vector3){} , ray_end);
    }
    
    return result;
}