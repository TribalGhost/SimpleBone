#version 330 core

in vec4 fragColor;
in vec3 fragPosition;

in float fragLineSize;

in float fragFakeDepth;
flat in vec3 fragLineStart;
flat in vec3 fragLineEnd;

uniform vec2 screenSize;

float sdSegment( in vec2 p, in vec2 a, in vec2 b )
{
    vec2 pa = p-a, ba = b-a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h );
}

out vec4 finalColor;

void main()
{

	finalColor = fragColor;

	vec2 position = fragPosition.xy;
	vec2 lineStart = fragLineStart.xy;
	vec2 lineEnd = fragLineEnd.xy;

	position.xy = ((position.xy - 1.0) / 2.0) * screenSize;
	lineStart.xy = ((lineStart.xy - 1.0) / 2.0) * screenSize;
	lineEnd.xy = ((lineEnd.xy - 1.0) / 2.0) * screenSize;

	float distance = sdSegment(position.xy , lineStart.xy, lineEnd.xy);
	//distance *= fragPosition.z;

	#if 1
	//why multiply 5??
	if ( distance * 5 < fragLineSize)
	{
		gl_FragDepth = fragFakeDepth;
	}
	else
	{
		gl_FragDepth = -1;
	}
	#endif

	float blur = 5;
	float alpha = smoothstep( fragLineSize + blur , fragLineSize - blur, distance *5);

	finalColor.a *= alpha;

	//finalColor.a += 0.1f;

}