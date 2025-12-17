#version 330 core

#define TWO_PI 6.2831855
#define E 2.71828

in vec2 fragTexCoord;

uniform float spread;
uniform float gridSize;

uniform sampler2D texture0;

uniform vec2 screenSize;

out vec4 finalColor;

float gaussian(float x)
{
	float sigmaSqu = spread * spread;
	return (1.0 / sqrt(TWO_PI * sigmaSqu)) * pow(E, -(x * x) / (2.0 * sigmaSqu));
}

void main()
{
	vec4 col = vec4(0.0, 0.0, 0.0 , 0.0);
	float gridSum = 0.0;

	int upper = ((int(gridSize) - 1) / 2);
	int lower = -upper;

	for (int x = lower; x <= upper; ++x)
	{
		float gauss = gaussian(float(x));
		gridSum += gauss;
		
		vec2 uv = fragTexCoord + vec2(screenSize.x * float(x) * 2.0, 0.0);
		col += gauss * texture(texture0 , uv);
	}

	col /= gridSum;
	
	//finalColor = vec4(gridSum, 0 , 0, 0);
	finalColor = col;
}