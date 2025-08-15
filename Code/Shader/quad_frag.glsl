#version 330 core

in vec4 fragColor;
in vec2 fragTexCoord;

uniform mat4 mvp;
uniform sampler2D texture0;

out vec4 finalColor;

void main()
{
	vec4 textureColor = texture(texture0, fragTexCoord);

	finalColor = textureColor;
	finalColor*= fragColor;

}