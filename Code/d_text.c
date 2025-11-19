
internal void char_to_wide_char(wchar_t * dst , char* src ,int length)
{
	
    if(!src) return;
    
	if(length == 0) { length = strlen(src); }
	length = MultiByteToWideChar(CP_UTF8, 0, src, length, 0, 0);
	
    if(dst) {
		MultiByteToWideChar(CP_UTF8, 0, src, length, dst, length);
		dst[length] = L'\0';
	}
    
}

global float string_space_scaler = 1;

internal D_GlyphInfo D_get_glyph_from_codepoint(wchar_t current_codepoint,D_FontContext * font_context)
{
	int hash = current_codepoint % GLYPH_INFO_HASHMAP_LENGTH;
    
	D_GlyphInfoNode* previous_node = 0;
	D_GlyphInfoNode** current_node_pointer = &font_context->glyph_hash_map[hash];
    
	bool glyph_not_exist = true;
    
	while ((*current_node_pointer))
	{
		previous_node = *current_node_pointer;
        
		if ((*current_node_pointer)->info.value != current_codepoint)
		{
			current_node_pointer = &(*current_node_pointer)->next_node;
		}
		else
		{
			glyph_not_exist = false;
			break;
		}
	}
    
	if (glyph_not_exist)
	{
		app_data->update_atlas = true;
		(*current_node_pointer) = font_context->all_glyph_info + font_context->all_glyph_info_count;
		font_context->all_glyph_info_count++;
        
		if (previous_node)
			previous_node->next_node = *current_node_pointer;
        
		D_GlyphInfoNode* current_node = *current_node_pointer;
		D_GlyphInfo* current_glyph_info = &current_node->info;
        
		current_glyph_info->value = current_codepoint;
        
		float scale_factor = stbtt_ScaleForPixelHeight(&font_context->font_info, font_context->glyph_resolution);
        
		stbtt_GetCodepointHMetrics(&font_context->font_info, current_codepoint, &current_glyph_info->advanceX, 0);
		current_glyph_info->advanceX *= scale_factor;
        
		int glyph_width = 0;
		int glyph_height = 0;
		int glyph_offsetX = 0;
		int glyph_offsetY = 0;
        
		unsigned char* glyph_bitmap = stbtt_GetCodepointSDF
		(
         &font_context->font_info,
         scale_factor,
         current_codepoint,
         5,
         180,
         180 / 5.0,
         &glyph_width,
         &glyph_height,
         &glyph_offsetX,
         &glyph_offsetY
         );
        
		current_glyph_info->offsetX = glyph_offsetX;
		current_glyph_info->offsetY = glyph_offsetY;
        
		//what is this?
		//font base line?
		int ascent = 0;
		stbtt_GetFontVMetrics(&font_context->font_info, &ascent, 0, 0);
		current_glyph_info->offsetY += ascent * scale_factor;
        
#if false
		stbtt_MakeCodepointBitmap
		(
         &FontContext->font_info,
         (unsigned char*)BitmapForCodepoint,
         glyph_width,
         glyph_height,
         glyph_width,
         scale_factor,
         scale_factor,
         current_codepoint
         );
#endif
        
		stbrp_rect rect = {};
        
		rect.w = glyph_width + font_context->glyph_atlas_padding * 2;
		rect.h = glyph_height + font_context->glyph_atlas_padding * 2;
        
		stbrp_pack_rects(&font_context->context, &rect, 1);
        
		current_glyph_info->glyph_rect.x = rect.x + (float)font_context->glyph_atlas_padding;
		current_glyph_info->glyph_rect.y = rect.y + (float)font_context->glyph_atlas_padding;
		current_glyph_info->glyph_rect.width = (float)glyph_width;
		current_glyph_info->glyph_rect.height = (float)glyph_height;
        
		if (rect.was_packed)
		{
			for (int BitmapY = 0; BitmapY < glyph_height; BitmapY++)
			{
				for (int BitmapX = 0; BitmapX < glyph_width; BitmapX++)
				{
					int row_y = (rect.y + BitmapY) * font_context->glyph_atlas_image.width;
					int row_x = (rect.x + BitmapX);
					unsigned char* pixel = ((unsigned char*)font_context->glyph_atlas_image.data) + row_y + row_x;
					*pixel = ((unsigned char*)glyph_bitmap)[BitmapY * glyph_width + BitmapX];
				}
			}
		}
        
		stbtt_FreeSDF(glyph_bitmap, font_context->font_info.userdata);
	}
    
	return (*current_node_pointer)->info;
}

