#version 330 core

in vec4 fragColor;
in vec2 fragTexCoord;

uniform mat4 mvp;
uniform sampler2DMS texture0;

out vec4 finalColor;

void main()
{
	vec2 iTmp = textureSize(texture0);
	vec2 tmp = floor(iTmp * fragTexCoord);
    
	vec4 textureColor;
	for(int i = 0; i < 4; ++i)
	{
		textureColor += texelFetch(texture0, ivec2(tmp), i);
	}
    textureColor/=4;
    
	finalColor = textureColor;
	finalColor*= fragColor;
    
}