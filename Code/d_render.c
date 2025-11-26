//api is going nut 
//too many overload

//if data when create by CreateBufferForGPU
//there will be information (_ShaderBuffer) store right at the head 
//the information define how the data should be pass to GPU.
internal ShaderBuffer* get_GPU_buffer_info(void * data)
{
    return ((ShaderBuffer*)(data))-1;
}

//it will allocate data on both cpu ram and gpu ram.
internal void * create_buffer_for_GPU(const char * buffer_name , int primitive_type,int component_count , int max_size)
{
    
    ShaderBuffer* current_buffer = allocate_temp(ShaderBuffer,1);
    
    current_buffer->primitive_data_type = primitive_type;
    current_buffer->name = buffer_name;
    
    int type_size = 0;
    switch (current_buffer->primitive_data_type)
    {
        case GL_UNSIGNED_SHORT: type_size = sizeof(unsigned short); break;
        case GL_FLOAT: type_size = sizeof(float); break;
    }
    
    current_buffer->primitive_data_size = type_size;
    current_buffer->max_data_size =max_size; 
    current_buffer->components_count = component_count;
    
    all_GPU_buffers[shader_buffer_count++] = current_buffer;
    
    if(shader_buffer_count>=MAX_GPU_BUFFER) CATCH;
    
    int data_size = current_buffer->max_data_size * current_buffer->components_count * type_size;
    
    unsigned char * buffer_data =allocate_temp_(data_size); 
    current_buffer->data =buffer_data;
    current_buffer->buffer_size = data_size;
    
    return buffer_data;
}

//only use in RenderStateInit().
//if it was call somewhere else you need to pass buffer to GPU manually

//this will generate a buffer on CPU and its corresponding buffer on CPU
//it storing information(_ShaderBuffer) on data head (use Get_GPU_BufferInfo() to get it)
//if the shader created in RenderStateInit have exact same vertex data type and name
//the vertex buffer will bind to it automatically
//you can call Update_GPU_VertexBufferAll() to update all buffer automatically
internal void * create_GPU_vertex_buffer(const char * buffer_name , int primitive_type,int component_count , int buffer_size )
{
    void * result = create_buffer_for_GPU(buffer_name,primitive_type,component_count,buffer_size);
    
    ShaderBuffer * shader_buffer  = get_GPU_buffer_info(result);
    
    glGenBuffers(1, &shader_buffer->buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, shader_buffer->buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, shader_buffer->buffer_size, 0, GL_DYNAMIC_DRAW);
    
    return result;
}

//Indics Buffer will update automactically once GameDraw is call
internal void * create_GPU_indics_buffer(const char * buffer_name , int primitive_type,int component_count , int buffer_size )
{
    void * result = create_buffer_for_GPU(buffer_name,primitive_type,component_count,buffer_size);
    
    ShaderBuffer * shader_buffer  = get_GPU_buffer_info(result);
    
    shader_buffer->is_indices_buffer = true;
    glGenBuffers(1, &shader_buffer->buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shader_buffer->buffer_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, shader_buffer->buffer_size, 0, GL_DYNAMIC_DRAW);
    
    return result; 
}

//opengl uniform data ,vertex buffer and indics buffer creation
//after all data are created, ShaderInit() will try bind buffers to shaders 
internal void render_state_init()
{
#define VERTEX_BUFFER_MAX 8192
#define VERTEX_BUFFER_INDICS_MAX 8192
    
    render_state.screen_frame_buffer = rlLoadFramebuffer();
    
	render_state.shader_uniform_name[SU_MVP] = "mvp";
	render_state.shader_uniform_type[SU_MVP] = RL_SHADER_UNIFORM_MATRIX;
    
	render_state.shader_uniform_name[SU_texture_0] = "texture0";
	render_state.shader_uniform_type[SU_texture_0] = RL_SHADER_UNIFORM_SAMPLER2D;
    
	render_state.shader_uniform_name[SU_screen_size] = "screenSize";
	render_state.shader_uniform_type[SU_screen_size] = RL_SHADER_UNIFORM_VEC2;
	
	render_state.shader_uniform_name[SU_grid_size] = "gridSize";
	render_state.shader_uniform_type[SU_grid_size] = RL_SHADER_UNIFORM_FLOAT;
    
	render_state.shader_uniform_name[SU_spread] = "spread";
	render_state.shader_uniform_type[SU_spread] = RL_SHADER_UNIFORM_FLOAT;
    
	render_state.shader_uniform_name[SU_elapsed_time] = "elapsedTime";
	render_state.shader_uniform_type[SU_elapsed_time] = RL_SHADER_UNIFORM_FLOAT;
    
    render_state.shader_uniform_name[SU_camera_position] = "cameraPosition";
    render_state.shader_uniform_type[SU_camera_position] = RL_SHADER_UNIFORM_VEC3;
    
    render_state.shader_uniform_name[SU_camera_direction] = "cameraDirection";
    render_state.shader_uniform_type[SU_camera_direction] = RL_SHADER_UNIFORM_VEC3;
    
    render_state.shader_uniform_name[SU_draw_flag] = "drawFlag";
    render_state.shader_uniform_type[SU_draw_flag] = RL_SHADER_UNIFORM_INT;
    
	//i don't like it
	//TODO: remove this
    for (int UniformIndex = 0; UniformIndex < SU_count; UniformIndex++)
	{
		unsigned char** uniform_data = render_state.shader_uniform_data + UniformIndex;
        
		switch (render_state.shader_uniform_type[UniformIndex])
		{
            case RL_SHADER_UNIFORM_MATRIX: *uniform_data = allocate_temp_(sizeof(Matrix)); break;
            case RL_SHADER_UNIFORM_SAMPLER2D: *uniform_data = allocate_temp_(sizeof(unsigned int)); break;
            case RL_SHADER_UNIFORM_INT: *uniform_data = allocate_temp_(sizeof(int)); break;
			case RL_SHADER_UNIFORM_VEC2: *uniform_data = allocate_temp_(sizeof(Vector2)); break;
			case RL_SHADER_UNIFORM_FLOAT: *uniform_data = allocate_temp_(sizeof(float)); break;
            case RL_SHADER_UNIFORM_VEC3: *uniform_data = allocate_temp_(sizeof(Vector3)); break;
            
			default: CATCH; break;
		}
	}
    
	//does using SOA better than AOS?
	//i can declare less things with AOS
    
	//is reusing buffer harmful?
	//i'm reusing buffer for all the shader for no reason
    
    //feel like nothing wrong can happen
    
    light_data.light_position = (Vector3*)create_GPU_vertex_buffer("lightPosition",GL_FLOAT , 3 , MAX_LIGHT);
    light_data.light_color = (Vector4*)create_GPU_vertex_buffer("lightColor",GL_FLOAT , 4 , MAX_LIGHT);
    light_data.light_size = (Vector2*)create_GPU_vertex_buffer("lightSize",GL_FLOAT , 2 , MAX_LIGHT);
    light_data.light_mul = (float*)create_GPU_vertex_buffer("lightMul",GL_FLOAT , 1 , MAX_LIGHT);
    
    //i don't know why they are revert
    
	//actually , reusing name is kinda bad
	//i can't belive it is working while doing thing wrong
    light_data.light_tex_coord[vertex_bottom_right] = (Vector2*)create_GPU_vertex_buffer("lightTexCoord_topLeft",GL_FLOAT , 2 , MAX_LIGHT);
    light_data.light_tex_coord[vertex_top_right] = (Vector2*)create_GPU_vertex_buffer("lightTexCoord_topRight",GL_FLOAT , 2 , MAX_LIGHT);
    light_data.light_tex_coord[vertex_bottom_left] = (Vector2*)create_GPU_vertex_buffer("lightTexCoord_bottomLeft",GL_FLOAT , 2 , MAX_LIGHT);
    light_data.light_tex_coord[vertex_top_left] = (Vector2*)create_GPU_vertex_buffer("lightTexCoord_bottomRight",GL_FLOAT , 2 , MAX_LIGHT);
	
    vertex_data.indices= (unsigned short *)create_GPU_indics_buffer("indics and don't touch it",GL_UNSIGNED_SHORT,1,VERTEX_BUFFER_INDICS_MAX);
    
    vertex_data.position = (Vector3*)create_GPU_vertex_buffer("vertexPosition",GL_FLOAT,3,VERTEX_BUFFER_MAX);
	vertex_data.rotation = (Vector4 * )create_GPU_vertex_buffer("rotation",GL_FLOAT,4,VERTEX_BUFFER_MAX);
    vertex_data.color = (Vector4*)create_GPU_vertex_buffer("vertexColor",GL_FLOAT,4,VERTEX_BUFFER_MAX);
    vertex_data.tex_coord= (Vector2*)create_GPU_vertex_buffer("vertexTexCoord",GL_FLOAT,2,VERTEX_BUFFER_MAX);
    vertex_data.normal = (Vector3*)create_GPU_vertex_buffer("vertexNormal",GL_FLOAT,3,VERTEX_BUFFER_MAX);
	
    vertex_data.line_start = (Vector3*)create_GPU_vertex_buffer("lineStart",GL_FLOAT,3,VERTEX_BUFFER_MAX);
    vertex_data.line_end = (Vector3*)create_GPU_vertex_buffer("lineEnd",GL_FLOAT,3,VERTEX_BUFFER_MAX);
	vertex_data.line_size = (float*)create_GPU_vertex_buffer("lineSize",GL_FLOAT,1,VERTEX_BUFFER_MAX);
	vertex_data.fake_depth = (float*)create_GPU_vertex_buffer("fakeDepth",GL_FLOAT,1,VERTEX_BUFFER_MAX);
    
	vertex_data.size = (Vector2 *)create_GPU_vertex_buffer("rectSize", GL_FLOAT, 2, VERTEX_BUFFER_MAX);
	vertex_data.corner_radius = (Vector4*)create_GPU_vertex_buffer("rectRadius", GL_FLOAT, 4, VERTEX_BUFFER_MAX);
    
    vertex_data.corner_color[vertex_bottom_right] = (Vector4*)create_GPU_vertex_buffer("vertexColor_topLeft",GL_FLOAT , 4 , VERTEX_BUFFER_MAX);
	vertex_data.corner_color[vertex_top_right] = (Vector4*)create_GPU_vertex_buffer("vertexColor_topRight",GL_FLOAT , 4 , VERTEX_BUFFER_MAX);
    vertex_data.corner_color[vertex_bottom_left] = (Vector4*)create_GPU_vertex_buffer("vertexColor_bottomLeft",GL_FLOAT , 4 , VERTEX_BUFFER_MAX);
    vertex_data.corner_color[vertex_top_left] = (Vector4*)create_GPU_vertex_buffer("vertexColor_bottomRight",GL_FLOAT , 4 , VERTEX_BUFFER_MAX);
    
    vertex_data.corner_tex_coord[vertex_bottom_right] = (Vector2*)create_GPU_vertex_buffer("vertexTexCoord_topLeft",GL_FLOAT , 2 , VERTEX_BUFFER_MAX);
    vertex_data.corner_tex_coord[vertex_top_right] = (Vector2*)create_GPU_vertex_buffer("vertexTexCoord_topRight",GL_FLOAT , 2 , VERTEX_BUFFER_MAX);
    vertex_data.corner_tex_coord[vertex_bottom_left] = (Vector2*)create_GPU_vertex_buffer("vertexTexCoord_bottomLeft",GL_FLOAT , 2 , VERTEX_BUFFER_MAX);
    vertex_data.corner_tex_coord[vertex_top_left] = (Vector2*)create_GPU_vertex_buffer("vertexTexCoord_bottomRight",GL_FLOAT , 2 , VERTEX_BUFFER_MAX);
    
    render_state.texture_list_to_delete = allocate_list(16);
}