//TODO : i have no idea how the glyph should be place
internal float get_glyph_width(D_GlyphInfo glyph_info , D_FontContext *font_info , float space_size_offset , float space_scaler , float scale)
{
	float font_advance_x = ((float)glyph_info.advanceX * scale) / (float)font_info->glyph_resolution;
	float font_offset_x = ((float)glyph_info.offsetX * scale) / (float)font_info->glyph_resolution;
    
	//why it need to multiply 2?
	return (space_size_offset + (font_advance_x * 2 + font_offset_x )*space_scaler);
}

internal void get_string_width(int count,wchar_t * codepoint_pointer,D_FontContext *font_info, float space_size_offset,float space_scaler,float scale,int * result_count,float * result_width)
{
	float total_space = 0;
	int current_count = 0;
    
	while (1)
	{
		wchar_t current_codepoint = *codepoint_pointer;
        
		if (!current_codepoint)
			break;
        
		if (current_count >= count && count != -1)
			break;
#if true
		D_GlyphInfo current_glyph_info = D_get_glyph_from_codepoint(current_codepoint, font_info);;
		D_Rectangle glyph_rect = current_glyph_info.glyph_rect;
        
		if (current_glyph_info.value != current_codepoint)
			CATCH;        
        
		total_space += get_glyph_width(current_glyph_info ,font_info , space_size_offset ,space_scaler , scale) ;
        
		codepoint_pointer++;
		current_count++;
        
#endif 
	}
    
	if(result_width)
		*result_width = total_space;
    
	if(result_count)
		*result_count = current_count;
}

internal float get_total_string_width(const wchar_t* text,float scale)
{
    
	int stub_count = 0;
	float width = 0;
	get_string_width(-1, (wchar_t*)text, &app_data->default_font, 0, string_space_scaler, scale,&stub_count,&width);
    
	return width;
}

internal float get_total_string_width_B(char * text,float scale)
{
    int text_length = strlen(text);
    wchar_t * temp_text = (wchar_t *)allocate_frame(wchar_t , text_length + 2);
    
    for(int char_index = 0 ; char_index < text_length ; char_index++)
    {
        temp_text[char_index] = text[char_index];
    }
    
    temp_text[text_length] = '\0';
    
	return get_total_string_width((const wchar_t *)temp_text, scale);
}

