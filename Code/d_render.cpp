//api is going nut 
//too many overload

//if data when create by CreateBufferForGPU
//there will be information (_ShaderBuffer) store right at the head 
//the information define how the data should be pass to GPU.
internal _ShaderBuffer* Get_GPU_BufferInfo(void * Data)
{
    return ((_ShaderBuffer*)(Data))-1;
}

//it will allocate data on both cpu ram and gpu ram.
internal void * CreateBufferForGPU(const char * BufferName , int PrimitiveType,int ComponentCount , int MaxSize)
{
    _ShaderBuffer* CurrentBuffer = AllocateTemp(_ShaderBuffer,1);
    
    CurrentBuffer->PrimitiveDataType = PrimitiveType;
    CurrentBuffer->Name = BufferName;
    
    int TypeSize = 0;
    switch (CurrentBuffer->PrimitiveDataType)
    {
        case GL_UNSIGNED_SHORT: TypeSize = sizeof(unsigned short); break;
        case GL_FLOAT: TypeSize = sizeof(float); break;
    }
    
    CurrentBuffer->PrimitiveDataSize = TypeSize;
    CurrentBuffer->MaxDataSize =MaxSize; 
    CurrentBuffer->ComponentsCount = ComponentCount;
    
    All_GPU_Buffers[ShaderBufferCount++] = CurrentBuffer;
    
    if(ShaderBufferCount>=MAX_GPU_BUFFER) ASSERT;
    
    int DataSize = CurrentBuffer->MaxDataSize * CurrentBuffer->ComponentsCount * TypeSize;
    
    unsigned char * BufferData =AllocateTemp_(DataSize); 
    CurrentBuffer->Data =BufferData;
    CurrentBuffer->BufferSize = DataSize;
    
    return BufferData;
}

//only use in RenderStateInit().
//if it was call somewhere else you need to pass buffer to GPU manually

//this will generate a buffer on CPU and its corresponding buffer on CPU
//it storing information(_ShaderBuffer) on data head (use Get_GPU_BufferInfo() to get it)
//if the shader created in RenderStateInit have exact same vertex data type and name
//the vertex buffer will bind to it automatically
//you can call Update_GPU_VertexBufferAll() to update all buffer automatically
internal void * Create_GPU_VertexBuffer(const char * BufferName , int PrimitiveType,int ComponentCount , int BufferSize )
{
    void * Result = CreateBufferForGPU(BufferName,PrimitiveType,ComponentCount,BufferSize);
    
    _ShaderBuffer * ShaderBuffer  = Get_GPU_BufferInfo(Result);
    
    glGenBuffers(1, &ShaderBuffer->BufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, ShaderBuffer->BufferHandle);
    glBufferData(GL_ARRAY_BUFFER, ShaderBuffer->BufferSize, 0, GL_DYNAMIC_DRAW);
    
    return Result;
}

//Indics Buffer will update automactically once GameDraw is call
internal void * Create_GPU_IndicsBuffer(const char * BufferName , int PrimitiveType,int ComponentCount , int BufferSize )
{
    void * Result = CreateBufferForGPU(BufferName,PrimitiveType,ComponentCount,BufferSize);
    
    _ShaderBuffer * ShaderBuffer  = Get_GPU_BufferInfo(Result);
    
    ShaderBuffer->IsIndicesBuffer = true;
    glGenBuffers(1, &ShaderBuffer->BufferHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ShaderBuffer->BufferHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ShaderBuffer->BufferSize, 0, GL_DYNAMIC_DRAW);
    
    return Result; 
}

//opengl uniform data ,vertex buffer and indics buffer creation
//after all data are created, ShaderInit() will try bind buffers to shaders 
internal void RenderStateInit()
{
#define VERTEX_BUFFER_MAX 8192
#define VERTEX_BUFFER_INDICS_MAX 4096
    
    RenderState = {};
    
	RenderState.ShaderUniformName[SU_MVP] = "mvp";
	RenderState.ShaderUniformType[SU_MVP] = RL_SHADER_UNIFORM_MATRIX;
    
	RenderState.ShaderUniformName[SU_Texture0] = "texture0";
	RenderState.ShaderUniformType[SU_Texture0] = RL_SHADER_UNIFORM_SAMPLER2D;
    
	RenderState.ShaderUniformName[SU_ScreenSize] = "screenSize";
	RenderState.ShaderUniformType[SU_ScreenSize] = RL_SHADER_UNIFORM_VEC2;
	
	RenderState.ShaderUniformName[SU_GridSize] = "gridSize";
	RenderState.ShaderUniformType[SU_GridSize] = RL_SHADER_UNIFORM_FLOAT;
    
	RenderState.ShaderUniformName[SU_Spread] = "spread";
	RenderState.ShaderUniformType[SU_Spread] = RL_SHADER_UNIFORM_FLOAT;
    
	RenderState.ShaderUniformName[SU_ElapsedTime] = "elapsedTime";
	RenderState.ShaderUniformType[SU_ElapsedTime] = RL_SHADER_UNIFORM_FLOAT;
    
	//i don't like it
	for (int UniformIndex = 0; UniformIndex < SU_COUNT; UniformIndex++)
	{
		unsigned char** UniformData = RenderState.ShaderUniformData + UniformIndex;
        
		switch (RenderState.ShaderUniformType[UniformIndex])
		{
            case RL_SHADER_UNIFORM_MATRIX: *UniformData = AllocateTemp_(sizeof(Matrix)); break;
            case RL_SHADER_UNIFORM_SAMPLER2D: *UniformData = AllocateTemp_(sizeof(unsigned int)); break;
            case RL_SHADER_UNIFORM_INT: *UniformData = AllocateTemp_(sizeof(int)); break;
			case RL_SHADER_UNIFORM_VEC2: *UniformData = AllocateTemp_(sizeof(Vector2)); break;
			case RL_SHADER_UNIFORM_FLOAT: *UniformData = AllocateTemp_(sizeof(float)); break;
            
			default: ASSERT; break; //break from crashing
		}
	}
    
	//does using SOA better than AOS?
	//i can declare less things with AOS
    
	//is reusing buffer harmful?
	//i'm reusing buffer for all the shader for no reason
    
    //feel like nothing wrong can happen
    
    LightData.LightPosition = (Vector3*)Create_GPU_VertexBuffer("lightPosition",GL_FLOAT , 3 , MAX_LIGHT);
    LightData.LightColor = (Vector4*)Create_GPU_VertexBuffer("lightColor",GL_FLOAT , 4 , MAX_LIGHT);
    LightData.LightSize = (Vector2*)Create_GPU_VertexBuffer("lightSize",GL_FLOAT , 2 , MAX_LIGHT);
    LightData.LightMul = (float*)Create_GPU_VertexBuffer("lightMul",GL_FLOAT , 1 , MAX_LIGHT);
    
    //i don't know why they are revert
    
	//actually , reusing name is kinda bad
	//i can't belive it is working while doing thing wrong
    LightData.LightTexCoord[Vertex_BottomRight] = (Vector2*)Create_GPU_VertexBuffer("lightTexCoord_topLeft",GL_FLOAT , 2 , MAX_LIGHT);
    LightData.LightTexCoord[Vertex_TopRight] = (Vector2*)Create_GPU_VertexBuffer("lightTexCoord_topRight",GL_FLOAT , 2 , MAX_LIGHT);
    LightData.LightTexCoord[Vertex_BottomLeft] = (Vector2*)Create_GPU_VertexBuffer("lightTexCoord_bottomLeft",GL_FLOAT , 2 , MAX_LIGHT);
    LightData.LightTexCoord[Vertex_TopLeft] = (Vector2*)Create_GPU_VertexBuffer("lightTexCoord_bottomRight",GL_FLOAT , 2 , MAX_LIGHT);
	
    VertexData.Indices= (unsigned short *)Create_GPU_IndicsBuffer("indics and don't touch it",GL_UNSIGNED_SHORT,1,VERTEX_BUFFER_INDICS_MAX);
    
    VertexData.Position = (Vector3*)Create_GPU_VertexBuffer("vertexPosition",GL_FLOAT,3,VERTEX_BUFFER_MAX);
	VertexData.Rotation = (Vector4 * )Create_GPU_VertexBuffer("rotation",GL_FLOAT,4,VERTEX_BUFFER_MAX);
    VertexData.Color = (Vector4*)Create_GPU_VertexBuffer("vertexColor",GL_FLOAT,4,VERTEX_BUFFER_MAX);
    VertexData.TexCoord= (Vector2*)Create_GPU_VertexBuffer("vertexTexCoord",GL_FLOAT,2,VERTEX_BUFFER_MAX);
    
    VertexData.LineStart = (Vector3*)Create_GPU_VertexBuffer("lineStart",GL_FLOAT,3,VERTEX_BUFFER_MAX);
    VertexData.LineEnd = (Vector3*)Create_GPU_VertexBuffer("lineEnd",GL_FLOAT,3,VERTEX_BUFFER_MAX);
	VertexData.LineSize = (float*)Create_GPU_VertexBuffer("lineSize",GL_FLOAT,1,VERTEX_BUFFER_MAX);
	VertexData.FakeDepth = (float*)Create_GPU_VertexBuffer("fakeDepth",GL_FLOAT,1,VERTEX_BUFFER_MAX);
    
	VertexData.Size = (Vector2 *)Create_GPU_VertexBuffer("rectSize", GL_FLOAT, 2, VERTEX_BUFFER_MAX);
	VertexData.CornerRadius = (Vector4*)Create_GPU_VertexBuffer("rectRadius", GL_FLOAT, 4, VERTEX_BUFFER_MAX);
    
    VertexData.CornerColor[Vertex_BottomRight] = (Vector4*)Create_GPU_VertexBuffer("vertexColor_topLeft",GL_FLOAT , 4 , VERTEX_BUFFER_MAX);
	VertexData.CornerColor[Vertex_TopRight] = (Vector4*)Create_GPU_VertexBuffer("vertexColor_topRight",GL_FLOAT , 4 , VERTEX_BUFFER_MAX);
    VertexData.CornerColor[Vertex_BottomLeft] = (Vector4*)Create_GPU_VertexBuffer("vertexColor_bottomLeft",GL_FLOAT , 4 , VERTEX_BUFFER_MAX);
    VertexData.CornerColor[Vertex_TopLeft] = (Vector4*)Create_GPU_VertexBuffer("vertexColor_bottomRight",GL_FLOAT , 4 , VERTEX_BUFFER_MAX);
    
    VertexData.CornerTexCoord[Vertex_BottomRight] = (Vector2*)Create_GPU_VertexBuffer("vertexTexCoord_topLeft",GL_FLOAT , 2 , VERTEX_BUFFER_MAX);
    VertexData.CornerTexCoord[Vertex_TopRight] = (Vector2*)Create_GPU_VertexBuffer("vertexTexCoord_topRight",GL_FLOAT , 2 , VERTEX_BUFFER_MAX);
    VertexData.CornerTexCoord[Vertex_BottomLeft] = (Vector2*)Create_GPU_VertexBuffer("vertexTexCoord_bottomLeft",GL_FLOAT , 2 , VERTEX_BUFFER_MAX);
    VertexData.CornerTexCoord[Vertex_TopLeft] = (Vector2*)Create_GPU_VertexBuffer("vertexTexCoord_bottomRight",GL_FLOAT , 2 , VERTEX_BUFFER_MAX);
    
}