internal unsigned int D_load_texture(void * data , int width, int height, int format, int mipmapCount)
{
    unsigned int new_texture_id = rlLoadTexture(data , width, height, format, mipmapCount);
    add_to_list_tail( new_texture_id , &render_state.texture_list_to_delete);
    return new_texture_id;
}

internal unsigned int D_load_texture_from_image(Image image)
{
    return D_load_texture(image.data, image.width, image.height, image.format, image.mipmaps);
}

internal void update_texture(unsigned int * id)
{
    
    if ((*id) != 0)
    {
        rlUnloadTexture((*id) );
    }
    
    (*id) = rlLoadTexture(0, app_data->window_size.x, app_data->window_size.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
}

internal void update_texture_multi_sample(unsigned int * id)
{
    
    if ((*id) != 0)
    {
        rlUnloadTexture((*id) );
    }
    
    (*id) = load_texture_multi_sample(app_data->window_size.x, app_data->window_size.y);
}

//it will compile all the shader and try bind buffer to it
//shaders are sort by using enum (S_Quad mean quad shader)
internal void shader_init()
{
    shader_compile_failed = false;
    
	GL_CATCH;
    
	app_data->default_font.glyph_atlas_texture = LoadTextureFromImage(app_data->default_font.glyph_atlas_image);
    
	GL_CATCH;
    
	//TODO : too long
	all_shader_inputs[S_point_light].shader_source_file_name[GS_vert] = "..\\Code\\Shader\\point_light_vert.glsl";
	all_shader_inputs[S_point_light].shader_source_file_name[GS_geo] = "..\\Code\\Shader\\point_light_geo.glsl";
	all_shader_inputs[S_point_light].shader_source_file_name[GS_frag] = "..\\Code\\Shader\\point_light_frag.glsl";
    
	all_shader_inputs[S_rect].shader_source_file_name[GS_vert] = "..\\Code\\Shader\\rect_vert.glsl";
	all_shader_inputs[S_rect].shader_source_file_name[GS_geo] = "..\\Code\\Shader\\rect_geo.glsl";
	all_shader_inputs[S_rect].shader_source_file_name[GS_frag] = "..\\Code\\Shader\\rect_frag.glsl";
    
	all_shader_inputs[S_quad].shader_source_file_name[GS_vert] = "..\\Code\\Shader\\quad_vert.glsl";
	all_shader_inputs[S_quad].shader_source_file_name[GS_frag] = "..\\Code\\Shader\\quad_frag.glsl";
	
	all_shader_inputs[S_quad_MS].shader_source_file_name[GS_vert] = "..\\Code\\Shader\\quad_vert.glsl";
	all_shader_inputs[S_quad_MS].shader_source_file_name[GS_frag] = "..\\Code\\Shader\\quad_MS_frag.glsl";
    
	all_shader_inputs[S_text].shader_source_file_name[GS_vert] = "..\\Code\\Shader\\text_vert.glsl";
	all_shader_inputs[S_text].shader_source_file_name[GS_frag] = "..\\Code\\Shader\\text_frag.glsl";
    
	all_shader_inputs[S_line].shader_source_file_name[GS_vert] = "..\\Code\\Shader\\line_vert.glsl";
	all_shader_inputs[S_line].shader_source_file_name[GS_frag] = "..\\Code\\Shader\\line_frag.glsl";
    
	all_shader_inputs[S_bloom].shader_source_file_name[GS_frag] = "..\\Code\\Shader\\bloom_frag.glsl";    
	all_shader_inputs[S_bloom].shader_source_file_name[GS_vert] = "..\\Code\\Shader\\bloom_vert.glsl";
    
	all_shader_inputs[S_blur_H].shader_source_file_name[GS_frag] = "..\\Code\\Shader\\blur_horizontal_frag.glsl";
	all_shader_inputs[S_blur_H].shader_source_file_name[GS_vert] = "..\\Code\\Shader\\blur_vert.glsl";
    
	all_shader_inputs[S_blur_V].shader_source_file_name[GS_frag] = "..\\Code\\Shader\\blur_vertical_frag.glsl";    
	all_shader_inputs[S_blur_V].shader_source_file_name[GS_vert] = "..\\Code\\Shader\\blur_vert.glsl";
    
	all_shader_inputs[S_spring].shader_source_file_name[GS_vert] = "..\\Code\\Shader\\quad_vert.glsl";
	all_shader_inputs[S_spring].shader_source_file_name[GS_frag] = "..\\Code\\Shader\\spring_frag.glsl";
    
	int shader_type_to_GL_shader_enum[GS_count] = {};
	shader_type_to_GL_shader_enum[GS_vert] = GL_VERTEX_SHADER;
	shader_type_to_GL_shader_enum[GS_geo] = GL_GEOMETRY_SHADER;
	shader_type_to_GL_shader_enum[GS_frag] = GL_FRAGMENT_SHADER;
    
	for (int shader_index = 0; shader_index < S_count; shader_index++)
	{
		ShaderInput* current_shader_property = all_shader_inputs + shader_index;
        current_shader_property->shader_type = (D_ShaderType)shader_index;
		unsigned int all_shader_type[GS_count] = {};
        
		for (int shader_type_index = 0; shader_type_index < GS_count; shader_type_index++)
		{
			if (!current_shader_property->shader_source_file_name[shader_type_index]) continue;
			
			char shader_path[256] = {};
			strcpy(shader_path, get_app_file_path(current_shader_property->shader_source_file_name[shader_type_index]));
            
			int shader_data_size = 0;
			const char * shader_code = LoadFileText(shader_path);
            
			int GL_shader_type = shader_type_to_GL_shader_enum[shader_type_index];
            
			all_shader_type[shader_type_index] = glCreateShader(GL_shader_type);
			glShaderSource(all_shader_type[shader_type_index], 1, &shader_code, NULL);
            
			GLint success = 0;
			glCompileShader(all_shader_type[shader_type_index]);
			glGetShaderiv(all_shader_type[shader_type_index], GL_COMPILE_STATUS, &success);
            
			if (success == GL_FALSE)
			{
				switch (GL_shader_type)
				{
                    case GL_VERTEX_SHADER: printf("SHADER: [ID %i] Failed to compile vertex shader code\n", all_shader_type[shader_type_index]); break;
                    case GL_FRAGMENT_SHADER: printf("SHADER: [ID %i] Failed to compile fragment shader code\n", all_shader_type[shader_type_index]); break;
                    case GL_GEOMETRY_SHADER: printf("SHADER: [ID %i] Failed to compile geometry shader code\n", all_shader_type[shader_type_index]); break;
                    
                    default: break;
				}
                
				int max_length = 0;
				glGetShaderiv(all_shader_type[shader_type_index], GL_INFO_LOG_LENGTH, &max_length);
                
				if (max_length > 0)
				{
					int length = 0;
					char* log = (char*)RL_CALLOC(max_length, sizeof(char));
					glGetShaderInfoLog(all_shader_type[shader_type_index], max_length, &length, log);
                    printf("\nShader Compile Fail\n");
					printf("SHADER: [ID %i] Compile error: %s", all_shader_type[shader_type_index], log);
					RL_FREE(log);
                    
					shader_compile_failed = true;
				}
			}
			else
			{
				switch (GL_shader_type)
				{
                    case GL_VERTEX_SHADER: printf("SHADER: [ID %i] Vertex shader compiled successfully\n", all_shader_type[shader_type_index]); break;
                    case GL_FRAGMENT_SHADER: printf("SHADER: [ID %i] Fragment shader compiled successfully\n", all_shader_type[shader_type_index]); break;
                    case GL_GEOMETRY_SHADER:printf("SHADER: [ID %i] Geomerty shader compiled successfully\n", all_shader_type[shader_type_index]); break;
                    
                    default: break;
				}
			}
		}
        
		unsigned int program = 0;
		GLint success = 0;
		program = glCreateProgram();
        
		for (int shader_type_index=0; shader_type_index < GS_count; shader_type_index++)
		{
			if (!all_shader_type[shader_type_index]) continue;
            
			glAttachShader(program, all_shader_type[shader_type_index]);
		}
        
		glLinkProgram(program);
        
		glGetProgramiv(program, GL_LINK_STATUS, &success);
        
		if (success == GL_FALSE)
		{
			printf("SHADER: [ID %i] Failed to link shader program\n", program);
            
			int max_length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);
            
			if (max_length > 0)
			{
				int length = 0;
				char* log = (char*)RL_CALLOC(max_length, sizeof(char));
				glGetProgramInfoLog(program, max_length, &length, log);
				
                printf("\nShader Link Fail\n");
                
                printf("SHADER: [ID %i] Link error: %s\n", program, log);
				RL_FREE(log);
			}
            
            shader_compile_failed =  true;
		}
		else
		{
			// Get the size of compiled shader program (not available on OpenGL ES 2.0)
			// NOTE: If GL_LINK_STATUS is GL_FALSE, program binary length is zero.
			//GLint binarySize = 0;
			//glGetProgramiv(id, GL_PROGRAM_BINARY_LENGTH, &binarySize);
            
			printf("SHADER: [ID %i] Program shader loaded successfully\n", program);
		}
        
        if(shader_compile_failed)
        {
            break;
        }
        
		current_shader_property->shader = program;
		unsigned int current_shader = current_shader_property->shader;
        
		for (int uniform_index = 0; uniform_index < SU_count; uniform_index++)
		{
			const char* uniform_name = render_state.shader_uniform_name[uniform_index];
			current_shader_property->uniform_location[uniform_index] = glGetUniformLocation(current_shader, uniform_name);
			GL_CATCH;
		}
        
        current_shader_property->vertex_buffer_location_array = allocate_temp(int,shader_buffer_count);
        
		for (int buffer_index = 0; buffer_index < shader_buffer_count; buffer_index++)
		{
			int location_index = -1;
			ShaderBuffer* current_buffer = all_GPU_buffers[buffer_index];
            
			if (current_buffer->is_uniform_buffer)
			{
				//i have no idea what you are
				int vertices_block_index = glGetUniformBlockIndex(current_shader, "VerticesBlock");
                
				if (vertices_block_index != -1)
				{
					glUniformBlockBinding(current_shader, vertices_block_index, 0);
					glBindBufferBase(GL_UNIFORM_BUFFER, vertices_block_index, current_buffer->buffer_handle);
				}
			}
			else
			{
				if (current_buffer->name)
				{
					location_index = glGetAttribLocation(current_shader, current_buffer->name);
				}
			}
            
			current_shader_property->vertex_buffer_location_array[buffer_index] = location_index;
		}
        
		glGenVertexArrays(1, &current_shader_property->vertex_array_object);
		glBindVertexArray(current_shader_property->vertex_array_object);
        
		for (int buffer_index = 0; buffer_index < shader_buffer_count; buffer_index++)
		{
			int buffer_location_index = current_shader_property->vertex_buffer_location_array[buffer_index];
			
			if (buffer_location_index == -1) continue;
            
			ShaderBuffer * current_buffer = all_GPU_buffers[buffer_index];
            
			if (current_buffer->is_uniform_buffer) continue;
            if(current_buffer->is_indices_buffer) continue;
            
			glBindBuffer(GL_ARRAY_BUFFER, current_buffer->buffer_handle);
			glEnableVertexAttribArray(buffer_location_index);
            
			glVertexAttribPointer(buffer_location_index, current_buffer->components_count, current_buffer->primitive_data_type, GL_FALSE, 0, 0);
		}
        
		GL_CATCH;
	}
    
	rlDisableVertexArray();
	rlDisableVertexBuffer();
	rlDisableVertexBufferElement();
}

