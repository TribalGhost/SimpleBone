//TODO: try merge file below
#define BUILD_D_WINDOWS
#include "d_windows_basic.h"
#include "d_gamedata.c"
#include "d_clientdata.c"
#include "d_renderdata.c"
#include "d_gamefunction.c"
#include "d_render.c"
#include "d_text.c"
#include "d_blender_file.h"

internal void key_call_back(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE)
	{
        bool key_removed = false;
        for(int key_index = 0 ; key_index < input_state->pressing_key_count ; key_index++)
        {
            int pressing_key = input_state->pressing_key[key_index];
            if(pressing_key == key)
            {
                input_state->pressing_key_count--;
                input_state->pressing_key[key_index] = input_state->pressing_key[input_state->pressing_key_count];
                key_removed = true;
                break;
            }
        }
        
        //if(!key_removed) CATCH;
        
		input_state->released_key[input_state->released_key_count++] = key;
        if(input_state->released_key_count >= INPUT_MAX_KEY) CATCH;
	}
    
	if (action == GLFW_PRESS)
	{
		int new_pressing_key_index = input_state->pressing_key_count++;
        input_state->pressing_key[new_pressing_key_index] = key;
		input_state->pressing_key_time[new_pressing_key_index] = 0;
        input_state->pressed_key[input_state->pressed_key_count++] = key;
        if(input_state->pressing_key_count >= INPUT_MAX_KEY) CATCH;
        if(input_state->pressed_key_count >= INPUT_MAX_KEY) CATCH;
    }
}

internal void mouse_call_back(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_RELEASE)
	{
        bool mouse_button_removed = false;
		for(int mouse_index = 0 ; mouse_index < input_state->pressing_mouse_count ; mouse_index++ )
        {
            int mouse_button = input_state->pressing_mouse[mouse_index];
            if(mouse_button == button)
            {
                input_state->pressing_mouse_count--;
                input_state->pressing_mouse[mouse_index] = input_state->pressing_mouse[input_state->pressing_mouse_count];
                mouse_button_removed = true;
                break;
            }
        }
        
        //not sure how you trigger this but this did happen
        //if(!mouse_button_removed) CATCH;
        
        input_state->released_mouse[input_state->released_mouse_count++] = button;
        if(input_state->released_mouse_count > INPUT_MAX_KEY) CATCH;
    }
    
	if (action == GLFW_PRESS)
	{
		input_state->pressed_mouse[input_state->pressed_mouse_count++] = button;
		input_state->pressing_mouse[input_state->pressing_mouse_count++] = button;
        
        if(input_state->pressed_mouse_count >= INPUT_MAX_KEY) CATCH;
        if(input_state->pressing_mouse_count >= INPUT_MAX_KEY) CATCH;
	}
}

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