//it will compile all the shader and try bind buffer to it
//shaders are sort by using enum (S_Quad mean quad shader)
internal void ShaderInit()
{
    ShaderCompileFailed = false;
    
	GL_CATCH;
    
	AppData->DefaultFont.GlyphAtlasTexture = LoadTextureFromImage(AppData->DefaultFont.GlyphAtlasImage);
    
	GL_CATCH;
    
	//TODO : too long
	AllShaderInputs[S_Point_Light].ShaderSourceFileName[GS_Vert] = "..\\Code\\Shader\\point_light_vert.glsl";
	AllShaderInputs[S_Point_Light].ShaderSourceFileName[GS_Geo] = "..\\Code\\Shader\\point_light_geo.glsl";
	AllShaderInputs[S_Point_Light].ShaderSourceFileName[GS_Frag] = "..\\Code\\Shader\\point_light_frag.glsl";
    
	AllShaderInputs[S_Rect].ShaderSourceFileName[GS_Vert] = "..\\Code\\Shader\\rect_vert.glsl";
	AllShaderInputs[S_Rect].ShaderSourceFileName[GS_Geo] = "..\\Code\\Shader\\rect_geo.glsl";
	AllShaderInputs[S_Rect].ShaderSourceFileName[GS_Frag] = "..\\Code\\Shader\\rect_frag.glsl";
    
	AllShaderInputs[S_Quad].ShaderSourceFileName[GS_Vert] = "..\\Code\\Shader\\quad_vert.glsl";
	AllShaderInputs[S_Quad].ShaderSourceFileName[GS_Frag] = "..\\Code\\Shader\\quad_frag.glsl";
	
	AllShaderInputs[S_Text].ShaderSourceFileName[GS_Vert] = "..\\Code\\Shader\\text_vert.glsl";
	AllShaderInputs[S_Text].ShaderSourceFileName[GS_Frag] = "..\\Code\\Shader\\text_frag.glsl";
    
	AllShaderInputs[S_Line].ShaderSourceFileName[GS_Vert] = "..\\Code\\Shader\\line_vert.glsl";
	AllShaderInputs[S_Line].ShaderSourceFileName[GS_Frag] = "..\\Code\\Shader\\line_frag.glsl";
    
	AllShaderInputs[S_Bloom].ShaderSourceFileName[GS_Frag] = "..\\Code\\Shader\\bloom_frag.glsl";    
	AllShaderInputs[S_Bloom].ShaderSourceFileName[GS_Vert] = "..\\Code\\Shader\\bloom_vert.glsl";
    
	AllShaderInputs[S_Blur_H].ShaderSourceFileName[GS_Frag] = "..\\Code\\Shader\\blur_horizontal_frag.glsl";
	AllShaderInputs[S_Blur_H].ShaderSourceFileName[GS_Vert] = "..\\Code\\Shader\\blur_vert.glsl";
    
	AllShaderInputs[S_Blur_V].ShaderSourceFileName[GS_Frag] = "..\\Code\\Shader\\blur_vertical_frag.glsl";    
	AllShaderInputs[S_Blur_V].ShaderSourceFileName[GS_Vert] = "..\\Code\\Shader\\blur_vert.glsl";
    
	AllShaderInputs[S_Spring].ShaderSourceFileName[GS_Vert] = "..\\Code\\Shader\\quad_vert.glsl";
	AllShaderInputs[S_Spring].ShaderSourceFileName[GS_Frag] = "..\\Code\\Shader\\spring_frag.glsl";
    
	int ShaderTypeToGLShaderEnum[GS_Count] = {};
	ShaderTypeToGLShaderEnum[GS_Vert] = GL_VERTEX_SHADER;
	ShaderTypeToGLShaderEnum[GS_Geo] = GL_GEOMETRY_SHADER;
	ShaderTypeToGLShaderEnum[GS_Frag] = GL_FRAGMENT_SHADER;
    
	for (int ShaderIndex = 0; ShaderIndex < S_COUNT; ShaderIndex++)
	{
		_ShaderInput* CurrentShaderProperty = AllShaderInputs + ShaderIndex;
        CurrentShaderProperty->ShaderType = (D_ShaderType)ShaderIndex;
		unsigned int AllShaderType[GS_Count] = {};
        
		for (int ShaderTypeIndex= 0; ShaderTypeIndex < GS_Count; ShaderTypeIndex++)
		{
			if (!CurrentShaderProperty->ShaderSourceFileName[ShaderTypeIndex]) continue;
			
			char ShaderPath[256] = {};
			strcpy(ShaderPath, GetAppFilePath(CurrentShaderProperty->ShaderSourceFileName[ShaderTypeIndex]));
            
			int ShaderDataSize = 0;
			const char * ShaderCode = LoadFileText(ShaderPath);
            
			int GLShaderType = ShaderTypeToGLShaderEnum[ShaderTypeIndex];
            
			AllShaderType[ShaderTypeIndex] = glCreateShader(GLShaderType);
			glShaderSource(AllShaderType[ShaderTypeIndex], 1, &ShaderCode, NULL);
            
			GLint success = 0;
			glCompileShader(AllShaderType[ShaderTypeIndex]);
			glGetShaderiv(AllShaderType[ShaderTypeIndex], GL_COMPILE_STATUS, &success);
            
			if (success == GL_FALSE)
			{
				switch (GLShaderType)
				{
                    case GL_VERTEX_SHADER: printf("SHADER: [ID %i] Failed to compile vertex shader code\n", AllShaderType[ShaderTypeIndex]); break;
                    case GL_FRAGMENT_SHADER: printf("SHADER: [ID %i] Failed to compile fragment shader code\n", AllShaderType[ShaderTypeIndex]); break;
                    case GL_GEOMETRY_SHADER: printf("SHADER: [ID %i] Failed to compile geometry shader code\n", AllShaderType[ShaderTypeIndex]); break;
                    
                    default: break;
				}
                
				int maxLength = 0;
				glGetShaderiv(AllShaderType[ShaderTypeIndex], GL_INFO_LOG_LENGTH, &maxLength);
                
				if (maxLength > 0)
				{
					int length = 0;
					char* log = (char*)RL_CALLOC(maxLength, sizeof(char));
					glGetShaderInfoLog(AllShaderType[ShaderTypeIndex], maxLength, &length, log);
                    printf("\nShader Compile Fail\n");
					printf("SHADER: [ID %i] Compile error: %s", AllShaderType[ShaderTypeIndex], log);
					RL_FREE(log);
                    
					ShaderCompileFailed = true;
				}
			}
			else
			{
				switch (GLShaderType)
				{
                    case GL_VERTEX_SHADER: printf("SHADER: [ID %i] Vertex shader compiled successfully\n", AllShaderType[ShaderTypeIndex]); break;
                    case GL_FRAGMENT_SHADER: printf("SHADER: [ID %i] Fragment shader compiled successfully\n", AllShaderType[ShaderTypeIndex]); break;
                    case GL_GEOMETRY_SHADER:printf("SHADER: [ID %i] Geomerty shader compiled successfully\n", AllShaderType[ShaderTypeIndex]); break;
                    
                    default: break;
				}
			}
		}
        
		unsigned int Program = 0;
		GLint success = 0;
		Program = glCreateProgram();
        
		for (int ShaderTypeIndex=0; ShaderTypeIndex < GS_Count; ShaderTypeIndex++)
		{
			if (!AllShaderType[ShaderTypeIndex]) continue;
            
			glAttachShader(Program, AllShaderType[ShaderTypeIndex]);
		}
        
		glLinkProgram(Program);
        
		glGetProgramiv(Program, GL_LINK_STATUS, &success);
        
		if (success == GL_FALSE)
		{
			printf("SHADER: [ID %i] Failed to link shader program\n", Program);
            
			int maxLength = 0;
			glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &maxLength);
            
			if (maxLength > 0)
			{
				int length = 0;
				char* log = (char*)RL_CALLOC(maxLength, sizeof(char));
				glGetProgramInfoLog(Program, maxLength, &length, log);
				
                printf("\nShader Link Fail\n");
                
                printf("SHADER: [ID %i] Link error: %s\n", Program, log);
				RL_FREE(log);
			}
            
            ShaderCompileFailed =  true;
		}
		else
		{
			// Get the size of compiled shader program (not available on OpenGL ES 2.0)
			// NOTE: If GL_LINK_STATUS is GL_FALSE, program binary length is zero.
			//GLint binarySize = 0;
			//glGetProgramiv(id, GL_PROGRAM_BINARY_LENGTH, &binarySize);
            
			printf("SHADER: [ID %i] Program shader loaded successfully\n", Program);
		}
        
        if(ShaderCompileFailed)
        {
            break;
        }
        
		CurrentShaderProperty->Shader = Program;
		unsigned int CurrentShader = CurrentShaderProperty->Shader;
        
		for (int UniformIndex = 0; UniformIndex < SU_COUNT; UniformIndex++)
		{
			const char* UniformName = RenderState.ShaderUniformName[UniformIndex];
			CurrentShaderProperty->UniformLocation[UniformIndex] = glGetUniformLocation(CurrentShader, UniformName);
			GL_CATCH;
		}
        
        CurrentShaderProperty->VertexBufferLocationArray = AllocateTemp(int,ShaderBufferCount);
        
		for (int BufferIndex = 0; BufferIndex < ShaderBufferCount; BufferIndex++)
		{
			int LocationIndex = -1;
			_ShaderBuffer* CurrentBuffer = All_GPU_Buffers[BufferIndex];
            
			if (CurrentBuffer->IsUniformBuffer)
			{
				//i have no idea what you are
				int VerticesBlockIndex = glGetUniformBlockIndex(CurrentShader, "VerticesBlock");
                
				if (VerticesBlockIndex != -1)
				{
					glUniformBlockBinding(CurrentShader, VerticesBlockIndex, 0);
					glBindBufferBase(GL_UNIFORM_BUFFER, VerticesBlockIndex, CurrentBuffer->BufferHandle);
				}
			}
			else
			{
				if (CurrentBuffer->Name)
				{
					LocationIndex = glGetAttribLocation(CurrentShader, CurrentBuffer->Name);
				}
			}
            
			CurrentShaderProperty->VertexBufferLocationArray[BufferIndex] = LocationIndex;
		}
        
		glGenVertexArrays(1, &CurrentShaderProperty->VertexArrayObject);
		glBindVertexArray(CurrentShaderProperty->VertexArrayObject);
        
		for (int BufferIndex = 0; BufferIndex < ShaderBufferCount; BufferIndex++)
		{
			int BufferLocationIndex = CurrentShaderProperty->VertexBufferLocationArray[BufferIndex];
			
			if (BufferLocationIndex == -1) continue;
            
			_ShaderBuffer *CurrentBuffer = All_GPU_Buffers[BufferIndex];
            
			if (CurrentBuffer->IsUniformBuffer) continue;
            if(CurrentBuffer->IsIndicesBuffer) continue;
            
			glBindBuffer(GL_ARRAY_BUFFER, CurrentBuffer->BufferHandle);
			glEnableVertexAttribArray(BufferLocationIndex);
            
			glVertexAttribPointer(BufferLocationIndex, CurrentBuffer->ComponentsCount, CurrentBuffer->PrimitiveDataType, GL_FALSE, 0, 0);
		}
        
		GL_CATCH;
	}
    
	rlDisableVertexArray();
	rlDisableVertexBuffer();
	rlDisableVertexBufferElement();
}