//it will pass all buffer that create by Create_GPU_VertexBuffer to GPU implicitly 
internal void update_all_GPU_vertex_buffer(ShaderInput * currentShader,int count)
{
	for (int buffer_index = 0; buffer_index < shader_buffer_count; buffer_index++)
	{
        ShaderBuffer* current_buffer = all_GPU_buffers[buffer_index];
        
		if (currentShader->vertex_buffer_location_array[buffer_index] == -1) 
        {
            if(current_buffer->is_uniform_buffer) 
            {
				//not supported
                
				CATCH;
                bool update_buffer = false;
                if(currentShader->shader_type == S_rect) update_buffer = true;
                
                if(!update_buffer) continue;
            }
            else
            {
                continue;
            }
            
        }
		
        if(current_buffer->is_indices_buffer) continue;
        
        int component_count = current_buffer->components_count;
        int data_count = count;
        int primtive_size = current_buffer->primitive_data_size;
        
        int size_to_update = component_count * data_count * primtive_size;
        
        GL_CATCH;
        
        glBindBuffer(GL_ARRAY_BUFFER, current_buffer->buffer_handle);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size_to_update, current_buffer->data);
        
        GL_CATCH;
	}
}

//a stateful draw call
//it will clear shader data count (but data is still there)
//it's behaviour is dedicated by current shader
//so some part is share among shader (Update_GPU_VertexBufferAll())
//but some weren't (for example S_Text use its own glyph texture)

//You should minimize calls to GameDraw
//for example in _D_DrawRect, it will check is the current shader or Texture still the same
//If not, it usually means it was drawing text ,a line or using different texture before.
//then it will call GameDraw() to draw previous accumulated data and reset data count to 0.(only count , the data still there)

internal void D_game_draw()
{
	
    //rlEnableFramebuffer(RenderState.AfterGameDrawFrameTexture.id);
    
	GL_CATCH;
    
	ShaderInput* current_shader_input = 0;
    
	current_shader_input = all_shader_inputs + render_state.current_shader_input;
	rlEnableShader(current_shader_input->shader);
	glBindVertexArray(current_shader_input->vertex_array_object);
    
	if (render_state.current_shader_input == S_line)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);  
		glDepthFunc(GL_NOTEQUAL);
	}
	else if(render_state.current_shader_input == S_quad)
    {
        
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glDisable(GL_CULL_FACE);
        
    }
    else
	{
		
        glDisable(GL_DEPTH_TEST);
        
	}
    
    //TODO: remove these
    //uniform should be pass in manually
	for (int uniform_index = 0; uniform_index < SU_count; uniform_index++)
	{
		unsigned char* uniform_data = render_state.shader_uniform_data[uniform_index];
        
		if (!uniform_data)
		{
			CATCH;
		}
        
		switch (render_state.shader_uniform_type[uniform_index])
		{
            case RL_SHADER_UNIFORM_MATRIX: rlSetUniformMatrix(current_shader_input->uniform_location[uniform_index], *(Matrix*)uniform_data); break;
		}
	}
    
	GL_CATCH;
    
	glActiveTexture(GL_TEXTURE0);
    
	if (render_state.current_shader_input == S_text)
	{
		if (app_data->update_atlas)
		{
			if (app_data->default_font.glyph_atlas_texture.id == 0)
			{
				app_data->default_font.glyph_atlas_texture = LoadTextureFromImage(app_data->default_font.glyph_atlas_image);
			}
			else
			{
				//maybe you don't wanna commit the whole thing?
				UpdateTexture(app_data->default_font.glyph_atlas_texture, app_data->default_font.glyph_atlas_image.data);
			}
		}
		app_data->update_atlas = false;
        
		glBindTexture(GL_TEXTURE_2D, app_data->default_font.glyph_atlas_texture.id);
	}
	else
	{
        if(render_state.current_texture)
        {
            glBindTexture(GL_TEXTURE_2D, render_state.current_texture);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, render_state.missing_texture);
        }
    }
    
    Vector3 camera_position = game_camera.position;
    Vector3 camera_direction = Vector3Subtract(game_camera.target , camera_position);
    camera_direction = Vector3Normalize(camera_direction);
    
	glUniform1i(current_shader_input->uniform_location[SU_texture_0], 0);    
	glUniform1f(current_shader_input->uniform_location[SU_elapsed_time] , render_state.elapsed_time);    	
	glUniform2f(current_shader_input->uniform_location[SU_screen_size] , app_data->window_size.x , app_data->window_size.y);
    glUniform3f(current_shader_input->uniform_location[SU_camera_position] , camera_position.x , camera_position.y , camera_position.z);
    glUniform3f(current_shader_input->uniform_location[SU_camera_direction] , camera_direction.x , camera_direction.y , camera_direction.z);
    glUniform1i(current_shader_input->uniform_location[SU_draw_flag] , render_state.draw_flag);
    
	GL_CATCH;
    
    update_all_GPU_vertex_buffer( current_shader_input , current_shader_input->vertices_count );
    
	//i don't need to use index am i ? ):
	//yea....
    
	//does my rect need it?
	//no i think
    
	//why im using it at the first place???
    
	//rect shader need it because the shader was written in 4 vertics not 6
	//and overall it reduce size (4 bytes smaller if i wasn't mistaken?)
    
	switch (render_state.current_shader_input)
	{
        case S_point_light: break;
		case S_rect: glDrawArrays(GL_POINTS, 0, current_shader_input->vertices_count); break;
        default:
        {
            ShaderBuffer* indices_buffer = get_GPU_buffer_info(vertex_data.indices);
            GL_CATCH;
            
            rlUpdateVertexBufferElements(indices_buffer->buffer_handle, indices_buffer->data, current_shader_input->indices_count * sizeof(unsigned short), 0);
            
            GL_CATCH;
            glDrawElements(GL_TRIANGLES, current_shader_input->indices_count, GL_UNSIGNED_SHORT, 0);
            GL_CATCH;
            break;
        }
	}
	
	rlDisableVertexArray();
	rlDisableVertexBuffer();
	rlDisableVertexBufferElement();
    
    current_shader_input->indices_count = 0;
    current_shader_input->vertices_count = 0;
    
	GL_CATCH;
}

internal unsigned int load_texture_multi_sample(int width, int height)
{
    
    GLuint tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,4,GL_RGBA8, width, height,GL_FALSE);
    
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE , 0);
    
    GL_CATCH;
    
    return tex;
}

internal unsigned int load_depth_texture_multi_sample(int width , int height)
{
    
    unsigned int id= 0;
	glGenTextures(1, &id);
	GL_CATCH;
    
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
	
    GL_CATCH;
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,4,GL_DEPTH_COMPONENT24, width, height,GL_FALSE);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
    //glTexStorage2DMultisample(, 1, GL_DEPTH_COMPONENT16 , width, height, GL_TRUE);
    GL_CATCH;
    
    GL_CATCH;
    
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE , 0);
    
    GL_CATCH;
    
    return id;
}

internal Quad rect_to_quad(Rect rect)
{
	Quad result = {};
	
	result.vertex_position[vertex_top_left].x -= rect.size.x * 0.5f;
	result.vertex_position[vertex_top_left].y += rect.size.y * 0.5f;
    
	result.vertex_position[vertex_top_right].x += rect.size.x * 0.5f;
	result.vertex_position[vertex_top_right].y += rect.size.y * 0.5f;
    
	result.vertex_position[vertex_bottom_left].x -= rect.size.x * 0.5f;
	result.vertex_position[vertex_bottom_left].y -= rect.size.y * 0.5f;
    
	result.vertex_position[vertex_bottom_right].x += rect.size.x * 0.5f;
	result.vertex_position[vertex_bottom_right].y -= rect.size.y * 0.5f;
    
	for (int vertex_index = 0; vertex_index < quad_vertex_count; vertex_index++)
	{
		result.vertex_position[vertex_index] = Vector3RotateByQuaternion(result.vertex_position[vertex_index] , rect.rotation);
        
		result.vertex_position[vertex_index].x += rect.position.x;
		result.vertex_position[vertex_index].y += rect.position.y;
		result.vertex_position[vertex_index].z += rect.position.z;
		
	}
    
	return result;
}

internal Quad rect_to_rhombus(Rect rect)
{
	Quad rhombus = {};
    
	rhombus.vertex_position[vertex_top_left].y += rect.size.y * 0.5f;
	rhombus.vertex_position[vertex_top_right].x += rect.size.x * 0.5f;
	
	rhombus.vertex_position[vertex_bottom_left].x -= rect.size.x * 0.5f;
	rhombus.vertex_position[vertex_bottom_right].y -= rect.size.y * 0.5f;
    
	for (int vertexIndex = 0; vertexIndex < quad_vertex_count; vertexIndex++)
	{
		rhombus.vertex_position[vertexIndex] = Vector3RotateByQuaternion(rhombus.vertex_position[vertexIndex] , rect.rotation);
        
		rhombus.vertex_position[vertexIndex].x += rect.position.x;
		rhombus.vertex_position[vertexIndex].y += rect.position.y;
	}
    
	return rhombus;
}

internal void quad_draw_check(int target_texture)
{
	bool clean_up_draw_state = false;
    
	if (render_state.current_shader_input != S_quad)
	{
		clean_up_draw_state = true;
	}
	
	if (render_state.current_texture!= target_texture)
	{
		clean_up_draw_state = true;
	}
    
    if(render_state.current_shader_input == S_quad)
    {
        //just check if it was the will too close
        if(all_shader_inputs[S_quad].vertices_count + 10 >=VERTEX_BUFFER_MAX)
        {
            clean_up_draw_state = true;
        }
        
        if(all_shader_inputs[S_quad].indices_count + 10 >=VERTEX_BUFFER_INDICS_MAX)
        {
            clean_up_draw_state  = true;
        }
    }
    
	if (clean_up_draw_state)
	{
		D_game_draw();
	}    
    
}

