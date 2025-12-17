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

global const char * AllocatorType_String[] = 
{
 "AT_temp",
 "AT_frame",
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

global const char * EditorType_String[] = 
{
 "edit_base_pose",
 "edit_animation",
 "edit_world",
 "demo",
 "edit_type_count",
};

global const char * MapEditType_String[] = 
{
 "MET_none",
 "MET_quad",
 "MET_box",
 "MET_count",
};

global const char * ShapeType_String[] = 
{
 "ST_invalid",
 "ST_quad",
 "ST_box",
};

global const char * SplitType_String[] = 
{
 "split_yz",
 "split_xz",
 "split_xy",
 "split_count",
};

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
internal bool compare_string_C(char * string_A, char * string_B , int count);
internal bool compare_string_W( wchar_t * string_A , wchar_t * string_B);
internal bool compare_string( char * string_A, char * string_B);
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
internal float ray_get_closest_point(Vector3 o , Vector3 n , Vector3 a);
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
internal void get_box_face( Rect * temp_box_rect , Box box);
internal void draw_box( Box box , Color box_color);
internal void draw_box_line(Box box , Color line_color , float line_size);
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
internal unsigned char * allocate_memory(int size , int allocate_type);
internal void allocate_buffer_( void ** buffer_data, int * buffer_data_size , int * buffer_count , int * buffer_capacity ,  int  data_size , int capacity , int allocate_type);
internal void reallocate_buffer_( void ** data , int * buffer_capacity , int buffer_data_size  , int allocate_type);
internal void reallocate_list(List * list , int allocate_type);
internal void reallocate_array(Array * array , int allocate_type);
internal unsigned int string_to_hash_W(wchar_t *s);
internal int hash_int(int key);
internal HashTable allocate_hash_table(int capacity, int allocate_type);
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
internal List allocate_list(int capacity , int allocate_type);
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
internal Array allocate_array(int capacity , int allocate_type);
internal void clear_array(Array * array);
internal void recheck_array(Array * array);
internal int add_to_array(Array * array);
internal bool delete_from_array(Array * array , int data_index);
internal bool array_full(Array * array);
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
internal int get_data_size(char * type_name);
internal int get_type_from_name(char * type_name);
internal void create_a_whole_new_world();
internal Vector3 mouse_on_plane(Vector3 plane_origin);
internal BoneSelectionResult bone_selection(Vector2 size , Color unactive_color , Color active_color);
internal void sort_bone_hash_table(int bone_index , HashTable * hash_table_by_bone);
internal Quad direction_to_quad(Vector3 direction , float width);
internal Vector3 * box_to_point(Box box);
internal bool box_collision_ray( Vector3 origin , Vector3 direction, Box box , int * hit_face , float * hit_time);
internal int float_to_grid(float x, float size);
internal Vector3 position_to_grid(Vector3 position , float size);
internal Vector3 get_farest_point_by_direction( Vector3 direction , Vector3 * points , int point_count);
internal Vector3 get_support_point(Vector3 direction);
internal bool same_direction_b(Vector3 start , Vector3 end_a , Vector3 end_b);
internal Vector3 triple_cross_product(Vector3 a , Vector3 b);
internal void search_triangle(GJK_State * state);
internal bool iterate_simplex( GJK_State * state);
internal void draw_simplex_triangle(Vector3 a , Vector3 b , Vector3 c);
internal void draw_simplex(GJK_State * state);
internal bool check_shape(Vector3 origin , Vector3 * vertices_a , int vertices_a_count , Vector3 * vertices_b , int vertices_b_count);
internal Vector3 closest_point_on_line(Vector3 a , Vector3 b , Vector3 point);
internal Vector3 closest_point_on_triangle(Vector3 a , Vector3 b , Vector3 c , Vector3 point);
internal bool check_shape_impact(ShapeImpactData * data);
internal int shape_cell_hash(int x , int y , int z);
internal bool iterate_cell_by_bound(CellIterator * iterator, Vector3 * vertices , int vertex_count , float cell_size);
internal bool bounding_box_collided(BoundingBoxNode box_a , BoundingBoxNode box_b);
internal void get_bound(Vector3 * vertices , int vertices_count , Vector3 * right_top_forward , Vector3 * left_bottom_backward );
internal BoundingBoxNode * split_bounding_box(BoundingBoxNode * buffer , int buffer_count , int split_type);
internal int cell_to_index(Int3 cell);
internal void generate_nav_mesh();
internal PathResult path_finding(Vector3 start , Vector3 end);
internal bool check_selected_bone_rotation( Bone * final_bone_array_copy, int single_bone_index , Clip * clip_to_assign);
internal void bone_selection_and_edit_bone_state( int current_frame_index);
internal void bone_mouse_menu( Bone * single_editing_bone , Clip * clip , int current_frame_index);
internal void iterate_bone_structure(Bone * bone_array , Bone * root_bone);
internal void update_bone_structure( Bone * bone_array);
internal void bone_IK_update_B(Bone * bone_array , Bone * base_pose_bone_array , int target_bone_index , int pole_bone_index , int IK_bone_index , int iteration_count , int bone_chain_max_length);
internal void bone_IK_update( Bone * bone_array , Bone * base_pose_bone_array , Bone * target_bone , Bone * pole_bone , Bone * IK_bone , int iteration_count , int bone_chain_max_length);
internal void _bone_IK_update( Bone * bone_array , Bone * target_bone , Bone * pole_bone , Bone * IK_bone , int iteration_count , int bone_chain_max_length);
internal void edit_map(Vector3 origin);
internal void draw_origin_grid(Vector3 origin);
internal void get_bone_from_existing_key_frame(Bone * bone_array , Clip * clip , int target_frame , int target_start_frame , int target_frame_count);
internal void add_bone_state(Bone * bone_array , Bone * add_bone_array , int bone_index);
internal void add_multiple_bone_state(Bone * bone_array , Bone * add_bone_array , int bone_array_count);
internal Bone * get_multiple_bone(int bone_array_count);
internal Bone * get_bone_pose_offset_from_clip( int clip_index , int target_frame , int target_frame_start, int target_frame_length);
internal float get_corner_weight(float vertical , float horizontal);
internal void single_update();
internal void viewport_update();
internal void game_update();
internal GAME_LOOP(game_loop);
internal unsigned char * get_data_buffer_by_name(char * name);
internal unsigned char * allocate_to_file_(char * name , int size , int count);
internal void save_data_to_file(char * path);
internal void save_game_state();
internal void save_map();
internal int reassign_bone_index(int previous_bone_index);
internal bool load_data_from_file(char * path);
internal void load_game_state();
internal void load_map();
internal GAME_UNLOAD(game_unload);
internal void game_init();
