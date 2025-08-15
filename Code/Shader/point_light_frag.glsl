
#version 330 core

flat in vec3 lightPosition;
flat in vec2 lightSize;
flat in float lightMul;

in vec3 fragWorldPosition;

in vec4 fragColor;
in vec2 fragTexCoord;

out vec4 finalColor;

uniform sampler2D texture0;

void main()
{
    finalColor = texture(texture0,fragTexCoord);
    
    vec2 fragOffset = fragWorldPosition.xy - lightPosition.xy;
    float lightStrength = (1.0 - length((fragOffset)/(lightSize * 0.5f))) ;
    
    finalColor *= lightStrength* fragColor;
}