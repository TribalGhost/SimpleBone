#version 330 core

layout (points) in;
layout (triangle_strip,max_vertices = 6) out;

in PointLightData
{
    vec3 lightPosition;
    vec4 lightColor;
    
    vec2 lightQuadSize;
    float lightMul;
    
    vec2 AfterGameDrawTexCoord[4];
    
} lightDataIn[];

uniform mat4 mvp;

flat out vec3 lightPosition;
flat out vec2 lightSize;
flat out float lightMul;

out vec3 fragWorldPosition;

out vec4 fragColor;
out vec2 fragTexCoord;

void main()
{
    lightPosition = lightDataIn[0].lightPosition;
    lightSize = lightDataIn[0].lightQuadSize;
    lightMul = lightDataIn[0].lightMul;
    fragColor = lightDataIn[0].lightColor;
    
    vec4 quadPosition[4];
    
    for(int i = 0 ; i < 4 ; i++)
    {
        quadPosition[i] = vec4(lightPosition,1.0);
    }
    
    quadPosition[0].x -= lightSize.x*0.5f;
    quadPosition[0].y += lightSize.y*0.5f;
    
    quadPosition[1].x += lightSize.x*0.5f;
    quadPosition[1].y += lightSize.y*0.5f;
    
    quadPosition[2].x -= lightSize.x*0.5f;
    quadPosition[2].y -= lightSize.y*0.5f;
    
    quadPosition[3].x += lightSize.x*0.5f;
    quadPosition[3].y -= lightSize.y*0.5f;
    
    for(int vertexIndex = 0 ; vertexIndex <4 ;vertexIndex++)
    {
        gl_Position =mvp * quadPosition[vertexIndex];
        fragWorldPosition = quadPosition[vertexIndex].xyz;
        fragTexCoord = lightDataIn[0].AfterGameDrawTexCoord[vertexIndex];
        
        EmitVertex();
    }
    
    //not sure what you doing here
    EndPrimitive();
}