internal void draw_model(D_Model * model , Bone * all_bone , Bone * all_initial_bone)
{
    
    int target_texture = render_state.default_white_image;
	D_game_draw();
    
	render_state.current_shader_input= S_quad;
	render_state.current_texture = target_texture;
    
	ShaderInput* shader_input = all_shader_inputs + S_quad;	
    
    start_color_seed = 121623;
    
    for(int array_index = 0 ; array_index < model->index_count ; array_index++)
    {
        
        int vertex_index = model->indices[array_index];
        
        Vector3 normal = model->normals[array_index];
        Vector3 position = model->vertices[vertex_index];
        
        DeformVertexSlice * deform_vertex_slice = model->deform_vertex_slice + vertex_index;
        
        Vector4 vertex_color = {};
        vertex_color.w = 1.0f;
        vertex_color.z = 1.0f;
        
        Vector3 total_position = {};
        
        float total_weight = 0;
        
        for(int slice_index = 0 ; slice_index < deform_vertex_slice->count ; slice_index++ )
        {
            
            int deform_vertex_index =  deform_vertex_slice->start + slice_index;
            DeformVertex deform_vertex = model->all_deform_vertex[deform_vertex_index];
            
            total_weight += deform_vertex.weight;
        }
        
        if(total_weight > 0.0001f)
        {
            
            for(int slice_index = 0 ; slice_index < deform_vertex_slice->count ; slice_index++ )
            {
                
                int deform_vertex_index =  deform_vertex_slice->start + slice_index;
                DeformVertex deform_vertex = model->all_deform_vertex[deform_vertex_index];
                
                Bone * deform_bone = all_bone + deform_vertex.bone_index;
                Vector3 bone_position = deform_bone->position;
                
                Bone * initial_bone = all_initial_bone + deform_vertex.bone_index;
                
                Vector3 position_relative_to_bone = Vector3Subtract( position , initial_bone->position);
                Vector3 rotated_position = Vector3RotateByQuaternion(position_relative_to_bone , deform_bone->rotation);
                Vector3 rotation_offset = Vector3Subtract(rotated_position , position_relative_to_bone);
                
                Vector3 bone_offset = Vector3Subtract( deform_bone->position , initial_bone->position);
                Vector3 total_offset = Vector3Add(rotation_offset , bone_offset);
                
                total_offset = Vector3Scale(total_offset , deform_vertex.weight / total_weight);
                
                total_position = Vector3Add(total_position , total_offset);
                
                Vector3 deformed_normal = Vector3RotateByQuaternion( normal , deform_bone->rotation);
                normal = Vector3Lerp(normal , deformed_normal , deform_vertex.weight);
                
                if(editor->selected_bone_count == 1)
                {
                    BoneSelection* selected_bone = editor->selected_bone_stack;
                    if(selected_bone->bone_index == deform_bone->bone_index)
                    {
                        Vector3 red_color = {1.0,0,0};
                        Vector3 green_color = {0,1.0,0};
                        Vector3 blue_color = {0,0,1.0};
                        
                        Vector3 weight_color = {};
                        
                        if(deform_vertex.weight < 0.5f)
                        {
                            float blue_to_green = deform_vertex.weight / 0.5f;
                            weight_color = Vector3Lerp( blue_color , green_color , blue_to_green);
                        }
                        else
                        {
                            float green_to_red = (deform_vertex.weight - 0.5f ) / 0.5f;
                            weight_color = Vector3Lerp( green_color , red_color , green_to_red);
                        }
                        
                        weight_color = Vector3Scale(weight_color , 1.5);
                        
                        vertex_color.x = weight_color.x;
                        vertex_color.y = weight_color.y;
                        vertex_color.z = weight_color.z;
                    }
                }
                
            }
            
        }
        
        position = Vector3Add(position , total_position);
        
        int data_index = shader_input->vertices_count + array_index;
        int data_index_index = shader_input->indices_count + array_index;
        
        vertex_data.position[data_index] = position;
        vertex_data.tex_coord[data_index] = (Vector2){0,0};
        vertex_data.normal[data_index] = normal;
        vertex_data.color[data_index] = vertex_color;
        vertex_data.indices[data_index_index] = data_index;
        
    }
    
    shader_input->vertices_count += model->index_count;
    shader_input->indices_count += model->index_count;
    
    render_state.draw_flag = DF_contour;
    D_game_draw();
    render_state.draw_flag = DF_none;
    
}

internal void draw_triangle(Vector3* all_vertices ,Vector4 vertex_color)
{
    
	int target_texture = render_state.default_white_image;
    
	quad_draw_check(target_texture);
    
	render_state.current_shader_input= S_quad;
	render_state.current_texture = target_texture;
    
	ShaderInput* shader_input = all_shader_inputs + S_quad;
    
	unsigned short vertex_index[3] = {};
	
	int indics_count = shader_input->indices_count;
	shader_input->indices_count += 3;
    
	int vertex_count = shader_input->vertices_count;    
	shader_input->vertices_count+=3;
    
	Vector2 texture_coord[3] = {};
    
    texture_coord[vertex_bottom_left] = (Vector2){ 0,1 };
    texture_coord[vertex_top_left] = (Vector2){ 1,1 };
    texture_coord[vertex_top_right] = (Vector2){ 1,0 };
    
	//i'm so confused with the vertics order     
	vertex_index[0] = vertex_count;
	vertex_data.position[vertex_count] = all_vertices[0];
	vertex_data.tex_coord[vertex_count] = texture_coord[0];
	vertex_data.color[vertex_count] = vertex_color;
	vertex_count++;
    
	vertex_index[1] = vertex_count;
	vertex_data.position[vertex_count] = all_vertices[1];
	vertex_data.tex_coord[vertex_count] = texture_coord[2];
	vertex_data.color[vertex_count] = vertex_color;
	vertex_count++;
    
	vertex_index[2] = vertex_count;
	vertex_data.position[vertex_count] = all_vertices[2];
	vertex_data.tex_coord[vertex_count] = texture_coord[1];
	vertex_data.color[vertex_count] = vertex_color;
	vertex_count++;
    
	vertex_data.indices[indics_count++] = vertex_index[0];
	vertex_data.indices[indics_count++] = vertex_index[1];
	vertex_data.indices[indics_count++] = vertex_index[2];
}

internal void draw_triangle_up(Quad quad , Color triangle_color )
{
	Vector3 vertex_position[3] = {};
	vertex_position[0] = quad.vertex_position[vertex_top_left];
	vertex_position[0].x += quad.vertex_position[vertex_top_right].x;	
	vertex_position[0].y += quad.vertex_position[vertex_top_right].y;
    
	vertex_position[0].x /= 2;
	vertex_position[0].y /= 2;
    
	vertex_position[1] = quad.vertex_position[vertex_bottom_right];
	vertex_position[2] = quad.vertex_position[vertex_bottom_left];
    
	draw_triangle(vertex_position,color_to_linear(triangle_color));
}

internal void draw_triangle_right(Quad quad, Color triangle_color)
{
	
	Vector3 vertex_position[3] = {};
	vertex_position[0] = quad.vertex_position[vertex_top_left];
	
	vertex_position[1] = quad.vertex_position[vertex_bottom_right];
	vertex_position[1].x += quad.vertex_position[vertex_top_right].x;	
	vertex_position[1].y += quad.vertex_position[vertex_top_right].y;
    
	vertex_position[1].x /= 2;
	vertex_position[1].y /= 2;
    
    
	vertex_position[2] = quad.vertex_position[vertex_bottom_left];
    
	draw_triangle(vertex_position,color_to_linear(triangle_color));
}

internal void draw_triangle_left(Quad quad, Color triangle_color)
{
	
	Vector3 vertex_position[3] = {};
	vertex_position[0] = quad.vertex_position[vertex_top_left];
	vertex_position[0].x += quad.vertex_position[vertex_bottom_left].x;	
	vertex_position[0].y += quad.vertex_position[vertex_bottom_left].y;
    
	vertex_position[0].x /= 2;
	vertex_position[0].y /= 2;
    
	vertex_position[1] = quad.vertex_position[vertex_bottom_right];
	vertex_position[2] = quad.vertex_position[vertex_top_right];
    
	draw_triangle(vertex_position,color_to_linear(triangle_color));
}

internal void draw_triangle_bottom(Quad quad, Color triangle_color)
{
	
	Vector3 vertex_position[3] = {};
	vertex_position[0] = quad.vertex_position[vertex_bottom_right];
	vertex_position[0].x += quad.vertex_position[vertex_bottom_left].x;	
	vertex_position[0].y += quad.vertex_position[vertex_bottom_left].y;
    
	vertex_position[0].x /= 2;
	vertex_position[0].y /= 2;
    
	vertex_position[1] = quad.vertex_position[vertex_top_left];
	vertex_position[2] = quad.vertex_position[vertex_top_right];
    
	draw_triangle(vertex_position,color_to_linear(triangle_color));
}

//use for drawing quadrilateral
internal void draw_quad_with_texture(Quad quad,int target_texture)
{
    
	quad_draw_check(target_texture);
    
	render_state.current_shader_input= S_quad;
	render_state.current_texture = target_texture;
    
	ShaderInput* shader_input = all_shader_inputs + S_quad;	
    
	unsigned short vertex_index[4] = {};
	
	int indics_count = shader_input->indices_count;
	shader_input->indices_count += 6;
    
	int vertex_count = shader_input->vertices_count;    
	shader_input->vertices_count+=4;
    
	Vector3* all_vertices = quad.vertex_position;
	Vector4* vertex_color = quad.vertex_color;
	Vector2 texture_coord[4] = {};
    
    texture_coord[vertex_bottom_left] = (Vector2){ 0,1 };
    texture_coord[vertex_top_left] = (Vector2){ 1,1 };
    texture_coord[vertex_top_right] = (Vector2){ 1,0 };
    texture_coord[vertex_bottom_right] = (Vector2){ 0, 0 };
    
	//i'm so confused with the vertics order     
	vertex_index[0] = vertex_count;
	vertex_data.position[vertex_count] = all_vertices[0];
	vertex_data.tex_coord[vertex_count] = texture_coord[0];
	vertex_data.color[vertex_count] = vertex_color[0];
	vertex_count++;
    
	vertex_index[1] = vertex_count;
	vertex_data.position[vertex_count] = all_vertices[1];
	vertex_data.tex_coord[vertex_count] = texture_coord[2];
	vertex_data.color[vertex_count] = vertex_color[1];
	vertex_count++;
    
	vertex_index[2] = vertex_count;
	vertex_data.position[vertex_count] = all_vertices[2];
	vertex_data.tex_coord[vertex_count] = texture_coord[1];
	vertex_data.color[vertex_count] = vertex_color[2];
	vertex_count++;
    
	vertex_index[3] = vertex_count;
	vertex_data.position[vertex_count] = all_vertices[3];
	vertex_data.tex_coord[vertex_count] = texture_coord[3];
	vertex_data.color[vertex_count] = vertex_color[3];
	vertex_count++;
    
	vertex_data.indices[indics_count++] = vertex_index[0];
	vertex_data.indices[indics_count++] = vertex_index[1];
	vertex_data.indices[indics_count++] = vertex_index[2];
	vertex_data.indices[indics_count++] = vertex_index[3];
	vertex_data.indices[indics_count++] = vertex_index[2];
	vertex_data.indices[indics_count++] = vertex_index[1];
    
}

