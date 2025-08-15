#version 330 core

in vec3 vertexPosition;
in vec4 rotation;
in vec2 rectSize;
in vec4 rectRadius;

in vec4 vertexColor_topLeft;
in vec4 vertexColor_topRight;
in vec4 vertexColor_bottomLeft;
in vec4 vertexColor_bottomRight;

in vec2 vertexTexCoord_topLeft;
in vec2 vertexTexCoord_topRight;
in vec2 vertexTexCoord_bottomLeft;
in vec2 vertexTexCoord_bottomRight;

out RectData
{
    vec3 position;
	vec4 rotation;
    vec2 size;
	vec4 radius;

	vec4 color[4];    
    vec2 texCoord[4];
    
} RectDataOut;


void main()
{
	RectDataOut.position = vertexPosition;
	RectDataOut.size = rectSize;
	RectDataOut.radius = rectRadius;
	RectDataOut.rotation = rotation;

	RectDataOut.color[0] = vertexColor_topLeft;
	RectDataOut.color[1] = vertexColor_topRight;
	RectDataOut.color[2] = vertexColor_bottomLeft;
	RectDataOut.color[3] = vertexColor_bottomRight;
	
	RectDataOut.texCoord[0] = vertexTexCoord_topLeft;
	RectDataOut.texCoord[1] = vertexTexCoord_topRight;
	RectDataOut.texCoord[2] = vertexTexCoord_bottomLeft;
	RectDataOut.texCoord[3] = vertexTexCoord_bottomRight;
}