//the sdf bitmap allocate its own memory and free everytime
//stop it?
internal void draw_text_with_font(D_FontContext* font_info, Rect rect , Color text_color, wchar_t* string, float scale, float space_size_offset, float space_scaler, bool centre , Vector3 up , Vector3 right)
{
    
	if (!string)
		return;
    
	int codepoints_count = 0;
    
	wchar_t* codepoint_pointer = (wchar_t*)string;
    
	float total_space = 0;
	get_string_width(-1, codepoint_pointer, font_info, space_size_offset, space_scaler, scale , 0 , &total_space);
    
	float centre_space = total_space * 0.5f;
	total_space = 0;
    
	codepoint_pointer = (wchar_t*)string;
    
	while (1)
	{
		wchar_t current_codepoint = *codepoint_pointer;
		codepoint_pointer++;
        
		if (!current_codepoint)
			break;
        
		int hash = current_codepoint % GLYPH_INFO_HASHMAP_LENGTH;
		D_GlyphInfoNode* previous_node = 0;
		D_GlyphInfoNode** current_node_pointer = &font_info->glyph_hash_map[hash];
        
		while ((*current_node_pointer))
		{
			previous_node = *current_node_pointer;
            
			if ((*current_node_pointer)->info.value != current_codepoint)
			{
				current_node_pointer = &(*current_node_pointer)->next_node;
			}
			else
			{
				break;
			}
		}
        
#if true
		D_GlyphInfo* current_glyph_info = &(*current_node_pointer)->info;
		D_Rectangle glyph_rect = current_glyph_info->glyph_rect;
        
		if (current_glyph_info->value != current_codepoint)
			CATCH;
        
		//There is over draw on the left and bottom
		//although it is transparent 
		//i wonder what it would be
        
		//it was right here
		//is the padding was wrong?
        
		D_Rectangle scr_coord =
		{
			glyph_rect.x - font_info->glyph_atlas_padding,
			glyph_rect.y - font_info->glyph_atlas_padding,
			glyph_rect.width + 2.0*font_info->glyph_atlas_padding,
			glyph_rect.height + 2.0*font_info->glyph_atlas_padding
		};
        
		float tex_coord_left = scr_coord.x / font_info->glyph_atlas_texture.width;
		float tex_coord_bottom = scr_coord.y / font_info->glyph_atlas_texture.height;
		float tex_coord_right = (scr_coord.x + scr_coord.width) / font_info->glyph_atlas_texture.width;
		float tex_coord_top = (scr_coord.y + scr_coord.height) / font_info->glyph_atlas_texture.height;
        
		int tex_coord_index = 0;
		Vector2 tex_coord[4] = {};
        
		tex_coord[tex_coord_index++] = (Vector2){ tex_coord_right, tex_coord_bottom };        
		tex_coord[tex_coord_index++] = (Vector2){ tex_coord_left, tex_coord_bottom };
		
		tex_coord[tex_coord_index++] = (Vector2){ tex_coord_right, tex_coord_top };
		tex_coord[tex_coord_index++] = (Vector2){ tex_coord_left, tex_coord_top };
        
		Vector2 text_size = {};
        
		text_size.x = ((float)(glyph_rect.width - font_info->glyph_atlas_padding) * scale)/ (float)font_info->glyph_resolution;
		text_size.y = ((float)(glyph_rect.height - font_info->glyph_atlas_padding) * scale)/ (float)font_info->glyph_resolution;
        
		Vector3 text_position = Vector3Scale(right , total_space);
        
		if (centre)
		{
			text_position = Vector3Subtract(text_position , Vector3Scale(right , centre_space));
		}
		
		float font_offset_x = ((float)current_glyph_info->offsetX * scale) / (float)font_info->glyph_resolution;
        
		text_position = Vector3Add(text_position , Vector3Scale( right , font_offset_x));
		text_position = Vector3Add(text_position , Vector3Scale( right , text_size.x));
        
		float font_offset_y = ((float)current_glyph_info->offsetY * scale) / (float)font_info->glyph_resolution;
        
		text_position = Vector3Subtract(text_position , Vector3Scale( up , font_offset_y));
		text_position = Vector3Subtract(text_position , Vector3Scale( up , text_size.y * 0.5f));
		text_position = Vector3Add(text_position , Vector3Scale( up , scale *0.5f));
        
		Rect text_rect = rect;
        
		text_position = Vector3RotateByQuaternion(text_position, text_rect.rotation);
		text_rect.position = Vector3Add(text_rect.position ,text_position);
        
		text_rect.size = text_size;
        
		Vector4 quad_color[4] = {};
        
		//what was i thinking?
		for (int vertex_index = 0; vertex_index < quad_vertex_count; vertex_index++)
		{
			quad_color[vertex_index].x = ((float)text_color.r /255.0);
			quad_color[vertex_index].y = ((float)text_color.g /255.0);
			quad_color[vertex_index].z = ((float)text_color.b /255.0);
			quad_color[vertex_index].w = ((float)text_color.a /255.0);
		}
        
		_draw_rect_text(text_rect ,quad_color, tex_coord , up , right);
		//DrawRectLine(TextQuad,Fade(WHITE,0.4));
        
		//the offset is broken now
		total_space += get_glyph_width(*current_glyph_info , font_info , space_size_offset , space_scaler , scale);
        
#endif 
	}
}

//TODO : i want size to be fix
internal void D_draw_text_screen(Vector3 position ,char * text , float size, Color text_color , bool centre)
{
	Rect rect = get_rect();
	rect.position = position;
    
	rect.size.y = size;
    
	int string_length = strlen(text);
	wchar_t * converted_string = allocate_frame(wchar_t , string_length + 1);
	for (int char_index = 0; char_index < string_length; char_index++) converted_string[char_index] = text [char_index];
    
	Vector3 right = {view_matrix.m0 , view_matrix.m4 , view_matrix.m8 };
	Vector3 up = { view_matrix.m1 , view_matrix.m5 , view_matrix.m9 };
    
	//this isn't correct at all
	//Up = Vector3Scale(Up , RemovePrespective(Up));
	//Right = Vector3Scale(Right, RemovePrespective(Right));
	
	draw_text_with_font(&app_data->default_font, rect , text_color, (wchar_t*)converted_string , size, 0 , string_space_scaler , centre , up , right);
}

internal void D_draw_text(Rect rect , const wchar_t* string, Color text_color, float scale, float space_size_offset, float space_scaler, bool centre)
{
	draw_text_with_font(&app_data->default_font, rect , text_color, (wchar_t*)string, scale, space_size_offset, space_scaler, centre , (Vector3){0,1,0} , (Vector3){1,0,0});
}

internal void D_draw_text_B(Rect rect, const wchar_t* string, Color text_color, bool centre)
{
	D_draw_text(rect,string,  text_color, rect.size.y , 0 , string_space_scaler, centre);
}

internal void D_draw_text_C(Vector2 position,const wchar_t* string, Color textColor, float size, bool centre)
{
    
	Rect rect= get_rect();
	rect.position.x = position.x;
	rect.position.y = position.y;
    
	rect.size.y = size;
    
	D_draw_text_B(rect, (wchar_t*)string, textColor, centre);
    
}