internal bool check_collision_quad_mouse(Quad quad)
{
    Vector2 mouse_world_position = {app_data->mouse_position.x, app_data->mouse_position.y};
    return check_collision_quad_point(quad, (Vector3){ mouse_world_position.x,mouse_world_position.y,0 });
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
        
        if (mouse_pressed(MOUSE_LEFT_BUTTON))
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
        Vector2 mouse_delta = app_data->mouse_position;
        mouse_delta.x -= editor->previous_mouse_position.x;
        mouse_delta.y -= editor->previous_mouse_position.y;
        
        (*pixel_position).x += mouse_delta.x;
        (*pixel_position).y += mouse_delta.y;
        
        draw_rect_D(drag_rect, 10, Fade(WHITE, 0.8));
        
        if (mouse_released(MOUSE_LEFT_BUTTON))
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
            if (mouse_pressed(MOUSE_BUTTON_LEFT))
            {
                menu->button_clicked = true;
            }
            
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

internal RayCollision get_collision_rect_3D_B(Rect rect)
{
	return get_collision_rect_3D(mouse_ray_3D, rect);
}

internal bool check_collision_rect_3D(Rect rect)
{
	return get_collision_rect_3D(mouse_ray_3D,rect).hit;
}

internal Vector3 mouse_on_plane(Vector3 plane_origin)
{
    Vector3 position = mouse_ray_3D.position;
    Vector3 end_point = Vector3Add(position , mouse_ray_3D.direction);
    
    Vector3 plane_normal = Vector3Subtract(world_camera.target , world_camera.position);
    
    float camera_intersect = get_line_intersect_with_plane_time( position , end_point , plane_normal , plane_origin);
    return Vector3Lerp(position , end_point , camera_intersect);
}

internal BoneSelectionResult bone_selection(Vector2 size , Color unactive_color , Color active_color)
{
    BoneSelectionResultDataBuffer data_buffer = {};
    //BoneSelectionResultData * data = 0;
    //data_buffer = allocate_buffer(&data , BoneSelectionResultData , 16 , AT_frame);
    allocate_buffer( &data_buffer , BoneSelectionResultData , 16 , AT_frame);
    
    for(int clip_bone_stack_index = 0 ; clip_bone_stack_index  < clip_bone_stack_count ; clip_bone_stack_index++)
    {
        ClipBone * clip_bone = clip_bone_stack + clip_bone_stack_index;
        
        for(int bone_index = 0 ; bone_index < selected_model->bone_buffer.count ; bone_index++)
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
                if(data_buffer.count == data_buffer.capacity)
                {
                    reallocate_buffer(&data_buffer , AT_temp);
                }
                
                BoneSelectionResultData * new_data = data_buffer.data + data_buffer.count++;
                
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
    result.data_count = data_buffer.count;
    result.data = data_buffer.data;
    
    return result;
}


internal void draw_arrow_line_EX(Vector3 start_position , Vector3 end_position , Color start_color , Color end_color)
{
    float arrow_line_width = 15;
    
    render_state.same_brush = true;
    draw_round_line(start_position , end_position, arrow_line_width, start_color , end_color);
    
    Vector3 line_direction = Vector3Normalize(Vector3Subtract(end_position , start_position));
    Vector3 line_vertical_direction = Vector3CrossProduct(line_direction , Vector3Subtract(world_camera.target , world_camera.position));
    line_vertical_direction = Vector3Normalize(line_vertical_direction);
    
    Vector3 arrow_offset_y = Vector3Scale(line_direction , -0.1f);
    
    Vector3 arrow_left_line = Vector3Scale(line_vertical_direction , 0.1);
    Vector3 arrow_right_line = Vector3Scale(line_vertical_direction  , -0.1);
    
    arrow_left_line = Vector3Add(arrow_left_line , end_position);
    arrow_right_line = Vector3Add(arrow_right_line , end_position);
    
    arrow_left_line = Vector3Add(arrow_left_line , arrow_offset_y );
    arrow_right_line = Vector3Add(arrow_right_line , arrow_offset_y );
    
    draw_round_line(end_position , arrow_left_line , arrow_line_width , end_color , end_color);
    draw_round_line(end_position , arrow_right_line , arrow_line_width , end_color , end_color);
    
    render_state.same_brush = false;
}

internal void draw_arrow_line_B(Vector3 start_position , Vector3 end_position , Color line_color)
{
    draw_arrow_line_EX( start_position , end_position , line_color , line_color);
}

internal void draw_arrow_ray_EX(Vector3 start_position , Vector3 direction  ,Color start_color , Color end_color)
{
    Vector3 end_position = Vector3Add(start_position , direction );
    draw_arrow_line_EX(start_position , end_position , start_color , end_color);
}

internal void draw_arrow_ray(Vector3 start_position , Vector3 direction  , Color line_color)
{
    draw_arrow_ray_EX(start_position , direction , line_color , line_color);
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
    draw_arrow_line_EX(bone->position , end_position , Fade(YELLOW , 0.4) ,RED);
    
    int bone_index = bone->bone_index;
    
    hash_table_iterate(child_bone_index , bone_index, &selected_model->bone_children_hash_table)
    {
        Bone * child_bone = bone_array + child_bone_index;
        iterate_and_draw_bone_arrow(bone_array , child_bone , stack_index + 1);
    }
    
}

//i just gonna sort all key frame instead of caching thing
//i "could" make another hash table for slot for hash table
internal void sort_bone_hash_table(int bone_index , HashTable * hash_table_by_bone)
{
    HashTableSlot * slot_array = hash_table_by_bone->slot_array;
    
    hash_table_iterate_ex(key_frame_index , key_frame_slot_index , -1 , bone_index , hash_table_by_bone)
    {
        HashTableSlot * slot = slot_array + key_frame_slot_index;
        KeyFrame * key_frame = all_key_frame_buffer.data + key_frame_index;
        
        if(slot->next_index == -1) break;
        
        HashTableSlot * next_slot = slot_array + slot->next_index;
        KeyFrame * next_key_frame = all_key_frame_buffer.data + next_slot->data_index;
        
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

internal void draw_simplex_triangle(Vector3 a , Vector3 b , Vector3 c)
{
    Vector3 all_vertices[3] = {};
    all_vertices[0] = a;
    all_vertices[1] = b;
    all_vertices[2] = c;
    
    draw_triangle(all_vertices , color_to_linear(Fade(RED , 0.2)));
    
    draw_round_line(a , b , 10 , Fade(RED , 0.5) , Fade(GREEN, 0.5));
    draw_round_line(b , c , 10 , Fade(GREEN , 0.5) , Fade(BLUE , 0.5));
    draw_round_line(c , a , 10 , Fade(BLUE , 0.5) , Fade(RED , 0.5));
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
        
        draw_arrow_ray(a , a_b_d_face_outward_direction  , RED);
        draw_arrow_ray(a , a_to_origin  , RED);
        
        draw_arrow_ray(b , b_c_d_face_outward_direction  , GREEN);
        draw_arrow_ray(b , b_to_origin  , GREEN);
        
        draw_arrow_ray(c , c_a_d_face_outward_direction  , BLUE);
        draw_arrow_ray(c , c_to_origin  , BLUE);
    }
}

internal void animation_timeline_GUI()
{
	Rect timeline_rect = get_rect();
	timeline_rect.position = (Vector3){app_data->window_size.x / 2 , 0};
	timeline_rect.size = (Vector2){ app_data->window_size.x - 60, 80};
	timeline_rect.position.y += timeline_rect.size.y * 0.5f;
	timeline_rect.position.y += 40;
    
	draw_rect_D(timeline_rect , 5 , Fade(BLACK , 0.2f));
    
	local_persist bool dragging_frame_pointer = false;
    
	if (mouse_released(MOUSE_BUTTON_LEFT))
	{
		dragging_frame_pointer = false;
	}
    
	float frame_position_offset_x = 20 * editor->timeline_scale;
    
	if (check_collision_rect_mouse(timeline_rect))
	{
		if (key_pressing(KEY_LEFT_CONTROL))
		{
			editor->timeline_scale += app_data->mouse_scroll_delta * 0.1f;
		}
		else
		{
			editor->timeline_slider_offset += app_data->mouse_scroll_delta * 0.1f;
		}
	}
    
	int scroll_offset = -editor->timeline_slider_offset / frame_position_offset_x;
    scroll_offset += editor->start_frame_index;
    
	if (scroll_offset < 0) scroll_offset = 0;
    
    float closest_frame_x = FLT_MAX;
    
	local_persist Vector3 pointer_position = {};
    
	int closest_frame_index_to_mouse = -1;
    
	for (int frame_index = scroll_offset ; ; frame_index++)
	{
		
        if (frame_index > (editor->start_frame_index + editor->timeline_frame_length - 1)) break;
        
		Rect frame_rect = get_rect();
		Vector3 frame_position = timeline_rect.position;
		frame_position.x += frame_index * frame_position_offset_x - timeline_rect.size.x * 0.5f + frame_position_offset_x * 0.5f + editor->timeline_slider_offset;
        
		if (frame_position.x > timeline_rect.size.x * 0.5 + timeline_rect.position.x) break;
        
		frame_rect.position = frame_position;
		frame_rect.size = (Vector2){ 2 , timeline_rect.size.y*0.8f};
        
		draw_rect_D(frame_rect,5,Fade(BLACK , 0.2f));
        
		wchar_t frame_index_string[64] = {};
		_swprintf(frame_index_string , L"%d" , frame_index);
        
		Rect text_rect = frame_rect;
		text_rect.size.y = 6;
        
		D_draw_text_B(text_rect, frame_index_string , WHITE  , true);
        
		float frame_to_mouse_x = mouse_position.x - frame_rect.position.x;
		if (frame_to_mouse_x < 0) frame_to_mouse_x *= -1;
        
		if (closest_frame_x > frame_to_mouse_x)
		{
			closest_frame_x = frame_to_mouse_x;
			closest_frame_index_to_mouse = frame_index;
		}
        
		if (frame_index == editor->current_frame_at_timeline)
		{
			pointer_position = frame_rect.position;
		}
        
    }
    
	if (dragging_frame_pointer)
	{
        editor->current_frame_at_timeline = closest_frame_index_to_mouse;
	}
    
    local_persist bool dragging_key_frame = false;
    
    //DrawKeyFrame
	for (int frame_index = scroll_offset;; frame_index++)
	{
		if (frame_index > editor->start_frame_index + editor->timeline_frame_length - 1) break;
        
        for(int stack_index = 0 ; stack_index < editor->selected_bone_count ; stack_index++)
        {
            BoneSelection * current_bone_selection = editor->selected_bone_stack + stack_index;
            
            Vector3 frame_position = timeline_rect.position;
            frame_position.y += timeline_rect.size.y * 0.4f;
			frame_position.x += frame_index * frame_position_offset_x - timeline_rect.size.x * 0.5f + frame_position_offset_x * 0.5f + editor->timeline_slider_offset;
            
            int bone_index = current_bone_selection->bone_index;
            Clip * clip = clip_array[current_bone_selection->clip_index];
            
            hash_table_iterate(key_frame_index , GetKeyFrameHash(frame_index , bone_index), &clip->key_frame_hash_table)
            {
                KeyFrame * key_frame = all_key_frame_buffer.data + key_frame_index;
                
                if(key_frame->bone_index != bone_index) continue;
                if (key_frame->frame_index != frame_index) continue;
                
                Rect key_frame_rect = get_rect();
                key_frame_rect.position = frame_position;
                
                key_frame_rect.size = (Vector2){ 20 , 20 };
                key_frame_rect.position.y -= 1.1f * key_frame_rect.size.y * stack_index;
                
                bool draggable = true;
                
                if(dragging_key_frame) draggable = false;
                if(!check_collision_rect_mouse(key_frame_rect)) draggable = false;
                
                if (draggable)
                {
                    if (mouse_pressed(MOUSE_BUTTON_LEFT))
                    {
                        dragging_key_frame = true;
                        add_to_list_tail(key_frame_index , &clip->dragging_key_frame_list);
                    }
                    
                    draw_circle(key_frame_rect , Fade(ORANGE , 0.6));
                }
                else
                {
                    draw_circle(key_frame_rect , Fade(ORANGE , 0.4));
                }
                
            }
            
        }
        
	}
    
	local_persist int previous_frame_index = -1;
    
	if(mouse_pressed_no_check(MOUSE_BUTTON_LEFT))
	{
		previous_frame_index = closest_frame_index_to_mouse;
	}
    
    //DragKeyFrame
    if(editor->selected_clip_index != -1)
    {
        
        for(int stack_index = 0 ; stack_index < editor->selected_bone_count ; stack_index++)
        {
            BoneSelection * current_bone_selection = editor->selected_bone_stack + stack_index;
            
            Clip * clip = clip_array[current_bone_selection->clip_index];
            
            if (mouse_released(MOUSE_BUTTON_LEFT))
            {
                dragging_key_frame = false;
                clear_list(&clip->dragging_key_frame_list);
            }
            
            if(mouse_pressing(MOUSE_BUTTON_LEFT))
            {
                
                list_foreach(key_frame_being_drag_index , &clip->dragging_key_frame_list)
                {
                    KeyFrame * key_frame_being_drag = all_key_frame_buffer.data + key_frame_being_drag_index;
                    
                    bool frame_existed = false;
                    hash_table_iterate(existed_key_frame_index , GetKeyFrameHash(closest_frame_index_to_mouse , key_frame_being_drag->bone_index) , &clip->key_frame_hash_table)
                    {
                        KeyFrame * key_frame = all_key_frame_buffer.data + existed_key_frame_index;
                        if(key_frame->bone_index != key_frame_being_drag->bone_index) continue;
                        
                        if(key_frame->frame_index == closest_frame_index_to_mouse)
                        {
                            frame_existed = true;
                            break;
                        }
                        
                    }
                    
                    if(!frame_existed)
                    {
                        
                        if(!delete_from_hash_table( GetKeyFrameHash(key_frame_being_drag->frame_index , key_frame_being_drag->bone_index ) , key_frame_being_drag - all_key_frame_buffer.data , &clip->key_frame_hash_table)) CATCH;
                        key_frame_being_drag->frame_index = closest_frame_index_to_mouse;
                        add_to_hash_table(GetKeyFrameHash(key_frame_being_drag->frame_index , key_frame_being_drag->bone_index ) , key_frame_being_drag_index , &clip->key_frame_hash_table);
                        //if(!delete_from_hash_table_by_slot_index( key_frame_being_drag->hash_table_slot_index ,GetKeyFrameHash(key_frame_being_drag->frame_index , key_frame_being_drag->bone_index ) , &clip->key_frame_hash_table )) CATCH;
                        
                        sort_bone_hash_table(key_frame_being_drag->bone_index , &clip->key_frame_hash_table_by_bone);
                        
#if 0
                        hash_table_iterate_ex(next_key_frame_index , slot_index , key_frame_being_drag->hash_table_by_bone_slot_index , key_frame_being_drag->bone_index , &clip->key_frame_hash_table_by_bone)
                        {
                            
                            KeyFrame * next_key_frame = all_key_frame + next_key_frame_index;
                            
                            if(next_key_frame->frame_index == key_frame_being_drag->frame_index) CATCH;
                            if(next_key_frame->frame_index > key_frame_being_drag->frame_index) break;
                            
                            HashTableSlot * next_key_frame_node = slot_array + next_key_frame->hash_table_by_bone_slot_index;
                            HashTableSlot * dragging_key_frame_node = slot_array + key_frame_being_drag->hash_table_by_bone_slot_index;
                            
                            int temp_data_index = next_key_frame_node->data_index;
                            next_key_frame_node->data_index = dragging_key_frame_node->data_index;
                            dragging_key_frame_node->data_index = temp_data_index;
                            
                            int temp_node_index = next_key_frame->hash_table_by_bone_slot_index;
                            next_key_frame->hash_table_by_bone_slot_index = key_frame_being_drag->hash_table_by_bone_slot_index;
                            key_frame_being_drag->hash_table_by_bone_slot_index = temp_node_index;
                            
                        }
                        
                        hash_table_iterate_reverse_ex(previous_key_frame_index , slot_index , key_frame_being_drag->hash_table_by_bone_slot_index , key_frame_being_drag->bone_index , &clip->key_frame_hash_table_by_bone)
                        {
                            KeyFrame * previous_key_frame = all_key_frame + previous_key_frame_index;
                            
                            if(previous_key_frame->frame_index == closest_frame_index_to_mouse) CATCH;
                            if(previous_key_frame->frame_index < closest_frame_index_to_mouse) break;
                            
                            HashTableSlot * previous_key_frame_node = slot_array + previous_key_frame->hash_table_by_bone_slot_index;
                            HashTableSlot * dragging_key_frame_node = slot_array + key_frame_being_drag->hash_table_by_bone_slot_index;
                            
                            int temp_data_index = previous_key_frame_node->data_index;
                            previous_key_frame_node->data_index = dragging_key_frame_node->data_index;
                            dragging_key_frame_node->data_index = temp_data_index;
                            
                            int temp_node_index = previous_key_frame->hash_table_by_bone_slot_index;
                            previous_key_frame->hash_table_by_bone_slot_index = key_frame_being_drag->hash_table_by_bone_slot_index;
                            key_frame_being_drag->hash_table_by_bone_slot_index = temp_node_index;
                            
                        }
                        
#endif
                    }
                }
            }
            
        }
    }
    
    
	Rect frame_pointer_rect = get_rect();
	frame_pointer_rect.position = pointer_position;
	frame_pointer_rect.size.y = timeline_rect.size.y * 1.2f;
	frame_pointer_rect.size.x = 10;
    
    bool hovering_pointer = false;
    
	if (check_collision_rect_mouse(timeline_rect))
	{
        hovering_pointer = true;
        
        if (mouse_pressed(MOUSE_BUTTON_LEFT))
		{
            dragging_frame_pointer = true;
		}
	}
    
	if (dragging_frame_pointer)
	{
		draw_rect_D(frame_pointer_rect  , 0 , Fade(YELLOW,0.5));
	}
    else if(hovering_pointer)
    {
		draw_rect_D(frame_pointer_rect  , 0 , Fade(WHITE,0.5));
    }
	else
	{
		draw_rect_D(frame_pointer_rect  , 0 , Fade(WHITE ,0.2));
	}
    
	Vector3 add_new_frame_button_position = timeline_rect.position;
	add_new_frame_button_position.y += timeline_rect.size.y*0.5f;
	add_new_frame_button_position.x -= timeline_rect.size.x*0.5f;
    
	DrawingMenu timeline_operation_menu  = start_draw_menu((Vector2){add_new_frame_button_position.x , add_new_frame_button_position.y} , true ,GMT_to_the_right);
	timeline_operation_menu.current_button_position.y += 45;
	timeline_operation_menu.button_extra_offset = 5;
    
    wchar_t clip_entry_string[16] ={};
    _swprintf(clip_entry_string , L"動畫 ： %d" , editor->selected_clip_index);
    draw_menu_button_W(&timeline_operation_menu , clip_entry_string );
    
	bool play_start_or_stop = false;
    
	if (draw_menu_button_W(&timeline_operation_menu , editor->playing ? L"播放中" : L"播放" ))
	{
		play_start_or_stop = true;
	}
    
	draw_menu_int_input(&timeline_operation_menu , L"開始幀 : %d" , &editor->start_frame_index);
	draw_menu_int_input(&timeline_operation_menu , L"幀長度 : %d" , &editor->timeline_frame_length);
    
	local_persist float start_time = 0;
	local_persist float end_time = 0;
    
	start_time = ((float)editor->start_frame_index) / ((float)FRAME_PER_SECOND);
	end_time = ((float)editor->start_frame_index + editor->timeline_frame_length) / ((float)FRAME_PER_SECOND);
    
	draw_menu_float_input(&timeline_operation_menu , L"開始時間 : %f" , &start_time);
	draw_menu_float_input(&timeline_operation_menu , L"結束時間 : %f" , &end_time);
    
	editor->start_frame_index = start_time * FRAME_PER_SECOND;
	editor->timeline_frame_length = (end_time - start_time ) * FRAME_PER_SECOND;
    
	if (key_pressed(KEY_SPACE))
	{
        play_start_or_stop = true;
	}
    
	if (play_start_or_stop)
	{
		editor->playing = !editor->playing;
	}
    
	if (!editor->playing)
	{
		editor->play_timer = 0;
	}
	else
	{
		editor->play_timer += DELTA_TIME;
        
		if (editor->play_timer > FRAME_TIME)
		{
			editor->play_timer -= FRAME_TIME;
			editor->current_frame_at_timeline++;
            
			if (editor->current_frame_at_timeline > editor->start_frame_index + editor->timeline_frame_length - 1)
			{
				editor->current_frame_at_timeline = editor->start_frame_index;
			}
		}
	}
    
    bool add_new_key_frame = false;
    bool remove_key_frame = false;
    
    if(editor->selected_bone_count >0)
    {
        add_new_key_frame = draw_menu_button_W(&timeline_operation_menu , L"添加幀");
        remove_key_frame = draw_menu_button_W(&timeline_operation_menu , L"刪除幀");
    }
    
    for(int stack_index = 0 ; stack_index < editor->selected_bone_count ; stack_index++)
    {
        BoneSelection * current_bone_selection = editor->selected_bone_stack + stack_index;
        int selected_bone_index = current_bone_selection->bone_index;
        
        //_Bone * selected_bone = BoneArray + Selectedbone_index;
        
        Clip * clip = clip_array[current_bone_selection->clip_index];
        
        if (add_new_key_frame)
        {
            bool frame_existed = false;
            
            hash_table_iterate(key_frame_index, GetKeyFrameHash(editor->current_frame_at_timeline , selected_bone_index), &clip->key_frame_hash_table)
            {
                KeyFrame * key_frame = all_key_frame_buffer.data + key_frame_index;
                
                if(key_frame->bone_index != selected_bone_index) continue;
                
                if (key_frame->frame_index == editor->current_frame_at_timeline)
                {
                    frame_existed = true;
                    break;
                }
                
            }
            
            if (!frame_existed)
            {
                if(buffer_full(all_key_frame_buffer))
                {
                    reallocate_buffer(&all_key_frame_buffer , AT_temp);
                }
                int new_key_frame_index = all_key_frame_buffer.count++;
                KeyFrame * new_key_frame = all_key_frame_buffer.data + new_key_frame_index;
                add_to_list_tail( new_key_frame_index , &clip->key_frame_active_list);
                
                (*new_key_frame) = (KeyFrame){};
                
                new_key_frame->bone_index = selected_bone_index;
                new_key_frame->frame_index = editor->current_frame_at_timeline;
                new_key_frame->bone_state.local_rotation = QuaternionIdentity();
                add_to_hash_table( GetKeyFrameHash(editor->current_frame_at_timeline , selected_bone_index) , new_key_frame_index , &clip->key_frame_hash_table);
                add_to_hash_table( selected_bone_index , new_key_frame_index , &clip->key_frame_hash_table_by_bone);
                
                sort_bone_hash_table(selected_bone_index , &clip->key_frame_hash_table_by_bone);
                
            }
        }
        
        if (remove_key_frame)
        {
            int frame_index = 0;
            int key_frame_hash = GetKeyFrameHash(editor->current_frame_at_timeline , selected_bone_index);
            hash_table_iterate_ex(Keyframe_index , SlotIndex , -1 , key_frame_hash , &clip->key_frame_hash_table)
            {
                KeyFrame * key_frame = all_key_frame_buffer.data + Keyframe_index;
                
                if(key_frame->bone_index != selected_bone_index) continue;
                
                if (key_frame->frame_index == editor->current_frame_at_timeline)
                {
                    delete_from_hash_table_by_slot_index(SlotIndex , key_frame_hash , &clip->key_frame_hash_table);
                    if(!delete_from_list(Keyframe_index , &clip->key_frame_active_list)) CATCH;
                    //if(!delete_from_hash_table_by_slot_index( key_frame->hash_table_by_bone_slot_index ,selected_bone_index, )) CATCH;
                    if(!delete_from_hash_table(selected_bone_index , Keyframe_index , &clip->key_frame_hash_table_by_bone)) CATCH;
                    
                    break;
                }
            }
            
        }
        
    }
    
#if 1
    
    if(editor->selected_clip_index != -1)
    {
        
        if(editor->selected_bone_count == 1 )
        {
            
            BoneSelection * bone_selection = editor->selected_bone_stack;
            int selected_bone_index = bone_selection->bone_index;
            
            Clip * clip = clip_array[bone_selection->clip_index];
            
            hash_table_iterate(key_frame_index , selected_bone_index , &clip->key_frame_hash_table_by_bone)
            {
                KeyFrame * key_frame = all_key_frame_buffer.data + key_frame_index;
                
                if(key_frame->bone_index != bone_selection->bone_index) continue;
                
                char key_frame_name_buffer[256] = {};
                
                sprintf(key_frame_name_buffer , "%d" , key_frame->frame_index);
                
                if (draw_menu_button(&timeline_operation_menu , key_frame_name_buffer))
                {
                    
                }
            }
        }
    }
#endif
}

//this one is weird as fuck
internal void bone_mouse_menu( Bone * single_editing_bone , Clip * clip , int current_frame_index)
{
    
    local_persist bool menu_enable = false;
    
	if (mouse_pressed(MOUSE_BUTTON_RIGHT))
	{
        if (editor->selected_bone_count == 1)
		{
			menu_enable = !menu_enable;
		}
	}
    
	if (menu_enable)
	{
        KeyFrame * editing_key_frame = 0;
        
        if(clip)
        {
            
            hash_table_iterate(key_frame_index , GetKeyFrameHash(current_frame_index , single_editing_bone->bone_index) , &clip->key_frame_hash_table)
            {
                KeyFrame * key_frame = all_key_frame_buffer.data + key_frame_index;
                
                if(key_frame->bone_index != single_editing_bone->bone_index) continue;
                if(key_frame->frame_index != current_frame_index) continue;
                
                editing_key_frame = key_frame;
                break;
            }
        }
        
        DrawingMenu menu = start_draw_menu_mouse();
        
		draw_menu_float_input(&menu , L"長度 : %f " , &single_editing_bone->state.end_point_offset.y);
        
		if (draw_menu_button_W(&menu , L"重置位置"))
		{
			if(editing_key_frame)
            {
                editing_key_frame->bone_state.local_position = (Vector3){};
            }
            else
            {
                single_editing_bone->state.local_position = (Vector3){};
            }
            
		}
        
		if (draw_menu_button_W(&menu , L"重置旋轉"))
		{
            if(editing_key_frame)
            {
                editing_key_frame->bone_state.local_rotation = QuaternionIdentity();
            }
            else
            {
                single_editing_bone->state.local_rotation = QuaternionIdentity();
            }
		}
        
        if(draw_menu_button_W(&menu , single_editing_bone->free_position ? L"鎖定位置" : L"自由位置" ))
        {
            single_editing_bone->free_position = !single_editing_bone->free_position;
        }
        
        int key_frame_count = 0;
        
        if(editing_key_frame)
        {
            if (draw_menu_button_W(&menu , L"重置貝塞爾曲綫"))
            {
                editing_key_frame->previous_bezier_offset = (Vector3){};
                editing_key_frame->next_bezier_offset = (Vector3){};
            }
        }
	}
}

internal void editor_GUI()
{
    GL_CATCH;
    glBindFramebuffer(GL_FRAMEBUFFER, render_state.screen_frame_buffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER , GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, render_state.interface_texture, 0);
    if(!rlFramebufferComplete(render_state.screen_frame_buffer)) CATCH;
    glBindFramebuffer(GL_FRAMEBUFFER , render_state.screen_frame_buffer);
    
    glClearDepth(0);
    glClearColor(0,0,0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
    
	if (mouse_pressed_no_check(MOUSE_BUTTON_RIGHT))
	{
        editor->operate_menu_position = app_data->mouse_position;
    }
    
    if(editor->selected_bone_count == 1)
    {
        BoneSelection * bone_selection = editor->selected_bone_stack;
        bool clip_not_found = true;
        
        ClipBone * clip_bone = clip_bone_stack + bone_selection->clip_bone_stack_index;
        Bone * editing_bone = clip_bone->final_bone_pose + bone_selection->bone_index;
        
        if(editor_type == edit_base_pose)
        {
            editing_bone = selected_model->bone_buffer.data + bone_selection->bone_index;
        }
        
        bone_mouse_menu( editing_bone , clip_array[clip_bone->clip_index] , editor->current_frame_at_timeline);
    }
    
    if(editor_type == edit_animation)
    {
        animation_timeline_GUI();
    }
    
    DrawingMenu bone_menu = start_draw_menu((Vector2){app_data->window_size.x - (500) ,app_data->window_size.y - 20} , true , GMT_to_the_right);
    bone_menu.button_extra_offset = 5;
    bone_menu.menu_type = GMT_descend;
    
    if(editor->selected_bone_count == 1)
    {
        Bone * selected_bone = selected_model->bone_buffer.data + editor->selected_bone_stack->bone_index;
        
        local_persist bool inputing_text = false;
        local_persist int input_cursor = 0;
        local_persist wchar_t all_character[64]={};
        local_persist wchar_t * bone_string = 0;
        
        if(!inputing_text)
        {
            
            if(draw_menu_button_W( &bone_menu , selected_bone->bone_name.string))
            {
                if(!selected_bone->from_blend_file)
                {
                    
                    inputing_text = true;
                    input_cursor = 0;
                    bone_string = selected_bone->bone_name.string;
                    
                }
            }
            
            wchar_t * parent_string = L"none";
            int parent_bone_index = selected_bone->parent_bone_index;
            
            if(parent_bone_index != -1)
            {
                parent_string = combine_string_W( L"parent bone : " , selected_model->bone_buffer.data[parent_bone_index].bone_name.string);
            }
            
            if(editor->assigning_parent_bone)
            {
                
                BoneSelectionResult result = bone_selection( (Vector2){40 , 40} , Fade(GREEN, 0.2) , Fade(GREEN , 0.8));
                
                for(int data_index = 0 ; data_index < result.data_count ; data_index++)
                {
                    //TODO: put model in clip bone
                    Bone * final_bone = clip_bone_stack[0].final_bone_pose;
                    Bone * selected_bone_final = final_bone + selected_bone->bone_index;
                    Bone * new_parent_bone = final_bone + result.data[data_index].bone_index;
                    
                    if(mouse_pressed(MOUSE_BUTTON_LEFT))
                    {
                        if(new_parent_bone->bone_index != selected_bone->bone_index)
                        {
                            
                            int previous_parent_index = selected_bone->parent_bone_index;
                            selected_bone->parent_bone_index = new_parent_bone->bone_index;
                            
                            list_foreach_EX(root_index , N_node_head , node_index , &selected_model->root_bone_list)
                            {
                                
                                if(root_index == selected_bone->bone_index)
                                {
                                    if(!delete_from_list(node_index , &selected_model->root_bone_list)) CATCH;
                                    break;
                                }
                                
                            };
                            
                            if(previous_parent_index != -1)
                            {
                                if(!delete_from_hash_table(previous_parent_index , selected_bone->bone_index , &selected_model->bone_children_hash_table))
                                {
                                    CATCH;
                                }
                            }
                            
                            selected_bone->parent_bone_index = new_parent_bone->bone_index;
                            add_to_hash_table( new_parent_bone->bone_index , selected_bone->bone_index , &selected_model->bone_children_hash_table);
                            editor->assigning_parent_bone = false;
                            
                            selected_bone->state.local_rotation = QuaternionMultiply(  QuaternionInvert(new_parent_bone->rotation) , selected_bone_final->rotation);
                            
                            selected_bone->state.local_position = Vector3Subtract(selected_bone_final->position , new_parent_bone->position);
                            selected_bone->state.local_position = Vector3RotateByQuaternion(selected_bone->state.local_position , QuaternionInvert(new_parent_bone->rotation));
                            
                            break;
                        }
                        
                    }
                    
                }
                
            }
            
            if(editor->assigning_IK_target_bone || editor->assigning_IK_pole_bone)
            {
                BoneSelectionResult result = bone_selection( (Vector2){40 , 40} , Fade(GREEN, 0.2) , Fade(GREEN , 0.8));
                
                if(mouse_pressed(MOUSE_BUTTON_LEFT))
                {
                    for(int data_index = 0 ; data_index < result.data_count ; data_index++)
                    {
                        int target_bone_index = result.data[data_index].bone_index;
                        if(target_bone_index != selected_bone->bone_index)
                        {
                            
                            if(editor->assigning_IK_target_bone)
                            {
                                selected_bone->IK_target_bone_index = target_bone_index;
                            }
                            else if(editor->assigning_IK_pole_bone)
                            {
                                selected_bone->IK_pole_bone_index = target_bone_index;
                            }
                            
                            editor->assigning_IK_target_bone = false;
                            editor->assigning_IK_pole_bone = false;
                            
                            break;
                        }
                        
                    }
                }
                
            }
            
            if(mouse_pressed(MOUSE_BUTTON_LEFT))
            {
                editor->assigning_parent_bone = false;
                editor->assigning_IK_target_bone = false;
                editor->assigning_IK_pole_bone = false;
            }
            
            if(draw_menu_button_W(&bone_menu , parent_string))
            {
                editor->assigning_parent_bone = true;
            }
            
            if(draw_menu_button_W(&bone_menu , selected_bone->IK_enable ? L"IK enable" : L"IK disable"))
            {
                selected_bone->IK_enable = !selected_bone->IK_enable;
            }
            
            if(selected_bone->IK_enable)
            {
                draw_menu_int_input(&bone_menu , L"chain length : %d" , &selected_bone->IK_chain_length);
                
                Bone * selected_model_bone = selected_model->bone_buffer.data;
                
                wchar_t * target_bone_string = L"none";
                wchar_t target_string[64] = {};
                if(selected_bone->IK_target_bone_index != -1) target_bone_string = selected_model_bone[selected_bone->IK_target_bone_index].bone_name.string;
                _swprintf(target_string , L"IK target : %s" , target_bone_string );
                
                wchar_t * pole_bone_string = L"none";
                wchar_t pole_string[64] = {};
                if(selected_bone->IK_pole_bone_index != -1) pole_bone_string = selected_model_bone[selected_bone->IK_pole_bone_index].bone_name.string;
                _swprintf(pole_string , L"IK pole : %s" , pole_bone_string);
                
                if(draw_menu_button_W(&bone_menu , target_string))
                {
                    editor->assigning_IK_target_bone = !editor->assigning_IK_target_bone;
                }
                
                if(draw_menu_button_W(&bone_menu , pole_string))
                {
                    editor->assigning_IK_pole_bone = !editor->assigning_IK_pole_bone;
                }
                
            }
            
        }
        else
        {
            
            int all_character_count = 0;
            
            draw_menu_button_W_EX( &bone_menu , selected_bone->bone_name.string , YELLOW , true);
            
            bool exit_input = false;
            
            if(key_pressed(KEY_ENTER)) exit_input = true;
            if(key_pressed(KEY_ESCAPE)) exit_input = true;
            if(mouse_pressed(MOUSE_BUTTON_LEFT)) exit_input = true;
            if(mouse_pressed(MOUSE_BUTTON_RIGHT)) exit_input = true;
            
            if(exit_input)
            {
                inputing_text = false;
            }
            
            int start_index = 0;
            for( ; bone_string[start_index]&&(start_index < FIXED_STRING_SIZE)  ; start_index++);
            
            if(key_pressed(KEY_BACKSPACE))
            {
                if(start_index>= 0)
                {
                    bone_string[start_index-1] = L'\0';
                }
            }
            
            for(;;)
            {
                unsigned int character = char_pressed();
                if(!character) break;
                all_character[all_character_count++] = character;
            }
            
            if(all_character_count > 0)
            {
                for(int buffer_index = 0; buffer_index < all_character_count ; buffer_index++)
                {
                    int string_index = buffer_index + start_index;
                    if(string_index >= FIXED_STRING_SIZE) break;
                    bone_string[string_index] = all_character[buffer_index];
                }
            }
        }
    }
    
    Vector2 top_right_bar_position = {20,app_data->window_size.y - 20};
    DrawingMenu top_right_bar = start_draw_menu(top_right_bar_position , true , GMT_to_the_right);
    top_right_bar.button_extra_offset = 8;
    
    DrawingMenu side_list_menu = top_right_bar;
    side_list_menu.menu_type = GMT_descend;
    side_list_menu.button_extra_offset = 5;
    draw_menu_button_W(&side_list_menu , 0);
    draw_menu_button_W(&side_list_menu , 0);
    
    if(net_state.is_server) draw_menu_button_W(&top_right_bar , L"主机" );
    
    for(int type_index = 0 ; type_index < editor_type_count ; type_index++)
    {
        wchar_t * name = 0;
        
        switch(type_index)
        {
            case edit_base_pose: name = L"调整基础姿态"; break;
            case edit_animation: name = L"动画"; break;
            case edit_world: name = L"编辑地图"; break;
            case demo: name = L"游戏"; break;
        }
        
        if(draw_menu_button_W_EX(&top_right_bar , name , YELLOW , editor_type == type_index))
        {
            editor_type = type_index;
        }
    }
    
    top_right_bar.current_button_position.x += 30;
    
    if(editor_type == edit_animation)
    {
        
        if(draw_menu_button_W(&top_right_bar , L"添加動畫"))
        {
            int clip_index = 0;
            bool emty_clip = false;
            
            for(; clip_index < MAX_CLIP ; clip_index++ )
            {
                if(!clip_array[clip_index])
                {
                    emty_clip = true;
                    break;
                }
            }
            
            if(!emty_clip) CATCH;
            
            Clip * new_clip = allocate_temp( Clip , 1);
            
            clip_array[clip_index] =new_clip;
            new_clip->clip_index = clip_index;
            new_clip->key_frame_active_list = allocate_list(CLIP_START_CAPACITY , AT_temp);
            new_clip->dragging_key_frame_list = allocate_list(CLIP_START_CAPACITY , AT_temp);
            new_clip->key_frame_hash_table = allocate_hash_table( CLIP_START_CAPACITY , AT_temp );
            new_clip->key_frame_hash_table_by_bone = allocate_hash_table( CLIP_START_CAPACITY , AT_temp);
        }
        
        if(editor->selected_clip_index != -1)
        {
            Clip * clip = clip_array[editor->selected_clip_index];
            
            if(draw_menu_button_W(&top_right_bar , L"刪除動畫"))
            {
                clip_array[editor->selected_clip_index] = 0;
                editor->selected_clip_index = -1;
                
                clip_bone_stack_count = 0;
                
                editor->selected_bone_count = 0;
            }
            
            if(draw_menu_button_W(&top_right_bar , clip->not_loop ? L"非循環" : L"循環")) clip->not_loop = !clip->not_loop;
        }
        
        
        draw_menu_int_input(&top_right_bar , L"IK迭代次數：%d"  , &editor->IK_iteration_count);
        
        if(draw_menu_button_W(&top_right_bar , editor->turn_off_bezier_curve ? L"開啓貝塞爾曲綫" : L"關閉貝塞爾曲綫")) editor->turn_off_bezier_curve = !editor->turn_off_bezier_curve;
        
        if(editor->selected_bone_count == 1)
        {
            BoneSelection * current_bone_selection = editor->selected_bone_stack;
            //draw_menu_button( &top_right_bar , enum_to_string(BoneTag)[current_bone_selection->bone_index]);
        }
        
        for(int clip_index = 0 ; clip_index < MAX_CLIP ; clip_index++)
        {
            Clip * clip = clip_array[clip_index];
            
            if(!clip) continue;
            
            //const char * aniamtion_name = enum_to_string(AnimationTag)[clip_index];
            //if(clip_index >= animation_tag_count) aniamtion_name = " None ";
            
            wchar_t clip_name[256] = {};
            _swprintf(clip_name , L"%d 動畫" , clip->clip_index );
            
            if(draw_menu_button_W_EX(&side_list_menu , clip_name , YELLOW , editor->selected_clip_index == clip_index ))
            {
                editor->selected_clip_index = clip_index;
                
                clip_bone_stack_count = 0;
                add_clip_bone_at_next_frame = true;
                ClipBone * new_clip_bone = clip_bone_stack + clip_bone_stack_count;
                
                (*new_clip_bone) = (ClipBone){};
                new_clip_bone->clip_index = clip_index;
                
                editor->selected_bone_count = 0;
            }
            
        }
        
    }
    
    if(editor_type == edit_base_pose)
    {
        if(draw_menu_button_W(&top_right_bar , L"添加骨頭"))
        {
            if(selected_model->bone_buffer.count == selected_model->bone_buffer.capacity)
            {
                reallocate_buffer(&selected_model->bone_buffer , AT_temp);
            }
            
            if(selected_model->initial_bone_buffer.count == selected_model->initial_bone_buffer.capacity)
            {
                reallocate_buffer(&selected_model->initial_bone_buffer , AT_temp);
            }
            
            int new_bone_index = selected_model->bone_buffer.count++;
            Bone * new_bone = selected_model->bone_buffer.data + new_bone_index;
            
            new_bone->bone_index = new_bone_index;
            new_bone->parent_bone_index = -1;
            new_bone->state.local_position = (Vector3){0,0,0};
            new_bone->state.local_rotation = QuaternionIdentity();
            new_bone->state.end_point_offset = (Vector3){0,1,0};
            new_bone->free_position = true;
            new_bone->from_blend_file = false;
            
            new_bone->IK_enable = false;
            new_bone->IK_chain_length = 0;
            new_bone->IK_target_bone_index = -1;
            new_bone->IK_pole_bone_index = -1;
            
            add_to_list_tail(new_bone_index , &selected_model->root_bone_list);
            
            int new_initial_bone_index = selected_model->initial_bone_buffer.count++;
            Bone * new_initial_bone = selected_model->initial_bone_buffer.data + new_initial_bone_index;
            (*new_initial_bone) = (*new_bone);
        }
    }
    
    local_persist bool box_menu_on = false;
    local_persist Vector2 box_menu_position = {};
    
    if(box_menu_on)
    {
        DrawingMenu menu = start_draw_menu(box_menu_position , true , GMT_descend);
        
        if(draw_menu_button_W(&menu , L"删除盒子"))
        {
            if(current_map_edit_type == MET_box)
            {
                int index_to_remove = last_clicked_box - box_in_map_buffer.data;
                delete_from_array( &box_in_map_array , index_to_remove);
            }
            
            if(current_map_edit_type == MET_camera_trigger)
            {
                int index_to_remove = last_clicked_box - camera_zone_buffer.data;
                delete_from_array(&camera_array , index_to_remove);
            }
            
            last_clicked_box = 0;
            box_menu_on = false;
        }
        
        if(draw_menu_button_W(&menu , L"重置旋转"))
        {
            last_clicked_box->rotation = QuaternionIdentity();
        }
    }
    
    capture_collision = false;
    
    if(store_multiple_frame_collision)
    {
        capture_collision = true;
        display_all_captured_collision = true;
    }
    else
    {
        if(capture_collision_non_stop)
        {
            capture_collision = true;
            collision_visual_buffer.count = 0;
            current_collision_visual_index = 0;
            collision_visual_offset = (Vector3){};
            display_all_captured_collision = true;
        }
        else
        {
            display_all_captured_collision = display_all_visual;
        }
    }
    
    if(buffer_full(frame_collision_buffer))
    {
        reallocate_buffer(&frame_collision_buffer , AT_temp);
    }
    
    if(editor_type != edit_world) box_menu_on = false;
    if(mouse_pressed_no_check(MOUSE_BUTTON_LEFT)) box_menu_on = false;
    
    if(editor_type != edit_world) box_menu_on = false;
    
    if(editor_type == edit_world)
    {
        if(mouse_pressed_no_check(MOUSE_BUTTON_RIGHT))
        {
            if(last_clicked_box)
            {
                box_menu_on = !box_menu_on;
                box_menu_position = mouse_position;
            }
        }
        
        for(int button_index = 0 ; button_index < MET_count ; button_index++)
        {
            wchar_t * button_text = 0;
            
            switch(button_index)
            {
                case MET_none: button_text = L"无"; break;
                case MET_quad: button_text = L"网格"; break;
                case MET_box: button_text = L"盒子"; break;
                case MET_camera_trigger: button_text = L"相机"; break;
            }
            
            if(draw_menu_button_W_EX(&top_right_bar , button_text , YELLOW , current_map_edit_type == button_index))
            {
                last_clicked_box = 0;
                current_map_edit_type = button_index;
            }
        }
        
        if(current_map_edit_type == MET_box || current_map_edit_type == MET_camera_trigger)
        {
            for(int button_index = 0 ; button_index < edit_count ; button_index++)
            {
                wchar_t * name = 0;
                switch(button_index)
                {
                    case edit_move: name = L"移动"; break;
                    case edit_rotate: name = L"旋转"; break;
                    case edit_stretch: name = L"拉伸"; break;
                    case edit_drag_new_box: name = L"创建新盒子"; break;
                    case edit_camera_offset: name = L"相机方向"; break;
                }
                
                if(button_index == edit_camera_offset)
                {
                    if(current_map_edit_type != MET_camera_trigger) continue;
                    if(!last_clicked_box) continue;
                    
                    draw_menu_button_W(&side_list_menu , 0);
                    draw_menu_button_W(&side_list_menu , 0);
                }
                
                if(draw_menu_button_W_EX(&side_list_menu , name , YELLOW ,  current_edit_type == button_index))
                {
                    current_edit_type = button_index;
                }
                
                if(button_index == edit_camera_offset)
                {
                    
                    if(last_clicked_box)
                    {
                        draw_menu_button_W(&side_list_menu , L"预设相机角度:");
                        
                        DrawingMenu camera_option_menu = side_list_menu;
                        camera_option_menu.current_button_position.x += 20;
                        
                        int count = 0;
                        Vector3 camera_offset[16] = {};
                        camera_offset[count++] = (Vector3){GRID_SIZE * 8 , -GRID_SIZE * 10, -GRID_SIZE * 8 };
                        camera_offset[count++] = (Vector3){0,-GRID_SIZE * 10 , GRID_SIZE * 8};
                        camera_offset[count++] = (Vector3){-GRID_SIZE * 8 , -GRID_SIZE * 10,0};
                        camera_offset[count++] = (Vector3){GRID_SIZE * 8 , -GRID_SIZE *10 ,0};
                        
                        for(int camera_option_index = 0 ; camera_option_index < count ; camera_option_index++)
                        {
                            Vector3 offset = camera_offset[camera_option_index];
                            
                            wchar_t option_string[64] = {};
                            _swprintf(option_string , L"%.4f %.4f %.4f" , offset.x , offset.y , offset.z);
                            
                            if(draw_menu_button_W(&camera_option_menu , option_string))
                            {
                                int trigger_index = last_clicked_box - camera_zone_buffer.data;
                                CameraTrigger * camera_trigger = camera_buffer.data + trigger_index;
                                camera_trigger->camera_target_offset = camera_offset[camera_option_index];
                            }
                        }
                    }
                }
            }
        }
        
#if 0
        if(draw_menu_button_W(&top_right_bar , L"生成导航网格"))
        {
            generate_nav_mesh();
        }
#endif
        
        local_persist bool show_frame_drop_down = false;
        
        if(draw_menu_button_W(&top_right_bar , store_multiple_frame_collision ? L"结束录制物理碰撞" : L"开始录制物理碰撞" ))
        {
            store_multiple_frame_collision = !store_multiple_frame_collision;
            if(store_multiple_frame_collision)
            {
                show_frame_drop_down = false;
                collision_visual_buffer.count = 0;
                frame_collision_buffer.count = 0;
            }
            else
            {
                display_all_visual = true;
                capture_collision_non_stop = false;
            }
        }
        
        if(!store_multiple_frame_collision)
        {
            if(draw_menu_button_W(&top_right_bar , capture_collision_non_stop ?  L"检视物理碰撞" : L"显示物理碰撞" ))
            {
                capture_collision_non_stop = !capture_collision_non_stop;
                frame_collision_buffer.count = 0;
            }
            
            if(!capture_collision_non_stop)
            {
                if(draw_menu_button_W(&top_right_bar , L"保存物理碰撞"))
                {
                    capture_collision = true;
                    collision_visual_buffer.count = 0;
                    current_collision_visual_index = 0;
                    collision_visual_offset = (Vector3){};
                    frame_collision_buffer.count = 0;
                }
            }
            
            if(frame_collision_buffer.count > 0)
            {
                wchar_t frame_buffer_string[64] = {};
                _swprintf(frame_buffer_string , L"帧：%d (%d)" , current_frame_collision_index , frame_collision_buffer.count);
                
                DrawingMenu frame_drop_down_menu = {};
                frame_drop_down_menu = top_right_bar;
                frame_drop_down_menu.menu_type = GMT_descend;
                
                draw_menu_button_W(&frame_drop_down_menu , 0);
                
                if(draw_menu_button_W(&top_right_bar , frame_buffer_string))
                {
                    show_frame_drop_down = !show_frame_drop_down;
                }
                
                if(show_frame_drop_down)
                {
                    for(int frame_index = 0 ; frame_index < frame_collision_buffer.count ; frame_index++)
                    {
                        FrameCollision frame_collision = frame_collision_buffer.data[frame_index];
                        if((frame_collision.slice_end - frame_collision.slice_start) > 0)
                        {
                            wchar_t frame_string[64] = {};
                            _swprintf(frame_string , L"%d:(%d->%d)" , frame_index , frame_collision.slice_start , frame_collision.slice_end);
                            
                            if(draw_menu_button_W_EX(&frame_drop_down_menu , frame_string , YELLOW , current_frame_collision_index == frame_index))
                            {
                                current_frame_collision_index = frame_index;
                            }
                        }
                    }
                }
            }
            
            if(collision_visual_buffer.count != 0)
            {
                if(draw_menu_button_W(&top_right_bar , display_all_visual ? L"显示单一迭代" : L"显示所有迭代" ))
                {
                    display_all_visual = !display_all_visual;
                }
                
                wchar_t collision_string[128] = {};
                
                if(!display_all_visual)
                {
                    if(draw_menu_button_W(&top_right_bar , L"<"))
                    {
                        if((current_collision_visual_index - 1) >= 0) current_collision_visual_index--;
                    }
                    
                    if(draw_menu_button_W(&top_right_bar , L">"))
                    {
                        if((current_collision_visual_index + 1) < collision_visual_buffer.count) current_collision_visual_index++;
                    }
                    
                    _swprintf( collision_string , L"迭代次数:%d 当前迭代:%d" , collision_visual_buffer.count , current_collision_visual_index + 1);
                }
                else
                {
                    _swprintf( collision_string , L"迭代次数:%d" , collision_visual_buffer.count);
                }
                
                draw_menu_button_W(&top_right_bar , collision_string);
            }
            
        }
        
        if(current_map_edit_type == MET_none)
        {
            if(draw_menu_button_W(&top_right_bar , L"添加參考坐標"))
            {
                if(list_full(&reference_frame_list))
                {
                    reallocate_buffer(&reference_frame_buffer , AT_temp);
                    reallocate_list(&reference_frame_list , AT_temp);
                }
                
                selected_reference_frame_index = add_to_list_tail_B(&reference_frame_list);
                reference_frame_buffer.data[selected_reference_frame_index] = (Vector3){};
            }
            
            if(selected_reference_frame_index != -1)
            {
                if(draw_menu_button_W(&top_right_bar , L"刪除參考坐標"))
                {
                    if(!delete_from_list(selected_reference_frame_index , &reference_frame_list)) CATCH;
                    selected_reference_frame_index = -1;
                }
            }
            
            if(draw_menu_button_W_EX(&side_list_menu , L"原點" , YELLOW , selected_reference_frame_index == -1))
            {
                selected_reference_frame_index = -1;
            }
            
            list_foreach(reference_frame_index , &reference_frame_list)
            {
                Vector3 reference_frame = reference_frame_buffer.data[reference_frame_index];
                wchar_t temp[256] = {};
                _swprintf(temp , L"%f %f %f" , reference_frame.x ,reference_frame.y , reference_frame.z);
                if(draw_menu_button_W_EX(&side_list_menu , temp , YELLOW , reference_frame_index == selected_reference_frame_index))
                {
                    selected_reference_frame_index = reference_frame_index;
                }
            }
        }
    }
    
    D_game_draw();
    rlDisableFramebuffer();
}

internal bool check_selected_bone_rotation( Bone * final_bone_array_copy, int single_bone_index , Clip * clip_to_assign)
{
    Bone * selected_bone = final_bone_array_copy + single_bone_index;
    
    Rect rotation_rect[R_count] = {};
    RayCollision rotation_rect_collision[R_count] = {};
    
    rotation_rect[R_Z] = get_rect();
    rotation_rect[R_Z].size = (Vector2){ 1, 1};
    rotation_rect[R_Z].position = selected_bone->position;
    rotation_rect[R_Z].rotation = selected_bone->rotation;
    
    rotation_rect[R_Y] = rotation_rect[R_Z];
    rotation_rect[R_Y].rotation = QuaternionMultiply(rotation_rect[R_Z].rotation , QuaternionFromEuler(DEG2RAD * 90,0,0));
    
    rotation_rect[R_X] = rotation_rect[R_Z];
    rotation_rect[R_X].rotation = QuaternionMultiply(rotation_rect[R_Z].rotation , QuaternionFromEuler(0,DEG2RAD * 90,0));
    
    bool hovering = false;
    for(int axis_index = R_Z ; axis_index < R_count ; axis_index++)
    {
        rotation_rect_collision[axis_index] = get_collision_rect_3D_B(rotation_rect[axis_index]);
        if(rotation_rect_collision[axis_index].hit)
        {
            hovering = true;
        }
    }
    
    local_persist Vector3 previous_drag_point = {};
    local_persist int dragging_axis = -1;
    
    if(mouse_pressed(MOUSE_BUTTON_LEFT))
    {
        dragging_axis = -1;
    }
    
    RayCollision closest_collision = {};
    closest_collision.distance = FLT_MAX;
    
    Color axis_clor = {};
    if(dragging_axis == R_Z) axis_clor = BLUE;
    if(dragging_axis == R_Y) axis_clor = GREEN;
    if(dragging_axis == R_X) axis_clor = RED;
    
    draw_rect_line_E(rotation_rect[dragging_axis] , axis_clor , 20);
    
    for(int axis_index = R_Z ; axis_index < R_count ; axis_index++)
    {
        bool found_closest_axis = true;
        
        if(!rotation_rect_collision[axis_index].hit) found_closest_axis = false;
        if(rotation_rect_collision[axis_index].distance > closest_collision.distance) found_closest_axis = false;
        
        if(found_closest_axis)
        {
            if(mouse_pressed(MOUSE_BUTTON_LEFT))
            {
                previous_drag_point = rotation_rect_collision[axis_index].point;
                dragging_axis = axis_index;
            }
            
            closest_collision = rotation_rect_collision[axis_index];
        }
    }
    
    if(!mouse_pressing(MOUSE_BUTTON_LEFT))
    {
        for(int axis_index = R_Z ; axis_index < R_count ; axis_index++)
        {
            draw_rect_line_E(rotation_rect[axis_index] , Fade(WHITE , 0.2 ) , 20);
        }
        
        dragging_axis = -1;
    }
    else
    {
        Vector3 hit_point = mouse_on_plane(selected_bone->position);
        
        Rect axis_rect = rotation_rect[dragging_axis];
        
        axis_rect.size = (Vector2){10000.0,10000.0};
        
        hit_point = get_collision_rect_3D_B(axis_rect).point;
        
        Vector3 bone_position = axis_rect.position;
        
        Quaternion rotation_offset = QuaternionIdentity();
        
        Quaternion invert_axis_rotation = QuaternionInvert(axis_rect.rotation);
        
        Vector3 direction_to_previous_point = Vector3Subtract(previous_drag_point , bone_position);
        direction_to_previous_point = Vector3RotateByQuaternion(direction_to_previous_point , invert_axis_rotation);
        
        Vector3 direction_to_point = Vector3Subtract(hit_point , bone_position);
        direction_to_point = Vector3RotateByQuaternion(direction_to_point , invert_axis_rotation);
        
        float previous_angle = atan2f(direction_to_previous_point.y , direction_to_previous_point.x);
        float current_angle = atan2f(direction_to_point.y , direction_to_point.x);
        
        Vector3 axis = {};
        
        if (dragging_axis == R_Z) axis = (Vector3){0,0,1};
        if (dragging_axis == R_Y) axis = (Vector3){0,-1,0};
        if (dragging_axis == R_X) axis = (Vector3){1,0,0};
        
        rotation_offset = QuaternionFromAxisAngle(axis, current_angle - previous_angle);
        
        selected_bone->state.local_rotation = QuaternionMultiply(selected_bone->state.local_rotation, rotation_offset);
        
        if(!editor->playing)
        {
            if(editor->selected_clip_index != -1)
            {
                //_Clip * ClipToAssign = ClipArray[editor->SelectedClipIndex];
                
                if(editor_type == edit_base_pose)
                {
                    selected_model->bone_buffer.data[single_bone_index].state.local_rotation = QuaternionMultiply(selected_model->bone_buffer.data[single_bone_index].state.local_rotation , rotation_offset);
                }
                
                int key_frame_count = 0;
                hash_table_iterate(key_frame_index , GetKeyFrameHash(editor->current_frame_at_timeline , single_bone_index) , &clip_to_assign->key_frame_hash_table)
                {
                    KeyFrame * key_frame = all_key_frame_buffer.data + key_frame_index;
                    if(key_frame->bone_index != single_bone_index) continue;
                    if(key_frame->frame_index != editor->current_frame_at_timeline) continue;
                    
                    key_frame->bone_state.local_rotation = QuaternionMultiply(key_frame->bone_state.local_rotation , rotation_offset);
                    
                    if(key_frame_count>0) CATCH;
                    key_frame_count++;
                }
                
            }
            
        }
        
        //SelectedBone->State.local_rotation = QuaternionNormalize(SelectedBone->State.local_rotation);
        //SelectedBone->State.local_rotation = QuaternionIdentity();
        
        //printf("Hit point : %f %f %f\n" ,HitPoint.x , HitPoint.y , HitPoint.z);
        
        previous_drag_point = hit_point;
    }
    
    return hovering;
}

//a bit crazy
//matrix did changed here
internal void bone_selection_and_edit_bone_state( int current_frame_index)
{
    if(key_pressed(KEY_E))
    {
        editor->control_rig = !editor->control_rig;
    }
    
    //this thing is messy
    local_persist bool redrag_selection = true;
    
    if(mouse_pressed(MOUSE_BUTTON_LEFT))
    {
        redrag_selection = true;
    }
    
    bool curve_visualization = true;
    
    if(editor_type != edit_animation) curve_visualization = false;
    if(editor->turn_off_bezier_curve) curve_visualization = false;
    if(clip_bone_stack_count == 0) curve_visualization = false;
    
    if(curve_visualization)
    {
        editor->selecting_bezier = false;
        
        local_persist bool interacting_with_bezier_curve = false;
        local_persist bool modify_key_frame_position_only = false;
        local_persist bool selected_previous_bezier_point = false;
        local_persist KeyFrame * selected_bezier_key_frame = 0;
        local_persist KeyFrame * selected_bezier_other_key_frame = 0;
        local_persist Vector3 selected_bezier_base_bone_offset = {};
        local_persist Quaternion selected_bezier_base_bone_rotation = {};
        //local_persist _Bone * SelectedBezierBone = 0;
        
        for(int select_stack_index = 0 ; select_stack_index < editor->selected_bone_count ; select_stack_index++)
        {
            BoneSelection * bone_selection = editor->selected_bone_stack + select_stack_index;
            ClipBone * selected_clip_bone = clip_bone_stack + bone_selection->clip_bone_stack_index;
            Clip * selected_clip = clip_array[bone_selection->clip_index];
            
            int selected_bone_index = bone_selection->bone_index;
            
            Bone * selected_final_bone = selected_clip_bone->final_bone_pose + selected_bone_index;
            Bone * selected_offset_bone = selected_clip_bone->bone_pose_offset + selected_bone_index;
            
            int previous_key_frame_index = -1;
            hash_table_iterate_reverse(key_frame_index , selected_bone_index , &selected_clip->key_frame_hash_table_by_bone)
            {
                previous_key_frame_index = key_frame_index;
                break;
            }
            
            KeyFrame * previous_key_frame = all_key_frame_buffer.data + previous_key_frame_index;
            
			Color key_frame_line_color = Fade(WHITE , 0.2f);
            
            //TODO : this isn't the correct path
            //this only take account the bone pose offset in current frame
            //to get the correct path you need to account other bone and keyframe offset
            //which mean you need have know every bone pose on every frame 
            
            //i haven't account for rotation yet
            
            //the rotation is only half done
            //origin doesn't work at all
            
            //draw_bezier_path
            hash_table_iterate(key_frame_index , selected_bone_index , &selected_clip->key_frame_hash_table_by_bone)
            {
                KeyFrame * key_frame = all_key_frame_buffer.data + key_frame_index;
                if(key_frame->bone_index != selected_bone_index) continue;
                
                Quaternion base_bone_rotation = selected_final_bone->rotation;
                Vector3 base_bone_offset =  Vector3RotateByQuaternion(selected_offset_bone->state.local_position , base_bone_rotation);
				Vector3 base_bone_position = Vector3Subtract( selected_final_bone->position , base_bone_offset);
                
                //DrawArrowRay(SelectedFinalBone->position , Vector3Negate(BaseBoneOffset) , RED);
                
                Vector3 previous_key_frame_offset_position = Vector3RotateByQuaternion(previous_key_frame->bone_state.local_position , base_bone_rotation);
                Vector3 previous_key_frame_position = Vector3Add( base_bone_position , previous_key_frame_offset_position);
                
				Vector3 key_frame_offset_position = Vector3RotateByQuaternion(key_frame->bone_state.local_position , base_bone_rotation);
                Vector3 key_frame_position = Vector3Add( base_bone_position , key_frame_offset_position);
                
                //DrawRoundLine(BaseBoneposition , KeyFrameposition , 5 , RED , WHITE);
                //DrawRoundLine(SelectedFinalBone->position , KeyFrameposition , 5 , GREEN , WHITE);
                
                Vector3 line_direction = Vector3Subtract(key_frame_position , previous_key_frame_position);
                line_direction = Vector3Scale(Vector3Normalize(line_direction),0.1f);
                
                Vector3 previous_key_frame_bezier_point = Vector3Add(line_direction , Vector3RotateByQuaternion(previous_key_frame->next_bezier_offset , base_bone_rotation));
                previous_key_frame_bezier_point = Vector3Add(previous_key_frame_bezier_point , previous_key_frame_position);
                
                Vector3 key_frame_bezier_point = Vector3Add(Vector3Negate(line_direction) , Vector3RotateByQuaternion(key_frame->previous_bezier_offset , base_bone_rotation));
                key_frame_bezier_point = Vector3Add(key_frame_bezier_point , key_frame_position);
                
                draw_round_line_B(previous_key_frame_position , previous_key_frame_bezier_point , 10 , Fade(BLUE,0.5f) );
                draw_round_line_B(key_frame_position , key_frame_bezier_point , 10 , Fade(RED,0.5f) );
                
                Rect previous_bezier_point_rect = get_billboard_rect(previous_key_frame_bezier_point , 0.1);
                Rect bezier_point_rect = get_billboard_rect(key_frame_bezier_point , 0.1);
                
                Color previous_bezier_point_color = Fade(BLUE , 0.5f);
                Color bezier_point_color = Fade(RED , 0.5f);
                Color key_frame_point_color = Fade(YELLOW , 0.5f);
                
                if(check_collision_rect_3D(previous_bezier_point_rect))
                {
                    
                    previous_bezier_point_color = BLUE;
                    if(mouse_pressed(MOUSE_BUTTON_LEFT))
                    {
                        
                        interacting_with_bezier_curve = true;
                        selected_bezier_key_frame = previous_key_frame;
                        selected_bezier_other_key_frame = key_frame;
                        selected_bezier_base_bone_offset = base_bone_position;
                        selected_bezier_base_bone_rotation = base_bone_rotation;
                        selected_previous_bezier_point = false;
                        
                    }
                    
                }
                
                if(check_collision_rect_3D(bezier_point_rect))
                {
                    bezier_point_color = RED;
                    if(mouse_pressed(MOUSE_BUTTON_LEFT))
                    {
                        
                        interacting_with_bezier_curve = true;
                        selected_bezier_key_frame = key_frame;
                        selected_bezier_other_key_frame = previous_key_frame;
                        selected_bezier_base_bone_offset = base_bone_position;
                        selected_bezier_base_bone_rotation = base_bone_rotation;
                        selected_previous_bezier_point = true;
                        
                    }
                }
                
                draw_circle( previous_bezier_point_rect , previous_bezier_point_color);
                draw_circle( bezier_point_rect , bezier_point_color);
                
                int frame_count = get_key_frame_count(previous_key_frame , key_frame ,editor->start_frame_index  , editor->timeline_frame_length );
                
                for(int frame_index = 0 ; frame_index < frame_count ; frame_index++)
                {
                    float bezier_start_time = ((float)frame_index) / ((float)frame_count);
                    float bezier_end_time = ((float)(frame_index+1)) / ((float)frame_count);
                    
                    Vector3 bezier_start_point = vector3_cubic_bezier(previous_key_frame_position , previous_key_frame_bezier_point , key_frame_bezier_point, key_frame_position  , bezier_start_time);
                    Vector3 bezier_end_point = vector3_cubic_bezier(previous_key_frame_position , previous_key_frame_bezier_point , key_frame_bezier_point, key_frame_position  , bezier_end_time);
                    
                    //BezierStartPoint = Vector3RotateByQuaternion(BezierStartPoint , QuaternionInvert(BaseBonerotation));
                    //BezierEndPoint = Vector3RotateByQuaternion(BezierEndPoint , QuaternionInvert(BaseBonerotation));
                    
                    //BezierStartPoint = Vector3Add(BezierStartPoint , BaseBoneposition);
                    //BezierEndPoint = Vector3Add(BezierEndPoint , BaseBoneposition);
                    
                    draw_round_line_B(bezier_start_point , bezier_end_point ,10 ,key_frame_line_color );
                }
                
                draw_round_line_B(previous_key_frame_position , key_frame_position,10 ,Fade(WHITE , 0.05f));
                
                Rect key_frame_rect = get_billboard_rect(key_frame_position , 0.1 );
                
                if(check_collision_rect_3D(key_frame_rect))
                {
                    key_frame_point_color = YELLOW;
                    
                    if(mouse_pressed(MOUSE_BUTTON_LEFT))
                    {
                        
                        interacting_with_bezier_curve = true;
                        modify_key_frame_position_only = true;
                        selected_bezier_base_bone_offset = base_bone_position;
                        selected_bezier_base_bone_rotation = base_bone_rotation;
                        selected_bezier_key_frame = key_frame;
                        
                    }
                }
                
                draw_circle(key_frame_rect , key_frame_point_color);
                
                previous_key_frame_index = key_frame_index;
                previous_key_frame = key_frame;
            }
            
        }
        
        if(mouse_released(MOUSE_BUTTON_LEFT))
        {
            modify_key_frame_position_only = false;
            
            interacting_with_bezier_curve = false;
        }
        
        if(interacting_with_bezier_curve)
        {
            
            local_persist Vector3 dragging_plane_point = {};
            local_persist Vector3 previous_dragging_point = {};
            
            editor->selecting_bezier = true;
            
            if(modify_key_frame_position_only)
            {
                
                if(mouse_pressed(MOUSE_BUTTON_LEFT))
                {
                    redrag_selection = false;
                    //DraggingPlanePoint = SelectedBezierKeyFrame->bone_state.local_position;
                    dragging_plane_point = Vector3Add( Vector3RotateByQuaternion(selected_bezier_key_frame->bone_state.local_position,selected_bezier_base_bone_rotation) , selected_bezier_base_bone_offset);
                    previous_dragging_point = dragging_plane_point;
                }
                
                //TODO : it just snap to the point instead adding offset
                Vector3 dragging_point = get_collision_quad_3D(get_billboard_quad(dragging_plane_point , 100000,100000)).point;
                
                Vector3 new_key_frame_offset = Vector3Subtract(dragging_point , previous_dragging_point);
                previous_dragging_point = dragging_point;
                new_key_frame_offset = Vector3RotateByQuaternion(new_key_frame_offset , QuaternionInvert(selected_bezier_base_bone_rotation));
                
                selected_bezier_key_frame->bone_state.local_position = Vector3Add(selected_bezier_key_frame->bone_state.local_position , new_key_frame_offset);
            }
            else
            {
                
                Vector3 start_point = {};
                Vector3 end_point = {};
                Vector3 bezier_offset = {};
                
                if(selected_previous_bezier_point)
                {
                    bezier_offset = selected_bezier_key_frame->previous_bezier_offset;
                }
                else
                {
                    bezier_offset = selected_bezier_key_frame->next_bezier_offset;
                }
                
                bezier_offset = Vector3RotateByQuaternion(bezier_offset , selected_bezier_base_bone_rotation);
                
                start_point = Vector3RotateByQuaternion(selected_bezier_key_frame->bone_state.local_position , selected_bezier_base_bone_rotation );
                end_point = Vector3RotateByQuaternion(selected_bezier_other_key_frame->bone_state.local_position , selected_bezier_base_bone_rotation );
                
                Vector3 direction = Vector3Subtract(end_point , start_point);
                direction = Vector3Scale(Vector3Normalize(direction),0.1f);
                
                Vector3 bezier_direction = Vector3Add(direction , bezier_offset);
                //BezierDirection = Vector3RotateByQuaternion(BezierDirection , SelectedBezierBaseBonerotation);
                
                Vector3 bezier_start = Vector3Add(Vector3RotateByQuaternion(selected_bezier_key_frame->bone_state.local_position , selected_bezier_base_bone_rotation) , selected_bezier_base_bone_offset);
                Vector3 bezier_end = Vector3Add(bezier_start , bezier_direction );
                
                Rect bezier_end_rect = get_billboard_rect(bezier_end , 0.1);
                
                if(mouse_pressed(MOUSE_BUTTON_LEFT))
                {
                    redrag_selection = false;
                    
                    dragging_plane_point = bezier_end;
                    previous_dragging_point = bezier_end;
                }
                
                //TODO : it just snap to the point instead adding offset
                Vector3 dragging_point = get_collision_quad_3D(get_billboard_quad(dragging_plane_point , 100000,100000)).point;
                Vector3 new_bezier_end_point = Vector3Add(bezier_end ,Vector3Subtract(dragging_point , previous_dragging_point));
                previous_dragging_point = dragging_point;
                Vector3 new_bezier_offset = Vector3Subtract(new_bezier_end_point , bezier_start);
                new_bezier_offset = Vector3Subtract(new_bezier_offset , direction);
                
                new_bezier_offset = Vector3RotateByQuaternion(new_bezier_offset , QuaternionInvert(selected_bezier_base_bone_rotation));
                
                if(selected_previous_bezier_point)
                {
                    selected_bezier_key_frame->previous_bezier_offset = new_bezier_offset;
                }
                else
                {
                    selected_bezier_key_frame->next_bezier_offset = new_bezier_offset;
                }
                
                draw_circle(bezier_end_rect , YELLOW);
                draw_round_line_B( bezier_start , bezier_end , 20  , YELLOW );
            }
            
        }
        
    }
    
    //hated it
    //terrible control
    
    //SingleSelectionAndMultiDrag
    local_persist bool dragging_bone_position = false;
    local_persist Vector3 previous_drag_point ={};
	local_persist Vector3 drag_plane_position = {};
    
    if(mouse_pressed(MOUSE_BUTTON_LEFT))
    {
        dragging_bone_position = false;
    }
    
    if(mouse_released(MOUSE_BUTTON_LEFT))
    {
        dragging_bone_position = false;
    }
    
    pruning_3D_line = false;
    change_matrix(viewport_screen_pixel_matrix);
    
    for(int clip_bone_stack_index = 0 ; clip_bone_stack_index  < clip_bone_stack_count ; clip_bone_stack_index++)
    {
        ClipBone * clip_bone = clip_bone_stack + clip_bone_stack_index;
        
        for(int bone_index = 0 ; bone_index < selected_model->bone_buffer.count ; bone_index++)
        {
            
            clip_bone->hovered_bone[bone_index] = false;
            
            Bone * final_bone = clip_bone->final_bone_pose + bone_index;
			Vector3 bone_interact_point ={};
            
            if(editor->control_rig)
            {
                if(!is_rig(bone_index)) continue;
                bone_interact_point = final_bone->position;
            }
            else
            {
                
                if(is_rig(bone_index)) continue;
                
                Vector3 bone_position_end = Vector3RotateByQuaternion(final_bone->state.end_point_offset , final_bone->rotation);
                bone_position_end = Vector3Add(bone_position_end , final_bone->position);
                
                bone_interact_point = bone_position_end;
                
            }
            
            Vector3 bone_screen_point = transform_vector(bone_interact_point , world_3D_to_screen_matrix);
            
            Rect bone_screen_rect = get_rect();
            bone_screen_rect.position = bone_screen_point;
            bone_screen_rect.size = (Vector2){40 ,40};
            
            Vector3 hit_point = mouse_on_plane(bone_interact_point);
            
            //TODO : this is dumb
            //Quad interact_quad = get_billboard_quad(bone_interact_point , 100000 , 100000 );
            //RayCollision position_quad_result = get_collision_quad_3D(interact_quad);
            
            Vector2 current_mouse_position = app_data->mouse_position;
            
            //click_and_select_bone
            if(check_collision_rect(bone_screen_rect , current_mouse_position))
            {
                clip_bone->hovered_bone[bone_index] = true;
                
                if(mouse_pressed(MOUSE_BUTTON_LEFT))
                {
                    
                    if(!clip_bone->selected_bone[bone_index])
                    {
                        if(key_pressing(KEY_LEFT_SHIFT))
                        {
                            BoneSelection * NewBoneSelection = editor->selected_bone_stack + editor->selected_bone_count++;
                            NewBoneSelection->clip_bone_stack_index = clip_bone_stack_index;
                            NewBoneSelection->clip_index = clip_bone->clip_index;
                            NewBoneSelection->bone_index = bone_index;
                            
                            clip_bone->selected_bone[bone_index] = true;
                        }
                        else
                        {
                            for(int StackIndex = 0 ; StackIndex < editor->selected_bone_count ; StackIndex++)
                            {
                                ClipBone * ClipBoneToUnSelect = clip_bone_stack + StackIndex;
                                
                                for(int select_bone_index = 0 ; select_bone_index < selected_model->bone_buffer.count ; select_bone_index++)
                                {
                                    ClipBoneToUnSelect->selected_bone[select_bone_index] = false;
                                }
                            }
                            
                            editor->selected_bone_count = 0;
                            
                            BoneSelection * NewBoneSelection = editor->selected_bone_stack + editor->selected_bone_count++;
                            NewBoneSelection->clip_bone_stack_index = clip_bone_stack_index;
                            NewBoneSelection->clip_index = clip_bone->clip_index;
                            NewBoneSelection->bone_index = bone_index;
                            
                            clip_bone->selected_bone[bone_index] = true;
                        }
                    }
                    
                    previous_drag_point = hit_point;
                    drag_plane_position = bone_interact_point;
                    
                    dragging_bone_position = true;
                    redrag_selection = false;
                    
                }
                
            }
            
        }
    }
    
    //ApplyDragOffsetTo_existKeyFrame
    if(dragging_bone_position)
    {
        
        Vector3 drag_point = mouse_on_plane(drag_plane_position);
        
        //Quad bone_centre_quad = get_billboard_quad(drag_plane_position, 100000.0f, 100000.0f);
        //Vector3 drag_point = get_collision_quad_3D(bone_centre_quad).point;
        //DrawQuadLine(GetBillboardQuad(DragPoint , 0.1 , 0.1) , WHITE , 8);
        
        Vector3 plane_offset= drag_point;
        plane_offset = Vector3Subtract(plane_offset, previous_drag_point);
        
        previous_drag_point = drag_point;
        
        for(int selected_bone_stack_index = 0 ; selected_bone_stack_index < editor->selected_bone_count ; selected_bone_stack_index++)
        {
            BoneSelection * dragging_bone_selection = editor->selected_bone_stack + selected_bone_stack_index;
            
            int dragging_bone_index = dragging_bone_selection->bone_index;
            ClipBone * dragging_clip_bone = clip_bone_stack + dragging_bone_selection->clip_bone_stack_index;
            Bone * dragging_bone = dragging_clip_bone->final_bone_pose + dragging_bone_index;
            
            //it just feel right to do it
            //also the world rotation is compute after
            //something could go wrong
            Quaternion revert_rotation = QuaternionMultiply(dragging_bone->rotation , QuaternionInvert(dragging_bone->state.local_rotation));
            //Revertrotation = selected_bone->state.local_rotation;
            plane_offset = Vector3RotateByQuaternion(plane_offset,QuaternionInvert(revert_rotation));
            //printf("Offset : %f %f %f\n" ,PlaneOffset.x , PlaneOffset.y , PlaneOffset.z);
            
            int assign_key_frame_count = 0;
            
            if (dragging_bone->free_position)
            {
                if(!editor->playing)
                {
                    
                    Clip * current_clip = clip_array[dragging_clip_bone->clip_index];
                    
                    hash_table_iterate(key_frame_index , GetKeyFrameHash(editor->current_frame_at_timeline , dragging_bone_index) , &current_clip->key_frame_hash_table)
                    {
                        
                        KeyFrame * key_frame = all_key_frame_buffer.data + key_frame_index;
                        if(key_frame->bone_index != dragging_bone_index) continue;
                        if(key_frame->frame_index != editor->current_frame_at_timeline) continue;
                        
                        key_frame->bone_state.local_position = Vector3Add(key_frame->bone_state.local_position , plane_offset);
                        
                        if(assign_key_frame_count>0) CATCH;
                        assign_key_frame_count++;
                    }
                    
                    if(editor_type == edit_base_pose)
                    {
                        selected_model->bone_buffer.data[dragging_bone_index].state.local_position = Vector3Add(selected_model->bone_buffer.data[dragging_bone_index].state.local_position , plane_offset);
                    }
                }
                
            }
            
        }
    }
    else
    {
        //SingleBonerotation
        
        pruning_3D_line = true;
        change_matrix(world_3D_matrix);
        if(editor->selected_bone_count == 1)
        {
            
            BoneSelection * single_bone_selection = editor->selected_bone_stack;
            
            int single_bone_index = single_bone_selection->bone_index;
            Bone * final_bone_array_copy = clip_bone_stack[single_bone_selection->clip_bone_stack_index].final_bone_pose;
            Bone * selected_bone =  final_bone_array_copy + single_bone_index;
            
            bool interacting_selected_bone = check_selected_bone_rotation( final_bone_array_copy , single_bone_index , clip_array[single_bone_selection->clip_index]);
            
            if(mouse_pressed(MOUSE_BUTTON_LEFT))
            {
                if(interacting_selected_bone)
                {
                    redrag_selection = false;
                }
                else
                {
                    editor->selected_bone_count = 0;
                    
                }
                
            }
        }
        
        pruning_3D_line = false;
        change_matrix(viewport_screen_pixel_matrix);
    }
    
    //MultiSelection
    local_persist Vector3 mouse_start_drag_position = {};
    Vector2 mouse_screen_position = app_data->mouse_position;
    
    if(mouse_pressed(MOUSE_BUTTON_LEFT))
    {
        mouse_start_drag_position.x = mouse_screen_position.x;
        mouse_start_drag_position.y = mouse_screen_position.y;
    }
    
    local_persist bool dragging_selection = false;
    
    if(redrag_selection)
    {
        
        Vector3 current_mouse_drag_position = {mouse_screen_position.x , mouse_screen_position.y , 0};
        
        Rect mouse_drag_rect = get_rect();
        mouse_drag_rect.position= Vector3Add(mouse_start_drag_position , current_mouse_drag_position);
        mouse_drag_rect.position.x /= 2.0f;
        mouse_drag_rect.position.y /= 2.0f;
        
        mouse_drag_rect.size.x = mouse_start_drag_position.x - current_mouse_drag_position.x;
        if(mouse_drag_rect.size.x < 0) mouse_drag_rect.size.x *= -1;
        
        mouse_drag_rect.size.y = mouse_start_drag_position.y - current_mouse_drag_position.y;
        if(mouse_drag_rect.size.y < 0) mouse_drag_rect.size.y *= -1;
        
        if(mouse_pressed(MOUSE_BUTTON_LEFT))
        {
            dragging_selection = true;
            if(!key_pressing(KEY_LEFT_SHIFT))
            {
                
                for(int StackIndex = 0 ; StackIndex < clip_bone_stack_count ; StackIndex++)
                {
                    ClipBone * ClipBoneToUnSelect = clip_bone_stack + StackIndex;
                    
                    for(int bone_index = 0 ; bone_index < selected_model->bone_buffer.count ; bone_index++)
                    {
                        ClipBoneToUnSelect->selected_bone[bone_index] = false;
                    }
                }
                
                editor->selected_bone_count = 0;
            }
        }
        
        if(dragging_selection)
        {
            draw_rect_line_E(mouse_drag_rect , WHITE , 5);
        }
        
        for(int clip_bone_stack_index = 0 ; clip_bone_stack_index < clip_bone_stack_count ; clip_bone_stack_index++)
        {
            ClipBone * current_clipBone = clip_bone_stack + clip_bone_stack_index;
            
            for(int bone_index = 0 ; bone_index < selected_model->bone_buffer.count ; bone_index++)
            {
                Bone * current_bone = current_clipBone->final_bone_pose + bone_index;
				Vector3 bone_screen_point = {};
                
                if(editor->control_rig)
                {
                    if(!is_rig(bone_index)) continue;
                    
                    bone_screen_point = current_bone->position;
                }
                else
                {
                    if(is_rig(bone_index)) continue;
                    
					Vector3 bone_position_end = Vector3RotateByQuaternion(current_bone->state.end_point_offset , current_bone->rotation);
                    bone_position_end = Vector3Add(bone_position_end , current_bone->position);
                    
                    bone_screen_point = bone_position_end;
                }
                
                bone_screen_point = transform_vector( bone_screen_point , world_3D_to_screen_matrix);
                
                Rect bone_screen_rect = get_rect();
                bone_screen_rect.position = bone_screen_point;
                bone_screen_rect.size = (Vector2){40 ,40};
                
                if(check_collision_rect_with_rect(mouse_drag_rect , bone_screen_rect ))
                {
                    if(dragging_selection)
                    {
                        current_clipBone->hovered_bone[bone_index] = true;
                        
                        if(mouse_released(MOUSE_BUTTON_LEFT))
                        {
                            if(!current_clipBone->selected_bone[bone_index])
                            {
                                current_clipBone->selected_bone[bone_index] = true;
                                
                                BoneSelection * new_selection = editor->selected_bone_stack + editor->selected_bone_count++;
                                
                                new_selection->clip_bone_stack_index = clip_bone_stack_index;
                                new_selection->clip_index = current_clipBone->clip_index;
                                new_selection->bone_index = bone_index;
                            }
                            
                        }
                    }
                    
                }
            }
        }
        
    }
    
    
    if(mouse_released(MOUSE_BUTTON_LEFT))
    {
        dragging_selection = false;
    }
    
    
    for(int clip_bone_stack_index = 0 ; clip_bone_stack_index < clip_bone_stack_count ; clip_bone_stack_index++)
    {
        ClipBone * current_clip_bone = clip_bone_stack + clip_bone_stack_index;
        
        for(int bone_index = 0 ; bone_index < selected_model->bone_buffer.count ; bone_index++)
        {
            
            Bone * bone = current_clip_bone->final_bone_pose + bone_index;
            
            Vector3 bone_screen_point = {};
            if(editor->control_rig)
            {
                if(!is_rig(bone_index)) continue;
                
                bone_screen_point = bone->position;
            }
            else
            {
                
                if(is_rig(bone_index)) continue;
                
                Vector3 bone_position_end = Vector3RotateByQuaternion(bone->state.end_point_offset , bone->rotation);
                bone_position_end = Vector3Add(bone_position_end , bone->position);
                bone_screen_point = bone_position_end;
                
            }
            
            bone_screen_point = transform_vector( bone_screen_point , world_3D_to_screen_matrix);
            
            Rect bone_screen_rect = get_rect();
            bone_screen_rect.position = bone_screen_point;
            bone_screen_rect.size = (Vector2){40 ,40};
            
            //DrawBoneSelection
#if 1
            if(current_clip_bone->selected_bone[bone_index])
            {
                draw_rect_line_E(bone_screen_rect , BLUE , 5);
            }
            else if(current_clip_bone->hovered_bone[bone_index])
            {
                draw_rect_line_E(bone_screen_rect , YELLOW , 5);
            }
            else
            {
                draw_rect_line_E(bone_screen_rect , Fade(YELLOW , 0.2f) , 5);
            }
            
            current_clip_bone->hovered_bone[bone_index] = false;
#endif
        }
    }
    
    pruning_3D_line = true;
    change_matrix(world_3D_matrix);
}

internal void iterate_bone_structure(Bone * bone_array , Bone * root_bone)
{
    
    Bone * bone_stack[256] = {};
    int bone_stack_count = 0;
    
    bone_stack[bone_stack_count++] = root_bone;
    
    //printf("\nBone Start \n");
    
    for (; bone_stack_count > 0;)
    {
        
        Bone * iterating_bone = bone_stack[--bone_stack_count];
        int iterating_bone_index = iterating_bone->bone_index;
        
        //Vector3 iterating_bone_end_position = Vector3Add(iterating_bone->position , iterating_bone->state.direction);
        
        hash_table_iterate(child_bone_index , iterating_bone_index , &selected_model->bone_children_hash_table)
        {
            
            Bone * child_bone = bone_array + child_bone_index;
            if(child_bone->parent_bone_index != iterating_bone_index)
            {
                continue;
            }
            
            Quaternion rotation = QuaternionMultiply(iterating_bone->rotation , child_bone->state.local_rotation);
            child_bone->rotation = rotation;
            
            Vector3 child_head = Vector3RotateByQuaternion(child_bone->state.local_position , iterating_bone->rotation);
            Vector3 child_tail = Vector3RotateByQuaternion(child_bone->state.end_point_offset, rotation);
            child_tail = Vector3Add(child_head , child_tail);
            
            child_bone->position = Vector3Add( child_head , iterating_bone->position);
            child_bone->end_position = Vector3Add( child_tail , iterating_bone->position);
            
            bone_stack[bone_stack_count++] = child_bone;
            
        }
        
    }
    
}

internal void update_bone_structure( Bone * bone_array)
{
    
    //what?
	list_foreach(root_bone_index , &selected_model->root_bone_list )
	{
        Bone * root_bone = bone_array + root_bone_index;
        
        root_bone->position = root_bone->state.local_position;
        root_bone->end_position = root_bone->state.end_point_offset;
        root_bone->rotation = root_bone->state.local_rotation;
        root_bone->end_position = Vector3RotateByQuaternion(root_bone->end_position , root_bone->rotation);
        root_bone->end_position = Vector3Add(root_bone->end_position , root_bone->position);
        
        //printf("%f %f %f \n" , RootBone->position.x , RootBone->position.y , RootBone->position.z);
        
        iterate_bone_structure(bone_array , root_bone);
	}
    
}

#define DEBUG_IK_DISPLAY 0
#define DEBUG_IK_POLE_DISPLAY 0

internal void bone_IK_update_EX( Bone * bone_array , Bone * base_pose_bone_array , Bone * target_bone , Bone * pole_bone , Bone * IK_bone , int iteration_count , int bone_chain_max_length)
{
    
    Bone * bone_stack[256] = {};
    int bone_stack_count = 0;
    
    Bone * bone_chain[256] = {};
    int bone_chain_count = 0;
    
    bone_stack[bone_stack_count++] = IK_bone;
    bone_chain[bone_chain_count++] = IK_bone;
    
    int bone_chain_length = 0;
    
    for(;;)
    {
        
        if(bone_stack_count <= 0) break;
        if(bone_chain_length >= bone_chain_max_length) break;
        bone_chain_length++;
        
        Bone * iterating_bone = bone_stack[--bone_stack_count];
        int iterating_bone_index = iterating_bone->bone_index;
        iterating_bone->state.local_position = (Vector3){};
        
        Bone * parent_bone = bone_array + iterating_bone->parent_bone_index;
        
        bone_chain[bone_chain_count++] = parent_bone;
        bone_stack[bone_stack_count++] = parent_bone;
        
    }
    
    Bone * start_bone = bone_chain[bone_chain_count - 1];
    Bone * end_bone = bone_chain[0];
    
    iterate_bone_structure(bone_array , start_bone);
    
    Vector3 chain_end_position = Vector3RotateByQuaternion(end_bone->state.end_point_offset , end_bone->state.local_rotation);
    chain_end_position = Vector3Add(chain_end_position , end_bone->position);
    
    Vector3 chain_start_to_target_direction = Vector3Subtract(target_bone->position , start_bone->position);
    Vector3 chain_start_to_end_direction = Vector3Subtract(chain_end_position , start_bone->position);
    chain_start_to_target_direction = Vector3Normalize(chain_start_to_target_direction);
    chain_start_to_end_direction = Vector3Normalize(chain_start_to_end_direction);
    
    Quaternion chain_rotation = QuaternionFromVector3ToVector3(chain_start_to_end_direction , chain_start_to_target_direction);
    
#if DEBUG_IK_POLE_DISPLAY
    draw_arrow_ray(start_bone->position, chain_start_to_target_direction , BLUE);
    draw_arrow_ray(start_bone->position, chain_start_to_end_direction , GREEN);
#endif
    
    Bone * pole_bone_in_edit = base_pose_bone_array + pole_bone->bone_index;
    Vector3 chain_start_to_base_pole_direction = Vector3Subtract(pole_bone_in_edit->position , start_bone->position );
    chain_start_to_base_pole_direction = Vector3Normalize(chain_start_to_base_pole_direction);
    
    Vector3 current_chain_right = Vector3CrossProduct(chain_start_to_base_pole_direction , chain_start_to_end_direction );
    Vector3 current_chain_forward = Vector3CrossProduct(chain_start_to_end_direction , current_chain_right);
    
    current_chain_right = Vector3RotateByQuaternion(current_chain_right , chain_rotation);
    current_chain_forward = Vector3RotateByQuaternion(current_chain_forward , chain_rotation);
    current_chain_forward = Vector3Normalize(current_chain_forward);
    
    Vector3 chain_start_to_pole_direction = Vector3Subtract(pole_bone->position , start_bone->position);
    Vector3 target_chain_right = Vector3CrossProduct( chain_start_to_pole_direction , chain_start_to_target_direction);
    Vector3 target_chain_forward = Vector3CrossProduct( chain_start_to_target_direction , target_chain_right );
    target_chain_forward = Vector3Normalize(target_chain_forward);
    
#if DEBUG_IK_POLE_DISPLAY
    draw_arrow_ray(start_bone->position, target_chain_right , PURPLE);
    draw_arrow_ray(start_bone->position, target_chain_forward , PINK);
    draw_arrow_ray(start_bone->position, current_chain_forward , RED);
#endif
    
    Quaternion rotate_to_toward_pole = QuaternionFromVector3ToVector3( current_chain_forward , target_chain_forward);
    chain_rotation = QuaternionMultiply(rotate_to_toward_pole , chain_rotation);
    
    float chain_length = 0;
    
    for(int bone_chain_index = bone_chain_count - 1 ; bone_chain_index >= 0  ; bone_chain_index--)
    {
        Bone * parent_bone = bone_chain[bone_chain_index];
        parent_bone->rotation = QuaternionMultiply( parent_bone->state.local_rotation , chain_rotation);
        
        chain_length += Vector3Length(parent_bone->state.end_point_offset);
        
        hash_table_iterate(child_index , parent_bone->bone_index , &selected_model->bone_children_hash_table)
        {
            
            Bone * child_bone = bone_array + child_index;
            
            child_bone->position = Vector3RotateByQuaternion(parent_bone->state.end_point_offset , parent_bone->rotation);
            child_bone->position = Vector3Add(child_bone->position , parent_bone->position);
            
        }
        
    }
    
    float start_to_target_distance = Vector3Distance(target_bone->position , start_bone->position);
    if(start_to_target_distance > chain_length) 
    {
        iterate_bone_structure(bone_array , bone_chain[0]);
        return;
    }
    
    for(int iteration_index = 0 ; iteration_index < iteration_count ; iteration_index++)
    {
        
        Vector3 end_position = IK_bone->position;
        end_position = Vector3Add(end_position , Vector3RotateByQuaternion(IK_bone->state.end_point_offset , IK_bone->rotation) );
        //DrawQuadLine(GetBillboardQuad(Endposition , 0.1 , 0.1 ) ,WHITE);
        
        Vector3 start_position = start_bone->position;
        //Startposition = Vector3Add(Startposition , StartBone->position);
        
        for(int bone_chain_index = bone_chain_count - 1 ; bone_chain_index >= 0 ; bone_chain_index--)
        {
            
            Bone * parent_bone = bone_chain[bone_chain_index];
            Bone * child_bone = bone_chain[bone_chain_index - 1];
            
            Vector3 direction_to_end_bone = Vector3Subtract(end_position ,parent_bone->position);
            Vector3 direction_to_target_bone = Vector3Subtract(target_bone->position ,parent_bone->position);
            
            //wtf???
            //normalizing it actually cause problem when bone get too close
            //there is more quaternion magic i have to learn yet
            
            //direction_to_end_bone = Vector3Normalize(direction_to_end_bone);
            //direction_to_target_bone = Vector3Normalize(direction_to_target_bone);
            
#if DEBUG_IK_DISPLAY
            draw_arrow_ray(parent_bone->position, direction_to_end_bone , BLUE);
            draw_arrow_ray(parent_bone->position, direction_to_target_bone , GREEN);
#endif
            
            //TODO : try Fabrik?
            Quaternion bone_rotation = QuaternionFromVector3ToVector3(direction_to_end_bone , direction_to_target_bone );
            parent_bone->rotation = QuaternionMultiply(bone_rotation , parent_bone->rotation);
            
            Vector3 parent_bone_end_offset = Vector3RotateByQuaternion( parent_bone->state.end_point_offset , parent_bone->rotation );
            
            if(bone_chain_index > 0)
            {
                child_bone->position = Vector3Add(parent_bone->position , parent_bone_end_offset);
            }
        }
        
    }
    
    iterate_bone_structure(bone_array , bone_chain[0]);
}

internal void bone_IK_update(Bone * bone_array , Bone * base_pose_bone_array , int target_bone_index , int pole_bone_index , int IK_bone_index , int iteration_count , int bone_chain_max_length)
{
    
    Bone * target_bone = bone_array + target_bone_index;
    Bone * pole_bone = bone_array + pole_bone_index;
    Bone * IK_bone = bone_array + IK_bone_index;
    
    bone_IK_update_EX( bone_array , base_pose_bone_array , target_bone , pole_bone , IK_bone , iteration_count , bone_chain_max_length);
    
}

internal bool drag_to_rotate( Vector3 position , Quaternion * rotation_to_modifying , bool * hovering)
{
    local_persist float start_angle = 0;
    local_persist Quaternion start_rotation = {};
    local_persist int rotating_index = -1;
    
    Quaternion rotation = (*rotation_to_modifying);
    
    Rect rect[R_count] = {};
    Color color[R_count] = {};
    
    rect[R_X].rotation = QuaternionMultiply(rotation , QuaternionFromEuler(0,DEG2RAD * 90,0));
    rect[R_Y].rotation = QuaternionMultiply(rotation , QuaternionFromEuler( -DEG2RAD * 90,0,0));
    rect[R_Z].rotation = rotation;
    
    color[R_X] = RED;
    color[R_Y] = GREEN;
    color[R_Z] = BLUE;
    
    int closest_axis_index = -1;
    float closest_hit_time = FLT_MAX;
    Vector2 angle_xy = {};
    Vector3 mouse_ray_end = Vector3Add(mouse_ray_3D.position , mouse_ray_3D.direction);
    local_persist Vector3 rotate_axis = {};
    local_persist Quaternion previous_rotation = {};
    
    if(rotating_index != -1)
    {
        if(mouse_released(MOUSE_BUTTON_LEFT)) rotating_index = -1;
    }
    
    for(int axis_index = 0 ; axis_index < R_count ; axis_index++)
    {
        rect[axis_index].position = Vector3Add(rect[axis_index].position , position);
        rect[axis_index].size = (Vector2){GRID_SIZE * 2.4f , GRID_SIZE * 2.4f};
        
        Vector3 normal = Vector3RotateByQuaternion((Vector3){0,0,1} , rect[axis_index].rotation);
        
        if(rotating_index == axis_index) normal = rotate_axis;
        
        float intersect_time = get_line_intersect_with_plane_time( mouse_ray_3D.position , mouse_ray_end , normal , position);
        
        Vector3 point = Vector3Lerp(mouse_ray_3D.position , mouse_ray_end , intersect_time);
        point = Vector3Subtract(point , position);
        
        if(rotating_index == axis_index)
        {
            point = Vector3RotateByQuaternion( point , QuaternionInvert(previous_rotation));
            angle_xy.x = point.x;
            angle_xy.y = point.y;
        }
        
        if(rotating_index == -1)
        {
            point = Vector3RotateByQuaternion( point , QuaternionInvert(rect[axis_index].rotation));
            
            if(fabs(point.x * 2.0) < rect[axis_index].size.x)
            {
                if(fabs(point.y * 2.0) < rect[axis_index].size.y)
                {
                    if(closest_hit_time > intersect_time)
                    {
                        if(hovering) (*hovering) = true;
                        closest_hit_time = intersect_time;
                        closest_axis_index = axis_index;
                        angle_xy.x = point.x;
                        angle_xy.y = point.y;
                        rotate_axis = normal;
                        previous_rotation = rect[axis_index].rotation;
                    }
                }
            }
        }
    }
    
    if(closest_axis_index != -1)
    {
        if(mouse_pressed(MOUSE_BUTTON_LEFT))
        {
            start_angle = atan2(angle_xy.y , angle_xy.x);
            rotating_index = closest_axis_index;
            start_rotation = rotation;
        }
    }
    
    if(rotating_index != -1)
    {
        float angle = atan2(angle_xy.y , angle_xy.x);
        float delta_angle = angle - start_angle;
        
        Quaternion rotation_offset = QuaternionFromAxisAngle(rotate_axis , delta_angle);
        (*rotation_to_modifying) = QuaternionMultiply(rotation_offset , start_rotation);
    }
    
    for(int axis_index = 0 ; axis_index < R_count ; axis_index++)
    {
        Color rect_color = color[axis_index];
        rect_color = Fade(rect_color , 0.4f);
        
        if(rotating_index == axis_index)
        {
            rect_color = Fade(rect_color , 1.0f);
        }
        else if(rotating_index == -1)
        {
            if(axis_index == closest_axis_index)
            {
                rect_color = Fade(rect_color , 0.8f);
            }
        }
        
        draw_rect_line_E(rect[axis_index] , rect_color , 15);
    }
    
    return (rotating_index != -1);
}

internal bool drag_to_move(Vector3 * position_to_modify , bool * hovering)
{
    Vector3 position = (*position_to_modify);
    
    Box right_box = get_box();
    Box up_box = get_box();
    Box forward_box = get_box();
    
    Vector3 box_right_direction = right_direction;
    Vector3 box_up_direction = up_direction;
    Vector3 box_forward_direction = forward_direction;
    
    Vector3 camera_to_position = Vector3Subtract(position , world_camera.position);
    
    if(Vector3DotProduct(camera_to_position , box_right_direction) > 0) box_right_direction = Vector3Negate(box_right_direction);
    if(Vector3DotProduct(camera_to_position , box_up_direction) > 0) box_up_direction = Vector3Negate(box_up_direction);
    if(Vector3DotProduct(camera_to_position , box_forward_direction) > 0) box_forward_direction = Vector3Negate(box_forward_direction);
    
    right_box.position = Vector3Lerp((Vector3){} , box_right_direction , 0.5f);
    up_box.position = Vector3Lerp((Vector3){} , box_up_direction , 0.5f);
    forward_box.position = Vector3Lerp((Vector3){} , box_forward_direction , 0.5f);
    
    right_box.position = Vector3Add(position , right_box.position);
    up_box.position = Vector3Add(position , up_box.position);
    forward_box.position = Vector3Add(position , forward_box.position);
    
    right_box.size = Vector3Add(box_right_direction , (Vector3){UNIT_SIZE , UNIT_SIZE , UNIT_SIZE});
    up_box.size = Vector3Add(box_up_direction , (Vector3){UNIT_SIZE , UNIT_SIZE , UNIT_SIZE});
    forward_box.size = Vector3Add(box_forward_direction , (Vector3){UNIT_SIZE , UNIT_SIZE , UNIT_SIZE});
    
    Vector3 camera_direction = Vector3Subtract(world_camera.target , world_camera.position);
    camera_direction = Vector3Negate(camera_direction);
    
    Color right_color = Fade(RED , 0.4f);
    Color up_color = Fade(GREEN , 0.4f);
    Color forward_color = Fade(BLUE , 0.4f);
    
    local_persist Vector3 origin_position = {};
    local_persist Vector3 previous_drag_point = {};
    local_persist bool dragging_origin = false;
    local_persist Vector3 drag_direction = {};
    
    Vector3 mouse_ray_position = mouse_ray_3D.position;
    Vector3 mouse_ray_target = Vector3Add(mouse_ray_3D.position , mouse_ray_3D.direction);
    
    float intersect_time = get_line_intersect_with_plane_time( mouse_ray_position , mouse_ray_target , camera_direction , position);
    Vector3 current_drag_point = Vector3Lerp(mouse_ray_position , mouse_ray_target , intersect_time);
    
    if(mouse_pressed_no_check(MOUSE_BUTTON_LEFT))
    {
        previous_drag_point = current_drag_point;
        origin_position = position;
    }
    
    Vector3 drag_offset = Vector3Subtract(current_drag_point , previous_drag_point);
    previous_drag_point = current_drag_point;
    
    local_persist int selected_axis = -1;
    
    if(!dragging_origin)
    {
        if(box_collision_ray(mouse_ray_3D.position , mouse_ray_3D.direction , right_box , 0 ,0)) 
        {
            if(mouse_pressed(MOUSE_BUTTON_LEFT))
            {
                dragging_origin = true;
                drag_direction = right_direction;
                selected_axis = D_right;
            }
            
            if(hovering) (*hovering) =true;
            right_color = Fade(right_color , 0.8f);
        }
        
        if(box_collision_ray(mouse_ray_3D.position , mouse_ray_3D.direction , up_box , 0 , 0)) 
        {
            if(mouse_pressed(MOUSE_BUTTON_LEFT))
            {
                dragging_origin = true;
                drag_direction = up_direction;
                selected_axis = D_up;
            }
            
            if(hovering) (*hovering) =true;
            up_color = Fade(up_color , 0.8f);
        }
        
        if(box_collision_ray(mouse_ray_3D.position , mouse_ray_3D.direction , forward_box , 0 , 0)) 
        {
            if(mouse_pressed(MOUSE_BUTTON_LEFT))
            {
                dragging_origin = true;
                drag_direction = forward_direction;
                selected_axis = D_forward;
            }
            
            if(hovering) (*hovering) =true;
            forward_color = Fade(forward_color , 0.8f);
        }
    }
    
    if(dragging_origin)
    {
        if(mouse_released(MOUSE_BUTTON_LEFT))
        {
            dragging_origin = false;
        }
        
        Vector3 origin_offset = Vector3Project(drag_offset , drag_direction);
        origin_position = Vector3Add(origin_position , origin_offset);
        
        Vector3 position_in_cell = position_to_grid(origin_position , UNIT_SIZE);
        
        (*position_to_modify) = position_in_cell;
        
        switch(selected_axis)
        {
            case D_right: right_color = RED; break;
            case D_up: up_color = GREEN; break;
            case D_forward: forward_color = BLUE; break;
        }
    }
    
    draw_arrow_ray(position , box_right_direction , right_color);
    draw_arrow_ray(position , box_up_direction , up_color);
    draw_arrow_ray(position , box_forward_direction , forward_color);
    
    return dragging_origin;
}

internal void edit_box(Vector3 origin , Vector3 grid_normal , Vector3 intersect_point)
{
    local_persist bool editing_box = false;
    local_persist bool rotating = false;
    
    local_persist bool start_stretch_box = false;
    local_persist bool stretching_box = false;
    local_persist int box_face_index_to_stretch = 0;
    
    if(!editing_box)
    {
        if(last_clicked_box)
        {
            Box * box = last_clicked_box;
            
            bool hovering = false;
            rotating = false;
            
            if(current_edit_type == edit_move)
            {
                if(drag_to_move(&box->position , &hovering))
                {
                    editing_box = true;
                }
            }
            
            if(current_edit_type == edit_rotate)
            {
                if(drag_to_rotate( box->position , &box->rotation , &hovering))
                {
                    editing_box = true;
                    rotating = true;
                }
            }
        }
    }
    else if(editing_box)
    {
        Box * box = last_clicked_box;
        Vector3 box_centre = box->position;
        
        if(rotating)
        {
            if(!drag_to_rotate( box->position , &box->rotation , 0))
            {
                editing_box = false;
            }
        }
        else
        {
            if(!drag_to_move(&box->position , 0))
            {
                editing_box = false;
            }
        }
    }
    
    if(current_edit_type == edit_stretch)
    {
        if(last_clicked_box)
        {
            int box_face_index = 0;
            float box_hit_time = 0;
            
            Box box = (*last_clicked_box);
            
            if(!stretching_box)
            {
                if(box_collision_ray(mouse_ray_3D.position , mouse_ray_3D.direction , box , &box_face_index , &box_hit_time))
                {
                    Vector3 face_offset = {};
                    Vector3 face_direction = {};
                    switch(box_face_index)
                    {
                        case face_right: face_offset.x += box.size.x * 0.5f; face_direction.x += 1; break;
                        case face_left: face_offset.x -= box.size.x * 0.5f; face_direction.x -= 1; break;
                        case face_top: face_offset.y += box.size.y * 0.5f; face_direction.y += 1; break;
                        case face_bottom: face_offset.y -= box.size.y * 0.5f; face_direction.y -= 1; break;
                        case face_front: face_offset.z += box.size.z * 0.5f; face_direction.z += 1; break;
                        case face_back: face_offset.z -= box.size.z * 0.5f; face_direction.z -= 1; break;
                        default: CATCH;
                    }
                    
                    face_direction = Vector3Scale(face_direction , UNIT_SIZE * 4);
                    face_direction = Vector3RotateByQuaternion(face_direction , box.rotation);
                    face_offset = Vector3RotateByQuaternion(face_offset , box.rotation);
                    Vector3 arrow_start = Vector3Add(face_offset , box.position);
                    
                    arrow_start = Vector3Add(mouse_ray_3D.position , Vector3Scale(mouse_ray_3D.direction , box_hit_time));
                    
                    draw_arrow_ray(arrow_start , face_direction , WHITE);
                    
                    if(mouse_pressed(MOUSE_BUTTON_LEFT))
                    {
                        stretching_box = true;
                        start_stretch_box = false;
                        box_face_index_to_stretch = box_face_index;
                    }
                }
            }
            
            if(stretching_box)
            {
                Vector3 plane_normal = Vector3Subtract(world_camera.target , world_camera.position);
                Vector3 line_start = mouse_ray_3D.position;
                Vector3 line_end = Vector3Add(mouse_ray_3D.position , mouse_ray_3D.direction);
                float plane_time = get_line_intersect_with_plane_time(line_start , line_end , plane_normal , box.position);
                Vector3 mouse_on_plane = Vector3Lerp(line_start , line_end , plane_time);
                
                Vector3 pole = {};
                
                bool invert_face = false; 
                switch(box_face_index_to_stretch)
                {
                    case face_left:
                    case face_bottom:
                    case face_back:
                    {
                        invert_face = true;
                    }
                    break;
                }
                
                switch(box_face_index_to_stretch)
                {
                    case face_right:
                    case face_left:
                    {
                        pole = right_direction;
                    }
                    break;
                    
                    case face_front:
                    case face_back:
                    {
                        pole = forward_direction;
                    }
                    break;
                    
                    case face_top:
                    case face_bottom:
                    {
                        pole = up_direction;
                    }
                    break;
                }
                
                pole = Vector3RotateByQuaternion(pole , box.rotation);
                
                Vector3 mouse_relative_to_box = Vector3Subtract(mouse_on_plane , box.position);
                local_persist Vector3 start_pole_position = {};
                
                Vector3 mouse_on_pole = Vector3Project( mouse_relative_to_box , pole);
                if(!start_stretch_box)
                {
                    start_pole_position = mouse_on_pole;
                    start_stretch_box = true;
                }
                
                Vector3 absolute_drag_offset = Vector3Subtract(mouse_on_pole , start_pole_position);
                
                Vector3 drag_offset = Vector3RotateByQuaternion(absolute_drag_offset , QuaternionInvert(box.rotation));
                drag_offset.x -= 0.001;
                drag_offset.y -= 0.001;
                drag_offset.z -= 0.001;
                drag_offset = position_to_grid(drag_offset , GRID_SIZE);
                absolute_drag_offset = Vector3RotateByQuaternion(drag_offset , box.rotation);
                
                //drag_offset = Vector3Project(drag_offset , pole);
                
                draw_arrow_ray(box.position , absolute_drag_offset , WHITE);
                
                if(invert_face)
                {
                    box.size = Vector3Subtract(box.size , drag_offset);
                }
                else
                {
                    box.size = Vector3Add(box.size , drag_offset);
                }
                
                box.size.x = fabs(box.size.x);
                box.size.y = fabs(box.size.y);
                box.size.z = fabs(box.size.z);
                
                box.position = Vector3Add(box.position , Vector3Scale(absolute_drag_offset,0.5));
                
                draw_box_line(box , WHITE , 5);
                
                if(mouse_released(MOUSE_BUTTON_LEFT))
                {
                    stretching_box = false;
                    (*last_clicked_box) = box;
                }
            }
        }
    }
    
    local_persist bool drag_and_make_box = false;
    local_persist float drag_timer = 0;
    local_persist Vector3 box_start_point = {};
    
    Array * current_box_array = 0;
    BoxBuffer * current_box_buffer = 0;
    
    if(current_map_edit_type == MET_box)
    {
        current_box_array = &box_in_map_array;
        current_box_buffer = &box_in_map_buffer;
    }
    
    if(current_map_edit_type == MET_camera_trigger)
    {
        current_box_array = &camera_array;
        current_box_buffer = &camera_zone_buffer;
    }
    
    Vector3 grid_offset = Vector3Subtract((Vector3){1,1,1} , grid_normal);
    
    Vector3 mouse_on_grid = intersect_point;
    mouse_on_grid = Vector3Subtract(mouse_on_grid , origin);
    mouse_on_grid = Vector3Subtract(mouse_on_grid , Vector3Scale(grid_normal , 0.001f));
    mouse_on_grid.x += GRID_SIZE * 0.5f * grid_offset.x;
    mouse_on_grid.y += GRID_SIZE * 0.5f * grid_offset.y;
    mouse_on_grid.z += GRID_SIZE * 0.5f * grid_offset.z;
    
    mouse_on_grid = position_to_grid(mouse_on_grid , GRID_SIZE);
    
    mouse_on_grid.x -= GRID_SIZE * grid_offset.x;
    mouse_on_grid.y -= GRID_SIZE * grid_offset.y;
    mouse_on_grid.z -= GRID_SIZE * grid_offset.z;
    
    mouse_on_grid = Vector3Add(mouse_on_grid , origin);
    
    //mouse_on_grid.x -= GRID_SIZE * 0.5f;
    //mouse_on_grid.y -= GRID_SIZE * 0.5f;
    //mouse_on_grid.z -= GRID_SIZE * 0.5f;
    
    if(drag_and_make_box)
    {
        drag_timer += DELTA_TIME;
        
        //i can make box to quad if it was flat
        Box dragging_box = get_box();
        
        dragging_box.size = Vector3Subtract(mouse_on_grid , box_start_point);
        dragging_box.size.x = fabs(dragging_box.size.x);
        dragging_box.size.y = fabs(dragging_box.size.y);
        dragging_box.size.z = fabs(dragging_box.size.z);
        
        //printf("%f %f %f\n" , dragging_box.size.x , dragging_box.size.y , dragging_box.size.z);
        
        dragging_box.position = Vector3Lerp(mouse_on_grid , box_start_point , 0.5f);
        draw_box_line(dragging_box , Fade(WHITE , 0.4 ) , 10);
        
        if(mouse_pressed(MOUSE_BUTTON_RIGHT))
        {
            drag_and_make_box = false;
        }
        
        if(mouse_released(MOUSE_BUTTON_LEFT))
        {
            if(drag_timer < 0.1) last_clicked_box = 0;
            
            drag_and_make_box = false;
            int size_count = 0;
            if(dragging_box.size.x > 0) size_count++;
            if(dragging_box.size.y > 0) size_count++;
            if(dragging_box.size.z > 0) size_count++;
            
            if(size_count >= 2)
            {
                if(array_full(current_box_array))
                {
                    reallocate_buffer(current_box_buffer ,  AT_temp);
                    reallocate_array(current_box_array , AT_temp);
                }
                
                int new_box_index = add_to_array(current_box_array);
                Box * new_box = current_box_buffer->data + new_box_index;
                (*new_box) = (Box){};
                (*new_box) = dragging_box;
                
                if(current_map_edit_type == MET_camera_trigger)
                {
                    camera_buffer.data[new_box_index] = (CameraTrigger){};
                }
            }
        }
    }
    
    if(current_edit_type == edit_drag_new_box)
    {
        hovering_box = 0;
        
        bool box_collided = false;
        float closest_box_hit_time = FLT_MAX;
        Box * closest_box = 0;
        int closest_box_face_index = -1;
        
        array_foreach(box_index , current_box_array)
        {
            Box * box = current_box_buffer->data + box_index;
            
            float box_hit_time = 0;
            int box_face_index = 0;
            if(box_collision_ray(mouse_ray_3D.position , mouse_ray_3D.direction , (*box) , &box_face_index , &box_hit_time))
            {
                box_collided = true;
                if(closest_box_hit_time > box_hit_time)
                {
                    closest_box_hit_time = box_hit_time;
                    closest_box = box;
                    closest_box_face_index = box_face_index;
                }
            }
        }
        
        if(box_collided)
        {
            if(mouse_pressed(MOUSE_BUTTON_LEFT))
            {
                last_clicked_box = closest_box;
            }
            
            hovering_box = closest_box;
        }
        
        if(mouse_pressed(MOUSE_BUTTON_LEFT))
        {
            if(!box_collided)
            {
                box_start_point = mouse_on_grid;
                drag_and_make_box = true;
                drag_timer = 0;
            }
        }
    }
}

internal void edit_map(Vector3 origin)
{
    Vector3 camera_direction = Vector3Subtract(world_camera.target , world_camera.position);
    
    Vector3 all_direction[3] = {up_direction  , right_direction , forward_direction};
    float dot_product = 0;
    grid_normal_index = -1;
    
    for(int i = 0 ; i < 3 ; i++)
    {
        float current_dot_product = Vector3DotProduct(camera_direction , all_direction[i]);
        current_dot_product = fabs(current_dot_product);
        
        if(current_dot_product > dot_product)
        {
            dot_product = current_dot_product;
            grid_normal_index = i;
        }
    }
    
    if(editor_type == edit_world)
    {
        //Vector3 origin
        Vector3 grid_normal = all_direction[grid_normal_index];
        grid_normal = Vector3Normalize(grid_normal);
        
        Vector3 mouse_ray_target = Vector3Add(mouse_ray_3D.position , mouse_ray_3D.direction);
        Vector3 mouse_ray_position = mouse_ray_3D.position;
        float intersect_time = get_line_intersect_with_plane_time(mouse_ray_position , mouse_ray_target , grid_normal , origin);
        Vector3 intersect_point = Vector3Lerp(mouse_ray_position , mouse_ray_target , intersect_time);
        
        Rect rect_in_cell = get_rect();
        Vector3 offset_point = Vector3Subtract(intersect_point , origin);
        rect_in_cell.position = position_to_grid(Vector3Add(offset_point , Vector3Scale(grid_normal , 0.0001f)) , GRID_SIZE);
        
        rect_in_cell.position.x -= GRID_SIZE * 0.5;
        rect_in_cell.position.y -= GRID_SIZE * 0.5;
        rect_in_cell.position.z -= GRID_SIZE * 0.5;
        
        rect_in_cell.position = Vector3Subtract(rect_in_cell.position , Vector3Scale(grid_normal , GRID_SIZE * 0.5) );
        rect_in_cell.position = Vector3Add(rect_in_cell.position , origin);
        
        rect_in_cell.size = (Vector2){GRID_SIZE , GRID_SIZE};
        rect_in_cell.rotation = QuaternionFromVector3ToVector3( (Vector3){0,0,1} , grid_normal);
        
        if(current_map_edit_type == MET_quad)
        {
            draw_rect_D(rect_in_cell , 0 , Fade(WHITE , 0.3));
        }
        
        if(within_viewport)
        {
            if(current_edit_type == edit_camera_offset)
            {
                if(edit_camera_offset)
                {
                    if(last_clicked_box)
                    {
                        int trigger_index = last_clicked_box - camera_zone_buffer.data;
                        CameraTrigger * trigger = camera_buffer.data + trigger_index;
                        
                        Vector3 camera_position = Vector3Add(trigger->camera_target_offset , last_clicked_box->position);
                        drag_to_move(&camera_position , 0);
                        //printf("%f %f %f\n" , camera_position.x , camera_position.y , camera_position.z);
                        trigger->camera_target_offset = Vector3Subtract(camera_position , last_clicked_box->position);
                    }
                }
            }
            
            bool enable_box_editing = false;
            
            if(current_map_edit_type == MET_box) enable_box_editing = true;
            if(current_map_edit_type == MET_camera_trigger) enable_box_editing = true;
            
            if(enable_box_editing)
            {
                edit_box(origin , grid_normal , intersect_point);
            }
            
            if(current_map_edit_type == MET_camera_trigger)
            {
                if(current_edit_type == edit_camera_offset)
                {
                    if(edit_camera_offset)
                    {
                        if(last_clicked_box)
                        {
                            int trigger_index = last_clicked_box - camera_zone_buffer.data;
                            CameraTrigger * trigger = camera_buffer.data + trigger_index;
                            
                            Vector3 camera_position = Vector3Add(trigger->camera_target_offset , last_clicked_box->position);
                            drag_to_move(&camera_position , 0);
                            //printf("%f %f %f\n" , camera_position.x , camera_position.y , camera_position.z);
                            trigger->camera_target_offset = Vector3Subtract(camera_position , last_clicked_box->position);
                        }
                    }
                }
            }
            
        }
        else if(current_map_edit_type == MET_quad)
        {
            bool collided = false;
            
            array_foreach(quad_index , &quad_in_map_array)
            {
                RayCollision collision = get_collision_quad_3D(quad_in_map_buffer.data[quad_index]);
                if(collision.hit)
                {
                    collided = true;
                    
                    if(mouse_pressed(MOUSE_BUTTON_LEFT))
                    {
                        if(!delete_from_array(&quad_in_map_array , quad_index)) CATCH;
                    }
                    
                    break;
                }
            }
            
            if(!collided)
            {
                if(mouse_pressed(MOUSE_BUTTON_LEFT))
                {
                    if(array_full(&quad_in_map_array))
                    {
                        reallocate_array(&quad_in_map_array , AT_temp);
                        reallocate_buffer(&quad_in_map_buffer , AT_temp );
                    }
                    Quad * new_quad = quad_in_map_buffer.data + add_to_array(&quad_in_map_array);
                    
                    (*new_quad) = rect_to_quad(rect_in_cell);
                }
            }
        }
    }
}

internal void draw_origin_grid(Vector3 origin)
{
    
#if 1
	int grid_hint_extend = 50;
    
    for (int axis_index = 0 ; axis_index < 3; axis_index++)
	{
		Color grid_line_color = {};
		float grid_line_size = 8;
        
        if (axis_index == 0) grid_line_color = Fade(RED,0.2f);
		if (axis_index == 1) grid_line_color = Fade(GREEN, 0.2f);
		if (axis_index == 2) grid_line_color = Fade(BLUE, 0.2f);
        
        if(grid_normal_index != axis_index) continue;
        
		for (int horizontal_index = 0; horizontal_index < grid_hint_extend * 2 +1; horizontal_index++)
		{
            float line_offset = grid_hint_extend * UNIT_SIZE;
			int line_index = horizontal_index - grid_hint_extend;
            
			Vector3 start_position = origin;
            
			if (axis_index == 2)
			{
				start_position.y -= UNIT_SIZE * grid_hint_extend;
				start_position.y += UNIT_SIZE * horizontal_index;
			}
			else if (axis_index == 1)
			{
				start_position.y -= UNIT_SIZE * grid_hint_extend;
				start_position.y += UNIT_SIZE * horizontal_index;
			}
			else
			{
				start_position.z -= UNIT_SIZE * grid_hint_extend;
				start_position.z += UNIT_SIZE * horizontal_index;
			}
            
			Vector3 left_point = start_position;
			Vector3 right_point = start_position;
            
			if (axis_index == 2)
			{
				left_point.x -= line_offset;
				right_point.x += line_offset;
			}
			else if (axis_index == 1)
			{
				left_point.z -= line_offset;
				right_point.z += line_offset;
			}
			else
			{
				left_point.x -= line_offset;
				right_point.x += line_offset;
			}
            
			Color line_color_H = grid_line_color;
			if (horizontal_index == grid_hint_extend) line_color_H = Fade(BLUE,0.5);
            
            if( horizontal_index % SUBDIVISION != 0) line_color_H = Fade(line_color_H , 0.05);
            
			int grid_distance = horizontal_index - grid_hint_extend;
            
			if (grid_distance < 0) grid_distance *= -1;
            
			line_color_H = Fade(line_color_H, ((float)line_color_H.a /255.0)*(1.0-(float)grid_distance/ (float)grid_hint_extend));
            
			draw_round_line(start_position, left_point,grid_line_size ,line_color_H , Fade(line_color_H ,0));
			draw_round_line(start_position, right_point ,grid_line_size ,line_color_H ,Fade(line_color_H ,0));
            
		}
        
		for (int vertical_index = 0; vertical_index < grid_hint_extend * 2 + 1; vertical_index++)
		{
            
			float line_offset = grid_hint_extend * UNIT_SIZE;
            
			int line_index = vertical_index - grid_hint_extend;
            
			Vector3 start_position = origin;
            
			if (axis_index == 2)
			{
				start_position.x -= UNIT_SIZE * grid_hint_extend;
				start_position.x += UNIT_SIZE * vertical_index;
			}
			else if (axis_index == 1)
			{
				start_position.z -= UNIT_SIZE * grid_hint_extend;
				start_position.z += UNIT_SIZE * vertical_index;
			}
			else
			{
				start_position.x -= UNIT_SIZE * grid_hint_extend;
				start_position.x += UNIT_SIZE * vertical_index;
			}
            
			Vector3 top_point = start_position;
			Vector3 bottom_point = start_position;
            
			if (axis_index == 2)
			{
				top_point.y += line_offset;
				bottom_point.y -= line_offset;
			}
			else if (axis_index == 1)
			{
				top_point.y += line_offset;
				bottom_point.y -= line_offset;
			}
			else
			{
				top_point.z += line_offset;
				bottom_point.z -= line_offset;
			}
            
			Color line_color_V = grid_line_color;
			if (vertical_index == grid_hint_extend) line_color_V = Fade(GREEN,0.5f);
            
			int grid_distance = vertical_index- grid_hint_extend;
            
			if (grid_distance < 0) grid_distance *= -1;
            
            if( vertical_index % SUBDIVISION != 0) line_color_V = Fade(line_color_V , 0.05);
            
			line_color_V = Fade(line_color_V, ((float)line_color_V.a / 255.0) * (1 - (float)grid_distance / (float)grid_hint_extend));
            
			draw_round_line(start_position, top_point,grid_line_size ,line_color_V ,Fade(line_color_V ,0));
			draw_round_line(start_position, bottom_point,grid_line_size ,line_color_V,Fade(line_color_V ,0));
		}
        
	}
#endif
    
}

internal void get_bone_from_existing_key_frame(Bone * bone_array , Clip * clip , int target_frame , int target_start_frame , int target_frame_count)
{
    
	for (int bone_index = 0; bone_index < selected_model->bone_buffer.count; bone_index++)
	{
        
        Bone * current_bone = bone_array + bone_index;
        
		bool no_interpolation = true;
        
		current_bone->free_bone = false;
        
        KeyFrame * start_key_frame = 0;
        
        KeyFrame * end_key_frame = 0;
        
        //TODO : this one is finding the last key frame for looping
        hash_table_iterate(key_frame_index , bone_index , &clip->key_frame_hash_table_by_bone)
        {
            
			KeyFrame * key_frame = all_key_frame_buffer.data + key_frame_index;
            if(key_frame->bone_index != bone_index) continue;
            
            end_key_frame = all_key_frame_buffer.data + key_frame_index;
            
        }
        
        KeyFrame * first_key_frame = 0;
        bool not_the_last_key_frame = false;
        
        //TODO : it is iterating frame to find closest frame
        hash_table_iterate(key_frame_index , bone_index , &clip->key_frame_hash_table_by_bone)
        {
            KeyFrame * key_frame = all_key_frame_buffer.data + key_frame_index;
            
            if(key_frame->bone_index != bone_index) continue;
            if(!first_key_frame) first_key_frame = key_frame;
            
            start_key_frame = end_key_frame;
            end_key_frame = key_frame;
            
            if(key_frame->frame_index > target_frame)
            {
                not_the_last_key_frame = true;
                break;
            }
        }
        
        if(!not_the_last_key_frame)
        {
            start_key_frame = end_key_frame;
            end_key_frame = first_key_frame;
        }
        
        KeyFrame * target_key_frame = 0;
        
        if(start_key_frame)
        {
            
            if(start_key_frame->frame_index == target_frame)
            {
                target_key_frame = start_key_frame;
                current_bone->free_bone = true;
            }
            
        }
        else
        {
            if(!end_key_frame)
            {
                current_bone->free_bone = true;
            }
        }
        
        if(start_key_frame)
        {
            if(end_key_frame)
            {
                no_interpolation = false;
            }
            
            if(start_key_frame == end_key_frame)
            {
                no_interpolation = true;
            }
            
        }
        
		if (no_interpolation)
		{
			if (target_key_frame)
			{
				current_bone->state = target_key_frame->bone_state;
			}
		}
		else
		{
            //printf("%d %d\n" ,  StartKeyFrame->frame_index , EndKeyFrame->frame_index);
            
			float frame_count = get_key_frame_count(start_key_frame , end_key_frame , target_start_frame , target_frame_count);
			float frame_offset = target_frame - start_key_frame->frame_index;
            
            if(target_frame < start_key_frame->frame_index)
            {
                int end_frame_index = target_start_frame + target_frame_count - 1;
                frame_offset = end_frame_index - start_key_frame->frame_index + target_frame - target_start_frame + 1;
            }
            
            float frame_time = frame_offset / frame_count;
            //printf("%f %f %f\n" , FrameOffset, FrameCount , FrameTime);
            
            Vector3 line_direction = Vector3Subtract(end_key_frame->bone_state.local_position , start_key_frame->bone_state.local_position);
            line_direction =Vector3Scale( Vector3Normalize(line_direction) , 0.1f);
            
            Vector3 bezier_line_start_position = start_key_frame->bone_state.local_position;
            Vector3 bezier_start_point = Vector3Add(line_direction , start_key_frame->next_bezier_offset);
            bezier_start_point = Vector3Add(bezier_line_start_position , bezier_start_point);
            
            Vector3 bezier_line_end_position = end_key_frame->bone_state.local_position;
            Vector3 bezier_end_point = Vector3Add( Vector3Negate(line_direction) , end_key_frame->previous_bezier_offset);
            bezier_end_point = Vector3Add(bezier_line_end_position , bezier_end_point);
            
			current_bone->state.local_position = vector3_cubic_bezier(bezier_line_start_position , bezier_start_point , bezier_end_point ,bezier_line_end_position ,frame_time );
            
            float ease_time = ease_in_out_cubic(frame_time);
			current_bone->state.local_rotation = QuaternionLerp(start_key_frame->bone_state.local_rotation , end_key_frame->bone_state.local_rotation ,ease_time );
            
		}
	}
    
}

internal void add_bone_state(Bone * bone_array , Bone * add_bone_array , int bone_index)
{
    BoneState bone_state = bone_array[bone_index].state;
    BoneState bone_state_to_add = add_bone_array[bone_index].state;
    
    bone_state.local_position = Vector3Add(bone_state.local_position , bone_state_to_add.local_position);
    bone_state.end_point_offset = Vector3Add(bone_state.end_point_offset , bone_state_to_add.end_point_offset);
    bone_state.local_rotation = QuaternionMultiply(bone_state.local_rotation ,bone_state_to_add.local_rotation );
    
    bone_array[bone_index].state = bone_state;
}

internal void add_multiple_bone_state(Bone * bone_array , Bone * add_bone_array , int bone_array_count)
{
    for(int bone_index = 0 ; bone_index < bone_array_count ; bone_index++)
    {
        add_bone_state(bone_array , add_bone_array , bone_index);
    }
}

internal Bone * get_multiple_bone(int bone_array_count)
{
    Bone * temp_bone = allocate_frame(Bone ,bone_array_count);
    
    for(int bone_index = 0 ; bone_index < bone_array_count ; bone_index++)
    {
        temp_bone[bone_index] = (Bone){};
        temp_bone[bone_index].rotation = QuaternionIdentity();
        temp_bone[bone_index].state.local_rotation = QuaternionIdentity();
    }
    
    return temp_bone;
}

internal Bone * get_bone_pose_offset_from_clip( int clip_index , int target_frame , int target_frame_start, int target_frame_length)
{
    
    Bone * bone_pose_offset = get_multiple_bone(selected_model->bone_buffer.count);
    
    for(int bone_index = 0 ; bone_index < selected_model->bone_buffer.count ; bone_index++)
    {
        get_bone_from_existing_key_frame(bone_pose_offset ,  clip_array[clip_index], target_frame , target_frame_start , target_frame_length);
    }
    
    return bone_pose_offset;
}

internal float get_corner_weight(float vertical , float horizontal)
{
	if(vertical > 0.5) vertical = 0.5f;
	if(horizontal > 0.5) horizontal = 0.5f;
    
	float result = vertical + horizontal;
    
	result -= 0.5f;
	result *= 2;
    
	if(result < 0) result = 0;
	//printf("v : %f , h : %f , %f \n" , Vertical , Horizontal , Result );
    
	return result;
}

internal void draw_collision_visual(CollisionVisual visual)
{
    draw_simplex_triangle( visual.a , visual.b , visual.c);
    draw_arrow_line_B( visual.closest_point , visual.ray_end  , Fade(PINK , 0.5f));
    if(visual.collided) draw_arrow_ray( visual.collision_point , visual.collision_normal , RED );
    
    ShapeUnion shape_a = visual.shape_a;
    ShapeUnion shape_b = visual.shape_b;
    
    if(shape_a.type == ST_box) draw_box_line(shape_a.box , WHITE , 5);
    if(shape_a.type == ST_quad) draw_quad_line(shape_a.quad , WHITE , 5);
    
    if(shape_b.type == ST_box) draw_box_line(shape_b.box , WHITE , 5);
    if(shape_b.type == ST_quad) draw_quad_line(shape_b.quad , WHITE , 5);
}

internal void viewport_update()
{
    //Vector3 ray_position = Vector3Add(mouse_ray_3D.position , mouse_ray_3D.direction);
    //draw_billboard_circle(ray_position , 0.1 , YELLOW);
    
    //TODO: wait these should be done once
    if(editor_type == edit_animation)
    {
        for(int stack_index = 0 ; stack_index < clip_bone_stack_count ; stack_index++)
        {
            ClipBone * current_clip_bone = clip_bone_stack + stack_index;
            Clip * current_clip = clip_array[current_clip_bone->clip_index];
            
            Bone * final_bone = current_clip_bone->final_bone_pose;
            
            update_bone_structure(base_pose_bone);
            
            if(editor_type == edit_animation)
            {
                for(int bone_index = 0 ; bone_index < selected_model->bone_buffer.count ; bone_index++)
                {
                    Bone * bone = final_bone + bone_index;
                    if(!bone->IK_enable) continue;
                    if(bone->IK_target_bone_index == -1) continue;
                    if(bone->IK_pole_bone_index == -1) continue;
                    
                    bone_IK_update(final_bone , base_pose_bone , bone->IK_target_bone_index , bone->IK_pole_bone_index , bone->bone_index , editor->IK_iteration_count , bone->IK_chain_length);
                }
            }
        }
    }
    
    Bone * bone_pose_to_draw_stack[64] = {};
    int bone_pose_to_draw_stack_count = 0;
    
    if(editor_type == edit_animation)
    {
        for(int stack_index = 0 ; stack_index < clip_bone_stack_count ; stack_index++)
        {
            bone_pose_to_draw_stack[bone_pose_to_draw_stack_count++] = clip_bone_stack[stack_index].final_bone_pose;
        }
    }
    else if(editor_type == edit_base_pose)
    {
        Bone * bone_array = allocate_frame(Bone , selected_model->bone_buffer.count);
        for(int bone_index = 0 ; bone_index < selected_model->bone_buffer.count ; bone_index++)
        {
            bone_array[bone_index] = selected_model->bone_buffer.data[bone_index];
        }
        
        update_bone_structure(bone_array);
        bone_pose_to_draw_stack[bone_pose_to_draw_stack_count++] = bone_array;
    }
    
    glClear(GL_DEPTH_BUFFER_BIT);
    
    Vector3 grid_origin = {};
    if(editor_type == edit_world)
    {
        if(selected_reference_frame_index != -1)
        {
            grid_origin = reference_frame_buffer.data[selected_reference_frame_index];
        }
    }
    
    D_game_draw();
    
    glClearDepth(1);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    if(editor_type == edit_world || editor_type == demo)
    {
        D_game_draw();
        render_state.draw_flag = 1;
        
        array_foreach(box_index , &box_in_map_array)
        {
            Box box = box_in_map_buffer.data[box_index];
            draw_box(box , PURPLE);
        }
        
        array_foreach(quad_index , &quad_in_map_array)
        {
            Quad quad = quad_in_map_buffer.data[quad_index];
            draw_quad_D(quad , PURPLE);
        }
        
        D_game_draw();
        render_state.draw_flag = 0;
        
        //draw_box_line(nav_mesh_start_box , YELLOW , 5);
        //draw_box_line(nav_mesh_whole_box , YELLOW , 10);
        
        int visual_buffer_start = 0;
        int visual_buffer_end = collision_visual_buffer.count;
        
        if(!store_multiple_frame_collision)
        {
            if(frame_collision_buffer.count > 0)
            {
                FrameCollision frame_collision = frame_collision_buffer.data[current_frame_collision_index];
                visual_buffer_start = frame_collision.slice_start;
                visual_buffer_end = frame_collision.slice_end;
                
                if(visual_buffer_end - visual_buffer_start)
                {
                    draw_arrow_ray(frame_collision.start , frame_collision.velocity , YELLOW);
                }
            }
        }
        
        if(display_all_captured_collision)
        {
            for(int buffer_index = visual_buffer_start ; buffer_index < visual_buffer_end ; buffer_index++)
            {
                draw_collision_visual(collision_visual_buffer.data[buffer_index]);
            }
        }
        else
        {
            draw_collision_visual(collision_visual_buffer.data[current_collision_visual_index]);
        }
        
        BoundingBoxNode * node_stack[128] = {};
        int node_stack_count = 0;
        
        node_stack[node_stack_count++] = bounding_box_root;
        
#if 0
        //TODO: i can't see the whole tree
        for(;;)
        {
            if(node_stack_count <= 0) break;
            
            node_stack_count--;
            BoundingBoxNode * node = node_stack[node_stack_count];
            if(node->left) node_stack[node_stack_count++] = node->left;
            if(node->right) node_stack[node_stack_count++] = node->right;
            
            Box node_box = get_box();
            node_box.position = Vector3Lerp(node->right_top_forward , node->left_bottom_backward , 0.5f);
            node_box.size = Vector3Subtract(node->right_top_forward , node->left_bottom_backward);
            if(node_box.size.x < 0) node_box.size.x *= -1;
            if(node_box.size.y < 0) node_box.size.y *= -1;
            if(node_box.size.z < 0) node_box.size.z *= -1;
            
            draw_box_line(node_box , Fade(WHITE , 0.2f) , 5);
        }
#endif
        
        if(current_map_edit_type == MET_camera_trigger)
        {
            array_foreach(zone_index , &camera_array)
            {
                Box zone = camera_zone_buffer.data[zone_index];
                CameraTrigger trigger = camera_buffer.data[zone_index];
                
                draw_box_line(zone , Fade(WHITE , 0.2f) , 5);
                draw_arrow_ray(zone.position , trigger.camera_target_offset , WHITE);
            }
        }
        
        if(current_map_edit_type == MET_box || current_map_edit_type == MET_camera_trigger)
        {
            if(last_clicked_box)
            {
                draw_box_line((*last_clicked_box) , WHITE , 5);
            }
            
            if(hovering_box)
            {
                if(hovering_box != last_clicked_box)
                {
                    draw_box_line((*hovering_box) , Fade(WHITE , 0.5f) , 3);
                }
            }
        }
        
        array_foreach(quad_index , &quad_in_map_array)
        {
            Quad quad = quad_in_map_buffer.data[quad_index];
            
            RayCollision collision = get_collision_quad_3D( quad );
            if(collision.hit)
            {
                draw_quad_line( quad , BLACK , 2);
            }
        }
        
        array_foreach(player_index , &player_array)
        {
            Player player = player_buffer.data[player_index];
            Box player_box = player.box;
            
            if(player.it_is_me)
            {
                BoundingBoxNode player_node = box_to_bound(player_box);
                
                array_foreach(array_index , &camera_array)
                {
                    Box zone = camera_zone_buffer.data[array_index];
                    CameraTrigger * trigger = camera_buffer.data + array_index;
                    
                    if(trigger->player_within) continue;
                    
                    BoundingBoxNode zone_node = box_to_bound(zone);
                    
                    if(bounding_box_collided(zone_node , player_node))
                    {
                        if(check_shape((Vector3){} , box_to_point(player_box) , box_vertex_count , box_to_point(zone) , box_vertex_count))
                        {
                            trigger->player_within = true;
                            add_to_list_head(array_index , &camera_within_list);
                        }
                    }
                }
                
                list_foreach_EX(camera_index , N_node_head , node_index , &camera_within_list)
                {
                    Box zone = camera_zone_buffer.data[camera_index];
                    CameraTrigger * trigger = camera_buffer.data + camera_index;
                    
                    if(editor_type == edit_world) 
                    {
                        if(current_map_edit_type == MET_camera_trigger)
                        {
                            draw_box_line(zone , YELLOW , 10);
                        }
                    }
                    
                    BoundingBoxNode zone_node = box_to_bound(zone);
                    
                    if(!check_shape((Vector3){} , box_to_point(player_box) , box_vertex_count , box_to_point(zone) , box_vertex_count))
                    {
                        trigger->player_within = false;
                        delete_from_list(node_index , &camera_within_list);
                    }
                }
                
                draw_box(player_box , GOLD);
            }
            else
            {
                draw_box(player_box , GRAY);
            }
            
            draw_arrow_ray(player.position , player.target_direction , WHITE);
            draw_box( player.hammer_box , GRAY);
            draw_box_line( player.hammer_box , BLACK , 5);
            
            //TODO: i should make a function that search by count
            list_foreach( camera_index , &camera_within_list)
            {
                last_camera_offset = (camera_buffer.data[camera_index].camera_target_offset);
                break;
            }
            
            game_camera_position = Vector3Lerp(game_camera_position , player.position , 0.3);
            game_camera_offset = Vector3Lerp(game_camera_offset , last_camera_offset , 0.1);
            
            draw_box_line(player_box , BLACK , 5);
        }
    }
    
    if(editor_type == edit_world) 
    {
        edit_map(grid_origin);
        draw_origin_grid(grid_origin);
    }
    
    if(bone_pose_to_draw_stack_count)
    {
        for(int model_index = 0 ; model_index < model_count ; model_index++)
        {
            D_Model * model = all_models + model_index;
            
            update_bone_structure(model->initial_bone_buffer.data);
            draw_model(model , bone_pose_to_draw_stack[0] , model->initial_bone_buffer.data);
        }
    }
    
    D_game_draw();
    
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    
    for(int stack_index = 0 ; stack_index < bone_pose_to_draw_stack_count ; stack_index++)
    {
        list_foreach(root_bone_index , &selected_model->root_bone_list)
        {
            iterate_and_draw_bone_arrow( bone_pose_to_draw_stack[stack_index] , bone_pose_to_draw_stack[stack_index] + root_bone_index , 0);
        }
    }
    
    if(within_viewport)
    {
        bool edit_bone_state = false;
        
        if(editor_type == edit_base_pose) edit_bone_state = true;
        if(editor_type == edit_animation) edit_bone_state = true;
        
        if(editor->assigning_parent_bone) edit_bone_state = false;
        if(editor->assigning_IK_target_bone) edit_bone_state = false;
        if(editor->assigning_IK_pole_bone) edit_bone_state = false;
        
        if(edit_bone_state)
        {
            bone_selection_and_edit_bone_state( editor->current_frame_at_timeline);
        }
        
        if(editor_type == edit_world)
        {
            if(current_map_edit_type == MET_none)
            {
                if(selected_reference_frame_index != -1)
                {
                    drag_to_move(&reference_frame_buffer.data[selected_reference_frame_index] , 0);
                }
            }
        }
    }
    
	render_state.fake_depth = 0;
	D_game_draw();
}

internal void game_update()
{
    input_state = &client_input_state;
    
    bool window_size_changed = false;
	local_persist Vector2 previous_windows_size = { -1 , -1};
    
	if (previous_windows_size.x != app_data->window_size.x)
	{
		window_size_changed = true;
	}
    
	if (previous_windows_size.y != app_data->window_size.y)
	{
		window_size_changed = true;
	}
    
	previous_windows_size = app_data->window_size;
    
	if (window_size_changed)
	{
		
        //why windows size changed to 0 ???????
		//TODO: wrap rlloadtexture
		if (app_data->window_size.x != 0)
		{
            
			if (app_data->window_size.y != 0)
			{
				
                if (render_state.game_world_depth_texture != 0)
				{
					rlUnloadTexture(render_state.game_world_depth_texture);
				}
                
                //TODO: what was i thinking
                //the texture didn't get clear at all
                if(render_state.game_world_depth_texture != 0)
                {
                    rlUnloadTexture(render_state.game_world_depth_texture);
                    render_state.game_world_depth_texture = load_depth_texture_multi_sample( app_data->window_size.x , app_data->window_size.y );
                }
                else
                {
                    render_state.game_world_depth_texture = load_depth_texture_multi_sample( app_data->window_size.x , app_data->window_size.y );
                    add_to_list_tail(render_state.game_world_depth_texture , &render_state.texture_list_to_delete);
                }
                
				update_texture_multi_sample(&render_state.game_world_texture);
				update_texture_multi_sample(&render_state.interface_texture);
                //update_texture(&render_state.lighting_texture);
				//update_texture(&render_state.bloom_texture);
				//update_texture(&render_state.first_blur_texture);
				//update_texture(&render_state.blur_texture);
			}
		}
	}
    
	GL_CATCH;
    
	editor->flat_color = true;
    
	double mouse_x = 0;
	double mouse_y = 0;
    
	glfwGetCursorPos(app_data->current_window, &mouse_x, &mouse_y);
    
	mouse_position = (Vector2){ mouse_x, app_data->window_size.y - mouse_y };
    
	app_data->mouse_position = mouse_position;
	
    editor->target_camera_zoom -= app_data->mouse_scroll_delta * 0.5f;
    
	if (editor->target_camera_zoom < 0.01f) editor->target_camera_zoom = 0.01f;
	if (editor->target_camera_zoom > 50.0f) editor->target_camera_zoom = 20.0f;
    
	app_data->top = tan((world_camera.fovy * 0.5) * DEG2RAD);
	app_data->right = app_data->top * (app_data->window_size.x / app_data->window_size.y);
    full_screen_pixel_matrix = MatrixOrtho(0 , app_data->window_size.x , 0 , app_data->window_size.y , screen_near_clip , screen_far_clip);
    
	update_float_input();
    
	editor->previous_mouse_position = editor->current_mouse_position;
	editor->current_mouse_position = app_data->mouse_position;
    
	Vector2 mouse_position_delta = editor->current_mouse_position;
	mouse_position_delta.x -= editor->previous_mouse_position.x;
	mouse_position_delta.y -= editor->previous_mouse_position.y;
    
    if(key_pressed(KEY_UP)) editor->IK_iteration_count++;
    if(key_pressed(KEY_DOWN)) editor->IK_iteration_count--;
    
    if(editor->IK_iteration_count < 0) editor->IK_iteration_count = 0;
    
    base_pose_bone = allocate_frame( Bone , selected_model->bone_buffer.count);
    
    for(int bone_index = 0 ; bone_index < selected_model->bone_buffer.count ; bone_index++)
    {
        base_pose_bone[bone_index] = selected_model->bone_buffer.data[bone_index];
    }
    
    if(add_clip_bone_at_next_frame)
    {
        add_clip_bone_at_next_frame = false;
        clip_bone_stack_count++;
    }
    
    for(int stack_index = 0 ; stack_index < clip_bone_stack_count ; stack_index++)
    {
        ClipBone * current_clip_bone = clip_bone_stack + stack_index;
        
        int allocate_bone_count = selected_model->bone_buffer.count;
        //TODO:make it not temp
        current_clip_bone->bone_pose_offset = allocate_frame(Bone , allocate_bone_count);
        current_clip_bone->final_bone_pose = allocate_frame(Bone , allocate_bone_count);
        current_clip_bone->selected_bone = allocate_frame(bool , allocate_bone_count);
        current_clip_bone->hovered_bone = allocate_frame(bool , allocate_bone_count);
        
        for(int bone_index = 0 ; bone_index < selected_model->bone_buffer.count ; bone_index++)
        {
            
            current_clip_bone->bone_pose_offset[bone_index] = (Bone){};
            current_clip_bone->bone_pose_offset[bone_index].rotation = QuaternionIdentity();
            current_clip_bone->bone_pose_offset[bone_index].state.local_rotation = QuaternionIdentity();
            
            current_clip_bone->final_bone_pose[bone_index] = base_pose_bone[bone_index];
        }
    }
    
    for(int stack_index = 0 ; stack_index < clip_bone_stack_count ; stack_index++)
    {
        ClipBone * current_clip_bone = clip_bone_stack + stack_index;
        Clip * current_clip = clip_array[current_clip_bone->clip_index];
        
        if(editor_type == edit_animation)
        {
            Bone * temp_bone = allocate_frame( Bone , selected_model->bone_buffer.count);
            
            for(int bone_index = 0 ; bone_index < selected_model->bone_buffer.count ; bone_index++)
            {
                temp_bone[bone_index] = (Bone){};
                temp_bone[bone_index].rotation = QuaternionIdentity();
                temp_bone[bone_index].state.local_rotation = QuaternionIdentity();
            }
            
            get_bone_from_existing_key_frame(temp_bone , clip_array[current_clip_bone->clip_index] , editor->current_frame_at_timeline , editor->start_frame_index , editor->timeline_frame_length);
            add_multiple_bone_state(current_clip_bone->bone_pose_offset , temp_bone , selected_model->bone_buffer.count);
            
        }
        
        Bone * final_bone = current_clip_bone->final_bone_pose;
        add_multiple_bone_state(final_bone, current_clip_bone->bone_pose_offset , selected_model->bone_buffer.count);
        
        update_bone_structure(final_bone);
    }
    
    pruning_3D_line = false;
	change_matrix(full_screen_pixel_matrix);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
    glViewport(0 , 0 ,app_data->window_size.x, app_data->window_size.y);
	
    editor_GUI();
    
#if 1
	GL_CATCH;
    
    
    glBindFramebuffer(GL_FRAMEBUFFER, render_state.screen_frame_buffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D_MULTISAMPLE, render_state.game_world_texture, 0);
    
    GL_CATCH;
    
    if(!rlFramebufferComplete(render_state.screen_frame_buffer)) CATCH;
    
    glBindFramebuffer(GL_FRAMEBUFFER, render_state.screen_frame_buffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, render_state.game_world_depth_texture, 0);
    rlEnableFramebuffer(render_state.screen_frame_buffer);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
	if(!rlFramebufferComplete(render_state.screen_frame_buffer)) CATCH;
    rlEnableFramebuffer(render_state.screen_frame_buffer);
#endif
    
	GL_CATCH;
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
    
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glDisable(GL_DEPTH_TEST);
    
    float vertical_split = 0.6;
    float horizontal_split = 0.5;
    
    if(editor_type == demo)
    {
        vertical_split = 1.0;
    }
    
    R_Rectangle primary_viewport = {};
    R_Rectangle secondary_viewport = {};
    R_Rectangle tertiary_viewport = {};
    
    primary_viewport.x = 0;
    primary_viewport.y = 0;
    primary_viewport.width = app_data->window_size.x * vertical_split;
    primary_viewport.height = app_data->window_size.y ;
    
    secondary_viewport.x = app_data->window_size.x * vertical_split;
    secondary_viewport.y = app_data->window_size.y * (1.0 - horizontal_split);
    secondary_viewport.width = app_data->window_size.x * (1.0 - vertical_split);
    secondary_viewport.height = app_data->window_size.y * horizontal_split;
    
    tertiary_viewport.x = app_data->window_size.x * vertical_split;
    tertiary_viewport.y = 0;
    tertiary_viewport.width = app_data->window_size.x * (1.0 - vertical_split);
    tertiary_viewport.height = app_data->window_size.y * (1.0 - horizontal_split);
    
    SplitViewport all_viewport[16] = {};
    int viewport_count = 0;
    
    editor->current_camera_zoom = Lerp(editor->current_camera_zoom , editor->target_camera_zoom , 0.2f);
    
    SplitViewport * primary_split = all_viewport + viewport_count++;
    primary_split->viewport = primary_viewport;
    primary_split->camera_up = (Vector3){0,1,0};
    primary_split->ortho = false;
    primary_split->camera_euler = editor->editor_camera_euler;
    primary_split->camera_offset = editor->editor_camera_offset;
    primary_split->camera_zoom = editor->current_camera_zoom;//i don't need this right?? 
    
    if(editor_type != demo)
    {
        Vector3 primary_euler = editor->editor_camera_euler;
        Quaternion primary_rotation = QuaternionFromEuler(primary_euler.x * DEG2RAD, primary_euler.y * DEG2RAD, primary_euler.z * DEG2RAD);
        Vector3 primary_camera_forward = {0,0,-editor->current_camera_zoom};
        primary_camera_forward = Vector3RotateByQuaternion(primary_camera_forward , primary_rotation);
        
        SplitViewport * secondary_spilt = all_viewport + viewport_count++;
        secondary_spilt->viewport = secondary_viewport;
        secondary_spilt->camera_euler = editor->editor_camera_euler;
        secondary_spilt->camera_euler.y += 90;
        secondary_spilt->camera_offset = editor->editor_camera_offset;
        secondary_spilt->camera_zoom = editor->current_camera_zoom;
        secondary_spilt->camera_up = (Vector3){0,1,0};
        secondary_spilt->ortho = true;
        
        SplitViewport * tertiary_spilt = all_viewport + viewport_count++;
        tertiary_spilt->viewport = tertiary_viewport;
        tertiary_spilt->camera_euler = editor->editor_camera_euler;
        tertiary_spilt->camera_euler.x += 90;
        tertiary_spilt->camera_offset = editor->editor_camera_offset;
        tertiary_spilt->camera_zoom = editor->current_camera_zoom;
        tertiary_spilt->camera_up = Vector3Negate(Vector3Normalize(primary_camera_forward));
        tertiary_spilt->ortho = true;
    }
    
    local_persist Vector2 last_mouse_position = {1,1};
    if(mouse_position.x > 0)
    {
        if(mouse_position.x <= app_data->window_size.x)
        {
            if(mouse_position.y > 0)
            {
                if(mouse_position.y <= app_data->window_size.y)
                {
                    last_mouse_position = mouse_position;
                }
            }
        }
    }
    
    local_persist int previous_first_viewport_index = 0;
    
    if(mouse_pressing(MOUSE_BUTTON_LEFT))
    {
        SplitViewport previous_split = all_viewport[previous_first_viewport_index];
        
        all_viewport[previous_first_viewport_index] = all_viewport[0];
        all_viewport[0] = previous_split;
    }
    else
    {
        //hope the viewport order doesn't matter
        for(int viewport_index = 0 ; viewport_index < viewport_count ; viewport_index++)
        {
            SplitViewport current_split = all_viewport[viewport_index];
            
            if(CheckCollisionPointRec(last_mouse_position , current_split.viewport))
            {
                all_viewport[viewport_index] = all_viewport[0];
                all_viewport[0] = current_split;
                previous_first_viewport_index = viewport_index;
                
                break;
            }
        }
    }
    
    for(int viewport_index = 0 ; viewport_index < viewport_count ; viewport_index++)
    {
        SplitViewport current_split = all_viewport[viewport_index];
        current_viewport = current_split.viewport;
        
        Quaternion camera_rotation = QuaternionFromEuler(current_split.camera_euler.x * DEG2RAD, current_split.camera_euler.y * DEG2RAD,current_split.camera_euler.z * DEG2RAD);
        float camera_zoom = current_split.camera_zoom;
        
        Vector3 camera_direction = Vector3RotateByQuaternion((Vector3){0,0,-camera_zoom} , camera_rotation);
        
        if(editor_type == demo)
        {
            world_camera.position = Vector3Add(game_camera_position , Vector3Negate(game_camera_offset));
            world_camera.target = game_camera_position;
            world_camera.fovy = 45;
        }
        else
        {
            world_camera.target = current_split.camera_offset;
            world_camera.position = Vector3Add(camera_direction , current_split.camera_offset);
            world_camera.fovy = 90;
        }
        
        world_camera.up = current_split.camera_up;
        
        view_matrix = MatrixLookAt(world_camera.position, world_camera.target, world_camera.up);
        double aspect = (double)current_viewport.width / (double)current_viewport.height;
        
        if(!current_split.ortho)
        {
            project_matrix = MatrixPerspective(world_camera.fovy * DEG2RAD, aspect , 0.1, 200);
            mouse_ray_3D = get_screen_to_world_ray_EX( (Vector2){mouse_x , mouse_y} ,  world_camera , current_viewport.x , current_viewport.y , current_viewport.width, current_viewport.height);
        }
        else
        {
            project_matrix = MatrixOrtho(-aspect * camera_zoom , aspect * camera_zoom , -camera_zoom , camera_zoom , 0.1, 200);
            
        }
        
        world_3D_matrix = MatrixMultiply(view_matrix, project_matrix);
        
        viewport_screen_pixel_matrix = MatrixOrtho(current_viewport.x , current_viewport.x + current_viewport.width , current_viewport.y , current_viewport.y + current_viewport.height , screen_near_clip , screen_far_clip);
        
        world_3D_to_screen_matrix = MatrixMultiply(world_3D_matrix , MatrixInvert(viewport_screen_pixel_matrix));
        
        if(current_split.ortho)
        {
            mouse_ray_3D.direction = Vector3Negate(Vector3Normalize(camera_direction));
            
            Vector3 mouse_on_screen = {mouse_position.x , mouse_position.y  , 0};
            mouse_on_screen = transform_vector(mouse_on_screen , (viewport_screen_pixel_matrix));
            mouse_on_screen = transform_vector(mouse_on_screen , MatrixInvert(project_matrix));
            mouse_on_screen = transform_vector(mouse_on_screen , MatrixInvert(view_matrix));
            mouse_ray_3D.position = mouse_on_screen;
        }
        
        editor->right = (Vector3){view_matrix.m0 , view_matrix.m4 , view_matrix.m8 };
        editor->up = (Vector3){ view_matrix.m1 , view_matrix.m5 , view_matrix.m9 };
        
        editor->billboard_rotation = QuaternionFromVector3ToVector3((Vector3){0,1,0} , editor->up );
        editor->billboard_rotation = QuaternionMultiply( editor->billboard_rotation ,  QuaternionFromVector3ToVector3( (Vector3){1,0,0} , editor->right));
        
        Vector3 camera_offset_result = {};
        Vector3 camera_euler_result = {};
        
        within_viewport = viewport_index == 0;
        
        if(within_viewport)
        {
            if (mouse_pressing(MOUSE_BUTTON_MIDDLE))
            {
                if (key_pressing(KEY_LEFT_SHIFT))
                {
                    Vector3 mouse_right = Vector3Scale( editor->right , -mouse_position_delta.x * 0.005f);
                    Vector3 mouse_up = Vector3Scale(editor->up , -mouse_position_delta.y * 0.005f);
                    
                    Vector3 mouse_drag_offset = Vector3Add(mouse_right , mouse_up);
                    
                    camera_offset_result = Vector3Scale(mouse_drag_offset , editor->current_camera_zoom * 0.2);
                }
                else
                {
                    camera_euler_result.y = -mouse_position_delta.x * 0.1;
                    camera_euler_result.x = -mouse_position_delta.y * 0.1;
                }
            }
            
            editor->editor_camera_offset = Vector3Add(editor->editor_camera_offset , camera_offset_result);
            editor->editor_camera_euler = Vector3Add(editor->editor_camera_euler , camera_euler_result);
        }
        
        glViewport(current_viewport.x , current_viewport.y ,current_viewport.width, current_viewport.height);
        
        pruning_3D_line = true;
        change_matrix(world_3D_matrix);
        
        draw_background();
        
        if(viewport_index == 0) 
        {
            input_state = &client_input_state;
            if(net_state.is_client) client_update();
            if(net_state.is_server) server_update();
        }
        
        input_state = &client_input_state;
        viewport_update();
    }
    
	glDisable(GL_DEPTH_TEST);
	rlDisableFramebuffer();
    editor->flat_color = true;
    glViewport(0 , 0 ,app_data->window_size.x , app_data->window_size.y);
    
#if 1
	if (editor->flat_color)
	{
        
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        GL_CATCH;
        
		draw_screen_flat_B(render_state.game_world_texture , WHITE , true);
	}
	else
	{
        
#if 0
        
		rlFramebufferAttach(render_state.screen_frame_buffer, render_state.lighting_texture, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
		if(!rlFramebufferComplete(render_state.screen_frame_buffer)) CATCH;
        
		rlEnableFramebuffer(render_state.screen_frame_buffer);
        
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_DST_ALPHA, GL_ONE, GL_ONE);
		glEnable(GL_BLEND);
        
		D_foreach(D_Light, CurrentLight, GameWorld->LightArray)
		{
			Vector4 LightColor = CurrentLight->LightColor;
			LightColor.w *= CurrentLight->LightStrength;
			AddLight(CurrentLight->Lightposition, CurrentLight->LightSize, LightColor);
		}
        
		ShaderInput * LightShaderInput = all_shader_inputs + S_point_light;
        
		rlEnableShader(LightShaderInput->Shader);
		glBindVertexArray(LightShaderInput->VertexArrayObject);
        
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, render_state.game_world_texture);
		glUniform1i(LightShaderInput->UniformLocation[SU_texture_0], 0);
        
		rlSetUniformMatrix(LightShaderInput->UniformLocation[SU_MVP] , MVP_2DWorld);
        
		update_all_GPU_vertex_buffer(LightShaderInput, light_data.light_count);
        
		glDrawArrays(GL_POINTS, 0, light_data.light_count);
        
        
		float TargetLightStrength = 0.6f;
        
		local_persist float LightStrength = 0.3f;
		LightStrength = Lerp(LightStrength , TargetLightStrength , 0.36f);
        
		draw_screen_flat(render_state.game_world_texture, Fade(WHITE, LightStrength ));
        
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		rlDisableFramebuffer();
        light_data.light_count = 0;
        
		rlFramebufferAttach(render_state.screen_frame_buffer, render_state.bloom_texture, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
		if(!rlFramebufferComplete(render_state.screen_frame_buffer)) CATCH;
        
		rlEnableFramebuffer(render_state.screen_frame_buffer);
        
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
		ShaderInput * bloom_shader = all_shader_inputs + S_bloom;
        
		rlEnableShader(bloom_shader->shader);
		glBindVertexArray(bloom_shader->vertex_array_object);
        
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D , render_state.lighting_texture);
		glUniform1i(bloom_shader->uniform_location[SU_texture_0], 0);
        
		glUniform2f(bloom_shader->uniform_location[SU_screen_size] , app_data->window_size.x , app_data->window_size.y);
        
		Vector3 * sreen_quad_pos = vertex_data.position;
		Vector4 * screen_quad_color_data = vertex_data.color;
		Vector2 * screen_quad_texCoord_data = vertex_data.tex_coord;
        
		int vertex_count = 0;
        
		sreen_quad_pos[vertex_count++] = (Vector3){ -1, 1, 0 };
		sreen_quad_pos[vertex_count++] = (Vector3){ 1, 1, 0 };
		sreen_quad_pos[vertex_count++] = (Vector3){ 1, -1, 0 };
        
		sreen_quad_pos[vertex_count++] = (Vector3){ -1, -1, 0 };
		sreen_quad_pos[vertex_count++] = (Vector3){ -1, 1, 0 };
		sreen_quad_pos[vertex_count++] = (Vector3){ 1, -1, 0 };
        
		vertex_count = 0;
        
		screen_quad_texCoord_data[vertex_count++] = (Vector2){ 0, 1 };
		screen_quad_texCoord_data[vertex_count++] = (Vector2){ 1, 1 };
		screen_quad_texCoord_data[vertex_count++] = (Vector2){ 1, 0 };
        
		screen_quad_texCoord_data[vertex_count++] = (Vector2){ 0, 0 };
		screen_quad_texCoord_data[vertex_count++] = (Vector2){ 0, 1 };
		screen_quad_texCoord_data[vertex_count++] = (Vector2){ 1, 0 };
        
		vertex_count = 0;
        
		screen_quad_color_data[vertex_count++] = (Vector4){1,1,1,1};
		screen_quad_color_data[vertex_count++] = (Vector4){1,1,1,1};
		screen_quad_color_data[vertex_count++] = (Vector4){1,1,1,1};
        
		screen_quad_color_data[vertex_count++] = (Vector4){1,1,1,1};
		screen_quad_color_data[vertex_count++] = (Vector4){1,1,1,1};
		screen_quad_color_data[vertex_count++] = (Vector4){1,1,1,1};
        
		update_all_GPU_vertex_buffer(bloom_shader, 6);
		glDrawArrays(GL_TRIANGLES, 0, 6);
        
		rlDisableFramebuffer();
        
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
		update_all_GPU_vertex_buffer(bloom_shader, 6);
        
		for (int shader_index = S_blur_H; shader_index <= S_blur_V; shader_index++)
		{
			unsigned int target_blur_texture = render_state.blur_texture;
            
			if (shader_index == S_blur_H) target_blur_texture = render_state.first_blur_texture;
            
			rlFramebufferAttach(render_state.screen_frame_buffer, target_blur_texture , RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
			if(!rlFramebufferComplete(render_state.screen_frame_buffer)) CATCH;
            
			rlEnableFramebuffer(render_state.screen_frame_buffer);
            
			ShaderInput * blur_shader = all_shader_inputs + shader_index;
            
			rlEnableShader(blur_shader ->shader);
			glBindVertexArray(blur_shader ->vertex_array_object);
            
			unsigned int base_texture = render_state.first_blur_texture;
			if (shader_index == S_blur_H) base_texture = render_state.bloom_texture;
            
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D , base_texture);
			glUniform1i(blur_shader ->uniform_location[SU_texture_0], 0);
            
			Vector2 screen_world = {app_data->window_size.x , app_data->window_size.y};
            
			glUniform2f(blur_shader->uniform_location[SU_screen_size] , 1.0f / app_data->window_size.x , 1.0f / app_data->window_size.x);
			glUniform1f(blur_shader->uniform_location[SU_grid_size] ,11);
			glUniform1f(blur_shader->uniform_location[SU_spread] , 10);
            
			//i just reusing data already exist in buffer
			update_all_GPU_vertex_buffer(blur_shader , 6);
			glDrawArrays(GL_TRIANGLES, 0, 6);
            
			rlDisableFramebuffer();
		}
        
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_DST_ALPHA, GL_ONE, GL_ONE);
        
		draw_screen_flat_B(render_state.lighting_texture ,Fade(WHITE,1.0f) , false);
		draw_screen_flat_B(render_state.blur_texture ,Fade(WHITE,0.4f), false);
        
		GL_CATCH;
        
#endif
        
	}
#endif
    
    
    pruning_3D_line = false;
	change_matrix(full_screen_pixel_matrix);
	glBlendFunc(GL_ONE , GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
    
	//i just reusing texture
	//maybe i should reuse more?
	draw_screen_flat_B(render_state.interface_texture , WHITE , true);
    
	D_game_draw();
    
	render_state.elapsed_time += DELTA_TIME;
    
}

internal GAME_LOOP(game_loop)
{
    if (shader_compile_failed) return;
    
    local_persist bool initialized_timer = false;
    local_persist double update_timer = 0;
    local_persist double previous_loop_time = 0;
    local_persist double elapsed_time = 0;
    
    if(!initialized_timer)
    {
        initialized_timer = true;
        previous_loop_time = time_stamp();
    }
    
    double current_time = time_stamp();
    elapsed_time += (current_time - previous_loop_time) / (1000.0 * 1000.0);
    
    double update_time = 1.0f / (double)D_UPDATE_RATE;
    
    //it got interuped by debugger or something
    if (elapsed_time > update_time * 10) elapsed_time = 0;
    
    int update_count = 0;
    
    for (;elapsed_time > update_time ; elapsed_time -= update_time )
    {
		app_data->mouse_scroll_delta = 0;
		app_data->codepoint_queue_count = 0;
		app_data->codepoint_queue_indedx = 0;
		
        input_state = &client_input_state;
        input_state->pressed_mouse_count = 0;
		input_state->released_mouse_count = 0;
		input_state->pressed_key_count = 0;
        input_state->released_key_count = 0;
        
        for(int key_index = 0 ; key_index < INPUT_MAX_KEY ; key_index++)
        {
            input_state->pressed_mouse_consumed[key_index] = false;
            input_state->pressing_key_time[key_index] += DELTA_TIME;
        }
        
        glfwPollEvents();
        
        double game_update_time = time_stamp();
        game_update();
        game_update_count++;
        
        double game_upate_time_delta = (time_stamp()-game_update_time) / (1000.0 * 1000.0);
        
        if (game_upate_time_delta > update_time)
        {
            printf( "update is lagging : %f \n", game_upate_time_delta );
        }
        
        frame_time_memory.current_memory = frame_time_memory.start_memory;
        glfwSwapBuffers(app_data->current_window);
    }
    
    previous_loop_time = current_time;
    
    //these things doesn't help
    //it still lock on 30 fps
    
    //glFinish();
    
    //this things freeze opengl ?
    //SwapBuffers(AppData->_HDC);
}

#define allocate_to_file(name , type , count) (type *)allocate_to_file_(name , sizeof(type) , count )

internal unsigned char * allocate_to_file_(char * name , int size , int count)
{
    int allocate_amount = size * count;
    
    if(allocate_amount == 0) return 0;
    
    (*current_data_header) = (DataHeader){};
    DataHeader * new_data_header = current_data_header;
    current_data_header++;
    
    strcpy(new_data_header->name.string , name);
    new_data_header->data_offset = (current_save_memory_location - save_memory);
    
	unsigned char* start = current_save_memory_location;
	current_save_memory_location += allocate_amount;
	if (current_save_memory_location >= save_memory + MAX_SAVE_SIZE)
	{
		CATCH;
	}
    
	memset(start, 0, allocate_amount);
    
    return start;
}

#define write_buffer( data , name , type , index ,  count) \
{ \
local_persist bool initialized = false;\
local_persist type * buffer = 0;\
if(!initialized) { initialized = true ; buffer = allocate_to_file(name , type , count); }\
buffer[index] = data;\
}

#define write_data( data , name , type ) (*(type*)allocate_to_file(name  , type , 1)) = data

internal void save_data_to_file(char * path)
{
    int save_header_size = (current_data_header - data_header_array) * sizeof(DataHeader);
    int save_size = current_save_memory_location - save_memory;
    
    FILE * save_file = fopen(path , "wb");
    
    fwrite(&save_header_size , sizeof(int) , 1, save_file);
    fwrite(&save_size , sizeof(int) , 1, save_file);
    
    fwrite( data_header_array , save_header_size , 1, save_file);
    fwrite( save_memory , save_size , 1, save_file);
    
    fclose(save_file);
}

internal void save_game_state()
{
    save_memory = malloc(MAX_SAVE_SIZE);
    data_header_array = malloc(MAX_HEADER_SIZE);
    
    current_save_memory_location = save_memory;
    current_data_header = data_header_array;
    
    write_data(editor_type , "editor_type" , int);
    write_data(editor->current_frame_at_timeline , "current_frame_index" , int);
    write_data(editor->start_frame_index , "start_frame_index" , int);
    write_data(editor->timeline_frame_length , "timeline_frame_length" , int);
    
    write_data(editor->selected_clip_index , "editor_selected_clip_index" , int);
    write_data(editor->editor_camera_offset , "editor_camera_offset" , Vector3);
    write_data(editor->editor_camera_euler , "editor_camera_euler" , Vector3);
    
    write_data(editor->target_camera_zoom , "editor_target_zoom" , float);
    write_data(editor->current_camera_zoom , "editor_current_zoom" , float);
    
    write_data(current_map_edit_type , "map_edit_type" , int);
    
    write_data(display_all_visual , "display_all_viusal" , bool);
    write_data(capture_collision_non_stop , "capture_collision_non_stop" , bool);
    
    int clip_count = 0;
    int key_frame_count = 0;
    for(int clip_index = 0 ; clip_index < MAX_CLIP; clip_index++) 
    {
        Clip * current_clip = clip_array[clip_index];
        
        if(current_clip)
        {
            clip_count++;
            
            current_clip->key_frame_count_to_save = 0;
            list_foreach(key_frame_index , &current_clip->key_frame_active_list) 
            {
                current_clip->key_frame_count_to_save++;
                key_frame_count++;
            }
        }
    }
    
    write_data( clip_count , "clip_count" , int);
    write_data( all_key_frame_buffer.count , "key_frame_count" , int);
    write_data( selected_model->bone_buffer.count , "base_bone_count" , int );
    
    for(int clip_array_index = 0 , clip_index = 0 , key_frame_index = 0 
        ; clip_array_index < MAX_CLIP ; 
        clip_array_index++)
    {
        
        Clip * current_clip = clip_array[clip_array_index];
        
        if(!current_clip) continue;
        
        write_buffer(current_clip->clip_name ,   "clip_name" ,FixedString ,clip_index , clip_count);
        write_buffer(current_clip->clip_index , "clip_index" ,  int ,clip_index , clip_count);
        write_buffer(current_clip->not_loop ,"clip_not_loop" , bool , clip_index ,  clip_count);
        
        write_buffer(key_frame_index , "clip_key_frame_offset" , int ,clip_index , clip_count);
        write_buffer(current_clip->key_frame_count_to_save , "clip_key_frame_count" , int ,clip_index , clip_count);
        
        list_foreach(array_index , &current_clip->key_frame_active_list)
        {
            KeyFrame * key_frame = all_key_frame_buffer.data + array_index;
            
            write_buffer(key_frame->bone_index , "bone_index" , int ,key_frame_index ,  key_frame_count);
            write_buffer(key_frame->bone_state.local_position , "local_position" ,  Vector3 ,key_frame_index , key_frame_count);
            write_buffer(key_frame->bone_state.local_rotation , "local_rotation" , Quaternion, key_frame_index  , key_frame_count);
            write_buffer(key_frame->bone_state.end_point_offset , "end_point_offset"  , Vector3 , key_frame_index, key_frame_count);
            write_buffer(key_frame->frame_index , "frame_index" , int , key_frame_index , key_frame_count);
            write_buffer(key_frame->next_bezier_offset , "next_bezier_offset" , Vector3 ,key_frame_index ,  key_frame_count);
            write_buffer(key_frame->previous_bezier_offset ,"previous_bezier_offset" , Vector3 , key_frame_index ,  key_frame_count);
            
            key_frame_index++;
            
        }
        
        clip_index++;
    }
    
    for(int bone_index = 0 ; bone_index < selected_model->bone_buffer.count ; bone_index++)
    {
        Bone * current_bone = selected_model->bone_buffer.data + bone_index;
        
        FixedStringW fixed_string_name = {};
        fixed_string_name = current_bone->bone_name;
        
        write_buffer(fixed_string_name ,  "base_bone_name" , FixedStringW ,bone_index , selected_model->bone_buffer.count);
        write_buffer(current_bone->from_blend_file , "bone_from_blend" , bool , bone_index , selected_model->bone_buffer.count);
        write_buffer(current_bone->IK_enable , "IK_enable" , bool , bone_index , selected_model->bone_buffer.count);
        write_buffer(current_bone->IK_chain_length , "IK_chain_length" , int , bone_index , selected_model->bone_buffer.count);
        write_buffer(current_bone->IK_target_bone_index , "IK_target" , int , bone_index , selected_model->bone_buffer.count);
        write_buffer(current_bone->IK_pole_bone_index , "IK_pole" , int , bone_index , selected_model->bone_buffer.count);
        write_buffer(current_bone->free_position , "base_bone_free_position" , bool , bone_index , selected_model->bone_buffer.count);
        
        write_buffer(current_bone->state.local_position ,"base_bone_position" , Vector3 , bone_index ,  selected_model->bone_buffer.count);
        write_buffer(current_bone->state.local_rotation ,  "base_bone_quaternion" , Quaternion ,bone_index , selected_model->bone_buffer.count);
        write_buffer(current_bone->state.end_point_offset ,  "base_bone_end_point_offset" , Vector3 ,bone_index , selected_model->bone_buffer.count);
        
        write_buffer(current_bone->parent_bone_index , "base_bone_parent" , int , bone_index , selected_model->bone_buffer.count);
    }
    
    save_data_to_file(get_app_file_path(game_state_save_name));
    
    free(save_memory);
    free(data_header_array);
}

internal void save_map()
{
    save_memory = malloc(MAX_SAVE_SIZE);
    data_header_array = malloc(MAX_HEADER_SIZE);
    
    current_save_memory_location = save_memory;
    current_data_header = data_header_array;
    
    int quad_count = quad_in_map_array.count;
    write_data(quad_count , "map_quad_count" , int);
    array_foreach_B(array_index , quad_index , &quad_in_map_array )
    {
        Quad quad = quad_in_map_buffer.data[array_index];
        
        write_buffer(quad.vertex_position[vertex_top_left] , "map_quad_top_left_vertex" , Vector3 , quad_index , quad_count);
        write_buffer(quad.vertex_position[vertex_top_right] , "map_quad_top_right_vertex" , Vector3 , quad_index , quad_count);
        write_buffer(quad.vertex_position[vertex_bottom_left] , "map_quad_bottom_left_vertex" , Vector3 , quad_index , quad_count);
        write_buffer(quad.vertex_position[vertex_bottom_right] , "map_quad_bottom_right_vertex" , Vector3 , quad_index , quad_count);
    }
    
    int box_count = box_in_map_array.count;
    write_data(box_count , "map_box_count" , int);
    array_foreach_B(array_index , box_index , &box_in_map_array)
    {
        Box box = box_in_map_buffer.data[array_index];
        
        write_buffer(box.position , "map_box_position" , Vector3 , box_index , box_count);
        write_buffer(box.size , "map_box_size" , Vector3 , box_index , box_count);
        write_buffer(box.rotation , "map_box_rotation" , Quaternion , box_index , box_count);
    }
    
    //clear_array(&camera_array);
    
    int entity_count = entity_layout_array.count;
    write_data(entity_count , "entity_count" , int);
    array_foreach_B(array_index , entity_index , &entity_layout_array)
    {
        Entity entity = entity_layout_buffer.data[array_index];
        
        write_buffer(entity.position , "entity_position" , Vector3 , entity_index , entity_count);
    }
    
    int trigger_count = camera_array.count;
    write_data(trigger_count , "camera_trigger_count" , int);
    array_foreach_B(array_index , trigger_box_index , &camera_array)
    {
        CameraTrigger trigger = camera_buffer.data[array_index];
        Box zone = camera_zone_buffer.data[array_index];
        
        write_buffer(zone.position , "camera_zone_position" , Vector3 , trigger_box_index , trigger_count);
        write_buffer(zone.size , "camera_zone_size" , Vector3 , trigger_box_index , trigger_count);
        write_buffer(zone.rotation , "camera_zone_rotation" , Quaternion , trigger_box_index , trigger_count);
        write_buffer(trigger.camera_target_offset , "camera_zone_offset" , Vector3 , trigger_box_index ,  trigger_count);
    }
    
    write_data(selected_reference_frame_index , "selected reference frame" , int);
    
    int reference_frame_count = 0;
    list_foreach(array_index , &reference_frame_list) reference_frame_count++;
    write_data(reference_frame_count , "reference frame count" , int);
    
    int reference_frame_index = 0;
    list_foreach(array_index , &reference_frame_list)
    {
        Vector3 reference_frame = reference_frame_buffer.data[array_index];
        write_buffer(reference_frame , "reference frame" , Vector3 , reference_frame_index , reference_frame_count);
        reference_frame_index++;
    }
    
    save_data_to_file(get_app_file_path(map_save_name));
    
    free(save_memory);
    free(data_header_array);
}

internal int reassign_bone_index(int previous_bone_index)
{
    if(previous_bone_index == -1) return -1;
    
    FixedStringW previous_bone_name = {};
    read_buffer(previous_bone_name , "base_bone_name" , FixedStringW , previous_bone_index);
    
    int parent_index = -1;
    
    for(int current_bone_index = 0 ; current_bone_index < selected_model->bone_buffer.count ; current_bone_index++)
    {
        Bone * parent_bone = selected_model->bone_buffer.data + current_bone_index;
        if(compare_string_W( previous_bone_name.string , parent_bone->bone_name.string ))
        {
            parent_index = current_bone_index;
            break;
        }
    }
    
    return parent_index;
}

//TODO: reassign bone index
internal void load_game_state()
{
    if(!load_data_from_file(get_app_file_path(game_state_save_name))) return;
    
    read_data(editor_type , "editor_type" , int);
    read_data(editor->current_frame_at_timeline , "current_frame_index" , int);
    read_data(editor->timeline_frame_length , "timeline_frame_length" , int);
    read_data(editor->start_frame_index , "start_frame_index" , int);
    
    read_data(editor->selected_clip_index , "editor_selected_clip_index" , int);
    read_data(editor->editor_camera_offset , "editor_camera_offset" , Vector3);
    read_data(editor->editor_camera_euler , "editor_camera_euler" , Vector3);
    read_data(editor->target_camera_zoom , "editor_target_zoom" , float);
    read_data(editor->current_camera_zoom , "editor_current_zoom" , float);
    
    read_data(current_map_edit_type , "map_edit_type" , int);
    
    read_data(display_all_visual , "display_all_viusal" , bool);
    read_data(capture_collision_non_stop , "capture_collision_non_stop" , bool);
    
    int previous_key_frame_count = 0;
    read_data(previous_key_frame_count , "key_frame_count" , int);
    
    int new_key_frame_capacity = 1;
    for(; new_key_frame_capacity < previous_key_frame_count; new_key_frame_capacity *= 2);
    allocate_buffer(&all_key_frame_buffer  , KeyFrame , new_key_frame_capacity , AT_temp);
    all_key_frame_buffer.count = previous_key_frame_count;
    
    for(int key_frame_index = 0 ; key_frame_index < all_key_frame_buffer.count ; key_frame_index++)
    {
        int key_frame_owner_clip_index = -1;
        
        KeyFrame * key_frame = all_key_frame_buffer.data + key_frame_index;
        
        read_buffer(key_frame->bone_index , "bone_index" , int ,key_frame_index);
        read_buffer(key_frame->bone_state.local_position , "local_position" ,  Vector3 ,key_frame_index);
        read_buffer(key_frame->bone_state.local_rotation , "local_rotation" , Quaternion, key_frame_index);
        read_buffer(key_frame->bone_state.end_point_offset , "end_point_offset"  , Vector3 , key_frame_index);
        read_buffer(key_frame->frame_index , "frame_index" , int , key_frame_index);
        read_buffer(key_frame->next_bezier_offset , "next_bezier_offset" , Vector3 ,key_frame_index);
        read_buffer(key_frame->previous_bezier_offset ,"previous_bezier_offset" , Vector3 , key_frame_index);
    }
    
    int clip_count = -1;
    read_data( clip_count , "clip_count" , int);
    Clip * new_clip_array = allocate_temp(Clip , clip_count);
    
    for(int clip_index = 0 ; clip_index < clip_count ; clip_index++)
    {
        clip_array[clip_index] = new_clip_array + clip_index;
        
        Clip * new_clip = new_clip_array + clip_index;
        new_clip->key_frame_active_list = allocate_list(CLIP_START_CAPACITY , AT_temp);
        new_clip->dragging_key_frame_list = allocate_list(CLIP_START_CAPACITY, AT_temp);
        new_clip->key_frame_hash_table_by_bone = allocate_hash_table(CLIP_START_CAPACITY, AT_temp);
        new_clip->key_frame_hash_table = allocate_hash_table(CLIP_START_CAPACITY, AT_temp);
        
        read_buffer(new_clip->clip_name ,   "clip_name" ,FixedString ,clip_index );
        read_buffer(new_clip->clip_index , "clip_index" ,  int ,clip_index );
        read_buffer(new_clip->not_loop ,"clip_not_loop" , bool , clip_index );
        
        int key_frame_start = -999999999;
        int key_frame_end = -999999999;
        
        read_buffer(key_frame_start , "clip_key_frame_offset" , int ,clip_index);
        read_buffer(key_frame_end , "clip_key_frame_count" , int ,clip_index);
        
        key_frame_end += key_frame_start;
        
        for(int key_frame_index = key_frame_start ; key_frame_index < key_frame_end ; key_frame_index++)
        {
            KeyFrame * key_frame = all_key_frame_buffer.data + key_frame_index;
            
            add_to_list_tail( key_frame_index , &new_clip->key_frame_active_list);
            
            add_to_hash_table(key_frame->bone_index , key_frame_index , &new_clip->key_frame_hash_table_by_bone);
            sort_bone_hash_table(key_frame->bone_index , &new_clip->key_frame_hash_table_by_bone);
            
            add_to_hash_table(GetKeyFrameHash(key_frame->frame_index , key_frame->bone_index) , key_frame_index , &new_clip->key_frame_hash_table);
        }
        
        //read_buffer(new_clip->key_frame_array_count ,"clip_key_frame_count" , int , clip_index);//new_clip->key_frame_array = allocate_temp(KeyFrame , new_clip->key_frame_count);
    }
    
    int previous_bone_count = -1;
    read_data(previous_bone_count , "base_bone_count",int);
    
    for(int previous_bone_index = 0 ; previous_bone_index < previous_bone_count ; previous_bone_index++)
    {
        
        FixedStringW previous_name = {};
        bool is_from_blend_file = 0;
        
        read_buffer(previous_name , "base_bone_name" , FixedStringW , previous_bone_index);
        read_buffer(is_from_blend_file , "bone_from_blend" , bool , previous_bone_index);
        
        Bone* bone_to_assign = 0;
        
        if(!is_from_blend_file)
        {
            if(buffer_full(selected_model->bone_buffer))
            {
                reallocate_buffer(&selected_model->bone_buffer , AT_temp);
            }
            
            if(buffer_full(selected_model->initial_bone_buffer))
            {
                reallocate_buffer(&selected_model->initial_bone_buffer , AT_temp);
            }
            
            selected_model->initial_bone_buffer.count++;
            int new_bone_index = selected_model->bone_buffer.count++;
            bone_to_assign = selected_model->bone_buffer.data + new_bone_index;
            bone_to_assign->bone_name = previous_name;
            bone_to_assign->from_blend_file = is_from_blend_file;
            bone_to_assign->bone_index = new_bone_index;
        }
        else
        {
            for(int current_bone_index = 0 ; current_bone_index < selected_model->bone_buffer.count ; current_bone_index++)
            {
                Bone * current_bone = selected_model->bone_buffer.data + current_bone_index;
                if(compare_string_W(current_bone->bone_name.string , previous_name.string))
                {
                    bone_to_assign = current_bone;
                    break;
                }
            }
        }
        
        read_buffer(bone_to_assign->IK_enable , "IK_enable" , bool , previous_bone_index);
        read_buffer(bone_to_assign->IK_chain_length , "IK_chain_length" , int , previous_bone_index);
        read_buffer(bone_to_assign->IK_target_bone_index , "IK_target" , int , previous_bone_index);
        read_buffer(bone_to_assign->IK_pole_bone_index , "IK_pole" , int , previous_bone_index);
        read_buffer(bone_to_assign->free_position , "base_bone_free_position" , bool , previous_bone_index);
        
        if(!bone_to_assign->from_blend_file)
        {
            
            read_buffer(bone_to_assign->state.local_position ,"base_bone_position" , Vector3 , previous_bone_index);
            read_buffer(bone_to_assign->state.local_rotation ,  "base_bone_quaternion" , Quaternion ,previous_bone_index);
            read_buffer(bone_to_assign->state.end_point_offset ,  "base_bone_end_point_offset" , Vector3 ,previous_bone_index);
            read_buffer(bone_to_assign->parent_bone_index , "base_bone_parent" , int , previous_bone_index);
            
        }
    }
    
    for(int bone_index = 0 ; bone_index < selected_model->bone_buffer.count ; bone_index++)
    {
        Bone * current_bone = selected_model->bone_buffer.data + bone_index;
        
        current_bone->IK_target_bone_index = reassign_bone_index(current_bone->IK_target_bone_index);
        current_bone->IK_pole_bone_index = reassign_bone_index(current_bone->IK_pole_bone_index);
        
        if(!current_bone->from_blend_file)
        {
            current_bone->parent_bone_index = reassign_bone_index(current_bone->parent_bone_index);
            
            if(current_bone->parent_bone_index == -1)
            {
                add_to_list_tail(bone_index, &selected_model->root_bone_list);
            }
            else
            {
                add_to_hash_table( current_bone->parent_bone_index , bone_index , &selected_model->bone_children_hash_table);
            }
            
        }
    }
    
    free(scratch_buffer_for_read);
}

internal GAME_UNLOAD(game_unload)
{
    
    mkdir(get_app_file_path("Game"));
    
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    glfwSwapBuffers(app_data->current_window);
    
    if (!shader_compile_failed)
    {
        save_game_state();
        save_map();
    }
    
    if (render_state.screen_frame_buffer != 0)
    {
        rlUnloadFramebuffer(render_state.screen_frame_buffer); //do i need to delete you?
    }
    
    list_foreach(texture_id, &render_state.texture_list_to_delete)
    {
        rlUnloadTexture(texture_id);
    }
    
    glDeleteTextures(1, &app_data->default_font.glyph_atlas_texture.id);
    
    for (int buffer_index = 0; buffer_index < shader_buffer_count; buffer_index++)
    {
        glDeleteBuffers(1, &all_GPU_buffers[buffer_index]->buffer_handle);
    }
    
    for (int shader_index = 0; shader_index < S_count; shader_index++)
    {
        glDeleteProgram(all_shader_inputs[shader_index].shader);
    }
    
    free(frame_time_memory.start_memory);
    free(run_time_memory.start_memory);
    free(arena_memory.start_memory);
    
    end_connection();
}

internal void client_init()
{
    allocate_buffer(&all_key_frame_buffer, KeyFrame , KEY_FRAME_CAPACITY , AT_temp);
    editor = allocate_temp(EditorData ,1);
    
    editor->timeline_scale = 1;
    editor->selected_clip_index = -1;
    editor->IK_iteration_count = 20;
    editor->assigning_parent_bone = false;
    
    editor->selected_bone_stack = allocate_temp(BoneSelection , 256);
    
    allocate_buffer(&collision_visual_buffer , CollisionVisual , 64 , AT_temp);
    allocate_buffer(&frame_collision_buffer , FrameCollision , 64 , AT_temp);
}

internal void game_init()
{
    world_camera.target = (Vector3){};
	world_camera.position = (Vector3){ 0,0,-1 };
    world_camera.projection = CAMERA_PERSPECTIVE;
	world_camera.up = (Vector3){ 0,1,0 };
	world_camera.fovy = 90;
    
    timeBeginPeriod(1);
    
    client_init();
    create_a_whole_new_world();
    
    double blend_file_load_time = time_stamp();
    //hope this doesn't screw me
    load_blend_file();
    printf("\nBlend. File Load Time: %f\n" , (time_stamp() - blend_file_load_time) / (1000.0 * 1000.0));
    
    render_state_init();
    shader_init();
    
    if (shader_compile_failed) return;
    
    glfwSetKeyCallback(app_data->current_window, key_call_back);
    glfwSetMouseButtonCallback(app_data->current_window, mouse_call_back);
    
    double game_load_time = time_stamp();
    load_game_state();
    load_map();
    printf("\nGame Load Time: %f\n" , (time_stamp() - game_load_time) / (1000.0 * 1000.0));
    
    clear_array(&quad_in_map_array);
    //generate_nav_mesh();
    
    search_queue_capacity = 128;
    search_queue = allocate_temp( Int3 , search_queue_capacity);
    
    for(int bone_index = 0 ; bone_index < selected_model->bone_buffer.count ; bone_index++)
    {
        selected_model->initial_bone_buffer.data[bone_index] = selected_model->bone_buffer.data[bone_index];
    }
    
    Image default_white_image = GenImageColor(20, 20, WHITE);
    render_state.default_white_image = D_load_texture_from_image(default_white_image);
    UnloadImage(default_white_image);
    
    Image missing_texture = GenImageCellular(20, 20, 2);
    render_state.missing_texture = D_load_texture_from_image(missing_texture);
    UnloadImage(missing_texture);
    
    box_rect[face_top].rotation = QuaternionFromEuler(-90 * DEG2RAD , 0 , 0);
    box_rect[face_bottom].rotation = QuaternionFromEuler( 90 * DEG2RAD , 0 , 0);
    box_rect[face_right].rotation = QuaternionFromEuler(0 , 90 * DEG2RAD , 0);
    box_rect[face_left].rotation = QuaternionFromEuler(0 , -90 * DEG2RAD , 0);
    box_rect[face_front].rotation = QuaternionFromEuler( 0 , 0 , 0);
    box_rect[face_back].rotation = QuaternionFromEuler(180 * DEG2RAD , 0 , 0);
    
    if(editor->selected_clip_index != -1)
    {
        if(clip_array[editor->selected_clip_index])
        {
            if(selected_model->bone_buffer.capacity)
            {
                ClipBone * new_clip_bone = clip_bone_stack + clip_bone_stack_count++;
                (*new_clip_bone) = (ClipBone){};
            }
        }
    }
}

extern GAME_LOAD(game_load)
{
	app_data = _AppData;
    app_data->game_loop = game_loop;
    app_data->game_unload = game_unload;
    
    frame_time_memory = (GameMemory){};
    frame_time_memory.size = 1024 * 1024 * 16;
    frame_time_memory.start_memory = (unsigned char*)malloc(frame_time_memory.size);
    frame_time_memory.current_memory = frame_time_memory.start_memory;
    
    run_time_memory = (GameMemory){};
    run_time_memory.size = 1024 * 1024 * 16;
    run_time_memory.start_memory = (unsigned char*)malloc(run_time_memory.size);
    run_time_memory.current_memory = run_time_memory.start_memory;
    
    arena_memory = (GameMemory){};
    arena_memory.size = 1024 * 1024 * 16;
    arena_memory.start_memory = (unsigned char *)malloc(arena_memory.size);
    arena_memory.current_memory = arena_memory.start_memory;
    
    game_init();
    
    net_state = (NetState){};
    net_state.listening_socket = -1;
    net_state.client_to_server_socket = -1;
    net_state.is_server = app_data->is_server;
    net_state.is_client = app_data->is_client;
    
    start_connection();
}