//it will pass all buffer that create by Create_GPU_VertexBuffer to GPU implicitly 
internal void Update_GPU_VertexBufferAll(_ShaderInput * CurrentShader,int Count)
{
	for (int BufferIndex = 0; BufferIndex < ShaderBufferCount; BufferIndex++)
	{
        _ShaderBuffer* CurrentBuffer = All_GPU_Buffers[BufferIndex];
        
		if (CurrentShader->VertexBufferLocationArray[BufferIndex] == -1) 
        {
            if(CurrentBuffer->IsUniformBuffer) 
            {
				//not supported
                
				ASSERT;
                bool UpdateBuffer = false;
                if(CurrentShader->ShaderType == S_Rect) UpdateBuffer = true;
                
                if(!UpdateBuffer) continue;
            }
            else
            {
                continue;
            }
            
        }
		
        if(CurrentBuffer->IsIndicesBuffer) continue;
        
        int ComponentCount = CurrentBuffer->ComponentsCount;
        int DataCount = Count;
        int PrimtiveSize = CurrentBuffer->PrimitiveDataSize;
        
        int SizeToUpdate = ComponentCount * DataCount * PrimtiveSize;
        
        GL_CATCH;
        
        glBindBuffer(GL_ARRAY_BUFFER, CurrentBuffer->BufferHandle);
        glBufferSubData(GL_ARRAY_BUFFER, 0, SizeToUpdate, CurrentBuffer->Data);
        
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

internal void D_GameDraw()
{
	//rlEnableFramebuffer(RenderState.AfterGameDrawFrameTexture.id);
    
	GL_CATCH;
    
	_ShaderInput* CurrentShaderInput = 0;
    
	CurrentShaderInput = AllShaderInputs + RenderState.CurrentShaderInput;
	rlEnableShader(CurrentShaderInput->Shader);
	glBindVertexArray(CurrentShaderInput->VertexArrayObject);
    
	if (RenderState.CurrentShaderInput == S_Line)
	{
		glEnable(GL_DEPTH_TEST);
        
		glDepthMask(GL_TRUE);  
        
		glDepthFunc(GL_NOTEQUAL);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
    
	for (int UniformIndex = 0; UniformIndex < SU_COUNT; UniformIndex++)
	{
		unsigned char* UniformData = RenderState.ShaderUniformData[UniformIndex];
        
		if (!UniformData)
		{
			ASSERT;
		}
        
		switch (RenderState.ShaderUniformType[UniformIndex])
		{
            case RL_SHADER_UNIFORM_MATRIX: rlSetUniformMatrix(CurrentShaderInput->UniformLocation[UniformIndex], *(Matrix*)UniformData); break;
		}
	}
    
	GL_CATCH;
    
	glActiveTexture(GL_TEXTURE0);
    
	if (RenderState.CurrentShaderInput == S_Text)
	{
		if (AppData->UpdateAtlas)
		{
			if (AppData->DefaultFont.GlyphAtlasTexture.id == 0)
			{
				AppData->DefaultFont.GlyphAtlasTexture = LoadTextureFromImage(AppData->DefaultFont.GlyphAtlasImage);
			}
			else
			{
				//maybe you don't wanna commit the whole thing?
				UpdateTexture(AppData->DefaultFont.GlyphAtlasTexture, AppData->DefaultFont.GlyphAtlasImage.data);
			}
		}
		AppData->UpdateAtlas = false;
        
		glBindTexture(GL_TEXTURE_2D, AppData->DefaultFont.GlyphAtlasTexture.id);
	}
	else
	{
        if(RenderState.CurrentTexture)
        {
            glBindTexture(GL_TEXTURE_2D, RenderState.CurrentTexture);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, RenderState.AllTextures[1].id);
        }
    }
    
	glUniform1i(CurrentShaderInput->UniformLocation[SU_Texture0], 0);    
	glUniform1f(CurrentShaderInput->UniformLocation[SU_ElapsedTime] , RenderState.ElapsedTime);    	
	glUniform2f(CurrentShaderInput->UniformLocation[SU_ScreenSize] , AppData->WindowSize.x , AppData->WindowSize.y);
    
	GL_CATCH;
    
    Update_GPU_VertexBufferAll(CurrentShaderInput,CurrentShaderInput->VerticesCount);
    
	//i don't need to use index am i ? ):
	//yea....
    
	//does my rect need it?
	//no i think
    
	//why im using it at the first place???
    
	//rect shader need it because the shader was written in 4 vertics not 6
	//and overall it reduce size (4 bytes smaller if i wasn't mistaken?)
    
	switch (RenderState.CurrentShaderInput)
	{
        case S_Point_Light: break;
		case S_Rect: glDrawArrays(GL_POINTS, 0, CurrentShaderInput->VerticesCount); break;
        default:
        {
            _ShaderBuffer* IndicesBuffer = Get_GPU_BufferInfo(VertexData.Indices);
            GL_CATCH;
            
            rlUpdateVertexBufferElements(IndicesBuffer->BufferHandle, IndicesBuffer->Data, CurrentShaderInput->IndicesCount * sizeof(unsigned short), 0);
            
            GL_CATCH;
            glDrawElements(GL_TRIANGLES, CurrentShaderInput->IndicesCount, GL_UNSIGNED_SHORT, 0);
            GL_CATCH;
            break;
        }
	}
	
	rlDisableVertexArray();
	rlDisableVertexBuffer();
	rlDisableVertexBufferElement();
    
    CurrentShaderInput->IndicesCount = 0;
    CurrentShaderInput->VerticesCount = 0;
    
	GL_CATCH;
}

//TODO : Something wrong
internal _Quad RectToQuad(_Rect Rect)
{
	_Quad Quad = {};
	DrawQuad(GetBillboardQuad(Rect.Position, 0.1 , 0.1) , PURPLE);
    
	Quad.VertexPosition[Vertex_TopLeft].x -= Rect.Size.x * 0.5f;
	Quad.VertexPosition[Vertex_TopLeft].y += Rect.Size.y * 0.5f;
    
	Quad.VertexPosition[Vertex_TopRight].x += Rect.Size.x * 0.5f;
	Quad.VertexPosition[Vertex_TopRight].y += Rect.Size.y * 0.5f;
    
	Quad.VertexPosition[Vertex_BottomLeft].x -= Rect.Size.x * 0.5f;
	Quad.VertexPosition[Vertex_BottomLeft].y -= Rect.Size.y * 0.5f;
    
	Quad.VertexPosition[Vertex_BottomRight].x += Rect.Size.x * 0.5f;
	Quad.VertexPosition[Vertex_BottomRight].y -= Rect.Size.y * 0.5f;
    
	for (int VertexIndex = 0; VertexIndex < Vertex_Count; VertexIndex++)
	{
		Quad.VertexPosition[VertexIndex] = Vector3RotateByQuaternion(Quad.VertexPosition[VertexIndex] , Rect.Rotation);
        
		Quad.VertexPosition[VertexIndex].x += Rect.Position.x;
		Quad.VertexPosition[VertexIndex].y += Rect.Position.y;
		DrawQuad(GetBillboardQuad(Quad.VertexPosition[VertexIndex], 0.05 , 0.05) , RED);
        
	}
    
	return Quad;
}

internal _Quad RectToRhombus(_Rect Rect)
{
	_Quad Rhombus = {};
    
	Rhombus.VertexPosition[Vertex_TopLeft].y += Rect.Size.y * 0.5f;
	Rhombus.VertexPosition[Vertex_TopRight].x += Rect.Size.x * 0.5f;
	
	Rhombus.VertexPosition[Vertex_BottomLeft].x -= Rect.Size.x * 0.5f;
	Rhombus.VertexPosition[Vertex_BottomRight].y -= Rect.Size.y * 0.5f;
    
	for (int VertexIndex = 0; VertexIndex < Vertex_Count; VertexIndex++)
	{
		Rhombus.VertexPosition[VertexIndex] = Vector3RotateByQuaternion(Rhombus.VertexPosition[VertexIndex] , Rect.Rotation);
        
		Rhombus.VertexPosition[VertexIndex].x += Rect.Position.x;
		Rhombus.VertexPosition[VertexIndex].y += Rect.Position.y;
	}
    
	return Rhombus;
}

internal void QuadDrawCallCheck(int TargetTexture)
{
	bool CleanUpDrawState = false;
    
	if (RenderState.CurrentShaderInput != S_Quad)
	{
		CleanUpDrawState = true;
	}
	
	if (RenderState.CurrentTexture!= TargetTexture)
	{
		CleanUpDrawState = true;
	}
    
    if(RenderState.CurrentShaderInput == S_Quad)
    {
        //just check if it was the will too close
        if(AllShaderInputs[S_Quad].VerticesCount + 10 >=VERTEX_BUFFER_MAX)
        {
            CleanUpDrawState = true;
        }
        
        if(AllShaderInputs[S_Quad].IndicesCount + 10 >=VERTEX_BUFFER_INDICS_MAX)
        {
            CleanUpDrawState  = true;
        }
    }
    
	if (CleanUpDrawState)
	{
		D_GameDraw();
	}    
}

internal void DrawTriangle(Vector3* AllVertices ,Vector4 VertexColor)
{                     
	int TargetTexture = RenderState.AllTextures[0].id;
    
	QuadDrawCallCheck(TargetTexture);
    
	RenderState.CurrentShaderInput= S_Quad;
	RenderState.CurrentTexture = TargetTexture;
    
	_ShaderInput* ShaderInput = AllShaderInputs + S_Quad;	
    
	unsigned short VertexIndex[3] = {};
	
	int IndicsCount = ShaderInput->IndicesCount;
	ShaderInput->IndicesCount += 3;
    
	int VertexCount = ShaderInput->VerticesCount;    
	ShaderInput->VerticesCount+=3;
    
	Vector2 TextureCoord[3] = {};
    
    TextureCoord[Vertex_BottomLeft] = { 0,1 };
    TextureCoord[Vertex_TopLeft] = { 1,1 };
    TextureCoord[Vertex_TopRight] = { 1,0 };
    
	//i'm so confused with the vertics order     
	VertexIndex[0] = VertexCount;
	VertexData.Position[VertexCount] = AllVertices[0];
	VertexData.TexCoord[VertexCount] = TextureCoord[0];
	VertexData.Color[VertexCount] = VertexColor;
	VertexCount++;
    
	VertexIndex[1] = VertexCount;
	VertexData.Position[VertexCount] = AllVertices[1];
	VertexData.TexCoord[VertexCount] = TextureCoord[2];
	VertexData.Color[VertexCount] = VertexColor;
	VertexCount++;
    
	VertexIndex[2] = VertexCount;
	VertexData.Position[VertexCount] = AllVertices[2];
	VertexData.TexCoord[VertexCount] = TextureCoord[1];
	VertexData.Color[VertexCount] = VertexColor;
	VertexCount++;
    
	VertexData.Indices[IndicsCount++] = VertexIndex[0];
	VertexData.Indices[IndicsCount++] = VertexIndex[1];
	VertexData.Indices[IndicsCount++] = VertexIndex[2];
}

internal void DrawTriangleUp(_Quad Quad , Color TriangleColor )
{
	Vector3 VertexPosition[3] = {};
	VertexPosition[0] = Quad.VertexPosition[Vertex_TopLeft];
	VertexPosition[0].x += Quad.VertexPosition[Vertex_TopRight].x;	
	VertexPosition[0].y += Quad.VertexPosition[Vertex_TopRight].y;
    
	VertexPosition[0].x /= 2;
	VertexPosition[0].y /= 2;
    
	VertexPosition[1] = Quad.VertexPosition[Vertex_BottomRight];
	VertexPosition[2] = Quad.VertexPosition[Vertex_BottomLeft];
    
	DrawTriangle(VertexPosition,ColorToLinear(TriangleColor));
}

internal void DrawTriangleRight(_Quad Quad, Color TriangleColor)
{
	
	Vector3 VertexPosition[3] = {};
	VertexPosition[0] = Quad.VertexPosition[Vertex_TopLeft];
	
	VertexPosition[1] = Quad.VertexPosition[Vertex_BottomRight];
	VertexPosition[1].x += Quad.VertexPosition[Vertex_TopRight].x;	
	VertexPosition[1].y += Quad.VertexPosition[Vertex_TopRight].y;
    
	VertexPosition[1].x /= 2;
	VertexPosition[1].y /= 2;
    
    
	VertexPosition[2] = Quad.VertexPosition[Vertex_BottomLeft];
    
	DrawTriangle(VertexPosition,ColorToLinear(TriangleColor));
}

internal void DrawTriangleLeft(_Quad Quad, Color TriangleColor)
{
	
	Vector3 VertexPosition[3] = {};
	VertexPosition[0] = Quad.VertexPosition[Vertex_TopLeft];
	VertexPosition[0].x += Quad.VertexPosition[Vertex_BottomLeft].x;	
	VertexPosition[0].y += Quad.VertexPosition[Vertex_BottomLeft].y;
    
	VertexPosition[0].x /= 2;
	VertexPosition[0].y /= 2;
    
	VertexPosition[1] = Quad.VertexPosition[Vertex_BottomRight];
	VertexPosition[2] = Quad.VertexPosition[Vertex_TopRight];
    
	DrawTriangle(VertexPosition,ColorToLinear(TriangleColor));
}

internal void DrawTriangleBottom(_Quad Quad, Color TriangleColor)
{
	
	Vector3 VertexPosition[3] = {};
	VertexPosition[0] = Quad.VertexPosition[Vertex_BottomRight];
	VertexPosition[0].x += Quad.VertexPosition[Vertex_BottomLeft].x;	
	VertexPosition[0].y += Quad.VertexPosition[Vertex_BottomLeft].y;
    
	VertexPosition[0].x /= 2;
	VertexPosition[0].y /= 2;
    
	VertexPosition[1] = Quad.VertexPosition[Vertex_TopLeft];
	VertexPosition[2] = Quad.VertexPosition[Vertex_TopRight];
    
	DrawTriangle(VertexPosition,ColorToLinear(TriangleColor));
}

//use for drawing quadrilateral
internal void DrawQuadWithTexture(_Quad Quad,int TargetTexture)
{
    
	QuadDrawCallCheck(TargetTexture);
    
	RenderState.CurrentShaderInput= S_Quad;
	RenderState.CurrentTexture = TargetTexture;
    
	_ShaderInput* ShaderInput = AllShaderInputs + S_Quad;	
    
	unsigned short VertexIndex[4] = {};
	
	int IndicsCount = ShaderInput->IndicesCount;
	ShaderInput->IndicesCount += 6;
    
	int VertexCount = ShaderInput->VerticesCount;    
	ShaderInput->VerticesCount+=4;
    
	Vector3* AllVertices = Quad.VertexPosition;
	Vector4* VertexColor = Quad.VertexColor;
	Vector2 TextureCoord[4] = {};
    
    TextureCoord[Vertex_BottomLeft] = { 0,1 };
    TextureCoord[Vertex_TopLeft] = { 1,1 };
    TextureCoord[Vertex_TopRight] = { 1,0 };
    TextureCoord[Vertex_BottomRight] = { 0, 0 };
    
	//i'm so confused with the vertics order     
	VertexIndex[0] = VertexCount;
	VertexData.Position[VertexCount] = AllVertices[0];
	VertexData.TexCoord[VertexCount] = TextureCoord[0];
	VertexData.Color[VertexCount] = VertexColor[0];
	VertexCount++;
    
	VertexIndex[1] = VertexCount;
	VertexData.Position[VertexCount] = AllVertices[1];
	VertexData.TexCoord[VertexCount] = TextureCoord[2];
	VertexData.Color[VertexCount] = VertexColor[1];
	VertexCount++;
    
	VertexIndex[2] = VertexCount;
	VertexData.Position[VertexCount] = AllVertices[2];
	VertexData.TexCoord[VertexCount] = TextureCoord[1];
	VertexData.Color[VertexCount] = VertexColor[2];
	VertexCount++;
    
	VertexIndex[3] = VertexCount;
	VertexData.Position[VertexCount] = AllVertices[3];
	VertexData.TexCoord[VertexCount] = TextureCoord[3];
	VertexData.Color[VertexCount] = VertexColor[3];
	VertexCount++;
    
	VertexData.Indices[IndicsCount++] = VertexIndex[0];
	VertexData.Indices[IndicsCount++] = VertexIndex[1];
	VertexData.Indices[IndicsCount++] = VertexIndex[2];
	VertexData.Indices[IndicsCount++] = VertexIndex[3];
	VertexData.Indices[IndicsCount++] = VertexIndex[2];
	VertexData.Indices[IndicsCount++] = VertexIndex[1];
    
}

//you can switch game view to 2d world or screen
//it will draw everything remaining once you change view
internal void ChangeMatrix(Matrix MVP_Matrix)
{
	D_GameDraw();
    
	(*(Matrix*)RenderState.ShaderUniformData[SU_MVP]) = MVP_Matrix;
	AppData->MVP_Matrix = MVP_Matrix;
}

internal void _D_DrawRect(_Rect Rect, Vector4 CornerRadius, int TargetTexture , Vector4 * VertexColor , Vector2 * TextureCoord)
{
	Vector3 Position = Rect.Position;
	Vector2 Size = Rect.Size;
	Quaternion Rotation = Rect.Rotation;
    
	bool CleanUpDrawState = false;
    
	if (RenderState.CurrentShaderInput != S_Rect)
	{
		CleanUpDrawState = true;
	}
    
    if (RenderState.CurrentTexture != TargetTexture)
	{
		CleanUpDrawState = true;
	}
    
    if(RenderState.CurrentShaderInput == S_Rect)
    {
        if(AllShaderInputs[S_Rect].VerticesCount + 10 >= VERTEX_BUFFER_MAX)
        {
            CleanUpDrawState = true;
        }
        
        if(AllShaderInputs[S_Rect].IndicesCount + 10 >=VERTEX_BUFFER_INDICS_MAX)
        {
            CleanUpDrawState  = true;
        }
    }
    
	if (CleanUpDrawState)
	{
		D_GameDraw();
	}
    
	RenderState.CurrentShaderInput = S_Rect;
	RenderState.CurrentTexture = TargetTexture;
    
	_ShaderInput* ShaderInput = AllShaderInputs + S_Rect;
	int VertexCount = ShaderInput->VerticesCount++;
    
	VertexData.Position[VertexCount] = Rect.Position;
    
	//why it is negative?
	VertexData.Rotation[VertexCount] = QuaternionToVector4(Rect.Rotation);
	VertexData.Size[VertexCount] = Rect.Size;
	VertexData.CornerRadius[VertexCount] = CornerRadius;
	
	for (int VertexIndex = 0; VertexIndex < Vertex_Count; VertexIndex++)
	{
		VertexData.CornerTexCoord[VertexIndex][VertexCount] = TextureCoord[VertexIndex];
		VertexData.CornerColor[VertexIndex][VertexCount] = VertexColor[VertexIndex];
	}
}

#if 0
//use for drawing button or sprite
//CornerRadius mean the roundness of the rect corners
internal void Depercreted_D_DrawRect(_Rect Rect, Vector4 CornerRadius,int TargetTexture)
{
	Vector3 Position = Rect.Position;
	Vector2 Size = Rect.Size;
	Quaternion Rotation = Rect.Rotation;
	Vector4* VertexColor = Rect.Color;
	Vector2* TextureCoord = Rect.TextureCoord;
    
	Vector3 AllVertices[Vertex_Count] = {};
    
	AllVertices[Vertex_TopRight]= { Size.x / 2, Size.y / 2, 0 };
	AllVertices[Vertex_TopLeft]= { -Size.x / 2, Size.y / 2, 0 };
	AllVertices[Vertex_BottomRight]= { Size.x / 2, -Size.y / 2, 0 };
	AllVertices[Vertex_BottomLeft]= { -Size.x / 2, -Size.y / 2, 0 };
    
	AllVertices[Vertex_TopRight] = Vector3RotateByQuaternion(AllVertices[Vertex_TopRight], Rotation);
	AllVertices[Vertex_TopLeft] = Vector3RotateByQuaternion(AllVertices[Vertex_TopLeft], Rotation);
	AllVertices[Vertex_BottomRight] = Vector3RotateByQuaternion(AllVertices[Vertex_BottomRight], Rotation);
	AllVertices[Vertex_BottomLeft] = Vector3RotateByQuaternion(AllVertices[Vertex_BottomLeft], Rotation);
    
	AllVertices[Vertex_TopRight].x += Position.x;
	AllVertices[Vertex_TopRight].y += Position.y;
	AllVertices[Vertex_TopRight].z += Position.z;
    
	AllVertices[Vertex_TopLeft].x += Position.x;
	AllVertices[Vertex_TopLeft].y += Position.y;
	AllVertices[Vertex_TopLeft].z += Position.z;
    
	AllVertices[Vertex_BottomRight].x += Position.x;
	AllVertices[Vertex_BottomRight].y += Position.y;
	AllVertices[Vertex_BottomRight].z += Position.z;
    
	AllVertices[Vertex_BottomLeft].x += Position.x;
	AllVertices[Vertex_BottomLeft].y += Position.y;
	AllVertices[Vertex_BottomLeft].z += Position.z;
    
	bool CleanUpDrawState = false;
    
	if (RenderState.CurrentShaderInput != S_Rect)
	{
		CleanUpDrawState = true;
	}
    
    if (RenderState.CurrentTexture != TargetTexture)
	{
		CleanUpDrawState = true;
	}
    
    if(RenderState.CurrentShaderInput == S_Rect)
    {
        if(AllShaderInputs[S_Rect].VerticesCount + 10 >= VERTEX_BUFFER_MAX)
        {
            CleanUpDrawState = true;
        }
        
        if(AllShaderInputs[S_Rect].IndicesCount + 10 >=VERTEX_BUFFER_INDICS_MAX)
        {
            CleanUpDrawState  = true;
        }
    }
    
	if (CleanUpDrawState)
	{
		D_GameDraw();
	}
    
	RenderState.CurrentShaderInput = S_Rect;
	RenderState.CurrentTexture = TargetTexture;
    
	_ShaderInput* ShaderInput = AllShaderInputs + S_Rect;
    
	Vector3 * VertexArray = VertexData.Position;
	Vector4 
        Vector4[4] * ColorArray = VertexData.CornerColor;
	Vector2[4] * TextureCoordArray = VertexData.CornerTexCoord;
	int* VertexCount = &ShaderInput->VerticesCount;
    
	unsigned short* IndicesArray = VertexData.Indices;
	int* IndicsCount = &ShaderInput->IndicesCount;
    
	int VertexIndex[4] = {};
	
	VertexIndex[0] = *VertexCount;
	VertexArray[*VertexCount] = { AllVertices[0].x,AllVertices[0].y,AllVertices[0].z,CornerRadius.x };
	TextureCoordArray[*VertexCount] = TextureCoord[0];
	ColorArray[*VertexCount] = VertexColor[0];
	(*VertexCount)++;
    
	VertexIndex[1] = *VertexCount;
	VertexArray[*VertexCount] = { AllVertices[1].x,AllVertices[1].y,AllVertices[1].z,CornerRadius.y };
	TextureCoordArray[*VertexCount] = TextureCoord[2];
	ColorArray[*VertexCount] = VertexColor[1];
	(*VertexCount)++;
    
	VertexIndex[2] = *VertexCount;
	VertexArray[*VertexCount] = { AllVertices[2].x,AllVertices[2].y,AllVertices[2].z,CornerRadius.z };
	TextureCoordArray[*VertexCount] = TextureCoord[1];
	ColorArray[*VertexCount] = VertexColor[2];
	(*VertexCount)++;
    
	VertexIndex[3] = *VertexCount;
	VertexArray[*VertexCount] = { AllVertices[3].x,AllVertices[3].y,AllVertices[3].z,CornerRadius.w };
	TextureCoordArray[*VertexCount] = TextureCoord[3];
	ColorArray[*VertexCount] = VertexColor[3];
	(*VertexCount)++;
    
	IndicesArray[(*IndicsCount)++] = VertexIndex[0];
	IndicesArray[(*IndicsCount)++] = VertexIndex[1];
	IndicesArray[(*IndicsCount)++] = VertexIndex[2];
	IndicesArray[(*IndicsCount)++] = VertexIndex[3];
	IndicesArray[(*IndicsCount)++] = VertexIndex[2];
	IndicesArray[(*IndicsCount)++] = VertexIndex[1];
    
}
#endif

internal void DrawRect(_Rect Rect , Vector4 RectColor, int TextureIndex)
{
	int TextureCoordCount = 0;
    
	Vector4 ColorArray[4] = {};
	Vector2 TextureCoordArray[4] = {};
    
	TextureCoordArray[TextureCoordCount++] = { 1,0 };
	TextureCoordArray[TextureCoordCount++] = { 1,1 };
	TextureCoordArray[TextureCoordCount++] = { 0,0 };
	TextureCoordArray[TextureCoordCount++] = { 0,1 };
	
    for (int VertexIndex = 0; VertexIndex < Vertex_Count; VertexIndex++)
	{
		ColorArray[VertexIndex] = RectColor;
	}
    
    _D_DrawRect(Rect, { 0,0,0,0}, TextureIndex , ColorArray , TextureCoordArray);
}

internal void DrawRect(_Rect Rect , Vector4 RectColor)
{
	DrawRect(Rect , RectColor, RenderState.AllTextures[0].id);
}

internal void DrawRectWithTexture(_Rect Rect , Vector4 Corners, Color RectColor, int TextureIndex)
{
	int TextureCoordCount = 0;
    
	Vector4 ColorArray[4] = {};
	Vector2 TextureCoordArray[4] = {};
    
	TextureCoordArray[TextureCoordCount++] = { 1,0 };
	TextureCoordArray[TextureCoordCount++] = { 1,1 };
	TextureCoordArray[TextureCoordCount++] = { 0,0 };
	TextureCoordArray[TextureCoordCount++] = { 0,1 };
	
    for (int VertexIndex = 0; VertexIndex < Vertex_Count; VertexIndex++)
	{
		ColorArray[VertexIndex].x = ((float)RectColor.r /255.0);
		ColorArray[VertexIndex].y = ((float)RectColor.g /255.0);
		ColorArray[VertexIndex].z = ((float)RectColor.b /255.0);
		ColorArray[VertexIndex].w = ((float)RectColor.a /255.0);
	}
    
    _D_DrawRect(Rect, Corners, TextureIndex , ColorArray , TextureCoordArray);
}

internal void DrawRectWithTexture(_Rect Rect , Color RectColor, int Texture)
{
	DrawRectWithTexture(Rect , { 0, 0, 0, 0 }, RectColor , Texture);
}

internal void DrawQuadAsRect(_Quad Quad,int TargetTexture)
{
	DrawQuadWithTexture(Quad,TargetTexture);
}

internal void DrawQuadAsRect(_Quad *Quad, int TargetTexture)
{
	DrawQuadAsRect(*Quad, TargetTexture);
}

internal void DrawQuadAsRect(_Quad Quad,int TargetTexture,Color QuadColor)
{
    for (int VertexIndex = 0; VertexIndex < Vertex_Count; VertexIndex++)
	{
		Quad.VertexColor[VertexIndex].x = ((float)QuadColor.r /255.0);
		Quad.VertexColor[VertexIndex].y = ((float)QuadColor.g /255.0);
		Quad.VertexColor[VertexIndex].z = ((float)QuadColor.b /255.0);
		Quad.VertexColor[VertexIndex].w = ((float)QuadColor.a /255.0);
	}
    
    DrawQuadAsRect(Quad, TargetTexture);
}

internal void DrawQuadAsRect(_Quad *Quad , int TargetTexture , Color QuadColor)
{
    DrawQuadAsRect( *Quad , TargetTexture , QuadColor);
}

internal void DrawQuad(_Quad* Quad, int TargetTexture)
{
	DrawQuadWithTexture(*Quad, TargetTexture);
}

internal void DrawQuad(_Quad* Quad)
{
	DrawQuadWithTexture(*Quad,RenderState.AllTextures[0].id);
}

internal void DrawQuad(Vector3 Position , float Size , Quaternion Rotation, Vector4 QuadColor )
{
	_Quad Quad = {};
    
	Quad.VertexPosition[Vertex_TopLeft].x -= Size * 0.5f;	
	Quad.VertexPosition[Vertex_TopLeft].y += Size * 0.5f;
	
	Quad.VertexPosition[Vertex_TopRight].x += Size * 0.5f;	
	Quad.VertexPosition[Vertex_TopRight].y += Size * 0.5f;
    
	Quad.VertexPosition[Vertex_BottomLeft].x -= Size * 0.5f;	
	Quad.VertexPosition[Vertex_BottomLeft].y -= Size * 0.5f;
    
	Quad.VertexPosition[Vertex_BottomRight].x += Size * 0.5f;	
	Quad.VertexPosition[Vertex_BottomRight].y -= Size * 0.5f;
    
	for (int VertexIndex = 0; VertexIndex < Vertex_Count; VertexIndex++)
	{
		Quad.VertexPosition[VertexIndex] = Vector3RotateByQuaternion(Quad.VertexPosition[VertexIndex] , Rotation);
		Quad.VertexPosition[VertexIndex] = Vector3Add(Quad.VertexPosition[VertexIndex] , Position);
		Quad.VertexColor[VertexIndex] = QuadColor;
	}
    
	DrawQuadWithTexture(Quad,RenderState.AllTextures[0].id);
}

internal void DrawQuad(_Quad Quad, Color QuadColor)
{
	for (int ColorIndex = 0; ColorIndex < Vertex_Count; ColorIndex++)
	{
		Quad.VertexColor[ColorIndex].x = ((float)QuadColor.r /255.0);
		Quad.VertexColor[ColorIndex].y = ((float)QuadColor.g /255.0);
		Quad.VertexColor[ColorIndex].z = ((float)QuadColor.b /255.0);
		Quad.VertexColor[ColorIndex].w = ((float)QuadColor.a /255.0);
	}
    
	DrawQuadWithTexture(Quad,RenderState.AllTextures[0].id);
}

internal void DrawQuad(_Quad * Quad, Color QuadColor)
{
	DrawQuad(*Quad, QuadColor);
}


internal void DrawRect(_Rect Rect, float CornerRoundness)
{
	DrawRectWithTexture(Rect  , { CornerRoundness, CornerRoundness, CornerRoundness, CornerRoundness }, WHITE, RenderState.AllTextures[0].id);
}

internal void DrawRect(_Rect Rect, float CornerRoundness, Color TargetColor)
{
	DrawRectWithTexture(Rect  , { CornerRoundness, CornerRoundness, CornerRoundness, CornerRoundness }, TargetColor, RenderState.AllTextures[0].id);
}

internal void DrawCircle(_Rect Rect, Color TargetColor)
{
	DrawRect(Rect, Rect.Size.x * 0.5f, TargetColor);
}

internal _Rect GetBillboardRect( Vector3 Position , float Size )
{
    _Rect Rect = {};
	Rect.Position = Position;
	Rect.Size = { Size,Size };
    Rect.Rotation = Editor->BillboardRotation;
    
    return Rect;
}

internal void DrawBillboardCircle(Vector3 Position , float Size, Color TargetColor)
{
    _Rect Rect = GetBillboardRect(Position , Size);
	DrawCircle(Rect, TargetColor);
}

internal void DrawCircle(Vector3 Position,float Size, Color TargetColor)
{
	_Rect Rect = GetRect();
	Rect.Position = Position;
	Rect.Size = { Size,Size };
    
	DrawCircle(Rect, TargetColor);
}

internal void DrawCircle(Vector2 Position,float Size, Color TargetColor)
{
    DrawCircle({Position.x,Position.y,0},Size,TargetColor);
}

internal void DrawCircle(Vector2 Position,float Size, Vector4 TargetColor)
{
	DrawCircle(Position , Size, LinearToColor(TargetColor));
}

internal void DrawCircle(Vector3 Position,float Size, Vector4 TargetColor)
{
	DrawCircle(Position , Size, LinearToColor(TargetColor));
}

internal void DrawLineQuad(_Quad Quad , Vector3 LineStart , Vector3 LineEnd , float LineSize)
{
    
	bool CleanUpDrawState = false;
    
	if (RenderState.CurrentShaderInput != S_Line)
	{
		CleanUpDrawState = true;
	}
	
	if (RenderState.CurrentTexture!= RenderState.AllTextures[0].id)
	{
		CleanUpDrawState = true;
	}
    
    if(RenderState.CurrentShaderInput == S_Line)
    {
        //just check if it was the will too close
        if(AllShaderInputs[S_Line].VerticesCount + 20 >=VERTEX_BUFFER_MAX)
        {
            CleanUpDrawState = true;
        }
        
        if(AllShaderInputs[S_Line].IndicesCount + 20 >=VERTEX_BUFFER_INDICS_MAX)
        {
            CleanUpDrawState  = true;
        }
    }
    
    //it seem depth can't go more than 1.0?
    //is depth normalized?
    if(RenderState.FakeDepth > 0.99f)
    {
        CleanUpDrawState = true;
        glClear(GL_DEPTH_BUFFER_BIT);
        RenderState.FakeDepth = 0;
    }
    
	if (CleanUpDrawState)
	{
		D_GameDraw();
	}    
    
    
	RenderState.CurrentShaderInput= S_Line;
	RenderState.CurrentTexture = RenderState.AllTextures[0].id ;
    
	_ShaderInput* ShaderInput = AllShaderInputs + S_Line;	
    
	unsigned short VertexIndex[4] = {};
	
	int IndicsCount = ShaderInput->IndicesCount;
	ShaderInput->IndicesCount += 6;
    
	int VertexCount = ShaderInput->VerticesCount;    
	ShaderInput->VerticesCount+=4;
    
	RenderState.FakeDepth += 0.001f;
    
	Vector3* AllVertices = Quad.VertexPosition;
	Vector4* VertexColor = Quad.VertexColor;
	Vector2 TextureCoord[4] = {};
    
    TextureCoord[Vertex_BottomLeft] = { 0,1 };
    TextureCoord[Vertex_TopLeft] = { 1,1 };
    TextureCoord[Vertex_TopRight] = { 1,0 };
    TextureCoord[Vertex_BottomRight] = { 0, 0 };
    
	//i'm so confused with the vertics order     
	VertexIndex[0] = VertexCount;
	VertexData.Position[VertexCount] = AllVertices[0];
	VertexData.TexCoord[VertexCount] = TextureCoord[0];
	VertexData.Color[VertexCount] = VertexColor[0];
	VertexData.LineStart[VertexCount] = LineStart;
	VertexData.LineEnd[VertexCount] = LineEnd;
	VertexData.LineSize[VertexCount] = LineSize;
	VertexData.FakeDepth[VertexCount] = RenderState.FakeDepth;
	VertexCount++;
    
	VertexIndex[1] = VertexCount;
	VertexData.Position[VertexCount] = AllVertices[1];
	VertexData.TexCoord[VertexCount] = TextureCoord[2];
	VertexData.Color[VertexCount] = VertexColor[1];
	VertexData.LineStart[VertexCount] = LineStart;
	VertexData.LineEnd[VertexCount] = LineEnd;
	VertexData.LineSize[VertexCount] = LineSize;
	VertexData.FakeDepth[VertexCount] = RenderState.FakeDepth;
	VertexCount++;
    
	VertexIndex[2] = VertexCount;
	VertexData.Position[VertexCount] = AllVertices[2];
	VertexData.TexCoord[VertexCount] = TextureCoord[1];
	VertexData.Color[VertexCount] = VertexColor[2];
	VertexData.LineStart[VertexCount] = LineStart;
	VertexData.LineEnd[VertexCount] = LineEnd;
	VertexData.LineSize[VertexCount] = LineSize;
	VertexData.FakeDepth[VertexCount] = RenderState.FakeDepth;
	VertexCount++;
    
	VertexIndex[3] = VertexCount;
	VertexData.Position[VertexCount] = AllVertices[3];
	VertexData.TexCoord[VertexCount] = TextureCoord[3];
	VertexData.Color[VertexCount] = VertexColor[3];
	VertexData.LineStart[VertexCount] = LineStart;
	VertexData.LineEnd[VertexCount] = LineEnd;
	VertexData.LineSize[VertexCount] = LineSize;
	VertexData.FakeDepth[VertexCount] = RenderState.FakeDepth;
	VertexCount++;
    
	VertexData.Indices[IndicsCount++] = VertexIndex[0];
	VertexData.Indices[IndicsCount++] = VertexIndex[1];
	VertexData.Indices[IndicsCount++] = VertexIndex[2];
	VertexData.Indices[IndicsCount++] = VertexIndex[3];
	VertexData.Indices[IndicsCount++] = VertexIndex[2];
	VertexData.Indices[IndicsCount++] = VertexIndex[1];
}

internal void DrawLineQuad(_Quad Quad , Color QuadColor , Vector3 LineStart , Vector3 LineEnd , float LineSize)
{
	
	Vector4 QuadLinearColor = ColorToLinear(QuadColor);
	
	for (int VertexIndex = 0; VertexIndex < Vertex_Count; VertexIndex++)
	{
		Quad.VertexColor[VertexIndex] = QuadLinearColor;
	}
    
	DrawLineQuad(Quad , LineStart , LineEnd , LineSize);
}

//add a point into game world
//to draw a line :
//AddPointToLine(Start , WHITE);
//AddPointToLine(End , WHITE);

//you can have gradient color by changing color between point

//and vary size!

internal void AddPointToLine(Vector3 Point, Vector4 PointColor , float PointSize)
{
    
	localPersist bool HavePreviousPoint = false;
    
	localPersist Vector3 PreviousPoint = {};
	localPersist Vector4 PreviousPointColor = {};
	localPersist float PreviousPointSize = 0;
    
	if (!HavePreviousPoint)
	{
        
		HavePreviousPoint = true;
		PreviousPoint = Point;
		PreviousPointColor = PointColor;
		PreviousPointSize = PointSize;
        
	}
	else
	{
		float PreviousPointSizeWidth  = (PreviousPointSize*0.5) / AppData->WindowSize.x;
		float PreviousPointSizeHeight  = (PreviousPointSize*0.5) / AppData->WindowSize.y;
		
		float PointSizeWidth  = (PointSize*0.5) / AppData->WindowSize.x;
		float PointSizeHeight  = (PointSize*0.5) / AppData->WindowSize.y;
        
		HavePreviousPoint = false;
        
		_Quad Line = {};
        
		Vector3 LineDirection = Point;
		LineDirection = Vector3Subtract(LineDirection , PreviousPoint);
		
		Vector3 LineVerticalDirection = { -LineDirection.y , LineDirection.x };
		LineVerticalDirection = Vector3Normalize(LineVerticalDirection);
        
		Vector3 StartPointOffset = {LineVerticalDirection.x * PreviousPointSizeWidth , LineVerticalDirection.y * PreviousPointSizeHeight };
        
		Line.VertexPosition[Vertex_TopLeft] = Vector3Add( PreviousPoint , StartPointOffset );
		Line.VertexPosition[Vertex_BottomLeft] = Vector3Subtract( PreviousPoint , StartPointOffset );
        
		Line.VertexColor[Vertex_TopLeft] = PreviousPointColor;
		Line.VertexColor[Vertex_BottomLeft] = PreviousPointColor;
        
		Vector3 EndPointOffset = {LineVerticalDirection.x * PointSizeWidth , LineVerticalDirection.y * PointSizeHeight };
		
		Line.VertexPosition[Vertex_TopRight] = Vector3Add( Point , EndPointOffset );
		Line.VertexPosition[Vertex_BottomRight] = Vector3Subtract( Point , EndPointOffset );
        
		Line.VertexColor[Vertex_TopRight] = PointColor;
		Line.VertexColor[Vertex_BottomRight] = PointColor;
        
		DrawLineQuad( Line , PreviousPoint,Point ,PointSize);
        
	}
}

internal void AddPointToLine(Vector3 Point, Color PointColor , float PointSize)
{
    AddPointToLine(Point,ColorToLinear(PointColor) , PointSize);
}

internal void AddPointToLine(Vector3 Point, Color PointColor)
{
	AddPointToLine(Point, PointColor , 0.01f);
}

internal void AddPointToLine2D(Vector2 Point, Vector4 PointColor,float PointSize)
{
	AddPointToLine({ Point.x, Point.y,0 }, PointColor , PointSize);
}

internal void AddPointToLine2D(Vector2 Point, Vector4 PointColor)
{
	AddPointToLine2D(Point, PointColor, 0.01f);
}

internal void AddPointToLine2D(Vector2 Point, Color PointColor , float PointSize)
{
    AddPointToLine({ Point.x, Point.y,0 }, ColorToLinear(PointColor) , PointSize);
}

internal void AddPointToLine2D(Vector2 Point, Color PointColor)
{
	AddPointToLine2D(Point, PointColor , 0.01f);
}

internal unsigned int LoadDepthTexture(int width, int height)
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

//steal from someone else

//am i high?
internal Vector3 isect_line_plane_v3(Vector3 p0, Vector3 p1, Vector3 p_co,Vector3  p_no)
{
    
    //p0, p1: Define the line.
    //p_co, p_no: define the plane:
	//p_co Is a point on the plane (plane coordinate).
	//p_no Is a normal vector defining the plane direction;
	//(does not need to be normalized).
    
	//Return a Vector or None (when the intersection can't be found).
    
	Vector3 u = Vector3Subtract(p1, p0);
	float dot = Vector3DotProduct(p_no, u);
	if (dot < 0) dot *= -1;
    
	if (dot > EPSILON)
	{
		
		//The factor of the point between p0 -> p1 (0 - 1)
		//if 'fac' is between (0 - 1) the point intersects with the segment.
		//Otherwise:
		// < 0.0: behind p0.
		// > 1.0: infront of p1.
		Vector3 w = Vector3Subtract(p0, p_co);
		float fac = -Vector3DotProduct(p_no, w) / dot;
		u = Vector3Scale(u, fac);
        
		return Vector3Add(p0, u);
	}
    
	//The segment is parallel to plane.
	return {FLT_MAX , FLT_MAX ,FLT_MAX };
}

//so far, solving equations seem to be better than using vector math

//im confused tho, the equation tell me the length of plane normal doesn't matter
internal float GetLineIntersectWithPlaneTime(Vector3 Start , Vector3 End , Vector3 PlaneNormal , Vector3 PlaneOrigin)
{
	float Result = PlaneNormal.x * (PlaneOrigin.x - Start.x) + PlaneNormal.y * (PlaneOrigin.y - Start.y) + PlaneNormal.z * (PlaneOrigin.z - Start.z);
	Result /= PlaneNormal.x * (End.x - Start.x) + PlaneNormal.y * (End.y - Start.y) + PlaneNormal.z * (End.z - Start.z);
    
	return Result;
}

//steal from raylib and modified
internal Vector3 TransformvectorDeviceCoord(Vector3 position , Matrix MVP)
{
    
    // Convert world position vector to quaternion
    Quaternion worldPos = { position.x, position.y, position.z, 1.0f };
    
    // Transform world position to view
    worldPos = QuaternionTransform(worldPos, MVP);
    
	//if (worldPos.w < 0) worldPos.w = - worldPos.w ;
    
    // Calculate normalized device coordinates (inverted y)
    
	//it was missing this
    Vector3 ndcPos = { worldPos.x/worldPos.w, worldPos.y/worldPos.w, worldPos.z/worldPos.w };
    
    // Calculate 2d screen position vector
    float width = AppData->WindowSize.x;
    float height = AppData->WindowSize.y;
    
    //Vector3 screenPosition = { (ndcPos.x + 1.0f)/2.0f*(float)width, (ndcPos.y + 1.0f)/2.0f*(float)height , ndcPos.z};
    
    return ndcPos;
}

internal Vector3 TransformVector(Vector3 Position , Matrix TransformMatrix)
{
    Vector3 Result = TransformvectorDeviceCoord( Position , TransformMatrix);
    Result.x *= (AppData->WindowSize.x / AppData->WindowSize.y);
    
    return Result;
}

internal float RemovePrespective(Vector3 q)
{
	return MVP_3DWorld.m3*q.x + MVP_3DWorld.m7*q.y + MVP_3DWorld.m11*q.z + MVP_3DWorld.m15;
}

internal _Quad GetBillboardQuad(Vector3 Point, float Width , float Height)
{
	_Quad BillboardQuad = {};
	BillboardQuad.VertexPosition[Vertex_TopLeft] = { -Width * 0.5f , Height * 0.5f};
	BillboardQuad.VertexPosition[Vertex_TopRight] = { Width * 0.5f , Height * 0.5f};
	BillboardQuad.VertexPosition[Vertex_BottomLeft] = { -Width * 0.5f , -Height * 0.5f};
	BillboardQuad.VertexPosition[Vertex_BottomRight] = {Width * 0.5f , -Height * 0.5f};
    
	Vector3 Up = { ViewMatrix.m1 , ViewMatrix.m5 , ViewMatrix.m9 };
	Vector3 Right = {ViewMatrix.m0 , ViewMatrix.m4 , ViewMatrix.m8 };
    
	//what if i multiply it first lol
	//TODO : fix it
    
	Up = Vector3Scale(Up , RemovePrespective(Up));
	Right = Vector3Scale(Right, RemovePrespective(Right));
    
	for (int VertexIndex = 0; VertexIndex < Vertex_Count; VertexIndex++)
	{
        
		Vector3 * CurrentVertex = BillboardQuad.VertexPosition + VertexIndex;
        
		Vector3 RightVector = Vector3Scale(Right , CurrentVertex->x );
		Vector3 UpVector = Vector3Scale(Up , CurrentVertex->y );
        
		(*CurrentVertex) = Vector3Add(RightVector, UpVector);
        
		CurrentVertex->x += Point.x;
		CurrentVertex->y += Point.y;
		CurrentVertex->z += Point.z;		
	}
    
	return BillboardQuad;
}

internal void DrawRoundLine(Vector3 Start , Vector3 End , float Size , Color LineColor)
{
    DrawRoundLine(Start , End ,  Size , LineColor , LineColor);
}

internal void DrawRoundLine(Vector3 Start , Vector3 End , float Size , Color StartColor , Color EndColor)
{
    
	Vector3 PlaneOrigin = AppData->GameCamera.position;
	Vector3 PlaneNormal = Vector3Subtract(AppData->GameCamera.target , AppData->GameCamera.position);
    
	Vector3 StartDirection = Vector3Subtract(Start , PlaneOrigin );
	Vector3 EndDirection = Vector3Subtract(End , PlaneOrigin );
    
	if (Vector3DotProduct(StartDirection  , PlaneNormal) < 0)
	{
		if (Vector3DotProduct(EndDirection  , PlaneNormal) < 0)
		{
			return;
		}
	}
    
	float IntersectTime = GetLineIntersectWithPlaneTime(Start , End, PlaneNormal , PlaneOrigin);
    
	if (IntersectTime < 1)
	{
		if (IntersectTime > 0)
		{
			
#if 1
			if (Vector3DotProduct(StartDirection  , PlaneNormal) < 0)
			{
				Start = End;
				IntersectTime = 1.0 - IntersectTime;
                
				Color TempColor = EndColor;
				EndColor = StartColor;
				StartColor = EndColor;
			}
            
			IntersectTime *= 0.9f;
			Vector3 IntersectPoint = Vector3Add(Start , Vector3Scale( Vector3Subtract(End , Start) , IntersectTime) );
            
			//printf("Intersect : %f %f %f \n" , IntersectPoint.x , IntersectPoint.y , IntersectPoint.z);
			//printf("End : %f %f %f \n" , End.x , End.y , End.z);
            
			End = IntersectPoint;
#endif
		}
	}
    
	//what am i doing?
	Vector3 ScreenStart = TransformvectorDeviceCoord(Start , AppData->MVP_Matrix);
	Vector3 ScreenEnd = TransformvectorDeviceCoord(End ,AppData->MVP_Matrix);
    
	//ScreenStart = PixelToWorldNoTransform(ScreenStart);
	//ScreenEnd = PixelToWorldNoTransform(ScreenEnd);
    
	Start = {ScreenStart.x , ScreenStart.y , 0};
	End = {ScreenEnd.x , ScreenEnd.y , 0};
    
	AddPointToLine(Start , StartColor, Size);
	AddPointToLine(End , EndColor, Size);
    
	float LineHeight = Size / AppData->WindowSize.y;
	float LineWidth = Size / AppData->WindowSize.x;
    
	_Quad SquareQuad = {};
	SquareQuad.VertexPosition[Vertex_TopLeft] = { -LineWidth * 0.5f , LineHeight * 0.5f};
	SquareQuad.VertexPosition[Vertex_TopRight] = { LineWidth * 0.5f , LineHeight * 0.5f};
	SquareQuad.VertexPosition[Vertex_BottomLeft] = { -LineWidth * 0.5f , -LineHeight * 0.5f};
	SquareQuad.VertexPosition[Vertex_BottomRight] = {LineWidth * 0.5f , -LineHeight * 0.5f};
    
	//this look kinda cool	
#if 0
	Vector3 StartCameraDirection = Vector3Subtract(AppData->GameCamera.position , Start);
	StartCameraDirection = Vector3Normalize(StartCameraDirection);
	Quaternion StartRotation = QuaternionFromVector3ToVector3({0,0,-1}, StartCameraDirection);
#endif
    
	Vector3 Right = { 1 , 0, 0};
	Vector3 Up = {0, 1, 0};
    
	_Quad StartQuad = SquareQuad;
    
	for (int VertexIndex = 0; VertexIndex < Vertex_Count; VertexIndex++)
	{
        
		Vector3 * CurrentVertex = StartQuad.VertexPosition + VertexIndex;
        
		Vector3 RightVector = Vector3Scale(Right , CurrentVertex->x);
		Vector3 UpVector = Vector3Scale(Up , CurrentVertex->y );
        
		(*CurrentVertex) = Vector3Add(RightVector, UpVector);
        
		//(*CurrentVertex) = Vector3Transform((*CurrentVertex) , StartViewMatrix );
		//(*CurrentVertex) = Vector3RotateByQuaternion( (*CurrentVertex) , StartRotation);
        
		CurrentVertex->x += Start.x;
		CurrentVertex->y += Start.y;
		CurrentVertex->z += Start.z;
		
	}
	
	DrawLineQuad(StartQuad, StartColor , Start , End , Size);
	//DrawCircle(Start , Size , LineColor);
	
	_Quad EndQuad = SquareQuad;
    
	for (int VertexIndex = 0; VertexIndex < Vertex_Count; VertexIndex++)
	{
        
		Vector3 * CurrentVertex = EndQuad.VertexPosition + VertexIndex;
        
		Vector3 RightVector = Vector3Scale(Right , CurrentVertex->x);
		Vector3 UpVector = Vector3Scale(Up , CurrentVertex->y );
        
		(*CurrentVertex) = Vector3Add(RightVector, UpVector);
        
		//(*CurrentVertex) = Vector3Transform((*CurrentVertex) , StartViewMatrix );
		//(*CurrentVertex) = Vector3RotateByQuaternion( (*CurrentVertex) , StartRotation);
        
		CurrentVertex->x += End.x;
		CurrentVertex->y += End.y;
		CurrentVertex->z += End.z;
		
	}
    
	DrawLineQuad(EndQuad, EndColor , Start ,End , Size);
	//DrawCircle(End , Size , LineColor);
    
}

//this thing create 8 drawcall total
internal void DrawRectLine(Vector2 StartPosition , Vector2 EndPosition , Vector4 LineColor)
{
	float PointSize = 0.005f;
	float LineSize = 0.005f;
    
	AddPointToLine2D(StartPosition, LineColor, LineSize);
	AddPointToLine2D({ EndPosition.x ,StartPosition.y }, LineColor, LineSize);
    
	AddPointToLine2D({ EndPosition.x ,StartPosition.y }, LineColor, LineSize);
	AddPointToLine2D(EndPosition, LineColor, LineSize);
    
    
	AddPointToLine2D(StartPosition, LineColor, LineSize);
	AddPointToLine2D({ StartPosition.x,EndPosition.y }, LineColor, LineSize);
    
	AddPointToLine2D({ StartPosition.x,EndPosition.y }, LineColor, LineSize);
	AddPointToLine2D(EndPosition, LineColor, LineSize);	
    
	DrawCircle({ EndPosition.x ,StartPosition.y ,0} , PointSize ,  LineColor);
	DrawCircle(EndPosition , PointSize , LineColor);
	DrawCircle({ StartPosition.x,EndPosition.y ,0}, PointSize , LineColor);
	DrawCircle(StartPosition  , PointSize , LineColor);
}

//this also create 8 drawcall total
internal void DrawQuadLine(_Quad Quad , Color LineColor , float LineSize)
{
    
    Vector3 TopRight = Quad.VertexPosition[Vertex_TopRight];
    Vector3 TopLeft = Quad.VertexPosition[Vertex_TopLeft];
    Vector3 BottomRight = Quad.VertexPosition[Vertex_BottomRight];
    Vector3 BottomLeft = Quad.VertexPosition[Vertex_BottomLeft];
    
	DrawRoundLine(TopRight , TopLeft ,LineSize , LineColor , LineColor);
	DrawRoundLine(TopLeft , BottomLeft ,LineSize , LineColor , LineColor);
	DrawRoundLine(BottomLeft , BottomRight ,LineSize , LineColor , LineColor);
	DrawRoundLine(BottomRight , TopRight ,LineSize , LineColor , LineColor);
}


internal void DrawRectLine(Vector2 StartPosition , Vector2 EndPosition,Color LineColor)
{
    DrawRectLine(StartPosition,EndPosition,ColorToLinear(LineColor));
}

internal void DrawRect(Vector3 Position,float Size, Color TargetColor , Quaternion Rotation)
{
	_Rect Rect = GetRect();
	Rect.Position = Position;
	Rect.Size = { Size,Size };
	Rect.Rotation = Rotation;
    
	DrawRect(Rect, 0 , TargetColor);
}

internal void DrawRect(Vector3 Position, float Size, Vector4 TargetColor , Quaternion Rotation)
{
	DrawRect(Position , Size ,LinearToColor(TargetColor) , Rotation);
}

internal void DrawRectLine(_Rect Rect ,Vector4 LineColor , float LineSize)
{
	Vector3 RectVertex[Vertex_Count] = {};
    
	RectVertex[Vertex_TopLeft].x -= Rect.Size.x * 0.5f;
	RectVertex[Vertex_TopLeft].y += Rect.Size.y * 0.5f;
	
	RectVertex[Vertex_TopRight].x += Rect.Size.x * 0.5f;
	RectVertex[Vertex_TopRight].y += Rect.Size.y * 0.5f;
	
	RectVertex[Vertex_BottomLeft].x -= Rect.Size.x * 0.5f;
	RectVertex[Vertex_BottomLeft].y -= Rect.Size.y * 0.5f;
	
	RectVertex[Vertex_BottomRight].x += Rect.Size.x * 0.5f;
	RectVertex[Vertex_BottomRight].y -= Rect.Size.y * 0.5f;
    
	for (int VertexIndex = 0 ; VertexIndex < Vertex_Count ; VertexIndex++)
	{
		RectVertex[VertexIndex] = Vector3RotateByQuaternion(RectVertex[VertexIndex] , Rect.Rotation);
		RectVertex[VertexIndex] = Vector3Add(RectVertex[VertexIndex] , Rect.Position);
	}
    
	Color ColorBytes = LinearToColor(LineColor);
    
	DrawRoundLine(RectVertex[Vertex_TopRight] , RectVertex[Vertex_TopLeft] , LineSize, ColorBytes , ColorBytes );
	DrawRoundLine(RectVertex[Vertex_TopLeft] , RectVertex[Vertex_BottomLeft] , LineSize, ColorBytes , ColorBytes );
	DrawRoundLine(RectVertex[Vertex_BottomLeft] , RectVertex[Vertex_BottomRight] , LineSize, ColorBytes , ColorBytes );
	DrawRoundLine(RectVertex[Vertex_BottomRight] , RectVertex[Vertex_TopRight] , LineSize, ColorBytes , ColorBytes );
}

internal void DrawRectLine(_Rect Rect, Vector4 LineColor)
{
	DrawRectLine(Rect, LineColor , 4);
}

internal void DrawRectLine(_Rect Rect, Color LineColor , float LineSize )
{
	DrawRectLine(Rect , ColorToLinear(LineColor) , LineSize );
}

internal void DrawRectLine(_Rect Rect , Color LineColor)
{
	DrawRectLine(Rect, LineColor , 4);
}

internal void DrawQuadLine(_Quad Quad, Color LineColor)
{
	float LineSize = 8;
	DrawQuadLine(Quad, LineColor , LineSize);
}

internal void DrawQuadLine(_Quad * Quad, Color LineColor)
{
	float LineSize = 0.01f;
	DrawQuadLine(*Quad, LineColor , LineSize);
}

internal void DrawRect(_Rect Rect,Color TargetColor)
{
	DrawRectWithTexture(Rect  , {0,0,0,0}, TargetColor, RenderState.AllTextures[0].id);
}

internal void DrawRect(_Rect Rect)
{
	DrawRect(Rect, WHITE);
}

//draw text only
internal void _DrawRectText(_Rect Quad ,Vector4* Color ,Vector2* TextureCoord , Vector3 Up ,Vector3 Right)
{
    bool CleanUpDrawState = false; 
    
	if (RenderState.CurrentShaderInput != S_Text)
	{
        CleanUpDrawState = true;
	}
    
    if(RenderState.CurrentShaderInput == S_Text)
    {
        if(AllShaderInputs[S_Text].VerticesCount+10 >= VERTEX_BUFFER_MAX)
        {
            CleanUpDrawState = true;
        }
        
        if(AllShaderInputs[S_Text].IndicesCount + 10 >=VERTEX_BUFFER_INDICS_MAX)
        {
            CleanUpDrawState  = true;
        }
    }
    
    if(CleanUpDrawState)
    {
        D_GameDraw();
    }
    
	RenderState.CurrentShaderInput = S_Text;
    
	_ShaderInput* ShaderInput = AllShaderInputs + S_Text;
    
	int VertexIndex[4] = {};
    
	unsigned short* IndicesArray = VertexData.Indices;
	int* IndicsCount = &ShaderInput->IndicesCount;
    
	Vector3* VertexArray = VertexData.Position;
	Vector4* ColorArray = VertexData.Color;
	Vector2* TextureCoordArray = VertexData.TexCoord;
	int* VertexCount = &ShaderInput->VerticesCount;
    
	Vector3 Position = Quad.Position;
	Vector2 Size = Quad.Size;
	Quaternion Rotation = Quad.Rotation;
    
	Vector3 UpOffset = Vector3Scale(Up, Size.y);
	Vector3 BottomOffset = Vector3Scale(Up, -Size.y);
	Vector3 RightOffset = Vector3Scale(Right, -Size.x);
	Vector3 LeftOffset = Vector3Scale(Right,Size.x);
    
	Vector3 VertexLeftTop = Vector3Add(LeftOffset, UpOffset);
	Vector3 VertexRightTop = Vector3Add(RightOffset,UpOffset);
	Vector3 VertexLeftBottom = Vector3Add(LeftOffset , BottomOffset);
	Vector3 VertexRightBottom = Vector3Add(RightOffset , BottomOffset);
    
	VertexLeftTop = Vector3RotateByQuaternion(VertexLeftTop, Rotation);
	VertexRightTop = Vector3RotateByQuaternion(VertexRightTop, Rotation);
	VertexLeftBottom = Vector3RotateByQuaternion(VertexLeftBottom, Rotation);
	VertexRightBottom = Vector3RotateByQuaternion(VertexRightBottom, Rotation);
    
	VertexLeftTop = Vector3Add(VertexLeftTop,Position);    
	VertexRightTop= Vector3Add(VertexRightTop,Position);
    VertexLeftBottom= Vector3Add(VertexLeftBottom,Position);
    VertexRightBottom= Vector3Add(VertexRightBottom,Position);
    
	VertexIndex[0] = *VertexCount;
    
	VertexArray[*VertexCount] = VertexLeftTop;
	TextureCoordArray[*VertexCount] = TextureCoord[0];
	ColorArray[*VertexCount] = Color[0];
	(*VertexCount)++;
    
	VertexIndex[1] = *VertexCount;
    
	VertexArray[*VertexCount] = VertexRightTop;
	TextureCoordArray[*VertexCount] = TextureCoord[1];
	ColorArray[*VertexCount] = Color[1];
	(*VertexCount)++;
    
	VertexIndex[2] = *VertexCount;
	
	VertexArray[*VertexCount] = VertexLeftBottom;
	TextureCoordArray[*VertexCount] = TextureCoord[2];
	ColorArray[*VertexCount] = Color[2];
	(*VertexCount)++;
    
	VertexIndex[3] = *VertexCount;
	
	VertexArray[*VertexCount] = VertexRightBottom;
	TextureCoordArray[*VertexCount] = TextureCoord[3];
	ColorArray[*VertexCount] = Color[3];
	(*VertexCount)++;
    
	IndicesArray[(*IndicsCount)++] = VertexIndex[0];
	IndicesArray[(*IndicsCount)++] = VertexIndex[1];
	IndicesArray[(*IndicsCount)++] = VertexIndex[2];
	IndicesArray[(*IndicsCount)++] = VertexIndex[3];
	IndicesArray[(*IndicsCount)++] = VertexIndex[2];
	IndicesArray[(*IndicsCount)++] = VertexIndex[1];
}

internal void DrawBackground()
{
	RenderState.CurrentTexture = RenderState.AllTextures[0].id;
    
	_ShaderInput * FlatShader = AllShaderInputs + S_Quad;
    
	rlEnableShader(FlatShader->Shader);
	glBindVertexArray(FlatShader->VertexArrayObject);
    
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, RenderState.CurrentTexture );
	glUniform1i(FlatShader->UniformLocation[SU_Texture0], 0);
    
	rlSetUniformMatrix(FlatShader->UniformLocation[SU_MVP] , MatrixIdentity());
    
	Vector3 * SreenQuadPos = VertexData.Position;
	Vector4 * ScreenQuadColorData = VertexData.Color;
	Vector2 * ScreenQuadTexCoordData = VertexData.TexCoord;
    
	int VertCount = 0;
    
	float Top= 1;
    
	SreenQuadPos[VertCount++] = { -1, Top, 0 };
	SreenQuadPos[VertCount++] = { 1, Top, 0 };
	SreenQuadPos[VertCount++] = { 1, -1, 0 };
    
	SreenQuadPos[VertCount++] = { -1, -1, 0 };
	SreenQuadPos[VertCount++] = { -1, Top, 0 };
	SreenQuadPos[VertCount++] = { 1, -1, 0 };
    
	VertCount = 0;
    
	ScreenQuadTexCoordData[VertCount++] = { 0, 1 };
	ScreenQuadTexCoordData[VertCount++] = { 1, 1 };
	ScreenQuadTexCoordData[VertCount++] = { 1, 0 };
    
	ScreenQuadTexCoordData[VertCount++] = { 0, 0 };
	ScreenQuadTexCoordData[VertCount++] = { 0, 1 };
	ScreenQuadTexCoordData[VertCount++] = { 1, 0 };
    
	VertCount = 0;
    
	Vector4 QuadColor[Vertex_Count] = {};
    
	Color TopColor = DARKGRAY;
	Color BottomColor = DARKGRAY;
    
	QuadColor[Vertex_TopLeft] = ColorToLinear(ColorMultiply(TopColor,0.8f));
	QuadColor[Vertex_TopRight] = ColorToLinear(ColorMultiply(TopColor,0.8f));
	QuadColor[Vertex_BottomLeft] = ColorToLinear(ColorMultiply(BottomColor ,0.6f));
	QuadColor[Vertex_BottomRight] = ColorToLinear(ColorMultiply(BottomColor ,0.6f));
    
	ScreenQuadColorData[VertCount++] = QuadColor[Vertex_TopLeft];
	ScreenQuadColorData[VertCount++] = QuadColor[Vertex_TopRight];
	ScreenQuadColorData[VertCount++] = QuadColor[Vertex_BottomRight];
    
	ScreenQuadColorData[VertCount++] = QuadColor[Vertex_BottomLeft];
	ScreenQuadColorData[VertCount++] = QuadColor[Vertex_TopLeft];
	ScreenQuadColorData[VertCount++] = QuadColor[Vertex_BottomRight];
    
	Update_GPU_VertexBufferAll(FlatShader, 6);
    
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

internal void DrawScreenFlat(int TextureIndex , Vector4 _Color)
{
	_ShaderInput * FlatShader = AllShaderInputs + S_Quad;
    
	rlEnableShader(FlatShader->Shader);
	glBindVertexArray(FlatShader->VertexArrayObject);
    
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureIndex);
	glUniform1i(FlatShader->UniformLocation[SU_Texture0], 0);
    
	rlSetUniformMatrix(FlatShader->UniformLocation[SU_MVP] , MatrixIdentity());
    
	Vector3 * SreenQuadPos = VertexData.Position;
	Vector4 * ScreenQuadColorData = VertexData.Color;
	Vector2 * ScreenQuadTexCoordData = VertexData.TexCoord;
    
	int VertCount = 0;
    
	SreenQuadPos[VertCount++] = { -1, 1, 0 };
	SreenQuadPos[VertCount++] = { 1, 1, 0 };
	SreenQuadPos[VertCount++] = { 1, -1, 0 };
    
	SreenQuadPos[VertCount++] = { -1, -1, 0 };
	SreenQuadPos[VertCount++] = { -1, 1, 0 };
	SreenQuadPos[VertCount++] = { 1, -1, 0 };
    
	VertCount = 0;
    
	ScreenQuadTexCoordData[VertCount++] = { 0, 1 };
	ScreenQuadTexCoordData[VertCount++] = { 1, 1 };
	ScreenQuadTexCoordData[VertCount++] = { 1, 0 };
    
	ScreenQuadTexCoordData[VertCount++] = { 0, 0 };
	ScreenQuadTexCoordData[VertCount++] = { 0, 1 };
	ScreenQuadTexCoordData[VertCount++] = { 1, 0 };
    
	VertCount = 0;
    
	ScreenQuadColorData[VertCount++] = _Color;
	ScreenQuadColorData[VertCount++] = _Color;
	ScreenQuadColorData[VertCount++] = _Color;
    
	ScreenQuadColorData[VertCount++] = _Color;
	ScreenQuadColorData[VertCount++] = _Color;
	ScreenQuadColorData[VertCount++] = _Color;
    
	Update_GPU_VertexBufferAll(FlatShader, 6);
    
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

internal void DrawScreenFlat(int TextureIndex ,Color _Color)
{
	DrawScreenFlat(TextureIndex , ColorToLinear(_Color));
}

internal void DrawSpring(_Quad Quad)
{
	bool CleanUpDrawState = false;
	int TargetTexture = RenderState.AllTextures[0].id;
    
	if (RenderState.CurrentShaderInput != S_Spring)
	{
		CleanUpDrawState = true;
	}
	
	if (RenderState.CurrentTexture!= TargetTexture)
	{
		CleanUpDrawState = true;
	}
    
    if(RenderState.CurrentShaderInput == S_Spring)
    {
        
        //just check if it was the will too close
        if(AllShaderInputs[S_Spring].VerticesCount + 10 >=VERTEX_BUFFER_MAX)
        {
            CleanUpDrawState = true;
        }
        
        if(AllShaderInputs[S_Spring].IndicesCount + 10 >=VERTEX_BUFFER_INDICS_MAX)
        {
            CleanUpDrawState  = true;
        }
    }
    
	if (CleanUpDrawState)
	{
		D_GameDraw();
	}
    
	RenderState.CurrentShaderInput= S_Spring;
	RenderState.CurrentTexture = TargetTexture;
    
	_ShaderInput* ShaderInput = AllShaderInputs + S_Spring;	
    
	unsigned short VertexIndex[4] = {};
	
	int IndicsCount = ShaderInput->IndicesCount;
	ShaderInput->IndicesCount += 6;
    
	int VertexCount = ShaderInput->VerticesCount;    
	ShaderInput->VerticesCount+=4;
    
	Vector3* AllVertices = Quad.VertexPosition;
	Vector4* VertexColor = Quad.VertexColor;
	Vector2 TextureCoord[4] = {};
    
    TextureCoord[Vertex_BottomLeft] = { 0,1 };
    TextureCoord[Vertex_TopLeft] = { 1,1 };
    TextureCoord[Vertex_TopRight] = { 1,0 };
    TextureCoord[Vertex_BottomRight] = { 0, 0 };
    
	//i'm so confused with the vertics order     
	VertexIndex[0] = VertexCount;
	VertexData.Position[VertexCount] = AllVertices[0];
	VertexData.TexCoord[VertexCount] = TextureCoord[0];
	VertexData.Color[VertexCount] = VertexColor[0];
	VertexCount++;
    
	VertexIndex[1] = VertexCount;
	VertexData.Position[VertexCount] = AllVertices[1];
	VertexData.TexCoord[VertexCount] = TextureCoord[2];
	VertexData.Color[VertexCount] = VertexColor[1];
	VertexCount++;
    
	VertexIndex[2] = VertexCount;
	VertexData.Position[VertexCount] = AllVertices[2];
	VertexData.TexCoord[VertexCount] = TextureCoord[1];
	VertexData.Color[VertexCount] = VertexColor[2];
	VertexCount++;
    
	VertexIndex[3] = VertexCount;
	VertexData.Position[VertexCount] = AllVertices[3];
	VertexData.TexCoord[VertexCount] = TextureCoord[3];
	VertexData.Color[VertexCount] = VertexColor[3];
	VertexCount++;
    
	VertexData.Indices[IndicsCount++] = VertexIndex[0];
	VertexData.Indices[IndicsCount++] = VertexIndex[1];
	VertexData.Indices[IndicsCount++] = VertexIndex[2];
	VertexData.Indices[IndicsCount++] = VertexIndex[3];
	VertexData.Indices[IndicsCount++] = VertexIndex[2];
	VertexData.Indices[IndicsCount++] = VertexIndex[1];
    
}