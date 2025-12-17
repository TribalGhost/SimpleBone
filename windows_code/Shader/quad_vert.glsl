#version 330 core

in vec3 vertexPosition;
in vec4 vertexColor;
in vec2 vertexTexCoord;
in vec3 vertexNormal;

out vec4 fragColor;
out vec2 fragTexCoord;
out vec3 fragPosition;
out vec3 fragNormal;

uniform mat4 mvp;

void main()
{
	fragColor= vertexColor;
	fragTexCoord = vertexTexCoord;
    fragNormal = vertexNormal;
	fragPosition = vertexPosition;
	
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}