#version 330 core


in vec2 fragTexCoord;

uniform float spread;
uniform float gridSize;

uniform sampler2D texture0;

uniform vec2 screenSize;

out vec4 finalColor;

#define TWO_PI 6.2831855
#define E 2.71828

float gaussian(float x)
{
	float sigmaSqu = spread * spread;
	return (1.0 / sqrt(TWO_PI * sigmaSqu)) * pow(E, -(x * x) / (2.0 * sigmaSqu));
}

void main()
{
	vec4 col = vec4(0.0, 0.0, 0.0 , 0.0);
	float gridSum = 0.0;

	int upper = ((int(gridSize )- 1) / 2);
	int lower = -upper;

	for (int y = lower; y <= upper; ++y)
	{
		float gauss = gaussian(float(y));
		gridSum += gauss;

		vec2 uv = fragTexCoord + vec2(0.0, screenSize.y * float(y));
		col += gauss * texture(texture0, uv);
	}

	col /= gridSum;
	finalColor = col ;
}