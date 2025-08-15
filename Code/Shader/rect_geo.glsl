#version 330 core

layout (points) in;
layout (triangle_strip,max_vertices = 6) out;

in RectData
{
    vec3 position;   
	vec4 rotation;
    vec2 size;
	vec4 radius;

	vec4 color[4];    
    vec2 texCoord[4];
    
} RectDataOut[];

uniform mat4 mvp;

flat out vec4 fragRotation;
flat out vec3 centrePosition;
flat out vec4 rectRadius;
flat out vec2 rectSize;

out vec3 fragPosition;
out vec4 fragColor;
out vec2 fragTexCoord;

vec3 quat_transform( vec4 q, vec3 v )
{
    return v + 2.*cross( q.xyz, cross( q.xyz, v ) + q.w*v ); 
}

void main()
{
    vec3 position = RectDataOut[0].position;
    vec2 size= RectDataOut[0].size;
	vec4 rotation = RectDataOut[0].rotation;

    vec4 quadPosition[4];

    for(int i = 0 ; i < 4 ; i++)
    {
        quadPosition[i] = vec4(0.0,0.0,0.0,1.0);
    }
    
	quadPosition[0].x -= size.x*0.5f;
    quadPosition[0].y += size.y*0.5f;
    
    quadPosition[1].x += size.x*0.5f;
    quadPosition[1].y += size.y*0.5f;
    
    quadPosition[2].x -= size.x*0.5f;
    quadPosition[2].y -= size.y*0.5f;
    
    quadPosition[3].x += size.x*0.5f;
    quadPosition[3].y -= size.y*0.5f;

    for(int i = 0 ; i < 4 ; i++)
    {
		quadPosition[i].xyz = quat_transform(rotation, quadPosition[i].xyz );
        quadPosition[i].xyz += position;
    }
        
    for(int vertexIndex = 0 ; vertexIndex <4 ;vertexIndex++)
    {
        gl_Position =mvp * quadPosition[vertexIndex];
        fragPosition = quadPosition[vertexIndex].xyz;
				
		//why it is negative?
		fragRotation = rotation;
        fragTexCoord = RectDataOut[0].texCoord[vertexIndex];
		fragColor = RectDataOut[0].color[vertexIndex];

		centrePosition = position;
		rectSize = size;
		rectRadius = RectDataOut[0].radius;
        
        EmitVertex();
    }
    
    //not sure what you doing here
    EndPrimitive();
}