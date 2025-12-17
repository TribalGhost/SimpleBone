#version 330 core

in vec3 fragPosition;
in vec4 fragColor;
in vec2 fragTexCoord;

uniform float elapsedTime;
uniform mat4 mvp;
uniform sampler2D texture0;

out vec4 finalColor;

void main()
{
	finalColor = texture(texture0, fragTexCoord);

	finalColor *= fragColor;

	finalColor.a = sin( (fragPosition.y * 240)+ elapsedTime *8 );
	finalColor.a *= 1.0 - fragTexCoord.y;
}