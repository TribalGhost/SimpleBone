#version 330 core
in vec3 vertexPosition;
in vec4 vertexColor;

in vec3 lineStart;
in vec3 lineEnd;

in float lineSize;
in float fakeDepth;

out vec4 fragColor;
out vec3 fragPosition;
out float fragLineSize;
out float fragFakeDepth;

flat out vec3 fragLineStart;
flat out vec3 fragLineEnd;

void main()
{
	fragColor = vertexColor;
	fragLineStart = lineStart;
	fragLineEnd = lineEnd ;
	fragPosition = vertexPosition;

	fragLineSize = lineSize;
	fragFakeDepth = fakeDepth;

	gl_Position = vec4(vertexPosition, 1.0);
}