internal void D_draw_text_D(Vector3 position, const wchar_t* string, Color text_color, float size, bool centre)
{
	Rect rect = get_rect();
	rect.position = position;
    
	rect.size.y = size;
    
	D_draw_text_B(rect, (wchar_t*)string, text_color, centre);
}

internal void D_draw_text_E(Vector3 position , float size , char * string , Color text_color , bool centre)
{
	int string_length = strlen(string);
	wchar_t * convert_string = allocate_frame(wchar_t , string_length + 1);
	for (int char_index = 0; char_index < string_length; char_index++) convert_string[char_index] = string[char_index];
    
	D_draw_text_D(position , convert_string, text_color , size , centre);
}

internal void D_draw_text_F(Vector2 position , float size , char * string , Color text_color , bool centre)
{
	int string_length = strlen(string);
	wchar_t * convert_string = allocate_frame(wchar_t , string_length + 1);
	for (int char_index = 0; char_index < string_length; char_index++) convert_string[char_index] = string[char_index];
    
	D_draw_text_C(position , convert_string, text_color , size , centre);
}

//TODO : it is still terrible
global wchar_t * modifying_string = 0;
global float * modifying_float = 0;
global int * modifying_int = 0;

global float float_result = 0;
global bool float_value_get = false;

internal void simple_float_input(wchar_t * target_string , float * target_float)
{
    
	modifying_string = target_string;
    modifying_float = target_float;
    _swprintf(modifying_string , L"%.4f", (*modifying_float));
    
}

internal void get_float_input(float * target_float)
{
	if (float_value_get)
	{
        
		if (modifying_float == target_float)
		{
			(*target_float) = float_result;
			float_value_get = false;
            
			modifying_float = 0;
			modifying_string = 0;
            
		}
        
	}
}

//don't like the string ):
#define draw_menu_float_input(MenuPointer , FormatString , TargetFloatPointer) \
{ \
local_persist wchar_t _DummString[64] = {}; \
if (modifying_string != _DummString)\
{ \
_swprintf(_DummString , FormatString , *(TargetFloatPointer)); \
}\
\
if (draw_menu_button_W(MenuPointer , _DummString))\
{ \
simple_float_input(_DummString, TargetFloatPointer); \
}\
get_float_input(TargetFloatPointer); \
}

internal void simple_int_input(wchar_t * target_string , int * target_int)
{
    modifying_string = target_string;
    modifying_int = target_int;
    _swprintf(modifying_string , L"%d",*modifying_int);
}

//i should put a bracket around it
#define draw_menu_int_input(MenuPointer , FormatString , TargetIntPointer) \
{\
local_persist wchar_t _DummyString[64] = {}; \
if (modifying_string != _DummyString)\
{ \
_swprintf(_DummyString , FormatString , *(TargetIntPointer)); \
}\
\
if (draw_menu_button_W(MenuPointer , _DummyString))\
{ \
simple_int_input(_DummyString, TargetIntPointer); \
} \
}

internal void update_float_input()
{
    bool finish_typing = false;
    
    if(modifying_string)
    {
        int string_end = 0;
        
        for(string_end = 0 ; (modifying_string[string_end]) && (modifying_string[string_end] != L'<') ; string_end++);
        
        if(key_pressed(KEY_BACKSPACE))
        {
            modifying_string[string_end] = 0;
            string_end--;
        }
        
        modifying_string[string_end] = L'<';
        modifying_string[string_end+1] = 0;
        
        bool break_by_user = false;
        
        if(mouse_button_pressed(MOUSE_BUTTON_LEFT)) break_by_user = true;
        if(mouse_button_pressed(MOUSE_BUTTON_RIGHT)) break_by_user = true;
        if(key_pressed(KEY_ESCAPE)) break_by_user = true;
        if(key_pressed(KEY_ENTER)) break_by_user = true;
        
        if(break_by_user)
        {
            finish_typing = true;
            modifying_string[string_end] = 0;
        }
        
        for(int new_char = 0;new_char = char_pressed() ; )
        {
            modifying_string[string_end++] = new_char;
        }
    }
    
    if(finish_typing)
    {
        if(modifying_float)
        {
            swscanf(modifying_string,L"%f",modifying_float);
            
			float_value_get = true;
			float_result = (*modifying_float);
        }
        else if(modifying_int)
        {
            swscanf(modifying_string,L"%d",modifying_int);
            
            modifying_int =0;
            modifying_string = 0;
        }
        else
        {
            CATCH;
        }
    }
}