//you can switch game view to 2d world or screen
//it will draw everything remaining once you change view
internal void change_matrix(Matrix matrix)
{
	D_game_draw();
    
	(*(Matrix*)render_state.shader_uniform_data[SU_MVP]) = matrix;
	current_matrix = matrix;
}

internal void _D_DrawRect(Rect rect, Vector4 corner_radius, int target_texture , Vector4 * vertex_color , Vector2 * texture_coord)
{
	Vector3 position = rect.position;
	Vector2 size = rect.size;
	Quaternion rotation = rect.rotation;
    
	bool clean_up_draw_state = false;
    
	if (render_state.current_shader_input != S_rect)
	{
		clean_up_draw_state = true;
	}
    
    if (render_state.current_texture != target_texture)
	{
		clean_up_draw_state = true;
	}
    
    if(render_state.current_shader_input == S_rect)
    {
        if(all_shader_inputs[S_rect].vertices_count + 10 >= VERTEX_BUFFER_MAX)
        {
            clean_up_draw_state = true;
        }
        
        if(all_shader_inputs[S_rect].indices_count + 10 >=VERTEX_BUFFER_INDICS_MAX)
        {
            clean_up_draw_state  = true;
        }
    }
    
	if (clean_up_draw_state)
	{
		D_game_draw();
	}
    
	render_state.current_shader_input = S_rect;
	render_state.current_texture = target_texture;
    
	ShaderInput* ShaderInput = all_shader_inputs + S_rect;
	int vertex_count = ShaderInput->vertices_count++;
    
	vertex_data.position[vertex_count] = rect.position;
    
	//why it is negative?
	vertex_data.rotation[vertex_count] = quaternion_to_vector4(rect.rotation);
	vertex_data.size[vertex_count] = rect.size;
	vertex_data.corner_radius[vertex_count] = corner_radius;
	
	for (int vertex_index = 0; vertex_index < quad_vertex_count; vertex_index++)
	{
		vertex_data.corner_tex_coord[vertex_index][vertex_count] = texture_coord[vertex_index];
		vertex_data.corner_color[vertex_index][vertex_count] = vertex_color[vertex_index];
	}
}

internal void draw_rect(Rect rect , Vector4 rect_color, int texture_index)
{
	int texture_coord_count = 0;
    
	Vector4 color_array[4] = {};
	Vector2 tex_coord_array[4] = {};
    
	tex_coord_array[texture_coord_count++] = (Vector2){ 1,0 };
	tex_coord_array[texture_coord_count++] = (Vector2){ 1,1 };
	tex_coord_array[texture_coord_count++] = (Vector2){ 0,0 };
	tex_coord_array[texture_coord_count++] = (Vector2){ 0,1 };
	
    for (int vertex_index = 0; vertex_index < quad_vertex_count; vertex_index++)
	{
		color_array[vertex_index] = rect_color;
	}
    
    _D_DrawRect(rect, (Vector4){ 0,0,0,0}, texture_index , color_array , tex_coord_array);
}

internal void draw_rect_B(Rect rect , Vector4 rectColor)
{
	draw_rect(rect , rectColor, render_state.default_white_image);
}

internal void draw_rect_with_texture(Rect rect , Vector4 corners, Color rect_color, int texture_index)
{
	int texture_coord_count = 0;
    
	Vector4 color_array[4] = {};
	Vector2 texture_coord_array[4] = {};
    
	texture_coord_array[texture_coord_count++] = (Vector2){ 1,0 };
	texture_coord_array[texture_coord_count++] = (Vector2){ 1,1 };
	texture_coord_array[texture_coord_count++] = (Vector2){ 0,0 };
	texture_coord_array[texture_coord_count++] = (Vector2){ 0,1 };
	
    for (int vertex_index = 0; vertex_index < quad_vertex_count; vertex_index++)
	{
		color_array[vertex_index].x = ((float)rect_color.r /255.0);
		color_array[vertex_index].y = ((float)rect_color.g /255.0);
		color_array[vertex_index].z = ((float)rect_color.b /255.0);
		color_array[vertex_index].w = ((float)rect_color.a /255.0);
	}
    
    _D_DrawRect(rect, corners, texture_index , color_array , texture_coord_array);
}

internal void draw_rect_with_texture_B(Rect rect , Color rect_color, int texture)
{
	draw_rect_with_texture(rect , (Vector4){ 0, 0, 0, 0 }, rect_color , texture);
}

internal void draw_quad_as_rect(Quad quad,int target_texture)
{
	draw_quad_with_texture(quad,target_texture);
}

internal void draw_quad_as_rect_B(Quad * quad, int target_texture)
{
	draw_quad_as_rect(*quad, target_texture);
}

internal void draw_quad_as_rect_C(Quad quad,int target_texture,Color quad_color)
{
    
    for (int vertex_index = 0; vertex_index < quad_vertex_count; vertex_index++)
	{
		quad.vertex_color[vertex_index].x = ((float)quad_color.r /255.0);
		quad.vertex_color[vertex_index].y = ((float)quad_color.g /255.0);
		quad.vertex_color[vertex_index].z = ((float)quad_color.b /255.0);
		quad.vertex_color[vertex_index].w = ((float)quad_color.a /255.0);
	}
    
    draw_quad_as_rect(quad, target_texture);
    
}

internal void draw_quad_as_rect_D(Quad * quad , int target_texture , Color quad_color)
{
    draw_quad_as_rect_C( *quad , target_texture , quad_color);
}

internal void draw_quad(Quad quad, int target_texture)
{
	draw_quad_with_texture(quad, target_texture);
}

internal void draw_quad_B(Quad quad)
{
	draw_quad_with_texture(quad,render_state.default_white_image);
}

internal void draw_quad_C(Vector3 position , float size , Quaternion rotation, Vector4 quad_color )
{
	Quad quad = {};
    
	quad.vertex_position[vertex_top_left].x -= size * 0.5f;	
	quad.vertex_position[vertex_top_left].y += size * 0.5f;
	
	quad.vertex_position[vertex_top_right].x += size * 0.5f;	
	quad.vertex_position[vertex_top_right].y += size * 0.5f;
    
	quad.vertex_position[vertex_bottom_left].x -= size * 0.5f;	
	quad.vertex_position[vertex_bottom_left].y -= size * 0.5f;
    
	quad.vertex_position[vertex_bottom_right].x += size * 0.5f;	
	quad.vertex_position[vertex_bottom_right].y -= size * 0.5f;
    
	for (int vertex_index = 0; vertex_index < quad_vertex_count; vertex_index++)
	{
		quad.vertex_position[vertex_index] = Vector3RotateByQuaternion(quad.vertex_position[vertex_index] , rotation);
		quad.vertex_position[vertex_index] = Vector3Add(quad.vertex_position[vertex_index] , position);
		quad.vertex_color[vertex_index] = quad_color;
	}
    
	draw_quad_with_texture(quad,render_state.default_white_image);
}

internal void draw_quad_D(Quad quad, Color quad_color)
{
    for (int ColorIndex = 0; ColorIndex < quad_vertex_count; ColorIndex++)
	{
		quad.vertex_color[ColorIndex].x = ((float)quad_color.r /255.0);
		quad.vertex_color[ColorIndex].y = ((float)quad_color.g /255.0);
		quad.vertex_color[ColorIndex].z = ((float)quad_color.b /255.0);
		quad.vertex_color[ColorIndex].w = ((float)quad_color.a /255.0);
	}
    
	draw_quad_with_texture(quad,render_state.default_white_image);
}

internal void draw_rect_C(Rect rect, float corner_roundness)
{
	draw_rect_with_texture(rect  , (Vector4){ corner_roundness, corner_roundness, corner_roundness, corner_roundness }, WHITE, render_state.default_white_image);
}

internal void draw_rect_D(Rect rect, float corner_roundness, Color target_color)
{
	draw_rect_with_texture(rect  , (Vector4){ corner_roundness, corner_roundness, corner_roundness, corner_roundness }, target_color, render_state.default_white_image);
}

internal void draw_circle(Rect rect, Color target_color)
{
	draw_rect_D(rect, rect.size.x * 0.5f, target_color);
}

internal Rect get_billboard_rect( Vector3 position , float size )
{
    Rect rect = {};
	rect.position = position;
	rect.size = (Vector2){ size,size };
    rect.rotation = editor->billboard_rotation;
    
    return rect;
}

internal void draw_billboard_circle(Vector3 position , float size, Color target_color)
{
    Rect rect = get_billboard_rect(position , size);
	draw_circle(rect, target_color);
}

internal void draw_circle_B(Vector3 position,float size, Color target_color)
{
	Rect rect = get_rect();
	rect.position = position;
	rect.size = (Vector2){ size,size };
    
	draw_circle(rect, target_color);
}

internal void draw_circle_C(Vector2 position,float size, Color target_color)
{
    draw_circle_B((Vector3){position.x,position.y,0},size,target_color);
}

internal void draw_circle_D(Vector2 position,float size, Vector4 target_color)
{
	draw_circle_C(position , size, linear_to_color(target_color));
}

internal void draw_circle_E(Vector3 position,float size, Vector4 target_color)
{
	draw_circle_B(position , size, linear_to_color(target_color));
}

