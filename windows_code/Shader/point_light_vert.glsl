#version 330 core

in vec3 lightPosition;
in vec4 lightColor;

in vec2 lightSize;
in float lightMul;

in vec2 lightTexCoord_topLeft;
in vec2 lightTexCoord_topRight;
in vec2 lightTexCoord_bottomLeft;
in vec2 lightTexCoord_bottomRight;

out PointLightData
{
    vec3 lightPosition;
    vec4 lightColor;
    
    vec2 lightQuadSize;
    float lightMul;
    
    vec2 AfterGameDrawTexCoord[4];
    
} lightDataOut;

void main()
{
    lightDataOut.lightPosition = lightPosition;
    lightDataOut.lightColor = lightColor;
    lightDataOut.lightQuadSize = lightSize;
    lightDataOut.lightMul = lightMul;
    
    lightDataOut.AfterGameDrawTexCoord[0] = lightTexCoord_topLeft;
    lightDataOut.AfterGameDrawTexCoord[1] = lightTexCoord_topRight;
    lightDataOut.AfterGameDrawTexCoord[2] = lightTexCoord_bottomLeft;
    lightDataOut.AfterGameDrawTexCoord[3] = lightTexCoord_bottomRight;
}