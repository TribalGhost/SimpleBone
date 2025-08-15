#version 330 core

layout (points) in;
layout (triangle_strip,max_vertices = 6) out;

in LineData
{
    vec3 position;   
    vec2 size;

	vec4 color[4];    
    vec2 texCoord[4];
    
} LineDataIn[];

uniform mat4 mvp;

out vec3 fragPosition;
out vec4 fragColor;
out vec2 fragTexCoord;

void main()
{
    vec3 position = RectDataOut[0].position;
    vec2 size= RectDataOut[0].size;

    vec4 quadPosition[4];
    
    for(int i = 0 ; i < 4 ; i++)
    {
        quadPosition[i] = vec4(position ,1.0);
    }
    
    quadPosition[0].x -= size.x*0.5f;
    quadPosition[0].y += size.y*0.5f;
    
    quadPosition[1].x += size.x*0.5f;
    quadPosition[1].y += size.y*0.5f;
    
    quadPosition[2].x -= size.x*0.5f;
    quadPosition[2].y -= size.y*0.5f;
    
    quadPosition[3].x += size.x*0.5f;
    quadPosition[3].y -= size.y*0.5f;
    
    for(int vertexIndex = 0 ; vertexIndex <4 ;vertexIndex++)
    {
        gl_Position =mvp * quadPosition[vertexIndex];
        fragPosition = quadPosition[vertexIndex].xyz;
        fragTexCoord = RectDataOut[0].texCoord[vertexIndex];
		fragColor = RectDataOut[0].color[vertexIndex];
        
        EmitVertex();
    }
    
    //not sure what you doing here
    EndPrimitive();
}