internal void draw_line_quad(Quad quad , Vector3 line_start , Vector3 line_end , float line_size)
{
    
	bool clean_up_draw_state = false;
    
	if (render_state.current_shader_input != S_line)
	{
		clean_up_draw_state = true;
	}
	
	if (render_state.current_texture!= render_state.default_white_image)
	{
		clean_up_draw_state = true;
	}
    
    if(render_state.current_shader_input == S_line)
    {
        //just check if it was the will too close
        if(all_shader_inputs[S_line].vertices_count + 20 >=VERTEX_BUFFER_MAX)
        {
            clean_up_draw_state = true;
        }
        
        if(all_shader_inputs[S_line].indices_count + 20 >=VERTEX_BUFFER_INDICS_MAX)
        {
            clean_up_draw_state  = true;
        }
    }
    
    //it seem depth can't go more than 1.0?
    //is depth normalized?
    if(render_state.fake_depth > 0.99f)
    {
        clean_up_draw_state = true;
        glClear(GL_DEPTH_BUFFER_BIT);
        render_state.fake_depth = 0;
    }
    
	if (clean_up_draw_state)
	{
		D_game_draw();
	}    
    
	render_state.current_shader_input= S_line;
	render_state.current_texture = render_state.default_white_image;
    
	ShaderInput* ShaderInput = all_shader_inputs + S_line;	
    
	unsigned short vertex_index[4] = {};
	
	int indics_count = ShaderInput->indices_count;
	ShaderInput->indices_count += 6;
    
	int vertex_count = ShaderInput->vertices_count;    
	ShaderInput->vertices_count+=4;
    
	render_state.fake_depth += 0.001f;
    
	Vector3* all_vertices = quad.vertex_position;
	Vector4* vertex_color = quad.vertex_color;
	Vector2 texture_coord[4] = {};
    
    texture_coord[vertex_bottom_left] = (Vector2){ 0,1 };
    texture_coord[vertex_top_left] = (Vector2){ 1,1 };
    texture_coord[vertex_top_right] = (Vector2){ 1,0 };
    texture_coord[vertex_bottom_right] = (Vector2){ 0, 0 };
    
	//i'm so confused with the vertics order     
	vertex_index[0] = vertex_count;
	vertex_data.position[vertex_count] = all_vertices[0];
	vertex_data.tex_coord[vertex_count] = texture_coord[0];
	vertex_data.color[vertex_count] = vertex_color[0];
	vertex_data.line_start[vertex_count] = line_start;
	vertex_data.line_end[vertex_count] = line_end;
	vertex_data.line_size[vertex_count] = line_size;
	vertex_data.fake_depth[vertex_count] = render_state.fake_depth;
	vertex_count++;
    
	vertex_index[1] = vertex_count;
	vertex_data.position[vertex_count] = all_vertices[1];
	vertex_data.tex_coord[vertex_count] = texture_coord[2];
	vertex_data.color[vertex_count] = vertex_color[1];
	vertex_data.line_start[vertex_count] = line_start;
	vertex_data.line_end[vertex_count] = line_end;
	vertex_data.line_size[vertex_count] = line_size;
	vertex_data.fake_depth[vertex_count] = render_state.fake_depth;
	vertex_count++;
    
	vertex_index[2] = vertex_count;
	vertex_data.position[vertex_count] = all_vertices[2];
	vertex_data.tex_coord[vertex_count] = texture_coord[1];
	vertex_data.color[vertex_count] = vertex_color[2];
	vertex_data.line_start[vertex_count] = line_start;
	vertex_data.line_end[vertex_count] = line_end;
	vertex_data.line_size[vertex_count] = line_size;
	vertex_data.fake_depth[vertex_count] = render_state.fake_depth;
	vertex_count++;
    
	vertex_index[3] = vertex_count;
	vertex_data.position[vertex_count] = all_vertices[3];
	vertex_data.tex_coord[vertex_count] = texture_coord[3];
	vertex_data.color[vertex_count] = vertex_color[3];
	vertex_data.line_start[vertex_count] = line_start;
	vertex_data.line_end[vertex_count] = line_end;
	vertex_data.line_size[vertex_count] = line_size;
	vertex_data.fake_depth[vertex_count] = render_state.fake_depth;
	vertex_count++;
    
	vertex_data.indices[indics_count++] = vertex_index[0];
	vertex_data.indices[indics_count++] = vertex_index[1];
	vertex_data.indices[indics_count++] = vertex_index[2];
	vertex_data.indices[indics_count++] = vertex_index[3];
	vertex_data.indices[indics_count++] = vertex_index[2];
	vertex_data.indices[indics_count++] = vertex_index[1];
}

internal void draw_line_quad_B(Quad quad , Color quad_color , Vector3 line_start , Vector3 line_end , float line_size)
{
	
	Vector4 quad_linear_color = color_to_linear(quad_color);
	
	for (int vertex_index = 0; vertex_index < quad_vertex_count; vertex_index++)
	{
		quad.vertex_color[vertex_index] = quad_linear_color;
	}
    
	draw_line_quad(quad , line_start , line_end , line_size);
}

//add a point into game world
//to draw a line :
//AddPointToLine(Start , WHITE);
//AddPointToLine(End , WHITE);

//you can have gradient color by changing color between point

//and vary size!

internal void add_point_to_line(Vector3 point, Vector4 point_color , float point_size)
{
    
	local_persist bool have_previous_point = false;
    
	local_persist Vector3 previous_point = {};
	local_persist Vector4 previous_point_color = {};
	local_persist float previous_point_size = 0;
    
	if (!have_previous_point)
	{
        
		have_previous_point = true;
		previous_point = point;
		previous_point_color = point_color;
		previous_point_size = point_size;
        
	}
	else
	{
		
        float previous_point_size_width  = (previous_point_size*0.5) / app_data->window_size.x;
		float previous_point_size_height  = (previous_point_size*0.5) / app_data->window_size.y;
		
		float point_size_width  = (point_size*0.5) / app_data->window_size.x;
		float point_size_height  = (point_size*0.5) / app_data->window_size.y;
        
		have_previous_point = false;
        
		Quad line = {};
        
		Vector3 line_direction = point;
		line_direction = Vector3Subtract(line_direction , previous_point);
		
		Vector3 line_vertical_direction = { -line_direction.y , line_direction.x };
		line_vertical_direction = Vector3Normalize(line_vertical_direction);
        
		Vector3 start_point_offset = {line_vertical_direction.x * previous_point_size_width , line_vertical_direction.y * previous_point_size_height };
        
		line.vertex_position[vertex_top_left] = Vector3Add( previous_point , start_point_offset );
		line.vertex_position[vertex_bottom_left] = Vector3Subtract( previous_point , start_point_offset );
        
		line.vertex_color[vertex_top_left] = previous_point_color;
		line.vertex_color[vertex_bottom_left] = previous_point_color;
        
		Vector3 end_point_offset = {line_vertical_direction.x * point_size_width , line_vertical_direction.y * point_size_height };
		
		line.vertex_position[vertex_top_right] = Vector3Add( point , end_point_offset );
		line.vertex_position[vertex_bottom_right] = Vector3Subtract( point , end_point_offset );
        
		line.vertex_color[vertex_top_right] = point_color;
		line.vertex_color[vertex_bottom_right] = point_color;
        
		draw_line_quad( line , previous_point,point ,point_size);
        
	}
}

internal void add_point_to_line_B(Vector3 point, Color point_color , float point_size)
{
    add_point_to_line(point,color_to_linear(point_color) , point_size);
}

internal void add_point_to_line_C(Vector3 point, Color point_color)
{
	add_point_to_line_B(point, point_color , 0.01f);
}

internal void add_point_to_line_2D(Vector2 point, Vector4 point_color,float point_size)
{
	add_point_to_line( (Vector3){ point.x, point.y,0 }, point_color , point_size);
}

internal void add_point_to_line_2D_B(Vector2 point, Vector4 point_color)
{
	add_point_to_line_2D(point, point_color, 0.01f);
}

internal void add_point_to_line_2D_C(Vector2 point, Color point_color , float point_size)
{
    add_point_to_line((Vector3){ point.x, point.y,0 }, color_to_linear(point_color) , point_size);
}

internal void add_point_to_line_2D_D(Vector2 point, Color point_color)
{
	add_point_to_line_2D_C(point, point_color , 0.01f);
}

internal unsigned int load_depth_texture(int width, int height)
{
	unsigned int id = 0;
	unsigned int glInternalFormat = GL_DEPTH_COMPONENT;
    
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
    
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
	glBindTexture(GL_TEXTURE_2D, 0);
    
	TRACELOG(RL_LOG_INFO, "TEXTURE: Depth texture loaded successfully");
	
	return id;
}

//so far, solving equations seem to be better than using vector math
//the equation tell me the length of plane normal doesn't matter
internal float get_line_intersect_with_plane_time(Vector3 start , Vector3 end , Vector3 plane_normal , Vector3 plane_origin)
{
	float result = plane_normal.x * (plane_origin.x - start.x) + plane_normal.y * (plane_origin.y - start.y) + plane_normal.z * (plane_origin.z - start.z);
	result /= plane_normal.x * (end.x - start.x) + plane_normal.y * (end.y - start.y) + plane_normal.z * (end.z - start.z);
    
	return result;
}

internal float ray_get_closest_point(Vector3 o , Vector3 n , Vector3 a)
{
    float result = Vector3DotProduct(n,a);
    result -= Vector3DotProduct(n,o);
    result /= Vector3DotProduct(n,n);
    
    return result;
}

//steal from raylib and modified
//this one xyz components get divide by z component
internal Vector3 transform_vector(Vector3 position , Matrix matrix)
{
    
    Quaternion world_pos = { position.x, position.y, position.z, 1.0f };
    world_pos = QuaternionTransform(world_pos, matrix);
    Vector3 ndc_Pos = { world_pos.x/world_pos.w, world_pos.y/world_pos.w, world_pos.z/world_pos.w };
    
    return ndc_Pos;
}

internal float remove_prespective(Vector3 q)
{
	return world_3D_matrix.m3*q.x + world_3D_matrix.m7*q.y + world_3D_matrix.m11*q.z + world_3D_matrix.m15;
}

internal Quad get_billboard_quad(Vector3 point, float width , float height)
{
	Quad billboard_quad = {};
	billboard_quad.vertex_position[vertex_top_left] = (Vector3){ -width * 0.5f , height * 0.5f};
	billboard_quad.vertex_position[vertex_top_right] = (Vector3){ width * 0.5f , height * 0.5f};
	billboard_quad.vertex_position[vertex_bottom_left] = (Vector3){ -width * 0.5f , -height * 0.5f};
	billboard_quad.vertex_position[vertex_bottom_right] = (Vector3){width * 0.5f , -height * 0.5f};
    
	Vector3 up = { view_matrix.m1 , view_matrix.m5 , view_matrix.m9 };
	Vector3 right = {view_matrix.m0 , view_matrix.m4 , view_matrix.m8 };
    
	//what if i multiply it first lol
	//TODO : fix it
    
	up = Vector3Scale(up , remove_prespective(up));
	right = Vector3Scale(right, remove_prespective(right));
    
	for (int vertex_index = 0; vertex_index < quad_vertex_count; vertex_index++)
	{
        
		Vector3 * current_vertex = billboard_quad.vertex_position + vertex_index;
        
		Vector3 right_vector = Vector3Scale(right , current_vertex->x );
		Vector3 up_vector = Vector3Scale(up , current_vertex->y );
        
		(*current_vertex) = Vector3Add(right_vector, up_vector);
        
		current_vertex->x += point.x;
		current_vertex->y += point.y;
		current_vertex->z += point.z;		
	}
    
	return billboard_quad;
}

//pune line differently in 3d space and screen
global bool pruning_3D_line = true;

