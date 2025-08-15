#version 330 core

in vec2 fragTexCoord;

uniform sampler2D texture0;
uniform vec2 screenSize;

uniform vec4 BaseColor;

out vec4 finalColor;

const float samples = 16;
const float quality = 4;

#if 1
void main()
{
	vec4 sum = vec4(0);

	//vec4 source = texture(texture0, fragTexCoord);

	vec2 sizeFactor = vec2(1,1) / screenSize * quality;

	int range = int((samples - 1) / 2);

	for (int x = -range; x <= range; x++)
	{
		for (int y = -range; y <= range; y++)
		{
			sum += texture(texture0, fragTexCoord + vec2(x, y) * sizeFactor);
		}
	}

	vec4 BaseColor = vec4( 0.2,0.2,0.15 ,1);
	finalColor = (sum / (samples * samples)) * 1.2f + BaseColor * 0.26f;
	//finalColor = source;
	//finalColor = vec4(1,1,0,1);
}

#endif
