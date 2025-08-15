
#version 330 core

in vec4 fragColor;
in vec2 fragTexCoord;

uniform sampler2D texture0;

out vec4 finalColor;

const float smoothing = 1.0 / 32.0;

void main()
{
	finalColor = fragColor;

	finalColor.a *= smoothstep(0.5 - smoothing, 0.8 + smoothing, texture(texture0, fragTexCoord).r);
	//finalColor=vec4(fragTexCoord,0,1);

}