internal void draw_round_line(Vector3 start , Vector3 end , float size , Color start_color , Color end_color)
{
    
    //man what is this
    if(pruning_3D_line)
    {
        
        Vector3 plane_origin = game_camera.position;
        Vector3 plane_normal = Vector3Subtract(game_camera.target , game_camera.position);
        
        Vector3 origin_to_start_direction = Vector3Subtract(start , plane_origin );
        Vector3 origin_to_end_direction = Vector3Subtract(end , plane_origin );
        
        if (Vector3DotProduct(origin_to_start_direction  , plane_normal) < 0)
        {
            if (Vector3DotProduct(origin_to_end_direction  , plane_normal) < 0)
            {
                return;
            }
        }
        
        float intersect_time = get_line_intersect_with_plane_time(start , end, plane_normal , plane_origin);
        
        if (intersect_time < 1)
        {
            if (intersect_time > 0)
            {
                
                if (Vector3DotProduct(origin_to_start_direction  , plane_normal) < 0)
                {
                    start = end;
                    intersect_time = 1.0 - intersect_time;
                    
                    Color temp_color = end_color;
                    end_color = start_color;
                    start_color = temp_color;
                }
                
                intersect_time *= 0.9f;
                Vector3 intersect_point = Vector3Add(start , Vector3Scale( Vector3Subtract(end , start) , intersect_time) );
                
                //printf("Intersect : %f %f %f \n" , IntersectPoint.x , IntersectPoint.y , IntersectPoint.z);
                //printf("End : %f %f %f \n" , End.x , End.y , End.z);
                
                end = intersect_point;
                
            }
        }
        
    }
    else
    {
        
        //i don't even know how this work
        //this is for pruning point in 3D space convert to 2d space rect line
        //the transform is wacky and things behind camera end up upfront
        //i just happen to found out the z is bigger than far clip when behind camera
        if((-start.z) > screen_far_clip )
        {
            if((-end.z) > screen_near_clip)
            {
                return;
            }
        }
        
    }
    
	//what am i doing?
	Vector3 screen_start = transform_vector(start , current_matrix);
	Vector3 screen_end = transform_vector(end , current_matrix);
    
    start = (Vector3){screen_start.x , screen_start.y , 0};
	end = (Vector3){screen_end.x , screen_end.y , 0};
    
	add_point_to_line_B(start , start_color, size);
	add_point_to_line_B(end , end_color, size);
    
	float line_height = size / current_viewport.height;
	float line_width = size / current_viewport.width;
    
	Quad square_quad = {};
	square_quad.vertex_position[vertex_top_left] = (Vector3){ -line_width * 0.5f , line_height * 0.5f};
	square_quad.vertex_position[vertex_top_right] = (Vector3){ line_width * 0.5f , line_height * 0.5f};
	square_quad.vertex_position[vertex_bottom_left] = (Vector3){ -line_width * 0.5f , -line_height * 0.5f};
	square_quad.vertex_position[vertex_bottom_right] = (Vector3){ line_width * 0.5f , -line_height * 0.5f};
    
	//this look kinda cool	
#if 0
	Vector3 StartCameraDirection = Vector3Subtract(app_data->game_camera.position , start);
	StartCameraDirection = Vector3Normalize(StartCameraDirection);
	Quaternion Startrotation = QuaternionFromVector3ToVector3({0,0,-1}, StartCameraDirection);
#endif
    
	Vector3 right = { 1 , 0, 0};
	Vector3 up = {0, 1, 0};
    
	Quad start_quad = square_quad;
    
	for (int vertex_index = 0; vertex_index < quad_vertex_count; vertex_index++)
	{
        
		Vector3 * current_vertex = start_quad.vertex_position + vertex_index;
        
		Vector3 right_vector = Vector3Scale(right , current_vertex->x);
		Vector3 up_vector = Vector3Scale(up , current_vertex->y );
        
		(*current_vertex) = Vector3Add(right_vector, up_vector);
        
		//(*CurrentVertex) = Vector3Transform((*CurrentVertex) , StartViewMatrix );
		//(*CurrentVertex) = Vector3RotateByQuaternion( (*CurrentVertex) , Startrotation);
        
		current_vertex->x += start.x;
		current_vertex->y += start.y;
		current_vertex->z += start.z;
		
	}
	
	draw_line_quad_B(start_quad, start_color , start , end , size);
	//DrawCircle(Start , size , LineColor);
	
	Quad end_quad = square_quad;
    
	for (int vertex_index = 0; vertex_index < quad_vertex_count; vertex_index++)
	{
        
		Vector3 * current_vertex = end_quad.vertex_position + vertex_index;
        
		Vector3 right_vector = Vector3Scale(right , current_vertex->x);
		Vector3 up_vector = Vector3Scale(up , current_vertex->y );
        
		(*current_vertex) = Vector3Add(right_vector, up_vector);
        
		//(*CurrentVertex) = Vector3Transform((*CurrentVertex) , StartViewMatrix );
		//(*CurrentVertex) = Vector3RotateByQuaternion( (*CurrentVertex) , Startrotation);
        
		current_vertex->x += end.x;
		current_vertex->y += end.y;
		current_vertex->z += end.z;
		
	}
    
	draw_line_quad_B(end_quad, end_color , start ,end , size);
	//DrawCircle(End , size , LineColor);
    
}

internal void draw_round_line_B(Vector3 start , Vector3 end , float size , Color line_color)
{
    draw_round_line(start , end ,  size , line_color , line_color);
}

//this thing create 8 drawcall total
internal void draw_rect_line(Vector2 start_position , Vector2 end_position , Vector4 line_color)
{
	float point_size = 0.005f;
	float line_size = 0.005f;
    
	add_point_to_line_2D(start_position, line_color, line_size);
	add_point_to_line_2D((Vector2){ end_position.x ,start_position.y }, line_color, line_size);
    
	add_point_to_line_2D((Vector2){ end_position.x ,start_position.y }, line_color, line_size);
	add_point_to_line_2D(end_position, line_color, line_size);
    
    
	add_point_to_line_2D(start_position, line_color, line_size);
	add_point_to_line_2D((Vector2){ start_position.x,end_position.y }, line_color, line_size);
    
	add_point_to_line_2D((Vector2){ start_position.x,end_position.y }, line_color, line_size);
	add_point_to_line_2D(end_position, line_color, line_size);	
    
	draw_circle_E((Vector3){ end_position.x ,start_position.y ,0} , point_size ,  line_color);
	draw_circle_D(end_position , point_size , line_color);
	draw_circle_E((Vector3){ start_position.x,end_position.y ,0}, point_size , line_color);
	draw_circle_D(start_position  , point_size , line_color);
}

//this also create 8 drawcall total
internal void draw_quad_line(Quad quad , Color line_color , float line_size)
{
    
    Vector3 top_right = quad.vertex_position[vertex_top_right];
    Vector3 top_left = quad.vertex_position[vertex_top_left];
    Vector3 bottom_right = quad.vertex_position[vertex_bottom_right];
    Vector3 bottom_left = quad.vertex_position[vertex_bottom_left];
    
	draw_round_line(top_right , top_left ,line_size , line_color , line_color);
	draw_round_line(top_left , bottom_left ,line_size , line_color , line_color);
	draw_round_line(bottom_left , bottom_right ,line_size , line_color , line_color);
	draw_round_line(bottom_right , top_right ,line_size , line_color , line_color);
}

internal void draw_rect_line_B(Vector2 start_position , Vector2 end_position , Color line_color)
{
    draw_rect_line(start_position,end_position,color_to_linear(line_color));
}

internal void draw_rect_E(Vector3 position,float size, Color target_color , Quaternion rotation)
{
	Rect rect = get_rect();
	rect.position = position;
	rect.size = (Vector2){ size,size };
	rect.rotation = rotation;
    
	draw_rect_D(rect, 0 , target_color);
}

internal void draw_rect_F(Vector3 position, float size, Vector4 target_color , Quaternion rotation)
{
	draw_rect_E(position , size ,linear_to_color(target_color) , rotation);
}

internal void draw_rect_line_C(Rect rect ,Vector4 line_color , float line_size)
{
	Vector3 rect_vertex[quad_vertex_count] = {};
    
	rect_vertex[vertex_top_left].x -= rect.size.x * 0.5f;
	rect_vertex[vertex_top_left].y += rect.size.y * 0.5f;
	
	rect_vertex[vertex_top_right].x += rect.size.x * 0.5f;
	rect_vertex[vertex_top_right].y += rect.size.y * 0.5f;
	
	rect_vertex[vertex_bottom_left].x -= rect.size.x * 0.5f;
	rect_vertex[vertex_bottom_left].y -= rect.size.y * 0.5f;
	
	rect_vertex[vertex_bottom_right].x += rect.size.x * 0.5f;
	rect_vertex[vertex_bottom_right].y -= rect.size.y * 0.5f;
    
	for (int vertex_index = 0 ; vertex_index < quad_vertex_count ; vertex_index++)
	{
		rect_vertex[vertex_index] = Vector3RotateByQuaternion(rect_vertex[vertex_index] , rect.rotation);
		rect_vertex[vertex_index] = Vector3Add(rect_vertex[vertex_index] , rect.position);
	}
    
	Color color_bytes = linear_to_color(line_color);
    
	draw_round_line(rect_vertex[vertex_top_right] , rect_vertex[vertex_top_left] , line_size, color_bytes , color_bytes );
	draw_round_line(rect_vertex[vertex_top_left] , rect_vertex[vertex_bottom_left] , line_size, color_bytes , color_bytes );
	draw_round_line(rect_vertex[vertex_bottom_left] , rect_vertex[vertex_bottom_right] , line_size, color_bytes , color_bytes );
	draw_round_line(rect_vertex[vertex_bottom_right] , rect_vertex[vertex_top_right] , line_size, color_bytes , color_bytes );
    
}

internal void draw_rect_line_D(Rect rect, Vector4 line_color)
{
	draw_rect_line_C(rect, line_color , 4);
}

internal void draw_rect_line_E(Rect rect, Color line_color , float line_size )
{
	draw_rect_line_C(rect , color_to_linear(line_color) , line_size );
}

internal void draw_rect_line_F( Rect rect , Color line_color)
{
	draw_rect_line_E(rect, line_color , 4);
}

internal void draw_quad_line_B( Quad quad, Color line_color)
{
	float Linesize = 8;
	draw_quad_line(quad, line_color , Linesize);
}

internal void draw_rect_G( Rect rect,Color target_color)
{
	draw_rect_with_texture(rect  , (Vector4){0,0,0,0}, target_color, render_state.default_white_image);
}

