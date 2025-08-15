#version 330 core

in vec3 vertexPosition;
in vec4 vertexColor;
in vec2 vertexTexCoord;

uniform mat4 mvp;

out vec4 fragColor;
out vec2 fragTexCoord;

void main()
{
	fragColor = vertexColor;
    
	fragTexCoord = vertexTexCoord;
	gl_Position = mvp * vec4(vertexPosition, 1.0);
}