//draw text only
internal void _draw_rect_text(Rect quad ,Vector4* color ,Vector2* texture_coord , Vector3 up ,Vector3 right)
{
    bool clean_up_draw_state = false; 
    
	if (render_state.current_shader_input != S_text)
	{
        clean_up_draw_state = true;
	}
    
    if(render_state.current_shader_input == S_text)
    {
        if(all_shader_inputs[S_text].vertices_count+10 >= VERTEX_BUFFER_MAX)
        {
            clean_up_draw_state = true;
        }
        
        if(all_shader_inputs[S_text].indices_count + 10 >=VERTEX_BUFFER_INDICS_MAX)
        {
            clean_up_draw_state  = true;
        }
    }
    
    if(clean_up_draw_state)
    {
        D_game_draw();
    }
    
	render_state.current_shader_input = S_text;
    
	ShaderInput* shader_input = all_shader_inputs + S_text;
    
	int vertex_index[4] = {};
    
	unsigned short* indices_array = vertex_data.indices;
	int* indics_count = &shader_input->indices_count;
    
	Vector3* vertex_array = vertex_data.position;
	Vector4* color_array = vertex_data.color;
	Vector2* tex_coord_array = vertex_data.tex_coord;
	int* vertex_count = &shader_input->vertices_count;
    
	Vector3 position = quad.position;
	Vector2 size = quad.size;
	Quaternion rotation = quad.rotation;
    
	Vector3 up_offset = Vector3Scale(up, size.y);
	Vector3 bottom_offset = Vector3Scale(up, -size.y);
	Vector3 right_offset = Vector3Scale(right, -size.x);
	Vector3 left_offset = Vector3Scale(right,size.x);
    
	Vector3 vertex_left_top = Vector3Add(left_offset, up_offset);
	Vector3 vertex_right_top = Vector3Add(right_offset,up_offset);
	Vector3 vertex_left_bottom = Vector3Add(left_offset , bottom_offset);
	Vector3 vertex_right_bottom = Vector3Add(right_offset , bottom_offset);
    
	vertex_left_top = Vector3RotateByQuaternion(vertex_left_top, rotation);
	vertex_right_top = Vector3RotateByQuaternion(vertex_right_top, rotation);
	vertex_left_bottom = Vector3RotateByQuaternion(vertex_left_bottom, rotation);
	vertex_right_bottom = Vector3RotateByQuaternion(vertex_right_bottom, rotation);
    
	vertex_left_top = Vector3Add(vertex_left_top,position);    
	vertex_right_top= Vector3Add(vertex_right_top,position);
    vertex_left_bottom= Vector3Add(vertex_left_bottom,position);
    vertex_right_bottom= Vector3Add(vertex_right_bottom,position);
    
	vertex_index[0] = *vertex_count;
    
	vertex_array[*vertex_count] = vertex_left_top;
	tex_coord_array[*vertex_count] = texture_coord[0];
	color_array[*vertex_count] = color[0];
	(*vertex_count)++;
    
	vertex_index[1] = *vertex_count;
    
	vertex_array[*vertex_count] = vertex_right_top;
	tex_coord_array[*vertex_count] = texture_coord[1];
	color_array[*vertex_count] = color[1];
	(*vertex_count)++;
    
	vertex_index[2] = *vertex_count;
	
	vertex_array[*vertex_count] = vertex_left_bottom;
	tex_coord_array[*vertex_count] = texture_coord[2];
	color_array[*vertex_count] = color[2];
	(*vertex_count)++;
    
	vertex_index[3] = *vertex_count;
	
	vertex_array[*vertex_count] = vertex_right_bottom;
	tex_coord_array[*vertex_count] = texture_coord[3];
	color_array[*vertex_count] = color[3];
	(*vertex_count)++;
    
	indices_array[(*indics_count)++] = vertex_index[0];
	indices_array[(*indics_count)++] = vertex_index[1];
	indices_array[(*indics_count)++] = vertex_index[2];
	indices_array[(*indics_count)++] = vertex_index[3];
	indices_array[(*indics_count)++] = vertex_index[2];
	indices_array[(*indics_count)++] = vertex_index[1];
}

internal void draw_background()
{
	render_state.current_texture = render_state.default_white_image;
    
	ShaderInput * flat_shader = all_shader_inputs + S_quad;
    
	rlEnableShader(flat_shader->shader);
	glBindVertexArray(flat_shader->vertex_array_object);
    
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, render_state.current_texture );
	glUniform1i(flat_shader->uniform_location[SU_texture_0], 0);
    
	rlSetUniformMatrix(flat_shader->uniform_location[SU_MVP] , MatrixIdentity());
    
	Vector3 * sreen_quad_pos = vertex_data.position;
	Vector4 * screen_quad_color_data = vertex_data.color;
	Vector2 * screen_quad_tex_coord_data = vertex_data.tex_coord;
    
	int vert_count = 0;
    
	float top= 1;
    
	sreen_quad_pos[vert_count++] = (Vector3){ -1, top, 0 };
	sreen_quad_pos[vert_count++] = (Vector3){ 1, top, 0 };
	sreen_quad_pos[vert_count++] = (Vector3){ 1, -1, 0 };
    
	sreen_quad_pos[vert_count++] = (Vector3){ -1, -1, 0 };
	sreen_quad_pos[vert_count++] = (Vector3){ -1, top, 0 };
	sreen_quad_pos[vert_count++] = (Vector3){ 1, -1, 0 };
    
	vert_count = 0;
    
	screen_quad_tex_coord_data[vert_count++] = (Vector2){ 0, 1 };
	screen_quad_tex_coord_data[vert_count++] = (Vector2){ 1, 1 };
	screen_quad_tex_coord_data[vert_count++] = (Vector2){ 1, 0 };
    
	screen_quad_tex_coord_data[vert_count++] = (Vector2){ 0, 0 };
	screen_quad_tex_coord_data[vert_count++] = (Vector2){ 0, 1 };
	screen_quad_tex_coord_data[vert_count++] = (Vector2){ 1, 0 };
    
	vert_count = 0;
    
	Vector4 quad_color[quad_vertex_count] = {};
    
	Color top_color = DARKGRAY;
	Color bottom_color = DARKGRAY;
    
	quad_color[vertex_top_left] = color_to_linear(color_multiply_B(top_color,0.8f));
	quad_color[vertex_top_right] = color_to_linear(color_multiply_B(top_color,0.8f));
	quad_color[vertex_bottom_left] = color_to_linear(color_multiply_B(bottom_color ,0.6f));
	quad_color[vertex_bottom_right] = color_to_linear(color_multiply_B(bottom_color ,0.6f));
    
	screen_quad_color_data[vert_count++] = quad_color[vertex_top_left];
	screen_quad_color_data[vert_count++] = quad_color[vertex_top_right];
	screen_quad_color_data[vert_count++] = quad_color[vertex_bottom_right];
    
	screen_quad_color_data[vert_count++] = quad_color[vertex_bottom_left];
	screen_quad_color_data[vert_count++] = quad_color[vertex_top_left];
	screen_quad_color_data[vert_count++] = quad_color[vertex_bottom_right];
    
	update_all_GPU_vertex_buffer(flat_shader, 6);
    
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

internal void draw_screen_flat(int texture_index , Vector4 color , bool multi_sample)
{
    //this is fucked
	ShaderInput * flat_shader = 0;
    
    if(multi_sample)
    {
        flat_shader = all_shader_inputs + S_quad_MS;
    }
    else
    {
        flat_shader = all_shader_inputs + S_quad;
    }
    
    GL_CATCH;
	rlEnableShader(flat_shader->shader);
	glBindVertexArray(flat_shader->vertex_array_object);
    
	glActiveTexture(GL_TEXTURE0);
    GL_CATCH;
	if(multi_sample)
    {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture_index);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, texture_index);
    }
    
    GL_CATCH;
	glUniform1i(flat_shader->uniform_location[SU_texture_0], 0);
    
	rlSetUniformMatrix(flat_shader->uniform_location[SU_MVP] , MatrixIdentity());
    
	Vector3 * sreen_quad_pos = vertex_data.position;
	Vector4 * screen_quad_color_data = vertex_data.color;
	Vector2 * screen_quad_tex_coord_data = vertex_data.tex_coord;
    
	int vert_count = 0;
    
	sreen_quad_pos[vert_count++] = (Vector3){ -1, 1, 0 };
	sreen_quad_pos[vert_count++] = (Vector3){ 1, 1, 0 };
	sreen_quad_pos[vert_count++] = (Vector3){ 1, -1, 0 };
    
	sreen_quad_pos[vert_count++] = (Vector3){ -1, -1, 0 };
	sreen_quad_pos[vert_count++] = (Vector3){ -1, 1, 0 };
	sreen_quad_pos[vert_count++] = (Vector3){ 1, -1, 0 };
    
	vert_count = 0;
    
	screen_quad_tex_coord_data[vert_count++] = (Vector2){ 0, 1 };
	screen_quad_tex_coord_data[vert_count++] = (Vector2){ 1, 1 };
	screen_quad_tex_coord_data[vert_count++] = (Vector2){ 1, 0 };
    
	screen_quad_tex_coord_data[vert_count++] = (Vector2){ 0, 0 };
	screen_quad_tex_coord_data[vert_count++] = (Vector2){ 0, 1 };
	screen_quad_tex_coord_data[vert_count++] = (Vector2){ 1, 0 };
    
	vert_count = 0;
    
	screen_quad_color_data[vert_count++] = color;
	screen_quad_color_data[vert_count++] = color;
	screen_quad_color_data[vert_count++] = color;
    
	screen_quad_color_data[vert_count++] = color;
	screen_quad_color_data[vert_count++] = color;
	screen_quad_color_data[vert_count++] = color;
    
	update_all_GPU_vertex_buffer(flat_shader, 6);
    
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

internal void draw_screen_flat_B(int texture_index ,Color color , bool multi_sample)
{
	draw_screen_flat(texture_index , color_to_linear(color) , multi_sample);
}

internal void get_box_face( Rect * temp_box_rect , Box box)
{
    Vector3 position = box.position;
    Vector3 size = box.size;
    Quaternion rotation = box.rotation;
    
    for(int face_index = 0 ; face_index < face_count ; face_index++) 
    {
        temp_box_rect[face_index] = box_rect[face_index];
    }
    
    Vector3 half_extend = size;
    half_extend.x *= 0.5f;
    half_extend.y *= 0.5f;
    half_extend.z *= 0.5f;
    
    temp_box_rect[face_top].position = (Vector3){0,half_extend.y,0};
    temp_box_rect[face_bottom].position = (Vector3){0,-half_extend.y,0};
    temp_box_rect[face_right].position = (Vector3){half_extend.x,0,0};
    temp_box_rect[face_left].position = (Vector3){-half_extend.x,0,0};
    temp_box_rect[face_front].position = (Vector3){0,0,half_extend.z};
    temp_box_rect[face_back].position = (Vector3){0,0,-half_extend.z};
    
    temp_box_rect[face_top].size = (Vector2){size.x,size.z};
    temp_box_rect[face_bottom].size = (Vector2){size.x,size.z};
    temp_box_rect[face_right].size = (Vector2){size.z,size.y};
    temp_box_rect[face_left].size = (Vector2){size.z,size.y};
    temp_box_rect[face_front].size = (Vector2){size.x,size.y};
    temp_box_rect[face_back].size = (Vector2){size.x,size.y};
    
    for(int face_index = 0 ; face_index < face_count ; face_index++) 
    {
        temp_box_rect[face_index].position = Vector3RotateByQuaternion(temp_box_rect[face_index].position , rotation);
        temp_box_rect[face_index].position = Vector3Add( temp_box_rect[face_index].position , position);
        
        temp_box_rect[face_index].rotation = QuaternionMultiply(rotation ,temp_box_rect[face_index].rotation );
    }
    
}

//TODO : if we need to draw many box maybe don't use this
internal void draw_box( Box box , Color box_color)
{
    Rect temp_box_rect[face_count] = {};
    
    get_box_face(temp_box_rect , box);
    
    for(int face_index = 0 ; face_index < face_count ; face_index++) 
    {
        draw_quad_D(rect_to_quad(temp_box_rect[face_index])  , box_color );
    }
}

internal void draw_box_line(Box box , Color line_color , float line_size)
{
    Rect temp_box_rect[face_count] = {};
    
    get_box_face(temp_box_rect , box);
    
    for(int face_index = 0 ; face_index < face_count ; face_index++) 
    {
        draw_quad_line(rect_to_quad(temp_box_rect[face_index])  , line_color , line_size );
    }
}